#pragma once
#include "Estrategia.h"
#include "Tanque.h"

// Persigue al jugador. Cada estrategia guarda su propio objetivo (CJugador hereda de CTanque).
class EstrategiaJugador : public Estrategia {
	CTanque* objetivo;
	int tiempoVisto = 0;          // NUEVO: ticks que lleva viendo al jugador
	int bloqueado = 0;            // ticks seguidos sin poder avanzar
	int retardo = 0;                // ticks que lleva sin disparar (para no disparar cada tick)
public:
	EstrategiaJugador(CTanque* jugador) : objetivo(jugador) {}
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};