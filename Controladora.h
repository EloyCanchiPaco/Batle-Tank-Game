#pragma once
#include "Mapa.h";
#include "Jugador.h";
#include "Enemigo.h";

class CControladora {
private:
	CMapa* mapa;
	CJugador* jugador;
	CEnemigo* enemigos;
public:
	CControladora();
	~CControladora() {};

	void Inicializar();
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo);
	void DibujarTanques(Graphics^ g, Bitmap^ bmpTanques);
};
