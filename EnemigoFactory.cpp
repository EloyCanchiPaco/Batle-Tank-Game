#include "EnemigoFactory.h"
CEnemigo* CEnemigoFactory::CrearEnemigo(ETipoTanque tipo, short id, int fila, int columna) {
    int vidas = 1;
    short velocidad = 10;
    switch (tipo) {
    case ETipoTanque::tipo1: vidas = 1; velocidad = 5;  break;  // básico
    case ETipoTanque::tipo2: vidas = 3; velocidad = 8;  break;  // fuerte
    case ETipoTanque::tipo3: vidas = 2; velocidad = 8;  break;
    case ETipoTanque::tipo4: vidas = 3; velocidad = 6;  break;
    case ETipoTanque::tipo5: vidas = 4; velocidad = 4;  break;
    }
    CEnemigo* enemigo = new CEnemigo(id, tipo, fila, columna, vidas);
	enemigo->setTipo(tipo);
    enemigo->setVelocidad(velocidad);
    return enemigo;
}
