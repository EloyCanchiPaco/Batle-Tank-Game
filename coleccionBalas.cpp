#include "ColeccionBalas.h"
//existe porque hay muchas balas a la vez, y alguien tiene que guardarlas, 
// actualizarlas y borrarlas. Sigue el mismo diseño que ya usas con CColeccionEnemigos.
CColeccionBalas::~CColeccionBalas() {
	for (CBala* b : balas) delete b;
	balas.clear();
}

void CColeccionBalas::Agregar(CBala* bala) {
	balas.push_back(bala);
}

// Ladrillo: el primer impacto rompe la mitad por la que entra la bala (queda la opuesta);
// el segundo impacto lo destruye por completo.
// Se asume que mitadArriba/Abajo/Izquierda/Derecha = la mitad que QUEDA (la que se dibuja).
static void golpearLadrillo(CBloque* b, EDireccion dirBala) {
	if (b->getMitad() == EMitadBloque::mitadCompleto) {
		switch (dirBala) {
		case EDireccion::arriba:    b->setMitad(EMitadBloque::mitadArriba);    break;  // entra por abajo
		case EDireccion::abajo:     b->setMitad(EMitadBloque::mitadAbajo);     break;  // entra por arriba
		case EDireccion::izquierda: b->setMitad(EMitadBloque::mitadIzquierda); break;  // entra por la derecha
		case EDireccion::derecha:   b->setMitad(EMitadBloque::mitadDerecha);   break;  // entra por la izquierda
		default: break;
		}
	}
	else {
		b->setTipo(ETipoBloque::piso);
	}
}

// Rectangulo (en pixeles) que ocupa de verdad el bloque de la celda (f, c).
// Un ladrillo de mitad solo ocupa la mitad que queda; el resto de la celda esta vacio.
static void rectSolido(CBloque* bloque, int f, int c, int& x, int& y, int& w, int& h) {
	x = c * ANCHO_IMAGEN;  y = f * ALTO_IMAGEN;
	w = ANCHO_IMAGEN;      h = ALTO_IMAGEN;
	ETipoBloque t = bloque->getTipo();
	if (t != ETipoBloque::destruible && t != ETipoBloque::noDestruible) return;   // el trofeo ocupa la celda entera
	switch (bloque->getMitad()) {
	case EMitadBloque::mitadArriba:    h = ALTO_IMAGEN / 2; break;
	case EMitadBloque::mitadAbajo:     y += ALTO_IMAGEN / 2; h = ALTO_IMAGEN / 2; break;
	case EMitadBloque::mitadIzquierda: w = ANCHO_IMAGEN / 2; break;
	case EMitadBloque::mitadDerecha:   x += ANCHO_IMAGEN / 2; w = ANCHO_IMAGEN / 2; break;
	default: break;
	}
}

// true si dos rectangulos se superponen
static bool solapaRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

// true si la bala se superpone con el tanque
static bool solapa(CBala* b, CTanque* t) {
	return solapaRect(b->getX(), b->getY(), CBala::TAM, CBala::TAM,
		t->getX(), t->getY(), ANCHO_IMAGEN, ALTO_IMAGEN);
}

void CColeccionBalas::actualizar(CMapa* mapa, CTanque* jugador, CColeccionEnemigos* enemigos) {
	for (int i = (int)balas.size() - 1; i >= 0; i--) {
		CBala* b = balas[i];
		b->mover();

		// 1) fuera del mapa
		int c1 = b->getX() / ANCHO_IMAGEN, c2 = (b->getX() + CBala::TAM - 1) / ANCHO_IMAGEN;
		int f1 = b->getY() / ALTO_IMAGEN, f2 = (b->getY() + CBala::TAM - 1) / ALTO_IMAGEN;
		if (b->getX() < 0 || b->getY() < 0 || c2 >= MAX_COLUMNAS || f2 >= MAX_FILAS) {
			b->desactivar();
		}
		else {
			// 2) bloques que toca la caja de la bala (como maximo 2x2 celdas)
			for (int f = f1; f <= f2 && b->estaActiva(); f++) {
				for (int c = c1; c <= c2 && b->estaActiva(); c++) {
					CBloque* bloque = mapa->getBloque(f, c);
					if (bloque == nullptr) continue;
					ETipoBloque t = bloque->getTipo();
					if (t != ETipoBloque::destruible && t != ETipoBloque::noDestruible &&
						t != ETipoBloque::trofeo) continue;      // piso, agua y arbol: la bala pasa

					int sx, sy, sw, sh;
					rectSolido(bloque, f, c, sx, sy, sw, sh);
					if (!solapaRect(b->getX(), b->getY(), CBala::TAM, CBala::TAM, sx, sy, sw, sh))
						continue;                                // choca con la parte VACIA de la celda

					if (t == ETipoBloque::destruible) golpearLadrillo(bloque, b->getDireccion());
					b->desactivar();
				}
			}
		}

		// 3) choque con tanques
		if (b->estaActiva()) {
			if (b->esDelJugador()) {
				for (int k = 0; k < enemigos->getCantidad(); k++) {
					CEnemigo* e = enemigos->getEnemigo(k);
					if (solapa(b, e)) {
						e->setVidas(e->getVidas() - 1);
						if (e->getVidas() <= 0) enemigos->EliminarPorId(e->getId());
						b->desactivar();
						break;   // la lista cambio, se sale del for
					}
				}
			}
			else if (solapa(b, jugador)) {
				jugador->setVidas(jugador->getVidas() - 1);
				// TODO: si vidas <= 0 -> game over
				b->desactivar();
			}
		}

		if (!b->estaActiva()) {
			delete b;
			balas.erase(balas.begin() + i);
		}
	}
}

void CColeccionBalas::dibujar(Graphics^ g, Bitmap^ bmpBala) {
	for (CBala* b : balas) b->dibujar(g, bmpBala);
}