#pragma once
#include <vector>
#include "Bala.h"
#include "Tanque.h"
#include "Mapa.h"
#include "ColeccionEnemigos.h"

class CColeccionBalas {
	std::vector<CBala*> balas;
public:
	~CColeccionBalas();
	void Agregar(CBala* bala);
	// mueve las balas y resuelve sus choques con mapa, enemigos y jugador
	void actualizar(CMapa* mapa, CTanque* jugador, CColeccionEnemigos* enemigos);
	void dibujar(Graphics^ g);
	void dibujar(Graphics^ g, Bitmap^ bmpBala);
};
