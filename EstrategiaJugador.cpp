#include "EstrategiaJugador.h"
#include "EstrategiaAleatoria.h"
#include "Enemigo.h"
#include "Mapa.h"

static bool esHorizontal(EDireccion d) {
	return d == EDireccion::izquierda || d == EDireccion::derecha;
}

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

// Llena dist con la distancia (en celdas) de cada celda hasta (fm, cm).
// Si destructibles es true, los ladrillos tambien cuentan como camino
// (se usa cuando no hay ruta libre, para acercarse lo mas posible).
static void calcularDistancias(CMapa* mapa, int fm, int cm, int dist[MAX_FILAS][MAX_COLUMNAS], bool destructibles) {
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
			if (!esPiso(mapa, nf, nc) && !(destructibles && esDestruible(mapa, nf, nc))) continue;
			if (dist[nf][nc] != -1) continue;
			dist[nf][nc] = dist[f][c] + 1;
			colaF[fin] = nf; colaC[fin] = nc; fin++;
		}
	}
}

void EstrategiaJugador::mover(CEnemigo* enemigo, CMapa* mapa) {
	EstrategiaAleatoria aleatoria;   // plan B (solo si se traba)

	if (objetivo == nullptr) return;

	int fe = aCelda(enemigo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int ce = aCelda(enemigo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);
	int fj = aCelda(objetivo->getY(), ALTO_IMAGEN, MAX_FILAS);
	int cj = aCelda(objetivo->getX(), ANCHO_IMAGEN, MAX_COLUMNAS);

	// Meta propia de cada enemigo: una celda libre al lado del jugador.
	// Segun el id, uno va arriba, otro abajo, otro a la izquierda, otro a la derecha.
	int fm = fj, cm = cj;
	int desfase = enemigo->getId() % 4;
	for (int i = 0; i < 4; i++) {
		int k = (i + desfase) % 4;
		if (esPiso(mapa, fj + df[k], cj + dc[k])) {
			fm = fj + df[k];
			cm = cj + dc[k];
			break;
		}
	}

	int dist[MAX_FILAS][MAX_COLUMNAS];
	calcularDistancias(mapa, fm, cm, dist, false);

	// Si desde donde esta no puede llegar a su meta, ir directo al jugador
	if (dist[fe][ce] == -1) {
		fm = fj; cm = cj;
		calcularDistancias(mapa, fm, cm, dist, false);
	}

	// Sin ruta libre (jugador encerrado): acercarse atravesando ladrillos
	if (dist[fe][ce] == -1) {
		calcularDistancias(mapa, fm, cm, dist, true);
	}

	if (fe == fm && ce == cm) return;   // ya llego a su meta

	// De los 4 vecinos, ir al que este mas cerca de la meta
	int mejor = -1, mejorDist = 1000000;
	for (int k = 0; k < 4; k++) {
		int nf = fe + df[k], nc = ce + dc[k];
		if (nf < 0 || nc < 0 || nf >= MAX_FILAS || nc >= MAX_COLUMNAS) continue;
		if (dist[nf][nc] == -1) continue;
		if (dist[nf][nc] < mejorDist) { mejorDist = dist[nf][nc]; mejor = k; }
	}

	if (mejor == -1) return;            // sin camino: espera

	const EDireccion dirs[4] = { EDireccion::arriba, EDireccion::abajo,
								 EDireccion::izquierda, EDireccion::derecha };
	EDireccion d = dirs[mejor];
	if (esHorizontal(enemigo->getDireccion()) != esHorizontal(d))
		enemigo->alinearACelda();       // al cambiar de eje, alinear para entrar a pasillos
	enemigo->setDireccion(d);

	bool siguienteEsPiso = esPiso(mapa, fe + df[mejor], ce + dc[mejor]);

	if (enemigo->puedeAvanzar(mapa))
		enemigo->CTanque::mover();
	else if (siguienteEsPiso)
		aleatoria.mover(enemigo, mapa);   // trabado sin motivo: paso aleatorio
	// si lo que tiene delante es un ladrillo, se queda esperando pegado a el
}