#pragma once
#include "Comun.h"
using namespace System::Drawing;

class CBala {
	int x, y;                 // esquina superior izquierda
	EDireccion direccion;
	short velocidad;
	bool activa;
	bool delJugador;          // true: la disparo el jugador; false: un enemigo
public:
	static const int TAM = 20;

	// (xTanque, yTanque) es la esquina del tanque que dispara
	CBala(int xTanque, int yTanque, EDireccion direccion, bool delJugador);
	void mover();
	void dibujar(Graphics^ g, Bitmap^ bmpBala);

	int getX();
	int getY();
	EDireccion getDireccion();
	bool estaActiva();
	void desactivar();
	bool esDelJugador();
};

