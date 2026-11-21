#include "JuegoFacade.h"

CJuegoFacade::CJuegoFacade() {
	controladora = new CControladora();
}

CJuegoFacade::~CJuegoFacade() {
	delete controladora;
}

void CJuegoFacade::Iniciar() {
	controladora->Inicializar();
}

// Una sola llamada dibuja mapa y tanques, en el orden correcto
void CJuegoFacade::Dibujar(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques,
	Bitmap^ bmpTrofeo, Bitmap^ bmpTanques, Bitmap^ bmpBala) {
	controladora->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo);
	controladora->DibujarTanques(g, bmpTanques);
	controladora->DibujarBalas(g, bmpBala);
}

// Una sola llamada: cambia la dirección y mueve
void CJuegoFacade::MoverJugador(EDireccion direccion) {
	controladora->setDireccion(direccion);
	controladora->moverJugador();
}
void CJuegoFacade::MoverEnemigos() {
	controladora->moverEnemigos();
}
// Una sola llamada: dispara y actualiza balas
void CJuegoFacade::Disparar() {
	controladora->jugadorDispara();
}

void CJuegoFacade::ActualizarBalas() {
	controladora->actualizarBalas();
}

