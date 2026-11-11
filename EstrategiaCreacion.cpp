#include "EstrategiaCreacion.h"
#include "EstrategiaJugador.h"
#include "EstrategiaTrofeo.h"
#include "EstrategiaVelocidad.h"

Estrategia* CreacionTipo1::crearMovimiento(CTanque* jugador,int nivel) { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo2::crearMovimiento(CTanque* jugador,int nivel) { return new EstrategiaJugador(jugador); }
Estrategia* CreacionTipo3::crearMovimiento(CTanque* jugador,int nivel) { return new EstrategiaTrofeo(); }
Estrategia* CreacionTipo4::crearMovimiento(CTanque* jugador,int nivel) { return new EstrategiaVelocidad(nivel); } 
Estrategia* CreacionTipo5::crearMovimiento(CTanque* jugador,int nivel) { return new EstrategiaAleatoria(); }
//Variables estáticas locales: se crean una sola vez y se reutilizan en cada llamadaporque las estrategias de creación no guardan estado.