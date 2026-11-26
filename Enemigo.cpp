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
	delete ajugador;     
	delete atrofeo;
	delete mayorVelocidad;
	delete combinada;
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
	delete ajugador;       ajugador = nullptr;        
	delete atrofeo;        atrofeo = nullptr;
	delete mayorVelocidad; mayorVelocidad = nullptr;
	delete combinada;  combinada = nullptr;

	if ((aleatorio = dynamic_cast<EstrategiaAleatoria*>(estrategia))) return;
	if ((ajugador = dynamic_cast<EstrategiaJugador*>(estrategia))) return;
	if ((atrofeo = dynamic_cast<EstrategiaTrofeo*>(estrategia))) return;
	if ((mayorVelocidad = dynamic_cast<EstrategiaVelocidad*>(estrategia))) return;
	if ((combinada = dynamic_cast<EstrategiaCombinada*>(estrategia))) return;
	delete estrategia;
}

void CEnemigo::mover(CMapa* mapa) {
	if (aleatorio) aleatorio->mover(this, mapa);
	else if (ajugador)        ajugador->mover(this, mapa);   
	else if (atrofeo)         atrofeo->mover(this, mapa);
	else if (mayorVelocidad)  mayorVelocidad->mover(this, mapa);
	else if (combinada)  combinada->mover(this, mapa);
	if (rand() % 30 == 0)           // disparo al azar, valido para todas las estrategias
		disparar();
}


