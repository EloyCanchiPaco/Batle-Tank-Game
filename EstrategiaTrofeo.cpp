#include "EstrategiaTrofeo.h"
#include "EstrategiaAleatoria.h"
#include "Enemigo.h"
#include "Mapa.h"
#include "Colision.h"
#include "ColeccionEnemigos.h"

// true: dispara al trofeo aunque haya ladrillos en medio (los va rompiendo)
static const bool DISPARAR_A_TRAVES_DE_LADRILLOS = true;

// Probabilidad (en %) de que, al llegar a una celda, empiece a "buscar" al azar.
// 0 = va siempre directo al trofeo. Sube el numero para que deambule mas.
static const int PROB_BUSQUEDA = 15;
// Cuando empieza a buscar, recorre entre 1 y MAX_PASOS_AZAR celdas al azar.
static const int MAX_PASOS_AZAR = 3;

static int aCelda(int valor, int tam, int maximo) {
	int c = (valor + tam / 2) / tam;
	if (c < 0) c = 0;
	if (c >= maximo) c = maximo - 1;
	return c;
}

static const int df[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

static bool esTipo(CMapa* mapa, int f, int c, ETipoBloque t) {
	if (f < 0 || c < 0 || f >= MAX_FILAS || c >= MAX_COLUMNAS) return false;
	CBloque* b = mapa->getBloque(f, c);
	return b != nullptr && b->getTipo() == t;
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

// BFS desde TODAS las celdas del trofeo. Los ladrillos cuentan como camino.
// bloq[f][c] = true: celda ocupada por otro tanque. Devuelve false si no hay trofeo.
static bool calcularDistancias(CMapa* mapa, int dist[MAX_FILAS][MAX_COLUMNAS],
	bool bloq[MAX_FILAS][MAX_COLUMNAS]) {
	int colaF[MAX_FILAS * MAX_COLUMNAS];
	int colaC[MAX_FILAS * MAX_COLUMNAS];
	int inicio = 0, fin = 0;

	for (int f = 0; f < MAX_FILAS; f++)
		for (int c = 0; c < MAX_COLUMNAS; c++) {
			dist[f][c] = -1;
			if (esTipo(mapa, f, c, ETipoBloque::trofeo)) {
				dist[f][c] = 0;
				colaF[fin] = f; colaC[fin] = c; fin++;
			}
		}
	if (fin == 0) return false;

	while (inicio < fin) {
		int f = colaF[inicio], c = colaC[inicio];
		inicio++;
		for (int k = 0; k < 4; k++) {
			int nf = f + df[k], nc = c + dc[k];
			if (!esTipo(mapa, nf, nc, ETipoBloque::piso) &&
				!esTipo(mapa, nf, nc, ETipoBloque::destruible)) continue;
			if (bloq[nf][nc]) continue;
			if (dist[nf][nc] != -1) continue;
			dist[nf][nc] = dist[f][c] + 1;
			colaF[fin] = nf; colaC[fin] = nc; fin++;
		}
	}
	return true;
}

// marca las celdas donde hay OTROS tanques (enemigos y jugador)
static void marcarTanques(CEnemigo* yo, bool bloq[MAX_FILAS][MAX_COLUMNAS]) {
	for (int f = 0; f < MAX_FILAS; f++)
		for (int c = 0; c < MAX_COLUMNAS; c++) bloq[f][c] = false;

	ContextoColision* ctx = yo->getContexto();
	if (ctx == nullptr) return;
	if (ctx->enemigos != nullptr)
		for (int i = 0; i < ctx->enemigos->getCantidad(); i++) {
			CEnemigo* o = ctx->enemigos->getEnemigo(i);
			if (o == nullptr || o == yo) continue;
			bloq[aCelda(o->getY(), ALTO_IMAGEN, MAX_FILAS)][aCelda(o->getX(), ANCHO_IMAGEN, MAX_COLUMNAS)] = true;
		}
	if (ctx->jugador != nullptr)
		bloq[aCelda(ctx->jugador->getY(), ALTO_IMAGEN, MAX_FILAS)]
		[aCelda(ctx->jugador->getX(), ANCHO_IMAGEN, MAX_COLUMNAS)] = true;
}

static void moverHaciaTrofeo(CEnemigo* enemigo, CMapa* mapa, int& bloqueado, int& pasosAzar) {
	// ---- Entre celdas: sigue recto; si lleva un rato bloqueado, da media vuelta ----
	if (enemigo->getX() % ANCHO_IMAGEN != 0 || enemigo->getY() % ALTO_IMAGEN != 0) {
		if (enemigo->puedeAvanzar()) { enemigo->CTanque::mover(); bloqueado = 0; }
		else if (++bloqueado > 6) {
			enemigo->setDireccion(opuesta(enemigo->getDireccion()));
			bloqueado = 0;
		}
		return;
	}

	EstrategiaAleatoria aleatoria;   // plan B

	const EDireccion dirs[4] = { EDireccion::arriba, EDireccion::abajo,
								 EDireccion::izquierda, EDireccion::derecha };

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	// 1) camino esquivando a los otros tanques
	bool bloq[MAX_FILAS][MAX_COLUMNAS];
	int dist[MAX_FILAS][MAX_COLUMNAS];
	marcarTanques(enemigo, bloq);
	if (!calcularDistancias(mapa, dist, bloq)) {     // no hay trofeo en el mapa
		aleatoria.mover(enemigo, mapa);
		return;
	}
	// 2) si no hay ninguno, camino ignorando tanques (esperara a que se muevan)
	if (dist[fe][ce] == -1) {
		for (int f = 0; f < MAX_FILAS; f++)
			for (int c = 0; c < MAX_COLUMNAS; c++) bloq[f][c] = false;
		calcularDistancias(mapa, dist, bloq);
	}

	int actual = dist[fe][ce];
	if (actual == -1) { aleatoria.mover(enemigo, mapa); return; }   // sin camino posible

	// vecinos que se acercan (dist = actual - 1)
	int cand[4], n = 0;
	for (int k = 0; k < 4; k++) {
		int nf = fe + df[k], nc = ce + dc[k];
		if (nf < 0 || nc < 0 || nf >= MAX_FILAS || nc >= MAX_COLUMNAS) continue;
		if (dist[nf][nc] == actual - 1) cand[n++] = k;
	}
	if (n == 0) return;
	if (n == 2 && rand() % 2) { int t = cand[0]; cand[0] = cand[1]; cand[1] = t; }

	// pegado al trofeo: lo mira y dispara
	if (actual == 1) {
		pasosAzar = 0;
		enemigo->setDireccion(dirs[cand[0]]);
		enemigo->disparar();
		return;
	}

	// ---- Busqueda al azar: unas celdas en una direccion libre cualquiera ----
	if (pasosAzar == 0 && rand() % 100 < PROB_BUSQUEDA)
		pasosAzar = 1 + rand() % MAX_PASOS_AZAR;

	if (pasosAzar > 0) {
		pasosAzar--;
		EDireccion atras = opuesta(enemigo->getDireccion());
		int libres[4], nl = 0;
		for (int k = 0; k < 4; k++) {
			if (!esTipo(mapa, fe + df[k], ce + dc[k], ETipoBloque::piso)) continue;
			if (dirs[k] == atras) continue;                  // no da media vuelta si hay otra opcion
			enemigo->setDireccion(dirs[k]);
			if (enemigo->puedeAvanzar()) libres[nl++] = k;
		}
		if (nl > 0) {
			enemigo->setDireccion(dirs[libres[rand() % nl]]);
			enemigo->CTanque::mover();
			return;
		}
		pasosAzar = 0;                                       // sin salida: vuelve al camino
	}

	// ---- Ir por el mejor camino; si hay ladrillo delante, lo rompe ----
	for (int i = 0; i < n; i++) {
		int k = cand[i];
		enemigo->setDireccion(dirs[k]);
		if (esTipo(mapa, fe + df[k], ce + dc[k], ETipoBloque::destruible)) {
			enemigo->disparar(); bloqueado = 0; return;
		}
		if (enemigo->puedeAvanzar()) { enemigo->CTanque::mover(); bloqueado = 0; return; }
	}

	// ---- Bloqueado por otro tanque: espera y luego se aparta ----
	if (++bloqueado >= 8 + rand() % 8) {
		bloqueado = 0;
		int libres[4], nl = 0;
		for (int k = 0; k < 4; k++) {
			if (!esTipo(mapa, fe + df[k], ce + dc[k], ETipoBloque::piso)) continue;
			enemigo->setDireccion(dirs[k]);
			if (enemigo->puedeAvanzar()) libres[nl++] = k;
		}
		if (nl > 0) {
			enemigo->setDireccion(dirs[libres[rand() % nl]]);
			enemigo->CTanque::mover();
		}
	}
}

// ---------------------------------------------------------------------------
// Disparar cuando ve el trofeo
// ---------------------------------------------------------------------------

// true si el bloque detiene una bala antes de llegar al trofeo
static bool frenaBala(CMapa* mapa, int f, int c) {
	if (esTipo(mapa, f, c, ETipoBloque::noDestruible)) return true;
	if (esTipo(mapa, f, c, ETipoBloque::destruible))   return !DISPARAR_A_TRAVES_DE_LADRILLOS;
	return false;
}

// Direccion en la que se ve la celda (ft, ct) desde (fe, ce), o ninguno
static EDireccion lineaHacia(CMapa* mapa, int fe, int ce, int ft, int ct) {
	if (fe == ft && ce != ct) {
		int paso = (ct > ce) ? 1 : -1;
		for (int c = ce + paso; c != ct; c += paso)
			if (frenaBala(mapa, fe, c)) return EDireccion::ninguno;
		return (paso > 0) ? EDireccion::derecha : EDireccion::izquierda;
	}
	if (ce == ct && fe != ft) {
		int paso = (ft > fe) ? 1 : -1;
		for (int f = fe + paso; f != ft; f += paso)
			if (frenaBala(mapa, f, ce)) return EDireccion::ninguno;
		return (paso > 0) ? EDireccion::abajo : EDireccion::arriba;
	}
	return EDireccion::ninguno;
}

// Si alguna celda del trofeo queda en su fila/columna sin obstaculos, apunta y dispara.
// Devuelve true si disparo (en ese tick no se mueve).
static bool dispararSiVeTrofeo(CEnemigo* enemigo, CMapa* mapa) {
	bool alineado = enemigo->getX() % ANCHO_IMAGEN == 0 && enemigo->getY() % ALTO_IMAGEN == 0;
	if (!alineado || !enemigo->puedeDisparar()) return false;

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	for (int f = 0; f < MAX_FILAS; f++)
		for (int c = 0; c < MAX_COLUMNAS; c++) {
			if (!esTipo(mapa, f, c, ETipoBloque::trofeo)) continue;
			EDireccion d = lineaHacia(mapa, fe, ce, f, c);
			if (d != EDireccion::ninguno) {
				enemigo->setDireccion(d);
				enemigo->disparar();
				return true;
			}
		}
	return false;
}

void EstrategiaTrofeo::mover(CEnemigo* enemigo, CMapa* mapa) {
	if (dispararSiVeTrofeo(enemigo, mapa)) return;     // 1) si lo ve, dispara
	moverHaciaTrofeo(enemigo, mapa, bloqueado, pasosAzar);   // 2) si no, se mueve
}