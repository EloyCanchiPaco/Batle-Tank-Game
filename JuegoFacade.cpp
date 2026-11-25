#include "JuegoFacade.h"

CJuegoFacade::CJuegoFacade() : controladora(nullptr) {
	// la partida se crea en Iniciar(), cuando el estado "jugando" empieza
}

CJuegoFacade::~CJuegoFacade() {
	delete controladora;
}

void CJuegoFacade::Iniciar(int nivel) {
	delete controladora;                      // descarta la partida anterior (si hubo)
	controladora = new CControladora();
	controladora->Inicializar(nivel);
}

bool CJuegoFacade::HaPerdido() {
	return controladora != nullptr && controladora->haPerdido();
}

// Una sola llamada dibuja mapa y tanques, en el orden correcto
void CJuegoFacade::Dibujar(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques,
	Bitmap^ bmpTrofeo, Bitmap^ bmpTanques, Bitmap^ bmpBala, Bitmap^ bmpExplosion, Bitmap^ bmpTrofeoD) {
	if (controladora == nullptr) return;
	controladora->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo, bmpTrofeoD);
	controladora->DibujarTanques(g, bmpTanques);
	controladora->DibujarBalas(g, bmpBala, bmpExplosion);
	controladora->DibujarHUD(g);
}

// Una sola llamada: cambia la direcci n y mueve
void CJuegoFacade::MoverJugador(EDireccion direccion) {
	controladora->setDireccion(direccion);
	controladora->moverJugador();
}

// Ninguna tecla presionada
void CJuegoFacade::DetenerJugador() {
	controladora->detenerJugador();
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