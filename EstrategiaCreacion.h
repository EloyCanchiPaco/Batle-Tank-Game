#pragma once
#include "Enemigo.h"
// ===== PATRON STRATEGY: CREACION DE ENEMIGOS =====
// Cada estrategia de creacion decide con que estrategia de MOVIMIENTO nace cada tipo de tanque.
// (Las vidas y velocidades se siguen definiendo en el switch de EnemigoFactory.)
class EstrategiaCreacion {
public:
	virtual ~EstrategiaCreacion() {}
	virtual Estrategia* crearMovimiento() = 0;
};

class CreacionTipo1 : public EstrategiaCreacion { public: Estrategia* crearMovimiento() override; };
class CreacionTipo2 : public EstrategiaCreacion { public: Estrategia* crearMovimiento() override; };
class CreacionTipo3 : public EstrategiaCreacion { public: Estrategia* crearMovimiento() override; };
class CreacionTipo4 : public EstrategiaCreacion { public: Estrategia* crearMovimiento() override; };
class CreacionTipo5 : public EstrategiaCreacion { public: Estrategia* crearMovimiento() override; };
