#pragma once
#include "Comun.h"

// ===== PATRON CHAIN OF RESPONSIBILITY: COLISIONES DE TANQUES =====
// Cada eslabon revisa UNA cosa. El tanque puede avanzar solo si ningun eslabon lo bloquea.
//   Limites -> Bloques -> Tanques

class CMapa;
class CTanque;
class CColeccionEnemigos;

// Lo que los eslabones necesitan conocer del juego
struct ContextoColision {
	CMapa* mapa = nullptr;
	CTanque* jugador = nullptr;
	CColeccionEnemigos* enemigos = nullptr;
};

// ---------- Interfaz del eslabon ----------
class Colision {
	Colision* siguiente = nullptr;
public:
	virtual ~Colision() { delete siguiente; }   // borrar el primero libera toda la cadena

	// Devuelve el eslabon agregado, para poder encadenar: a->setSiguiente(b)->setSiguiente(c)
	Colision* setSiguiente(Colision* s) { siguiente = s; return s; }

	// true si el tanque PUEDE ir a (nx, ny)
	bool permite(CTanque* tanque, int nx, int ny, ContextoColision* ctx) {
		if (bloquea(tanque, nx, ny, ctx)) return false;           // este eslabon lo impide
		return siguiente ? siguiente->permite(tanque, nx, ny, ctx) : true;   // pasa al siguiente
	}
protected:
	virtual bool bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) = 0;
};

// ---------- Eslabones concretos ----------
// 1) El tanque no puede salirse del mapa
class ColisionLimites : public Colision {
protected:
	bool bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) override;
};

// 2) El tanque no puede entrar en bloques solidos (usa el rectangulo real de los ladrillos de mitad)
class ColisionBloques : public Colision {
	bool atraviesaTrofeo;   // true: puede pisar el trofeo (el jugador, para tomarlo)
	int margen;
public:
	ColisionBloques(bool atraviesaTrofeo = false, int margen = 2);
protected:
	bool bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) override;
};

// 3) El tanque no puede meterse encima de otro tanque
class ColisionTanques : public Colision {
protected:
	bool bloquea(CTanque* tanque, int nx, int ny, ContextoColision* ctx) override;
};

