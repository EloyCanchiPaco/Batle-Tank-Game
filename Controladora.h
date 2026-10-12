#pragma once
#include "Mapa.h";
#include "Jugador.h"
#include "Enemigo.h"
#include "ColeccionEnemigos.h"
#include "EnemigoFactory.h"

class CControladora {
private:
	CMapa* mapa;
	CJugador* jugador;
	CColeccionEnemigos* Enemigos;
	CBloque* trofeo;
public:
	CControladora();
	~CControladora();

	void Inicializar();
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo);
	void DibujarTanques(Graphics^ g, Bitmap^ bmpTanques);
};
