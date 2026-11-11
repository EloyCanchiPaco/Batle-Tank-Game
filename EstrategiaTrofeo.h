#pragma once
#include "Estrategia.h"
#include "Tanque.h"

class CEnemigo;
class CMapa;

class EstrategiaTrofeo : public Estrategia {
public:
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};