#include "Tanque.h"
#include "iostream"
#include <algorithm>
#include <cstdlib>
#include "Colision.h"

CTanque::CTanque(short id, int posX, int posY, int vidas) {
	this->id = id;
	this->vidas = vidas;
	this->velocidad = 10;
	this->posX = posX;
	this->posY = posY;
}

CTanque::~CTanque() {}

void CTanque::setId(short id) {
	this->id = id;
}
void CTanque::setVidas(short vidas) {
	this->vidas = vidas;
}
void CTanque::setDireccion(EDireccion direccion) {
	this->direccion = direccion;
}
void CTanque::setVelocidad(short velocidad) {
	this->velocidad = velocidad;
}
void CTanque::setX(int posX) {
	this->posX = posX;
}
void CTanque::setY(int posY) {
	this->posY = posY;
}

short CTanque::getId() {
	return this->id;
}
short CTanque::getVidas() {
	return this->vidas;
}
EDireccion CTanque::getDireccion() {
	return this->direccion;
}
short CTanque::getVelocidad() {
	return this->velocidad;
}
int CTanque::getX() {
	return this->posX;
}
int CTanque::getY() {
	return this->posY;
}
//funcion movimiento usando direccion
void CTanque::mover() {

	switch (direccion) {
	case EDireccion::arriba:    posY -= velocidad; break;
	case EDireccion::abajo:    posY += velocidad; break;
	case EDireccion::izquierda:    posX -= velocidad; break;
	case EDireccion::derecha:    posX += velocidad; break;
	default: break;
	}
}
//funciones para las balas 
void CTanque::disparar() {
	if (enfriamiento == 0) quiereDisparar = true;
}

CBala* CTanque::sacarBala(bool delJugador, int espera) {
	if (!quiereDisparar) return nullptr;
	quiereDisparar = false;
	if (direccion == EDireccion::ninguno) return nullptr;
	enfriamiento = espera;
	return new CBala(posX, posY, direccion, delJugador);
}

void CTanque::actualizarDisparo() {
	if (enfriamiento > 0) enfriamiento--;
}
void CTanque::setColision(Colision* cadena, ContextoColision* ctx) {
	this->colision = cadena;
	this->contexto = ctx;
}

// ¿puede dar el siguiente paso en su direccion actual?
bool CTanque::puedeAvanzar() {
	std::cout << "pos(" << posX << "," << posY << ") dir=" << (int)direccion
		<< " ANCHO=" << ANCHO_IMAGEN << " ALTO=" << ALTO_IMAGEN
		<< " vel=" << velocidad << std::endl;
	int destX = posX, destY = posY;
	switch (direccion) {
	case EDireccion::arriba:    destY -= velocidad; break;
	case EDireccion::abajo:     destY += velocidad; break;
	case EDireccion::izquierda: destX -= velocidad; break;
	case EDireccion::derecha:   destX += velocidad; break;
	default: return false;
	}
	if (colision == nullptr) return true;
	bool ok = colision == nullptr || colision->permite(this, destX, destY, contexto);
	std::cout << "dest(" << destX << "," << destY << ") permite=" << ok << std::endl;
	return ok;
	return colision->permite(this, destX, destY, contexto);

}

void CTanque::alinearACelda() {
	int x0 = (posX / ANCHO_IMAGEN) * ANCHO_IMAGEN;       // celda de la izquierda
	int y0 = (posY / ALTO_IMAGEN) * ALTO_IMAGEN;         // celda de arriba
	int x1 = x0 + ANCHO_IMAGEN, y1 = y0 + ALTO_IMAGEN;

	int cx = (posX - x0 <= x1 - posX) ? x0 : x1;         // la mas cercana en X
	int cy = (posY - y0 <= y1 - posY) ? y0 : y1;         // la mas cercana en Y
	int ox = (cx == x0) ? x1 : x0;                       // la otra en X
	int oy = (cy == y0) ? y1 : y0;                       // la otra en Y

	// en orden: la mas cercana primero y luego las demas
	int px[4] = { cx, ox, cx, ox };
	int py[4] = { cy, cy, oy, oy };
	for (int i = 0; i < 4; i++) {
		if (colision == nullptr || colision->permite(this, px[i], py[i], contexto)) {
			posX = px[i];
			posY = py[i];
			return;
		}
	}
	// si ninguna esta libre, se queda donde esta
}
// Antes de avanzar en la direccion d, el tanque debe estar centrado en el eje perpendicular.
// Si no lo esta, da un paso hacia el centro de su celda y devuelve false.
// Devuelve true si ya esta centrado (o si no puede centrarse), y entonces puede avanzar.
bool CTanque::centrarParaAvanzar(EDireccion d) {
	bool vertical = (d == EDireccion::arriba || d == EDireccion::abajo);
	int medio = (vertical ? ANCHO_IMAGEN : ALTO_IMAGEN) / 2;   // paso de alineacion: media celda
	int actual = vertical ? posX : posY;

	// Los dos multiplos de media celda mas cercanos (el mas cercano primero)
	int bajo = (actual / medio) * medio;
	int alto = bajo + medio;
	int cercano = (actual - bajo <= alto - actual) ? bajo : alto;
	int otro = (cercano == bajo) ? alto : bajo;
	int candidatos[2] = { cercano, otro };

	for (int i = 0; i < 2; i++) {
		int destino = candidatos[i];

		// el tanque, ya alineado en ese destino, ¿cabe?
		int fx = vertical ? destino : posX;
		int fy = vertical ? posY : destino;
		if (colision != nullptr && !colision->permite(this, fx, fy, contexto)) continue;

		if (actual == destino) return true;   // ya esta alineado y libre: puede avanzar

		// da un paso hacia ese destino
		int paso = std::min((int)velocidad * 2, abs(destino - actual)) * (destino > actual ? 1 : -1);
		int nx = posX + (vertical ? paso : 0);
		int ny = posY + (vertical ? 0 : paso);

		if (colision == nullptr || colision->permite(this, nx, ny, contexto)) {
			posX = nx;
			posY = ny;
			return false;                     // se esta alineando, falta seguir
		}
	}
	return true;                              // no puede alinearse: que intente avanzar tal cual
}