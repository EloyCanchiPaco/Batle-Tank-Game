#include "EstrategiaCreacion.h"

// Asignacion de movimiento por tipo (editable por el equipo)
Estrategia* CreacionTipo1::crearMovimiento() { return new EstrategiaAleatoria(); }///cambiar al poner las otras estrategias
Estrategia* CreacionTipo2::crearMovimiento() { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo3::crearMovimiento() { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo4::crearMovimiento() { return new EstrategiaAleatoria(); }
Estrategia* CreacionTipo5::crearMovimiento() { return new EstrategiaAleatoria(); }
