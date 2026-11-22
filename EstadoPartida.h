#pragma once
#include "Observador.h"

// ===== PATRON SINGLETON + SUJETO DEL OBSERVER =====
// Una sola instancia con los datos de la partida: score, nivel, vidas y enemigos restantes.
// Cualquiera la consulta con EstadoPartida::getInstancia().
// Cuando un dato cambia, avisa a sus observadores (el PanelHUD).
class EstadoPartida : public Sujeto {
	int puntuacion = 0;
	int nivel = 1;
	int vidas = 3;
	int enemigosRestantes = 0;

	EstadoPartida() {}                                   // privado: nadie crea otra
public:
	EstadoPartida(const EstadoPartida&) = delete;        // y no se puede copiar
	EstadoPartida& operator=(const EstadoPartida&) = delete;

	static EstadoPartida& getInstancia();

	int getPuntuacion() { return puntuacion; }
	int getNivel() { return nivel; }
	int getVidas() { return vidas; }
	int getEnemigosRestantes() { return enemigosRestantes; }

	// Empieza una partida nueva: score en 0 y datos iniciales
	void nuevaPartida(int nivel, int vidas, int enemigosRestantes);

	// Cada setter solo notifica si el valor realmente cambio
	void sumarPuntos(int puntos);
	void setNivel(int nivel);
	void setVidas(int vidas);
	void setEnemigosRestantes(int cantidad);
};