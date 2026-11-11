#pragma once
#include "Enemigo.h"

class CEnemigoFactory {
public:
    static CEnemigo* CrearEnemigo(ETipoTanque tipo, short id, int fila, int columna, CTanque* jugador,int nivel);  
};

