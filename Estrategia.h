#pragma once
// ===== PATRON STRATEGY: MOVIMIENTO DE ENEMIGOS =====
// Interfaz comun. Cada estrategia concreta decide COMO se mueve el enemigo.
class CEnemigo;
class CMapa;

class Estrategia {
public:
	virtual ~Estrategia() {}
	// Se llama una vez por tick del timer, por cada enemigo.
	virtual void mover(CEnemigo* enemigo, CMapa* mapa) = 0;
};
