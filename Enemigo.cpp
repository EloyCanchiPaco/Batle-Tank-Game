#include "Enemigo.h"
#include "Bloque.h"
#include "Mapa.h"
CEnemigo::CEnemigo(short id, short tipo, int fila, int columna, int vidas) : 
	CTanque(id, columna* ANCHO_IMAGEN, fila* ALTO_IMAGEN, vidas) {
	this->tipo = (ETipoTanque)tipo;
	this->setDireccion(EDireccion::abajo);
	this->imagenX = 0;    
}
//descontructor
CEnemigo::~CEnemigo() {
	delete aleatorio;
	//delete ajugador;       // TODO companeros
	//delete atrofeo;
	//delete mayorVelocidad;
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
	Rectangle origen = Rectangle((columnaBase+imagenX) * ANCHO_IMAGEN, (filaBase + fila) * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
	Rectangle zoom = Rectangle(getX(), getY(), ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);

	g->DrawImage(bmpTanques, zoom, origen, GraphicsUnit::Pixel);

};
void CEnemigo::animar() {
	if (this->imagenX >= 0 && this->imagenX < 2) {
		this->imagenX++;
	}
	if (this->imagenX == 2) {
		while (this->imagenX != 0) {
			this->imagenX--;
		}
	}
}
//
void CEnemigo::setEstrategia(Estrategia* estrategia) {
	delete aleatorio; aleatorio = nullptr;
	//delete ajugador;       ajugador = nullptr;        // TODO companeros
	//delete atrofeo;        atrofeo = nullptr;
	//delete mayorVelocidad; mayorVelocidad = nullptr;

	if ((aleatorio = dynamic_cast<EstrategiaAleatoria*>(estrategia))) return;
	//if ((ajugador = dynamic_cast<EstrategiaJugador*>(estrategia))) return;
	//if ((atrofeo = dynamic_cast<EstrategiaTrofeo*>(estrategia))) return;
	//if ((mayorVelocidad = dynamic_cast<EstrategiaVelocidad*>(estrategia))) return;
	delete estrategia;
}

void CEnemigo::mover(CMapa* mapa) {
	if (aleatorio) aleatorio->mover(this, mapa);
	//else if (ajugador)        ajugador->mover(this, mapa);   // TODO companeros
	//else if (atrofeo)         atrofeo->mover(this, mapa);
	//else if (mayorVelocidad)  mayorVelocidad->mover(this, mapa);
}

bool CEnemigo::puedeAvanzar(CMapa* mapa) {
	int nx = getX(), ny = getY();
	short v = getVelocidad();
	switch (getDireccion()) {
	case EDireccion::arriba:    ny -= v; break;
	case EDireccion::abajo:     ny += v; break;
	case EDireccion::izquierda: nx -= v; break;
	case EDireccion::derecha:   nx += v; break;
	default: return false;
	}
	if (nx < 0 || ny < 0 ||
		nx + ANCHO_IMAGEN > MAX_COLUMNAS * ANCHO_IMAGEN ||
		ny + ALTO_IMAGEN  > MAX_FILAS * ALTO_IMAGEN) return false;

	int c1 = nx / ANCHO_IMAGEN, c2 = (nx + ANCHO_IMAGEN - 1) / ANCHO_IMAGEN;
	int f1 = ny / ALTO_IMAGEN, f2 = (ny + ALTO_IMAGEN - 1) / ALTO_IMAGEN;
	for (int f = f1; f <= f2; f++)
		for (int c = c1; c <= c2; c++) {
			CBloque* b = mapa->getBloque(f, c);
			if (b == nullptr || b->getTipo() != ETipoBloque::piso) return false;
		}
	return true;
}

void CEnemigo::alinearACelda() {
	setX(((getX() + ANCHO_IMAGEN / 2) / ANCHO_IMAGEN) * ANCHO_IMAGEN);
	setY(((getY() + ALTO_IMAGEN / 2) / ALTO_IMAGEN) * ALTO_IMAGEN);
}
