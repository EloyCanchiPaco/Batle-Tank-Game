#include "Colision.h"
#include "Tanque.h"
#include "Mapa.h"
#include "ColeccionEnemigos.h"

static bool solapaRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

// ---------------------------------------------------------------- Limites
bool ColisionLimites::bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) {
	return nx < 0 || ny < 0 ||
		nx + ANCHO_IMAGEN > MAX_COLUMNAS * ANCHO_IMAGEN ||
		ny + ALTO_IMAGEN  > MAX_FILAS * ALTO_IMAGEN;
}

// ---------------------------------------------------------------- Bloques
ColisionBloques::ColisionBloques(bool atraviesaTrofeo) {
	this->atraviesaTrofeo = atraviesaTrofeo;
}

// Rectangulo que ocupa de verdad el bloque. Un ladrillo de mitad solo ocupa la mitad que queda.
// (mismo criterio que CColeccionBalas: mitadX = la mitad que esta presente)
static void rectSolido(CBloque* b, int f, int c, int& x, int& y, int& w, int& h) {
	x = c * ANCHO_IMAGEN;  y = f * ALTO_IMAGEN;
	w = ANCHO_IMAGEN;      h = ALTO_IMAGEN;
	if (b->getTipo() != ETipoBloque::destruible) return;
	switch (b->getMitad()) {
	case EMitadBloque::mitadArriba:    h = ALTO_IMAGEN / 2; break;
	case EMitadBloque::mitadAbajo:     y += ALTO_IMAGEN / 2; h = ALTO_IMAGEN / 2; break;
	case EMitadBloque::mitadIzquierda: w = ANCHO_IMAGEN / 2; break;
	case EMitadBloque::mitadDerecha:   x += ANCHO_IMAGEN / 2; w = ANCHO_IMAGEN / 2; break;
	default: break;
	}
}

bool ColisionBloques::bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) {
	if (ctx == nullptr || ctx->mapa == nullptr) return false;

	// Rectángulo de colisión del tanque, un poco más pequeño que el sprite
	const int MARGEN = 2;
	int tx = nx + MARGEN, ty = ny + MARGEN;
	int tw = ANCHO_IMAGEN - 2 * MARGEN, th = ALTO_IMAGEN - 2 * MARGEN;

	// Celdas que toca ese rectángulo reducido (usa tx, ty, tw, th)
	int c1 = tx / ANCHO_IMAGEN, c2 = (tx + tw - 1) / ANCHO_IMAGEN;
	int f1 = ty / ALTO_IMAGEN, f2 = (ty + th - 1) / ALTO_IMAGEN;

	for (int f = f1; f <= f2; f++)
		for (int c = c1; c <= c2; c++) {
			if (f < 0 || c < 0 || f >= MAX_FILAS || c >= MAX_COLUMNAS) return true;
			CBloque* b = ctx->mapa->getBloque(f, c);
			if (b == nullptr) return true;

			ETipoBloque tipo = b->getTipo();
			if (tipo == ETipoBloque::piso) continue;
			if (tipo == ETipoBloque::trofeo && atraviesaTrofeo) continue;

			int sx, sy, sw, sh;
			rectSolido(b, f, c, sx, sy, sw, sh);
			if (solapaRect(tx, ty, tw, th, sx, sy, sw, sh)) return true;   // <- aquí
		}
	return false;
}
// ---------------------------------------------------------------- Tanques
// true si moverse a (nx, ny) dejaria al tanque encima de 'otro'.
// Si ya estaban superpuestos, se permite el movimiento (para que puedan separarse).
static bool chocaCon(CTanque* t, int nx, int ny, CTanque* otro) {
	if (otro == nullptr || otro == t) return false;
	bool antes = solapaRect(t->getX(), t->getY(), ANCHO_IMAGEN, ALTO_IMAGEN,
		otro->getX(), otro->getY(), ANCHO_IMAGEN, ALTO_IMAGEN);
	if (antes) return false;
	return solapaRect(nx, ny, ANCHO_IMAGEN, ALTO_IMAGEN,
		otro->getX(), otro->getY(), ANCHO_IMAGEN, ALTO_IMAGEN);
}

bool ColisionTanques::bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) {
	if (ctx == nullptr) return false;
	if (chocaCon(tanque, nx, ny, ctx->jugador)) return true;
	if (ctx->enemigos != nullptr)
		for (int i = 0; i < ctx->enemigos->getCantidad(); i++)
			if (chocaCon(tanque, nx, ny, ctx->enemigos->getEnemigo(i))) return true;
	return false;
}