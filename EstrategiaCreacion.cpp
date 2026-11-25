#include "EstrategiaCreacion.h"
#include "EstrategiaJugador.h"
#include "EstrategiaTrofeo.h"
#include "EstrategiaVelocidad.h"
#include "EstrategiaAleatoria.h"
#include "EstrategiaCombinada.h"

Estrategia* CreacionTipo1::crearMovimiento(CTanque* jugador, int nivel) { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo2::crearMovimiento(CTanque* jugador, int nivel) { return new EstrategiaJugador(jugador); }
// Par de estrategias: ColeccionEnemigos reparte mitad y mitad entre los enemigos vivos.
// Cambia aqui la combinacion que quieras.
Estrategia* CreacionTipo3::crearMovimiento(CTanque* jugador, int nivel) { return new EstrategiaCombinada(new EstrategiaJugador(jugador), new EstrategiaTrofeo()); }
Estrategia* CreacionTipo4::crearMovimiento(CTanque* jugador, int nivel) { return new EstrategiaCombinada(new EstrategiaTrofeo(), new EstrategiaAleatoria()); }
Estrategia* CreacionTipo5::crearMovimiento(CTanque* jugador, int nivel) { return new EstrategiaCombinada(new EstrategiaTrofeo(), new EstrategiaVelocidad(nivel));
}