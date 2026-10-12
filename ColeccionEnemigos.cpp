#include "ColeccionEnemigos.h"

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
    for (CEnemigo* e : enemigos) {
        e->dibujar(g, bmpTanques);
    }
}