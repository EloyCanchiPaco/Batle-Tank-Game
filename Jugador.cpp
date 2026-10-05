#include "Jugador.h"

void CJugador::setNombre(std::string nombre) {
	this->nombre = nombre;
}
void CJugador::setPuntuacion(int puntuacion) {
	this->puntuacion = puntuacion;
}
std::string CJugador::getNombre() {
	return this->nombre;
}
int CJugador::getPuntuacion() {
	return this->puntuacion;
}

void CJugador::dibujar(Graphics^ g, Bitmap^ bmpTanques) {
	int fila = 0;
	switch (getDireccion()) {
		case EDireccion::arriba:	fila = 0; break;
		case EDireccion::derecha:		fila = 1; break;
		case EDireccion::abajo:		fila = 2; break;
		case EDireccion::izquierda:	fila = 3; break;
		default:					fila = 0; break;
	}
	Rectangle origen = Rectangle(0 * ANCHO_IMAGEN, fila * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
	Rectangle zoom = Rectangle(getX(), getY(), ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
	g->DrawImage(bmpTanques, zoom, origen, GraphicsUnit::Pixel);
};
CJugador::CJugador(short id, int fila, int columna, int vidas) : 
	CTanque(id, columna* ANCHO_IMAGEN, fila* ALTO_IMAGEN, vidas) {
	this->nombre = "Eloy";
	this->puntuacion = 0;
	this->setDireccion(EDireccion::arriba);
}

void CJugador::mover() {

}

