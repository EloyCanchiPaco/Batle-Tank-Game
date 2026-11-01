#pragma once
#include "Estrategia.h"
// YA IMPLEMENTADA (sirve de ejemplo para las demas)
class EstrategiaAleatoria : public Estrategia {
public:
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};
