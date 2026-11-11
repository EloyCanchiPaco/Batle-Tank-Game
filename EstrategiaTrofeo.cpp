#include "EstrategiaTrofeo.h"
#include "EstrategiaTrofeo.h"
#include "EstrategiaAleatoria.h"
#include "Enemigo.h"
#include "Mapa.h"

static int aCelda(int valor, int tam, int maximo) {
	int c = (valor + tam / 2) / tam;
	if (c < 0) c = 0;
	if (c >= maximo) c = maximo - 1;
	return c;
}
static bool esHorizontal(EDireccion d) {
	return d == EDireccion::izquierda || d == EDireccion::derecha;
}

static const int df[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

static bool esTipo(CMapa* mapa, int f, int c, ETipoBloque t) {
	if (f < 0 || c < 0 || f >= MAX_FILAS || c >= MAX_COLUMNAS) return false;
	CBloque* b = mapa->getBloque(f, c);
	return b != nullptr && b->getTipo() == t;
}

// BFS desde TODAS las celdas del trofeo. Devuelve false si no hay trofeo.
static bool calcularDistancias(CMapa* mapa, int dist[MAX_FILAS][MAX_COLUMNAS], bool destructibles) {
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
				!(destructibles && esTipo(mapa, nf, nc, ETipoBloque::destruible))) continue;
			if (dist[nf][nc] != -1) continue;
			dist[nf][nc] = dist[f][c] + 1;
			colaF[fin] = nf; colaC[fin] = nc; fin++;
		}
	}
	return true;
}

void EstrategiaTrofeo::mover(CEnemigo* enemigo, CMapa* mapa) {
	EstrategiaAleatoria aleatoria;   // plan B

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	int dist[MAX_FILAS][MAX_COLUMNAS];
	if (!calcularDistancias(mapa, dist, false)) {   // no hay trofeo en el mapa
		aleatoria.mover(enemigo, mapa);
		return;
	}
	// Sin ruta libre: atravesar ladrillos para acercarse
	if (dist[fe][ce] == -1) calcularDistancias(mapa, dist, true);

	// Ya esta pegado al trofeo (la celda vecina es el trofeo): se queda
	for (int k = 0; k < 4; k++)
		if (esTipo(mapa, fe + df[k], ce + dc[k], ETipoBloque::trofeo)) return;

	int mejor = -1, mejorDist = 1000000;
	for (int k = 0; k < 4; k++) {
		int nf = fe + df[k], nc = ce + dc[k];
		if (nf < 0 || nc < 0 || nf >= MAX_FILAS || nc >= MAX_COLUMNAS) continue;
		if (dist[nf][nc] == -1) continue;
		if (dist[nf][nc] < mejorDist) { mejorDist = dist[nf][nc]; mejor = k; }
	}
	if (mejor == -1) { aleatoria.mover(enemigo, mapa); return; }

	const EDireccion dirs[4] = { EDireccion::arriba, EDireccion::abajo,
								 EDireccion::izquierda, EDireccion::derecha };
	EDireccion d = dirs[mejor];
	if (esHorizontal(enemigo->getDireccion()) != esHorizontal(d))
		enemigo->alinearACelda();
	enemigo->setDireccion(d);

	bool siguienteEsPiso = esTipo(mapa, fe + df[mejor], ce + dc[mejor], ETipoBloque::piso);
	if (enemigo->puedeAvanzar(mapa))
		enemigo->CTanque::mover();
	else if (siguienteEsPiso)
		aleatoria.mover(enemigo, mapa);
	// si delante hay ladrillo, espera pegado a el
}