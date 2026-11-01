#pragma once
#include <vector>
#include "Enemigo.h"
#include "Mapa.h"

class CColeccionEnemigos {
    std::vector<CEnemigo*> enemigos;
    bool hayEnemigoEn(int fila, int columna);   // nuevo, privado
public:
    ~CColeccionEnemigos();
    void Agregar(CEnemigo* Enemigo);
    void EliminarPorId(short id);
    CEnemigo* getEnemigo(int indice);
    int getCantidad();
    void dibujar(Graphics^ g, Bitmap^ bmpTanques);
    void generarPorNivel(CMapa* mapa, int filaJugador, int columnaJugador);
};