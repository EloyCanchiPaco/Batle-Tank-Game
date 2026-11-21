#pragma once
#pragma once
#include "Controladora.h"

class CJuegoFacade {
private:
	CControladora* controladora;   // subsistema que coordina el juego

public:
	CJuegoFacade();
	~CJuegoFacade();

	void Iniciar();
	void Dibujar(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques,
		Bitmap^ bmpTrofeo, Bitmap^ bmpTanques, Bitmap^ bmpBala);
	void MoverJugador(EDireccion direccion);
	void MoverEnemigos();
	void Disparar();
	void ActualizarBalas();

	
};


