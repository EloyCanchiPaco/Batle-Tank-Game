#pragma once
#include "Estrategia.h"
#include "Tanque.h"

class CEnemigo;
class CMapa;

class EstrategiaTrofeo : public Estrategia {
	int bloqueado = 0;      // ticks seguidos sin poder avanzar
	int pasosAzar = 0;      // celdas que le quedan de "búsqueda" al azar
public:
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};