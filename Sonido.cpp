#include "Sonido.h"
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#pragma comment(lib, "winmm.lib")

// Archivos esperados (en la carpeta "sonidos\" junto a "imagenes\"):
//   disparo.wav  explosion.wav  golpe.wav  motor.wav  inicio.wav

static bool iniciado = false;
static int sigDisparo = 0, sigExplosion = 0, sigGolpe = 0;

static const int VOCES_DISPARO = 3;
static const int VOCES_EXPLOSION = 3;
static const int VOCES_GOLPE = 2;

// ---------- Motor: waveOut con bucle, siempre abierto ----------
static std::vector<char> datosMotor;
static HWAVEOUT hWave = NULL;
static WAVEHDR cabecera;
static bool motorAbierto = false;
static bool motorCorriendo = false;

static void abrirMotor() {
	std::ifstream f("sonidos\\motor.wav", std::ios::binary);
	datosMotor.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	size_t n = datosMotor.size();
	if (n < 44) return;

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
			memcpy(&fmt, p + pos + 8, 16);      // formato PCM basico
			fmt.cbSize = 0;
		}
		else if (memcmp(p + pos, "data", 4) == 0) {
			datos = p + pos + 8;
			tam = (DWORD)((pos + 8 + sz <= n) ? sz : n - pos - 8);
			break;
		}
		pos += 8 + sz + (sz & 1);
	}
	if (!datos || fmt.nChannels == 0) return;

	if (waveOutOpen(&hWave, WAVE_MAPPER, &fmt, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) return;

	ZeroMemory(&cabecera, sizeof(cabecera));
	cabecera.lpData = (LPSTR)datos;
	cabecera.dwBufferLength = tam;
	cabecera.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
	cabecera.dwLoops = 0x7FFFFFFF;               // bucle "infinito"
	waveOutPrepareHeader(hWave, &cabecera, sizeof(WAVEHDR));
	waveOutPause(hWave);                          // empieza en pausa
	waveOutWrite(hWave, &cabecera, sizeof(WAVEHDR));
	motorAbierto = true;
}

static void cerrarMotor() {
	if (!motorAbierto) return;
	waveOutReset(hWave);
	waveOutUnprepareHeader(hWave, &cabecera, sizeof(WAVEHDR));
	waveOutClose(hWave);
	hWave = NULL;
	motorAbierto = false;
	motorCorriendo = false;
}

// ---------- Efectos por MCI ----------
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
	if (!iniciado) return;
	std::string a = alias(nombre, siguiente);
	siguiente = (siguiente + 1) % voces;
	mci("play " + a + " from 0");
}

void Sonido::iniciar() {
	if (iniciado) return;
	abrirMotor();
	abrir("disparo", VOCES_DISPARO);
	abrir("explosion", VOCES_EXPLOSION);
	abrir("golpe", VOCES_GOLPE);
	abrir("inicio", 1);
	iniciado = true;
}


void Sonido::cerrar() {
	if (!iniciado) return;
	cerrarMotor();
	mci("close all");
	iniciado = false;
}
void Sonido::disparo() { reproducir("disparo", VOCES_DISPARO, sigDisparo); }
void Sonido::explosion() { reproducir("explosion", VOCES_EXPLOSION, sigExplosion); }
void Sonido::golpe() { reproducir("golpe", VOCES_GOLPE, sigGolpe); }

void Sonido::inicioNivel() {
	if (!iniciado) return;
	mci("play inicio0 from 0");
}

void Sonido::motor(bool moviendose) {
	if (!iniciado || !motorAbierto) return;
	if (moviendose && !motorCorriendo) {
		waveOutRestart(hWave);
		motorCorriendo = true;
	}
	else if (!moviendose && motorCorriendo) {
		waveOutPause(hWave);
		motorCorriendo = false;
	}
}