#include "Sonido.h"

// Todo este archivo es codigo nativo (necesario para usar hilos de Windows en un proyecto C++/CLI)
#pragma managed(push, off)
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstring>
#pragma comment(lib, "winmm.lib")

// Archivos esperados (en la carpeta "sonidos\" junto a "imagenes\"), WAV PCM de 16 bits:
//   disparo.wav  explosion.wav  golpe.wav  inicio.wav  motor.wav  musica.wav
// Si algun sonido falla, el motivo aparece en la ventana "Salida" de Visual Studio.
//
// Todo el audio usa waveOut y el VOLUMEN se aplica directamente sobre las muestras,
// asi funciona igual en cualquier equipo.
//   - CBufferWav     : carga un WAV y le aplica el volumen
//   - CBucleSonido   : un sonido en bucle (motor, musica)
//   - CEfectoSonido  : un efecto corto con varias "voces" para que se solapen
//   - CSonido        : Singleton que coordina todo (ver Sonido.h)

// =====================================================================
//  CBufferWav: carga un WAV PCM y mantiene una copia con el volumen aplicado
// =====================================================================
class CBufferWav {
public:
	bool cargar(const char* ruta) {
		ok = false;
		std::ifstream f(ruta, std::ios::binary);
		if (!f) {
			OutputDebugStringA((std::string(ruta) + ": no se encontro\n").c_str());
			return false;
		}
		archivo.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
		size_t n = archivo.size();
		if (n < 44) {
			OutputDebugStringA((std::string(ruta) + ": archivo vacio o invalido\n").c_str());
			return false;
		}

		const char* p = archivo.data();
		ZeroMemory(&fmt, sizeof(fmt));
		original = nullptr;
		tam = 0;

		// buscar los bloques "fmt " y "data" del WAV
		size_t pos = 12;
		while (pos + 8 <= n) {
			DWORD sz = *(const DWORD*)(p + pos + 4);
			if (memcmp(p + pos, "fmt ", 4) == 0 && pos + 8 + 16 <= n) {
				memcpy(&fmt, p + pos + 8, 16);
				fmt.cbSize = 0;
			}
			else if (memcmp(p + pos, "data", 4) == 0) {
				original = p + pos + 8;
				tam = (DWORD)((pos + 8 + sz <= n) ? sz : n - pos - 8);
				break;
			}
			pos += 8 + sz + (sz & 1);
		}
		if (!original || fmt.nChannels == 0 || fmt.wFormatTag != WAVE_FORMAT_PCM) {
			OutputDebugStringA((std::string(ruta) + ": no es WAV PCM (usar PCM 16 bits)\n").c_str());
			return false;
		}
		salida.assign(original, original + tam);
		ok = true;
		return true;
	}

	// 1.0 = volumen original del archivo, 0.0 = silencio
	void aplicarVolumen(float vol) {
		if (!ok) return;
		int g = (int)(vol * 256.0f);
		if (fmt.wBitsPerSample == 16) {
			const short* src = (const short*)original;
			short* dst = (short*)salida.data();
			size_t n = tam / 2;
			for (size_t i = 0; i < n; i++)
				dst[i] = (short)((src[i] * g) >> 8);
		}
		else if (fmt.wBitsPerSample == 8) {
			for (size_t i = 0; i < tam; i++) {
				int s = (unsigned char)original[i] - 128;
				salida[i] = (char)(128 + ((s * g) >> 8));
			}
		}
	}

	bool valido() const { return ok; }
	WAVEFORMATEX* getFormato() { return &fmt; }
	char* getDatos() { return salida.data(); }      // copia con el volumen aplicado
	DWORD getTam() const { return tam; }

private:
	std::vector<char> archivo;                      // archivo original completo
	const char* original = nullptr;                 // audio PCM dentro de 'archivo'
	std::vector<char> salida;                       // copia que se reproduce
	WAVEFORMATEX fmt;
	DWORD tam = 0;
	bool ok = false;
};

// =====================================================================
//  CBucleSonido: un sonido en bucle (motor, musica)
// =====================================================================
class CBucleSonido {
public:
	void abrir(const char* ruta) {
		if (!wav.cargar(ruta)) return;
		if (waveOutOpen(&h, WAVE_MAPPER, wav.getFormato(), 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
			OutputDebugStringA("no se pudo abrir el dispositivo de audio\n");
			return;
		}
		waveOutSetVolume(h, 0xFFFFFFFF);              // volumen del dispositivo al maximo
		wav.aplicarVolumen(vol);

		ZeroMemory(&cab, sizeof(cab));
		cab.lpData = (LPSTR)wav.getDatos();
		cab.dwBufferLength = wav.getTam();
		cab.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		cab.dwLoops = 0x7FFFFFFF;                     // bucle "infinito"
		waveOutPrepareHeader(h, &cab, sizeof(WAVEHDR));
		waveOutPause(h);                              // empieza en pausa
		waveOutWrite(h, &cab, sizeof(WAVEHDR));
		cargado = true;
	}

	void reanudar() { if (cargado && !corriendo) { waveOutRestart(h); corriendo = true; } }
	void pausar() { if (cargado && corriendo) { waveOutPause(h);   corriendo = false; } }

	void setVolumen(float v) {                        // 0.0 a 1.0
		vol = v;
		wav.aplicarVolumen(vol);
	}

	void cerrar() {
		if (!cargado) return;
		waveOutReset(h);
		waveOutUnprepareHeader(h, &cab, sizeof(WAVEHDR));
		waveOutClose(h);
		h = NULL;
		cargado = false;
		corriendo = false;
	}

private:
	CBufferWav wav;
	HWAVEOUT h = NULL;
	WAVEHDR cab;
	bool cargado = false;
	bool corriendo = false;
	float vol = 1.0f;
};

// =====================================================================
//  CEfectoSonido: efecto corto con varias voces (se pueden solapar)
// =====================================================================
class CEfectoSonido {
public:
	void abrir(const char* ruta, int voces) {
		if (!wav.cargar(ruta)) return;
		wav.aplicarVolumen(vol);
		hs.assign(voces, (HWAVEOUT)NULL);
		cabs.resize(voces);
		for (int i = 0; i < voces; i++) {
			if (waveOutOpen(&hs[i], WAVE_MAPPER, wav.getFormato(), 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
				OutputDebugStringA((std::string(ruta) + ": no se pudo abrir el dispositivo de audio\n").c_str());
				hs[i] = NULL;
				continue;
			}
			waveOutSetVolume(hs[i], 0xFFFFFFFF);
			ZeroMemory(&cabs[i], sizeof(WAVEHDR));
			cabs[i].lpData = (LPSTR)wav.getDatos();   // todas las voces comparten el mismo buffer
			cabs[i].dwBufferLength = wav.getTam();
			waveOutPrepareHeader(hs[i], &cabs[i], sizeof(WAVEHDR));
		}
		siguiente = 0;
	}

	void reproducir() {
		if (hs.empty()) return;
		int v = siguiente;
		siguiente = (siguiente + 1) % (int)hs.size();
		if (!hs[v]) return;
		if (cabs[v].dwFlags & WHDR_INQUEUE) waveOutReset(hs[v]);   // la voz sigue sonando: la corta
		waveOutWrite(hs[v], &cabs[v], sizeof(WAVEHDR));
	}

	void setVolumen(float v) {                        // 0.0 a 1.0
		vol = v;
		wav.aplicarVolumen(vol);
	}

	void cerrar() {
		for (size_t i = 0; i < hs.size(); i++) {
			if (!hs[i]) continue;
			waveOutReset(hs[i]);
			waveOutUnprepareHeader(hs[i], &cabs[i], sizeof(WAVEHDR));
			waveOutClose(hs[i]);
		}
		hs.clear();
		cabs.clear();
	}

private:
	CBufferWav wav;
	std::vector<HWAVEOUT> hs;
	std::vector<WAVEHDR> cabs;
	int siguiente = 0;
	float vol = 1.0f;
};

// =====================================================================
//  Datos internos de CSonido
// =====================================================================
enum Orden { ORD_DISPARO, ORD_EXPLOSION, ORD_GOLPE , ORD_MENU };

struct DatosSonido {
	bool iniciado = false;

	// cola de ordenes entre el juego y el hilo de sonido
	CRITICAL_SECTION candado;
	std::vector<int> cola;
	HANDLE evento = NULL;
	HANDLE hilo = NULL;
	volatile bool terminar = false;

	// efectos
	CEfectoSonido efDisparo, efExplosion, efGolpe, efMenu;

	// sonidos en bucle
	CBucleSonido motor;
	CBucleSonido musica;
	CBucleSonido musicaMenu;                          // inicio.wav: suena en el menu

	float volMusica = 0.5f;                           // 30 % por defecto
	float volEfectos = 0.1f;
	ULONGLONG ultimoMovimiento = 0;
};

static void encolar(DatosSonido* d, int orden) {
	if (!d->iniciado) return;
	EnterCriticalSection(&d->candado);
	if (d->cola.size() < 16) d->cola.push_back(orden);   // limite para que no se acumulen
	LeaveCriticalSection(&d->candado);
	SetEvent(d->evento);                                  // despierta al hilo
}

// Hilo de sonido: el juego solo encola ordenes y este hilo las ejecuta
static DWORD WINAPI hiloSonido(LPVOID param) {
	DatosSonido* d = (DatosSonido*)param;
	while (true) {
		WaitForSingleObject(d->evento, INFINITE);
		if (d->terminar) break;

		std::vector<int> pendientes;
		EnterCriticalSection(&d->candado);
		pendientes.swap(d->cola);
		LeaveCriticalSection(&d->candado);

		for (size_t i = 0; i < pendientes.size(); i++) {
			switch (pendientes[i]) {
			case ORD_DISPARO:   d->efDisparo.reproducir(); break;
			case ORD_EXPLOSION: d->efExplosion.reproducir(); break;
			case ORD_GOLPE:     d->efGolpe.reproducir(); break;
			case ORD_MENU:      d->efMenu.reproducir(); break;
			}
		}
	}
	return 0;
}

// =====================================================================
//  CSonido (Singleton)
// =====================================================================
CSonido& CSonido::getInstancia() {
	static CSonido instancia;
	return instancia;
}

CSonido::CSonido() : d(new DatosSonido()) {}

CSonido::~CSonido() {
	cerrar();
	delete d;
}

void CSonido::iniciar() {
	if (d->iniciado) return;
	InitializeCriticalSection(&d->candado);
	d->evento = CreateEvent(NULL, FALSE, FALSE, NULL);
	d->terminar = false;
	d->cola.clear();

	// volumen antes de abrir, para que ya nazcan con el volumen correcto
	d->efDisparo.setVolumen(d->volEfectos);
	d->efExplosion.setVolumen(d->volEfectos);
	d->efGolpe.setVolumen(d->volEfectos);
	d->motor.setVolumen(d->volEfectos);
	d->musica.setVolumen(d->volMusica);
	d->musicaMenu.setVolumen(d->volMusica);
	d->efMenu.setVolumen(d->volEfectos);

	d->efDisparo.abrir("sonidos\\disparo.wav", 3);
	d->efExplosion.abrir("sonidos\\explosion.wav", 3);
	d->efGolpe.abrir("sonidos\\golpe.wav", 2);
	d->motor.abrir("sonidos\\motor.wav");
	d->musica.abrir("sonidos\\musica.wav");
	d->musicaMenu.abrir("sonidos\\inicio.wav");
	d->efMenu.abrir("sonidos\\menu.wav", 2);


	d->iniciado = true;
	d->hilo = CreateThread(NULL, 0, hiloSonido, d, 0, NULL);
}

void CSonido::cerrar() {
	if (!d->iniciado) return;
	d->iniciado = false;
	d->terminar = true;
	SetEvent(d->evento);
	WaitForSingleObject(d->hilo, 2000);                // espera a que el hilo termine
	CloseHandle(d->hilo);
	d->hilo = NULL;
	CloseHandle(d->evento);
	d->evento = NULL;
	DeleteCriticalSection(&d->candado);

	d->efDisparo.cerrar();
	d->efExplosion.cerrar();
	d->efGolpe.cerrar();
	d->motor.cerrar();
	d->musica.cerrar();
	d->musicaMenu.cerrar();
	d->efMenu.cerrar();
}

void CSonido::disparo() { encolar(d, ORD_DISPARO); }
void CSonido::explosion() { encolar(d, ORD_EXPLOSION); }
void CSonido::golpe() { encolar(d, ORD_GOLPE); }
void CSonido::seleccion() { encolar(d, ORD_MENU); }

void CSonido::motor(bool moviendose) {
	if (!d->iniciado) return;
	ULONGLONG ahora = GetTickCount64();
	if (moviendose) {
		d->ultimoMovimiento = ahora;
		d->motor.reanudar();
	}
	else if (ahora - d->ultimoMovimiento > 80) {       // 80 ms de gracia
		d->motor.pausar();
	}
}

void CSonido::musica(bool sonar) {
	if (!d->iniciado) return;
	if (sonar) d->musica.reanudar();
	else d->musica.pausar();
}

void CSonido::musicaMenu(bool sonar) {
	if (!d->iniciado) return;
	if (sonar) d->musicaMenu.reanudar();
	else d->musicaMenu.pausar();
}

static float limitar(float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); }

void CSonido::setVolumenMusica(float v) {
	d->volMusica = limitar(v);
	d->musica.setVolumen(d->volMusica);
	d->musicaMenu.setVolumen(d->volMusica);
}

void CSonido::setVolumenEfectos(float v) {
	d->volEfectos = limitar(v);
	d->efDisparo.setVolumen(d->volEfectos);
	d->efExplosion.setVolumen(d->volEfectos);
	d->efGolpe.setVolumen(d->volEfectos);
	d->motor.setVolumen(d->volEfectos);
	d->efMenu.setVolumen(d->volEfectos);
}

float CSonido::getVolumenMusica() const { return d->volMusica; }
float CSonido::getVolumenEfectos() const { return d->volEfectos; }

#pragma managed(pop)