#pragma once
#include "Enemigo.h"
// ===== PATRON STRATEGY: CREACION DE ENEMIGOS =====
// Cada estrategia de creacion decide con que estrategia de MOVIMIENTO nace cada tipo de tanque.
// Recibe al jugador por si la estrategia de movimiento lo necesita.
class EstrategiaCreacion {
public:
	virtual ~EstrategiaCreacion() {}
	virtual Estrategia* crearMovimiento(CTanque* jugador) = 0;
};

class CreacionTipo1 : public EstrategiaCreacion { public: Estrategia* crearMovimiento(CTanque* jugador) override; };
class CreacionTipo2 : public EstrategiaCreacion { public: Estrategia* crearMovimiento(CTanque* jugador) override; };
class CreacionTipo3 : public EstrategiaCreacion { public: Estrategia* crearMovimiento(CTanque* jugador) override; };
class CreacionTipo4 : public EstrategiaCreacion { public: Estrategia* crearMovimiento(CTanque* jugador) override; };
class CreacionTipo5 : public EstrategiaCreacion { public: Estrategia* crearMovimiento(CTanque* jugador) override; };