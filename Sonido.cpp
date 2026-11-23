#include "Sonido.h"

// Todo este archivo es codigo nativo (necesario para usar hilos de Windows en un proyecto C++/CLI)
#pragma managed(push, off)
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstdio>
#pragma comment(lib, "winmm.lib")

// Archivos esperados (en la carpeta "sonidos\" junto a "imagenes\"):
//   disparo.wav  explosion.wav  golpe.wav  motor.wav  inicio.wav
// Si algun sonido falla, el motivo aparece en la ventana "Salida" de Visual Studio.
//
// Los efectos (disparo, explosion, golpe, inicio) los reproduce un HILO APARTE:
// el juego solo anota la orden en una cola y sigue, asi no se traba esperando al audio.

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
	printf("sonido: encolar %d (iniciado=%d)\n", orden, (int)iniciado);
	if (!iniciado) return;
	EnterCriticalSection(&candado);
	if (cola.size() < 16) cola.push_back(orden);      // limite para que no se acumulen
	LeaveCriticalSection(&candado);
	SetEvent(evento);                                  // despierta al hilo
}

// ---------- Motor: PlaySound en bucle (el archivo se guarda en memoria) ----------
static std::vector<char> datosMotor;
static bool motorCargado = false;
static bool motorCorriendo = false;

static void abrirMotor() {
	std::ifstream f("sonidos\\motor.wav", std::ios::binary);
	if (!f) {
		OutputDebugStringA("motor.wav: no se encontro en la carpeta sonidos\n");
		return;
	}
	datosMotor.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	motorCargado = datosMotor.size() > 44;
	if (!motorCargado) OutputDebugStringA("motor.wav: archivo vacio o invalido\n");
}

static void cerrarMotor() {
	PlaySoundA(NULL, NULL, 0);
	motorCorriendo = false;
	motorCargado = false;
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
		printf("MCI: %s -> %s\n", comando.c_str(), msg);
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
	printf("hilo de sonido: iniciado\n");           
	int sigDisparo = 0, sigExplosion = 0, sigGolpe = 0;

	// abre los sonidos en este mismo hilo
	abrir("disparo", VOCES_DISPARO);
	abrir("explosion", VOCES_EXPLOSION);
	abrir("golpe", VOCES_GOLPE);
	abrir("inicio", 1);
	OutputDebugStringA("hilo de sonido: sonidos abiertos\n");    

	while (true) {
		WaitForSingleObject(evento, INFINITE);
		if (terminar) break;

		std::vector<int> pendientes;
		EnterCriticalSection(&candado);
		pendientes.swap(cola);
		LeaveCriticalSection(&candado);

		for (size_t i = 0; i < pendientes.size(); i++) {
				printf("hilo de sonido: orden %d\n", pendientes[i]);
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
	if (moviendose && !motorCorriendo) {
		if (!PlaySoundA(datosMotor.data(), NULL, SND_MEMORY | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
			OutputDebugStringA("motor.wav: PlaySound no pudo reproducirlo (formato no soportado?)\n");
		motorCorriendo = true;
	}
	else if (!moviendose && motorCorriendo) {
		PlaySoundA(NULL, NULL, 0);
		motorCorriendo = false;
	}
}

#pragma managed(pop)