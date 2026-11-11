#pragma once
#include "Estrategia.h"
#include "EstrategiaAleatoria.h"

class CEnemigo;
class CMapa;

class EstrategiaVelocidad : public Estrategia {
	int nivel;
	bool aplicada;
	EstrategiaAleatoria aleatoria;
public:
	EstrategiaVelocidad(int nivel) : nivel(nivel), aplicada(false) {}
	void mover(CEnemigo* enemigo, CMapa* mapa) override;
};