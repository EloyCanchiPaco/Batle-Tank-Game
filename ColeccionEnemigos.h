#pragma once
#include <vector>
#include "Enemigo.h"

class CColeccionEnemigos {
    std::vector<CEnemigo*> enemigos;
public:
    ~CColeccionEnemigos();
    void Agregar(CEnemigo* Enemigo);// arreglar
    void EliminarPorId(short id);
    CEnemigo* getEnemigo(int indice);
    int getCantidad();
    void dibujar(Graphics^ g, Bitmap^ bmpTanques);
};