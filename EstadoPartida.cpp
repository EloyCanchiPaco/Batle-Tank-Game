#include "EstadoPartida.h"

EstadoPartida& EstadoPartida::getInstancia() {
	static EstadoPartida instancia;      // se crea la primera vez y se reutiliza siempre
	return instancia;
}

void EstadoPartida::nuevaPartida(int nivel, int vidas, int enemigosRestantes) {
	this->puntuacion = 0;
	this->nivel = nivel;
	this->vidas = (vidas < 0) ? 0 : vidas;
	this->enemigosRestantes = enemigosRestantes;
	notificar();
}

void EstadoPartida::sumarPuntos(int puntos) {
	if (puntos == 0) return;
	puntuacion += puntos;
	notificar();
}

void EstadoPartida::setNivel(int nivel) {
	if (this->nivel == nivel) return;
	this->nivel = nivel;
	notificar();
}

void EstadoPartida::setVidas(int vidas) {
	if (vidas < 0) vidas = 0;
	if (this->vidas == vidas) return;
	this->vidas = vidas;
	notificar();
}

void EstadoPartida::setEnemigosRestantes(int cantidad) {
	if (enemigosRestantes == cantidad) return;
	enemigosRestantes = cantidad;
	notificar();
}