#include "ColeccionEnemigos.h"
#include "EnemigoFactory.h"
#include "EstrategiaCombinada.h"
#include <cstdlib>

CColeccionEnemigos::~CColeccionEnemigos() {
    for (CEnemigo* e : enemigos) delete e;
    enemigos.clear();
}

void CColeccionEnemigos::Agregar(CEnemigo* enemigo) {
    enemigos.push_back(enemigo);
}

void CColeccionEnemigos::EliminarPorId(short id) {
    for (int i = 0; i < (int)enemigos.size(); i++) {
        if (enemigos[i]->getId() == id) {
            delete enemigos[i];
            enemigos.erase(enemigos.begin() + i);
            return;
        }
    }
}

CEnemigo* CColeccionEnemigos::getEnemigo(int indice) {
    if (indice < 0 || indice >= (int)enemigos.size()) return nullptr;
    return enemigos[indice];
}

void CColeccionEnemigos::dibujar(Graphics^ g, Bitmap^ bmpTanques) {
    for (int i = 0; i < (int)enemigos.size(); i++) {
        enemigos[i]->dibujar(g, bmpTanques);
        enemigos[i]->animar();
    }
}

int CColeccionEnemigos::getCantidad() {
    return (int)enemigos.size();
}

int CColeccionEnemigos::getRestantes() {
    return (int)enemigos.size() + pendientes;
}

// true si algun enemigo ocupa (o se superpone con) esa celda
bool CColeccionEnemigos::celdaOcupada(int fila, int columna) {
    int x = columna * ANCHO_IMAGEN, y = fila * ALTO_IMAGEN;
    for (int k = 0; k < (int)enemigos.size(); k++) {
        int dx = abs(enemigos[k]->getX() - x);
        int dy = abs(enemigos[k]->getY() - y);
        if (dx < ANCHO_IMAGEN && dy < ALTO_IMAGEN) return true;
    }
    return false;
}

// busca una celda de piso libre, en la mitad de arriba del mapa y lejos del jugador
bool CColeccionEnemigos::buscarCelda(int& fila, int& columna) {
    const int DISTANCIA_MINIMA = 6;               // en celdas
    const int FILAS_APARICION = MAX_FILAS / 2;    // solo las filas de la mitad de arriba

    int fj = filaJugador, cj = columnaJugador;
    if (jugador != nullptr) {
        fj = (jugador->getY() + ALTO_IMAGEN / 2) / ALTO_IMAGEN;
        cj = (jugador->getX() + ANCHO_IMAGEN / 2) / ANCHO_IMAGEN;
    }

    // celdas desde las que SE PUEDE llegar al jugador (por piso o rompiendo ladrillos)
    bool alcanzable[MAX_FILAS][MAX_COLUMNAS];
    for (int f = 0; f < MAX_FILAS; f++)
        for (int c = 0; c < MAX_COLUMNAS; c++) alcanzable[f][c] = false;
    {
        int colaF[MAX_FILAS * MAX_COLUMNAS], colaC[MAX_FILAS * MAX_COLUMNAS];
        int inicio = 0, fin = 0;
        if (fj < 0) fj = 0;
        if (fj >= MAX_FILAS) fj = MAX_FILAS - 1;
        if (cj < 0) cj = 0;
        if (cj >= MAX_COLUMNAS) cj = MAX_COLUMNAS - 1;
        alcanzable[fj][cj] = true;
        colaF[fin] = fj; colaC[fin] = cj; fin++;
        const int df[4] = { -1, 1, 0, 0 };
        const int dc[4] = { 0, 0, -1, 1 };
        while (inicio < fin) {
            int f = colaF[inicio], c = colaC[inicio];
            inicio++;
            for (int k = 0; k < 4; k++) {
                int nf = f + df[k], nc = c + dc[k];
                if (nf < 0 || nc < 0 || nf >= MAX_FILAS || nc >= MAX_COLUMNAS) continue;
                if (alcanzable[nf][nc]) continue;
                CBloque* bl = mapa->getBloque(nf, nc);
                if (bl == nullptr) continue;
                ETipoBloque t = bl->getTipo();
                if (t != ETipoBloque::piso && t != ETipoBloque::destruible) continue;
                alcanzable[nf][nc] = true;
                colaF[fin] = nf; colaC[fin] = nc; fin++;
            }
        }
    }

    for (int intento = 0; intento < 500; intento++) {
        int f = rand() % FILAS_APARICION;
        int c = rand() % (MAX_COLUMNAS - 1);
        if (!alcanzable[f][c]) continue;

        CBloque* b = mapa->getBloque(f, c);
        if (b == nullptr || b->getTipo() != ETipoBloque::piso) continue;
        if (abs(f - fj) + abs(c - cj) < DISTANCIA_MINIMA) continue;
        if (celdaOcupada(f, c)) continue;

        fila = f; columna = c;
        return true;
    }
    return false;
}

// hace aparecer un enemigo. Devuelve false si no encontro lugar (se reintenta luego)
bool CColeccionEnemigos::aparecerUno() {
    int fila, columna;
    if (!buscarCelda(fila, columna)) return false;

    CEnemigo* e = CEnemigoFactory::CrearEnemigo(tipoNivel, (short)idSiguiente++, fila, columna, jugador, nivel);

    // si el enemigo nace con un par de estrategias, le toca UNA sola mitad.
    // Se reparte mitad y mitad entre los enemigos que estan vivos.
    if (e->getCombinada() != nullptr) {
        int n0 = 0, n1 = 0;
        for (int k = 0; k < (int)enemigos.size(); k++) {
            if (enemigos[k]->getVariante() == 0) n0++;
            else if (enemigos[k]->getVariante() == 1) n1++;
        }
        int variante;
        if (n0 < n1)      variante = 0;
        else if (n1 < n0) variante = 1;
        else              variante = 1 - ultimaVariante;   // empate: alterna
        ultimaVariante = variante;

        Estrategia* elegida = e->getCombinada()->extraer(variante);
        e->setEstrategia(elegida);      // borra el par (la otra mitad) y deja solo la elegida
        e->setVariante(variante);
    }

    if (cadena != nullptr) e->setColision(cadena, contexto);    // los nuevos tambien chocan
    Agregar(e);
    pendientes--;
    return true;
}

void CColeccionEnemigos::setColision(Colision* cadena, ContextoColision* contexto) {
    this->cadena = cadena;
    this->contexto = contexto;
}

void CColeccionEnemigos::iniciarNivel(CMapa* mapa, int filaJugador, int columnaJugador, CTanque* jugador) {
    ETipoTanque tipos[] = { tipo1, tipo2, tipo3, tipo4, tipo5 };
    int totales[] = { 10, 12, 15, 18, 20 };        // enemigos EN TOTAL por nivel: aqui se edita

    this->mapa = mapa;
    this->filaJugador = filaJugador;
    this->columnaJugador = columnaJugador;
    this->jugador = jugador;

    nivel = mapa->getNivel();
    if (nivel < 1) nivel = 1;
    if (nivel > 5) nivel = 5;
    tipoNivel = tipos[nivel - 1];
    pendientes = totales[nivel - 1];
    maxEnPantalla = 5;                             // maximo vivos a la vez
    espera = 0;
    ultimaVariante = 1;

    // primera tanda
    for (int i = 0; i < maxEnPantalla && pendientes > 0; i++)
        aparecerUno();
}

void CColeccionEnemigos::actualizarGeneracion() {
    if (mapa == nullptr || pendientes <= 0) return;
    if (espera > 0) { espera--; return; }

    if ((int)enemigos.size() < maxEnPantalla) {
        if (aparecerUno())
            espera = 60;       // ticks de pausa antes del siguiente
    }
}