#include "EstrategiaJugador.h"
#include "EstrategiaAleatoria.h"
#include "Enemigo.h"
#include "Mapa.h"
#include "Colision.h"
#include "ColeccionEnemigos.h"


static int aCelda(int valor, int tam, int maximo) {
	int c = (valor + tam / 2) / tam;      // celda mas cercana
	if (c < 0) c = 0;
	if (c >= maximo) c = maximo - 1;
	return c;
}

static const int df[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

static bool esPiso(CMapa* mapa, int f, int c) {
	if (f < 0 || c < 0 || f >= MAX_FILAS || c >= MAX_COLUMNAS) return false;
	CBloque* b = mapa->getBloque(f, c);
	return b != nullptr && b->getTipo() == ETipoBloque::piso;
}

static bool esDestruible(CMapa* mapa, int f, int c) {
	if (f < 0 || c < 0 || f >= MAX_FILAS || c >= MAX_COLUMNAS) return false;
	CBloque* b = mapa->getBloque(f, c);
	return b != nullptr && b->getTipo() == ETipoBloque::destruible;
}

// BFS desde (fm, cm). Los ladrillos cuentan como camino (se rompen a tiros).
// bloq[f][c] = true: celda ocupada por otro tanque, no se puede pasar por ahi.
static void calcularDistancias(CMapa* mapa, int fm, int cm,
	int dist[MAX_FILAS][MAX_COLUMNAS], bool bloq[MAX_FILAS][MAX_COLUMNAS]) {
	for (int f = 0; f < MAX_FILAS; f++)
		for (int c = 0; c < MAX_COLUMNAS; c++) dist[f][c] = -1;

	int colaF[MAX_FILAS * MAX_COLUMNAS];
	int colaC[MAX_FILAS * MAX_COLUMNAS];
	int inicio = 0, fin = 0;

	dist[fm][cm] = 0;
	colaF[fin] = fm; colaC[fin] = cm; fin++;

	while (inicio < fin) {
		int f = colaF[inicio];
		int c = colaC[inicio];
		inicio++;
		for (int k = 0; k < 4; k++) {
			int nf = f + df[k], nc = c + dc[k];
			if (!esPiso(mapa, nf, nc) && !esDestruible(mapa, nf, nc)) continue;
			if (bloq[nf][nc]) continue;
			if (dist[nf][nc] != -1) continue;
			dist[nf][nc] = dist[f][c] + 1;
			colaF[fin] = nf; colaC[fin] = nc; fin++;
		}
	}
}
// marca las celdas donde hay OTROS enemigos
static void marcarTanques(CEnemigo* yo, bool bloq[MAX_FILAS][MAX_COLUMNAS]) {
	for (int f = 0; f < MAX_FILAS; f++)
		for (int c = 0; c < MAX_COLUMNAS; c++) bloq[f][c] = false;

	ContextoColision* ctx = yo->getContexto();
	if (ctx == nullptr || ctx->enemigos == nullptr) return;
	for (int i = 0; i < ctx->enemigos->getCantidad(); i++) {
		CEnemigo* o = ctx->enemigos->getEnemigo(i);
		if (o == nullptr || o == yo) continue;
		int f = aCelda(o->getY(), ALTO_IMAGEN, MAX_FILAS);
		int c = aCelda(o->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);
		bloq[f][c] = true;
	}
}
static EDireccion opuesta(EDireccion d) {
	switch (d) {
	case EDireccion::arriba:    return EDireccion::abajo;
	case EDireccion::abajo:     return EDireccion::arriba;
	case EDireccion::izquierda: return EDireccion::derecha;
	case EDireccion::derecha:   return EDireccion::izquierda;
	default:                    return d;
	}
}

static void perseguir(CTanque* objetivo, CEnemigo* enemigo, CMapa* mapa, int& bloqueado) {
	// ---- Entre celdas: sigue recto; si lleva un rato bloqueado, da media vuelta ----
	if (enemigo->getX() % ANCHO_IMAGEN != 0 || enemigo->getY() % ALTO_IMAGEN != 0) {
		if (enemigo->puedeAvanzar()) { enemigo->CTanque::mover(); bloqueado = 0; }
		else if (++bloqueado > 6) {
			enemigo->setDireccion(opuesta(enemigo->getDireccion()));
			bloqueado = 0;
		}
		return;
	}
	if (objetivo == nullptr) return;

	const EDireccion dirs[4] = { EDireccion::arriba, EDireccion::abajo,
								 EDireccion::izquierda, EDireccion::derecha };

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);
	int fj = aCelda(objetivo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int cj = aCelda(objetivo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	// 1) camino esquivando a los otros tanques
	bool bloq[MAX_FILAS][MAX_COLUMNAS];
	int dist[MAX_FILAS][MAX_COLUMNAS];
	marcarTanques(enemigo, bloq);
	calcularDistancias(mapa, fj, cj, dist, bloq);

	// 2) si no hay ninguno, camino ignorando tanques (esperara a que se muevan)
	if (dist[fe][ce] == -1) {
		for (int f = 0; f < MAX_FILAS; f++)
			for (int c = 0; c < MAX_COLUMNAS; c++) bloq[f][c] = false;
		calcularDistancias(mapa, fj, cj, dist, bloq);
	}

	int actual = dist[fe][ce];
	if (actual <= 0) return;                       // -1: sin camino, 0: misma celda

	// vecinos que se acercan (dist = actual - 1)
	int cand[4], n = 0;
	for (int k = 0; k < 4; k++) {
		int nf = fe + df[k], nc = ce + dc[k];
		if (nf < 0 || nc < 0 || nf >= MAX_FILAS || nc >= MAX_COLUMNAS) continue;
		if (dist[nf][nc] == actual - 1) cand[n++] = k;
	}
	if (n == 0) return;
	if (n == 2 && rand() % 2) { int t = cand[0]; cand[0] = cand[1]; cand[1] = t; }

	// pegado al jugador: lo mira y dispara
	if (actual == 1) {
		enemigo->setDireccion(dirs[cand[0]]);
		enemigo->disparar();
		return;
	}

	// ir por el mejor camino; si hay ladrillo delante, lo rompe
	for (int i = 0; i < n; i++) {
		int k = cand[i];
		enemigo->setDireccion(dirs[k]);
		if (esDestruible(mapa, fe + df[k], ce + dc[k])) { enemigo->disparar(); bloqueado = 0; return; }
		if (enemigo->puedeAvanzar()) { enemigo->CTanque::mover(); bloqueado = 0; return; }
	}

	// 3) bloqueado por otro tanque: espera unos ticks y luego se aparta por cualquier lado libre
	if (++bloqueado >= 8 + rand() % 8) {           // tiempo al azar: rompe la simetria entre dos tanques
		bloqueado = 0;
		int libres[4], nl = 0;
		for (int k = 0; k < 4; k++) {
			if (!esPiso(mapa, fe + df[k], ce + dc[k])) continue;
			enemigo->setDireccion(dirs[k]);
			if (enemigo->puedeAvanzar()) libres[nl++] = k;
		}
		if (nl > 0) {
			enemigo->setDireccion(dirs[libres[rand() % nl]]);
			enemigo->CTanque::mover();
		}
	}
}
// true si el bloque detiene una bala (agua y arbol no)
static bool bloqueaBala(CMapa* mapa, int f, int c) {
	CBloque* b = mapa->getBloque(f, c);
	if (b == nullptr) return false;
	ETipoBloque t = b->getTipo();
	return t == ETipoBloque::destruible || t == ETipoBloque::noDestruible || t == ETipoBloque::trofeo;
}
// Direccion en la que el enemigo ve al jugador (misma fila o columna y sin bloques en medio)
// Si no lo ve, devuelve ninguno.
static EDireccion lineaDeTiro(CMapa* mapa, int fe, int ce, int fj, int cj) {
	if (fe == fj && ce != cj) {
		int paso = (cj > ce) ? 1 : -1;
		for (int c = ce + paso; c != cj; c += paso)
			if (bloqueaBala(mapa, fe, c)) return EDireccion::ninguno;
		return (paso > 0) ? EDireccion::derecha : EDireccion::izquierda;
	}
	if (ce == cj && fe != fj) {
		int paso = (fj > fe) ? 1 : -1;
		for (int f = fe + paso; f != fj; f += paso)
			if (bloqueaBala(mapa, f, ce)) return EDireccion::ninguno;
		return (paso > 0) ? EDireccion::abajo : EDireccion::arriba;
	}
	return EDireccion::ninguno;
}

void EstrategiaJugador::mover(CEnemigo* enemigo, CMapa* mapa) {
	if (objetivo == nullptr) return;

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);
	int fj = aCelda(objetivo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int cj = aCelda(objetivo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	bool alineado = enemigo->getX() % ANCHO_IMAGEN == 0 && enemigo->getY() % ALTO_IMAGEN == 0;
	EDireccion d = lineaDeTiro(mapa, fe, ce, fj, cj);   // ninguno si no te ve

	if (d != EDireccion::ninguno) {
		if (alineado) {
			enemigo->setDireccion(d);   // te mira
			enemigo->disparar();        // dispara en cuanto puede
			return;                     // se queda apuntando mientras te vea
		}
		// entre celdas no gira, pero si ya va hacia ti, dispara sin parar
		if (enemigo->getDireccion() == d) enemigo->disparar();
	}
	perseguir(objetivo, enemigo, mapa, bloqueado);
}