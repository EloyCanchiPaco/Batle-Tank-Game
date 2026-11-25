#pragma once
#pragma once
#include "Controladora.h"

class CJuegoFacade {
private:
	CControladora* controladora;   // subsistema que coordina el juego

public:
	CJuegoFacade();
	~CJuegoFacade();

	void Iniciar(int nivel);    // partida nueva del nivel indicado (descarta la anterior)
	bool HaPerdido();            // true si se destruyo el trofeo o el jugador no tiene vidas
	void Dibujar(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques,
		Bitmap^ bmpTrofeo, Bitmap^ bmpTanques, Bitmap^ bmpBala, Bitmap^ bmpExplosion, Bitmap^ bmpTrofeoD);
	void MoverJugador(EDireccion direccion);
	void DetenerJugador();
	void MoverEnemigos();
	void Disparar();
	void ActualizarBalas();


};


