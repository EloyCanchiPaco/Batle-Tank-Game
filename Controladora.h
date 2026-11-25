#pragma once
#include "Mapa.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "ColeccionEnemigos.h"
#include "EnemigoFactory.h"
#include "ColeccionBalas.h"
#include "Colision.h"
#include "EstadoPartida.h"
#include "PanelHUD.h"

class CControladora {
private:
	CMapa* mapa;
	CJugador* jugador;
	CColeccionEnemigos* Enemigos;
	CBloque* trofeo;
	CColeccionBalas* balas;
	ContextoColision contexto;
	Colision* cadenaJugador;
	Colision* cadenaEnemigos;
	CPanelHUD* hud;
	bool derrota = false;            // se destruyo el trofeo o el jugador se quedo sin vidas
public:
	CControladora();
	~CControladora();

	void Inicializar(int nivel);      // arma la partida del nivel indicado (1 a 5)
	bool haPerdido();
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo, Bitmap^ bmpTrofeoD);
	void DibujarTanques(Graphics^ g, Bitmap^ bmpTanques);
	void setDireccion(EDireccion direccion);
	void moverJugador();
	void moverEnemigos();
	void jugadorDispara();
	void DibujarBalas(Graphics^ g, Bitmap^ bmpBala, Bitmap^ bmpExplosion);
	void DibujarHUD(Graphics^ g);
	void actualizarBalas();
	void detenerJugador();
	void recogerDisparos();
	void reproducirSonidos();
	void actualizarEstado(int enemigosAntes);

};
