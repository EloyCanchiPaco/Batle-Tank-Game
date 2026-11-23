#include "Bala.h"

CBala::CBala(int xTanque, int yTanque, EDireccion direccion, bool delJugador) {
	// nace en el centro del tanque
	this->x = xTanque + (ANCHO_IMAGEN - TAM) / 2;
	this->y = yTanque + (ALTO_IMAGEN - TAM) / 2;
	this->direccion = direccion;
	this->velocidad = 16;
	this->activa = true;
	this->delJugador = delJugador;
}

void CBala::mover() {
	switch (direccion) {
	case EDireccion::arriba:    y -= velocidad; break;
	case EDireccion::abajo:     y += velocidad; break;
	case EDireccion::izquierda: x -= velocidad; break;
	case EDireccion::derecha:   x += velocidad; break;
	default: break;
	}
}

// bala.png: 4 sprites de 32x32 en una fila -> arriba, abajo, izquierda, derecha
void CBala::dibujar(Graphics^ g, Bitmap^ bmpBala) {
	int columna = (int)direccion - 1;          // arriba=0, abajo=1, izquierda=2, derecha=3
	if (columna < 0) columna = 0;
	int tam = bmpBala->Height;                 // alto real de cada sprite (4 sprites en una fila)
	Rectangle origen = Rectangle(columna * tam, 0, tam, tam);
	Rectangle destino = Rectangle(x, y, TAM, TAM);
	g->DrawImage(bmpBala, destino, origen, GraphicsUnit::Pixel);
}

int CBala::getX() { return x; }
int CBala::getY() { return y; }
EDireccion CBala::getDireccion() { return direccion; }
bool CBala::estaActiva() { return activa; }
void CBala::desactivar() { activa = false; }
bool CBala::esDelJugador() { return delJugador; }