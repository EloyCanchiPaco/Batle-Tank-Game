#pragma once
#include "Enemigo.h"

enum class EClaseEnemigo {
    Normal,
    Fuerte
};

class CEnemigoFactory {
public:
    static CEnemigo* CrearEnemigo(ETipoTanque tipo, short id, int fila, int columna, CTanque* jugador );//static  chequear  
};

