#pragma once
#include <vector>
#include "Bala.h"
#include "Tanque.h"
#include "Mapa.h"
#include "ColeccionEnemigos.h"

class CColeccionBalas {
	std::vector<CBala*> balas;

	// ---- explosiones: nacen de los choques de las balas ----
	struct Explosion { int x, y, tam, cuadro, restante; };
	std::vector<Explosion> explosiones;
	void crearExplosion(int cx, int cy, bool grande, int tam);
	void explosionEnBala(CBala* b);              // mediana, donde choco la bala
public:
	~CColeccionBalas();
	void Agregar(CBala* bala);
	// mueve las balas, resuelve sus choques y anima las explosiones
	void actualizar(CMapa* mapa, CTanque* jugador, CColeccionEnemigos* enemigos);
	// bmpExplosion = explosion.png: 3 cuadros de 64x64 (chico, mediano, grande)
	void dibujar(Graphics^ g, Bitmap^ bmpBala, Bitmap^ bmpExplosion);   // balas y explosiones
	std::vector<EEventoBala>& getEventos() { return eventos; }
private:
	std::vector<EEventoBala> eventos;
};