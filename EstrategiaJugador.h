#pragma once
#include "Estrategia.h"
#include "Tanque.h"

// Persigue al jugador. Cada estrategia guarda su propio objetivo (CJugador hereda de CTanque).
class EstrategiaJugador : public Estrategia {
	CTanque* objetivo;
public:
	EstrategiaJugador(CTanque* jugador) : objetivo(jugador) {}
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};