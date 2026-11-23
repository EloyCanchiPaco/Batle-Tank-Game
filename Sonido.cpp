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

// Archivos esperados (en la carpeta "sonidos\" junto a "imagenes\"):
//   disparo.wav  explosion.wav  golpe.wav  motor.wav  inicio.wav
// Si algun sonido falla, el motivo aparece en la ventana "Salida" de Visual Studio.
//
// - Los efectos (disparo, explosion, golpe, inicio) los reproduce un HILO APARTE:
//   el juego solo anota la orden en una cola y sigue.
// - El motor usa waveOut con el dispositivo siempre abierto: solo se pausa/reanuda.

enum Orden { ORD_DISPARO, ORD_EXPLOSION, ORD_GOLPE, ORD_INICIO };

static bool iniciado = false;

static const int VOCES_DISPARO = 3;
static const int VOCES_EXPLOSION = 3;
static const int VOCES_GOLPE = 2;

// ---------- Cola de ordenes entre el juego y el hilo de sonido ----------
static CRITICAL_SECTION candado;
static std::vector<int> cola;
static HANDLE evento = NULL;
static HANDLE hilo = NULL;
static volatile bool terminar = false;

static void encolar(int orden) {
	if (!iniciado) return;
	EnterCriticalSection(&candado);
	if (cola.size() < 16) cola.push_back(orden);      // limite para que no se acumulen
	LeaveCriticalSection(&candado);
	SetEvent(evento);                                  // despierta al hilo
}

// ---------- Motor: waveOut en bucle, dispositivo siempre abierto ----------
static std::vector<char> datosMotor;
static HWAVEOUT hWave = NULL;
static WAVEHDR cabecera;
static bool motorCargado = false;
static bool motorCorriendo = false;
static ULONGLONG ultimoMovimiento = 0;

static void abrirMotor() {
	std::ifstream f("sonidos\\motor.wav", std::ios::binary);
	if (!f) {
		OutputDebugStringA("motor.wav: no se encontro en la carpeta sonidos\n");
		return;
	}
	datosMotor.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	size_t n = datosMotor.size();
	if (n < 44) {
		OutputDebugStringA("motor.wav: archivo vacio o invalido\n");
		return;
	}

	const char* p = datosMotor.data();
	WAVEFORMATEX fmt;
	ZeroMemory(&fmt, sizeof(fmt));
	const char* datos = nullptr;
	DWORD tam = 0;

	// buscar los bloques "fmt " y "data" del WAV
	size_t pos = 12;
	while (pos + 8 <= n) {
		DWORD sz = *(const DWORD*)(p + pos + 4);
		if (memcmp(p + pos, "fmt ", 4) == 0 && pos + 8 + 16 <= n) {
			memcpy(&fmt, p + pos + 8, 16);
			fmt.cbSize = 0;
		}
		else if (memcmp(p + pos, "data", 4) == 0) {
			datos = p + pos + 8;
			tam = (DWORD)((pos + 8 + sz <= n) ? sz : n - pos - 8);
			break;
		}
		pos += 8 + sz + (sz & 1);
	}
	if (!datos || fmt.nChannels == 0) {
		OutputDebugStringA("motor.wav: formato no soportado (use PCM 16 bits)\n");
		return;
	}

	if (waveOutOpen(&hWave, WAVE_MAPPER, &fmt, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
		OutputDebugStringA("motor.wav: no se pudo abrir el dispositivo de audio\n");
		return;
	}

	ZeroMemory(&cabecera, sizeof(cabecera));
	cabecera.lpData = (LPSTR)datos;
	cabecera.dwBufferLength = tam;
	cabecera.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
	cabecera.dwLoops = 0x7FFFFFFF;                     // bucle "infinito"
	waveOutPrepareHeader(hWave, &cabecera, sizeof(WAVEHDR));
	waveOutPause(hWave);                               // empieza en pausa
	waveOutWrite(hWave, &cabecera, sizeof(WAVEHDR));
	motorCargado = true;
}

static void cerrarMotor() {
	if (!motorCargado) return;
	waveOutReset(hWave);
	waveOutUnprepareHeader(hWave, &cabecera, sizeof(WAVEHDR));
	waveOutClose(hWave);
	hWave = NULL;
	motorCargado = false;
	motorCorriendo = false;
}

// ---------- Efectos por MCI (solo los usa el hilo de sonido) ----------
static std::string alias(const char* nombre, int voz) {
	return std::string(nombre) + std::to_string(voz);
}

static void mci(const std::string& comando) {
	MCIERROR err = mciSendStringA(comando.c_str(), NULL, 0, NULL);
	if (err) {
		char msg[256];
		mciGetErrorStringA(err, msg, 256);
		OutputDebugStringA((comando + " -> " + msg + "\n").c_str());
	}
}

static void abrir(const char* nombre, int voces) {
	for (int v = 0; v < voces; v++)
		mci(std::string("open \"sonidos\\") + nombre + ".wav\" type waveaudio alias " + alias(nombre, v));
}

static void reproducir(const char* nombre, int voces, int& siguiente) {
	std::string a = alias(nombre, siguiente);
	siguiente = (siguiente + 1) % voces;               // la proxima vez usa otra voz
	mci("play " + a + " from 0");
}

// ---------- Hilo de sonido ----------
static DWORD WINAPI hiloSonido(LPVOID) {
	int sigDisparo = 0, sigExplosion = 0, sigGolpe = 0;

	// abre los sonidos en este mismo hilo
	abrir("disparo", VOCES_DISPARO);
	abrir("explosion", VOCES_EXPLOSION);
	abrir("golpe", VOCES_GOLPE);
	abrir("inicio", 1);

	while (true) {
		WaitForSingleObject(evento, INFINITE);
		if (terminar) break;

		std::vector<int> pendientes;
		EnterCriticalSection(&candado);
		pendientes.swap(cola);
		LeaveCriticalSection(&candado);

		for (size_t i = 0; i < pendientes.size(); i++) {
			switch (pendientes[i]) {
			case ORD_DISPARO:   reproducir("disparo", VOCES_DISPARO, sigDisparo); break;
			case ORD_EXPLOSION: reproducir("explosion", VOCES_EXPLOSION, sigExplosion); break;
			case ORD_GOLPE:     reproducir("golpe", VOCES_GOLPE, sigGolpe); break;
			case ORD_INICIO:    mci("play inicio0 from 0"); break;
			}
		}
	}
	mci("close all");
	return 0;
}

// ---------- Interfaz publica ----------
void Sonido::iniciar() {
	if (iniciado) return;
	InitializeCriticalSection(&candado);
	evento = CreateEvent(NULL, FALSE, FALSE, NULL);
	terminar = false;
	cola.clear();
	abrirMotor();
	iniciado = true;
	hilo = CreateThread(NULL, 0, hiloSonido, NULL, 0, NULL);
}

void Sonido::cerrar() {
	if (!iniciado) return;
	iniciado = false;
	cerrarMotor();
	terminar = true;
	SetEvent(evento);
	WaitForSingleObject(hilo, 2000);                   // espera a que el hilo termine
	CloseHandle(hilo);
	hilo = NULL;
	CloseHandle(evento);
	evento = NULL;
	DeleteCriticalSection(&candado);
}

void Sonido::disparo() { encolar(ORD_DISPARO); }
void Sonido::explosion() { encolar(ORD_EXPLOSION); }
void Sonido::golpe() { encolar(ORD_GOLPE); }
void Sonido::inicioNivel() { encolar(ORD_INICIO); }

void Sonido::motor(bool moviendose) {
	if (!iniciado || !motorCargado) return;
	ULONGLONG ahora = GetTickCount64();

	if (moviendose) {
		ultimoMovimiento = ahora;
		if (!motorCorriendo) {
			waveOutRestart(hWave);
			motorCorriendo = true;
		}
	}
	else if (motorCorriendo && ahora - ultimoMovimiento > 80) {   // 80 ms de gracia
		waveOutPause(hWave);
		motorCorriendo = false;
	}
}

#pragma managed(pop)