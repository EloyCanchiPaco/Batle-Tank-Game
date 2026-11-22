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
public:
	CControladora();
	~CControladora();

	void Inicializar();
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo);
	void DibujarTanques(Graphics^ g, Bitmap^ bmpTanques);
	void setDireccion(EDireccion direccion);
	void moverJugador();
	void moverEnemigos();
	void jugadorDispara();
	void actualizarBalas();
	void DibujarBalas(Graphics^ g, Bitmap^ bmpBala);
	void DibujarHUD(Graphics^ g);
	void detenerJugador();

};
