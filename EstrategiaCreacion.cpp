#include "EstrategiaCreacion.h"
#include "EstrategiaJugador.h"
// TODO companeros: al crear su estrategia, agregar aqui su #include
//#include "EstrategiaTrofeo.h"
//#include "EstrategiaVelocidad.h"

// Asignacion de movimiento por tipo (editable por el equipo)
Estrategia* CreacionTipo1::crearMovimiento(CTanque* jugador) { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo2::crearMovimiento(CTanque* jugador) { return new EstrategiaJugador(jugador); }
Estrategia* CreacionTipo3::crearMovimiento(CTanque* jugador) { return new EstrategiaAleatoria(); } // TODO: EstrategiaTrofeo
Estrategia* CreacionTipo4::crearMovimiento(CTanque* jugador) { return new EstrategiaAleatoria(); } // TODO: EstrategiaVelocidad
Estrategia* CreacionTipo5::crearMovimiento(CTanque* jugador) { return new EstrategiaAleatoria(); }