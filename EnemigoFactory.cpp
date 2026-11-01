#include "EnemigoFactory.h"
#include "EstrategiaCreacion.h"

CEnemigo* CEnemigoFactory::CrearEnemigo(ETipoTanque tipo, short id, int fila, int columna, CTanque* jugador) {
    int vidas = 1;
    short velocidad = 10;
    static CreacionTipo1 t1; static CreacionTipo2 t2; static CreacionTipo3 t3;
    static CreacionTipo4 t4; static CreacionTipo5 t5;
    EstrategiaCreacion* creacion = &t1;   // estrategia de creacion (define el movimiento)

    switch (tipo) {
    case ETipoTanque::tipo1: vidas = 1; velocidad = 5;  creacion = &t1; break;  // básico
    case ETipoTanque::tipo2: vidas = 3; velocidad = 8;  creacion = &t2; break;  // fuerte
    case ETipoTanque::tipo3: vidas = 2; velocidad = 8;  creacion = &t3; break;
    case ETipoTanque::tipo4: vidas = 3; velocidad = 6;  creacion = &t4; break;
    case ETipoTanque::tipo5: vidas = 4; velocidad = 4;  creacion = &t5; break;
    }
    CEnemigo* enemigo = new CEnemigo(id, tipo, fila, columna, vidas);
    enemigo->setTipo(tipo);
    enemigo->setVelocidad(velocidad);
    enemigo->setEstrategia(creacion->crearMovimiento(jugador));
    return enemigo;
}
