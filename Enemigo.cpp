#include "Enemigo.h"

CEnemigo::CEnemigo(short id, short tipo, int fila, int columna, int vidas) : 
	CTanque(id, columna* ANCHO_IMAGEN, fila* ALTO_IMAGEN, vidas) {
	this->tipo = (ETipoTanque)tipo;
	this->setDireccion(EDireccion::abajo);
}

void CEnemigo::setTipo(ETipoTanque tipo) {
	this->tipo = tipo;
}
ETipoTanque CEnemigo::getTipo() {
	return this->tipo;
}

void CEnemigo::dibujar(Graphics^ g, Bitmap^ bmpTanques) {
	int fila = 0;
	switch (getDireccion()) {
		case EDireccion::arriba:	fila = 0; break;
		case EDireccion::derecha:	fila = 1; break;
		case EDireccion::abajo:		fila = 2; break;
		case EDireccion::izquierda:	fila = 3; break;
		default:					fila = 0; break;
	}
	int filaBase = 8 + tipo;
	int columnaBase = 0;

	// 2. Si es el tipo5 (valor 16 en el enum), ajustamos a la columna 6 y fila 20
	if (tipo == ETipoTanque::tipo5) {
		columnaBase = 6;
		filaBase = 20; // Fila 20 en la columna 6
	}
	Rectangle origen = Rectangle(columnaBase * ANCHO_IMAGEN, (filaBase + fila) * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
	Rectangle zoom = Rectangle(getX(), getY(), ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);

	g->DrawImage(bmpTanques, zoom, origen, GraphicsUnit::Pixel);

};