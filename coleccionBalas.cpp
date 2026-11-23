#include "ColeccionBalas.h"
//#include "Sonido.h"

// Cuadro de explosion.png (0 = chico, 1 = mediano, 2 = grande) y cuantos ticks se ve cada tipo.
static const int CUADRO_IMPACTO = 1, TICKS_IMPACTO = 3;
static const int CUADRO_GRANDE = 2, TICKS_GRANDE = 6;
static const int TAM_IMPACTO = 64;          // tamano en pantalla de la explosion mediana
static const int TAM_ENEMIGO = 96;          // enemigo destruido
static const int TAM_TROFEO = 128;          // aguila destruida

CColeccionBalas::~CColeccionBalas() {
	for (CBala* b : balas) delete b;
	balas.clear();
}

void CColeccionBalas::Agregar(CBala* b) {
	balas.push_back(b);                      // tu código actual
	eventos.push_back(b->esDelJugador() ? EEventoBala::disparoJugador
		: EEventoBala::disparoEnemigo);
}

void CColeccionBalas::crearExplosion(int cx, int cy, bool grande, int tam) {
	Explosion e = { cx, cy, tam, grande ? CUADRO_GRANDE : CUADRO_IMPACTO, grande ? TICKS_GRANDE : TICKS_IMPACTO };
	explosiones.push_back(e);
}

// Explosion mediana en el centro de la bala, sin salirse de los limites del mapa
void CColeccionBalas::explosionEnBala(CBala* b) {
	int x = b->getX() + CBala::TAM / 2;
	int y = b->getY() + CBala::TAM / 2;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x > MAX_COLUMNAS * ANCHO_IMAGEN) x = MAX_COLUMNAS * ANCHO_IMAGEN;
	if (y > MAX_FILAS * ALTO_IMAGEN) y = MAX_FILAS * ALTO_IMAGEN;
	crearExplosion(x, y, false, TAM_IMPACTO);
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
// Un bloque de mitad (ladrillo o gris) solo ocupa la mitad que queda; el resto de la celda esta vacio.
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
//

void CColeccionBalas::actualizar(CMapa* mapa, CTanque* jugador, CColeccionEnemigos* enemigos) {
	for (int i = (int)explosiones.size() - 1; i >= 0; i--)
		if (--explosiones[i].restante <= 0)
			explosiones.erase(explosiones.begin() + i);

	for (int i = (int)balas.size() - 1; i >= 0; i--) {
		CBala* b = balas[i];
		b->mover();

		// 1) fuera del mapa
		int c1 = b->getX() / ANCHO_IMAGEN, c2 = (b->getX() + CBala::TAM - 1) / ANCHO_IMAGEN;
		int f1 = b->getY() / ALTO_IMAGEN, f2 = (b->getY() + CBala::TAM - 1) / ALTO_IMAGEN;
		if (b->getX() < 0 || b->getY() < 0 || c2 >= MAX_COLUMNAS || f2 >= MAX_FILAS) {
			explosionEnBala(b);
			b->desactivar();
		}
		else {
			// 2) bloques
			for (int f = f1; f <= f2 && b->estaActiva(); f++) {
				for (int c = c1; c <= c2 && b->estaActiva(); c++) {
					CBloque* bloque = mapa->getBloque(f, c);
					if (bloque == nullptr) continue;
					ETipoBloque t = bloque->getTipo();
					if (t != ETipoBloque::destruible && t != ETipoBloque::noDestruible &&
						t != ETipoBloque::trofeo) continue;

					int sx, sy, sw, sh;
					rectSolido(bloque, f, c, sx, sy, sw, sh);
					if (!solapaRect(b->getX(), b->getY(), CBala::TAM, CBala::TAM, sx, sy, sw, sh))
						continue;

					if (t == ETipoBloque::destruible) {
						golpearLadrillo(bloque, b->getDireccion());
						eventos.push_back(EEventoBala::ladrilloGolpeado);
					}

					if (t == ETipoBloque::trofeo && bloque->getDurabilidad() > 0) {
						bloque->setDurabilidad(0);
						crearExplosion(c * ANCHO_IMAGEN + ANCHO_IMAGEN / 2, f * ALTO_IMAGEN + ALTO_IMAGEN / 2, true, TAM_TROFEO);
						eventos.push_back(EEventoBala::trofeoDestruido);
					}
					else {
						explosionEnBala(b);
					}
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
						if (e->getVidas() <= 0) {
							crearExplosion(e->getX() + ANCHO_IMAGEN / 2, e->getY() + ALTO_IMAGEN / 2, true, TAM_ENEMIGO);
							eventos.push_back(EEventoBala::enemigoDestruido);
							enemigos->EliminarPorId(e->getId());
						}
						else {
							eventos.push_back(EEventoBala::enemigoGolpeado);
							explosionEnBala(b);
						}
						b->desactivar();
						break;
					}
				}
			}
			else if (solapa(b, jugador)) {
				jugador->setVidas(jugador->getVidas() - 1);
				eventos.push_back(EEventoBala::jugadorGolpeado);
				explosionEnBala(b);
				b->desactivar();
			}
		}

		if (!b->estaActiva()) {
			delete b;
			balas.erase(balas.begin() + i);
		}
	}
}

void CColeccionBalas::dibujar(Graphics^ g, Bitmap^ bmpBala, Bitmap^ bmpExplosion) {
	for (CBala* b : balas) b->dibujar(g, bmpBala);

	if (bmpExplosion == nullptr) return;
	int tamCuadro = bmpExplosion->Height;        // cada cuadro es cuadrado (64x64)
	for (size_t i = 0; i < explosiones.size(); i++) {
		Explosion& e = explosiones[i];
		Rectangle origen = Rectangle(e.cuadro * tamCuadro, 0, tamCuadro, tamCuadro);
		Rectangle destino = Rectangle(e.x - e.tam / 2, e.y - e.tam / 2, e.tam, e.tam);
		g->DrawImage(bmpExplosion, destino, origen, GraphicsUnit::Pixel);
	}
}