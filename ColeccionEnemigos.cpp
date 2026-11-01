#include "ColeccionEnemigos.h"
#include "EnemigoFactory.h"

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
bool CColeccionEnemigos::hayEnemigoEn(int fila, int columna) {
    for (int k = 0; k < (int)enemigos.size(); k++) {
        if (enemigos[k]->getX() == columna * ANCHO_IMAGEN &&
            enemigos[k]->getY() == fila * ALTO_IMAGEN)
            return true;
    }
    return false;
}
void CColeccionEnemigos::generarPorNivel(CMapa* mapa, int filaJugador, int columnaJugador, CTanque* jugador) {
    ETipoTanque tipos[] = { tipo1, tipo2, tipo3, tipo4, tipo5 };
    int cantidades[] = { 3, 4, 5, 6, 7 };           // enemigos por nivel: aquí se edita

    int nivel = mapa->getNivel();
    if (nivel < 1) nivel = 1;
    if (nivel > 5) nivel = 5;

    ETipoTanque tipoNivel = tipos[nivel - 1];          // nivel 4 -> tipo4
    int cantidad = cantidades[nivel - 1];

    for (int i = 0; i < cantidad; i++) {
        int fila = -1, columna = -1;

        for (int intento = 0; intento < 500; intento++) {
            int f = rand() % MAX_FILAS;
            int c = rand() % (MAX_COLUMNAS-1);

            CBloque* b = mapa->getBloque(f, c);
            if (b == nullptr || b->getTipo() != ETipoBloque::piso) continue;   // solo sobre piso
            if (f == filaJugador && c == columnaJugador) continue;             // no sobre el jugador
            if (hayEnemigoEn(f, c)) continue;                                  // no sobre otro enemigo

            fila = f;
            columna = c;
            break;
        }

        if (fila == -1) continue;   // no encontró celda libre: omite este enemigo

        Agregar(CEnemigoFactory::CrearEnemigo(tipoNivel, (short)enemigos.size(), fila, columna, jugador));
    }
}