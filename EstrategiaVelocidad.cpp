#include "EstrategiaVelocidad.h"
#include "Enemigo.h"
#include "Mapa.h"

void EstrategiaVelocidad::mover(CEnemigo* enemigo, CMapa* mapa) {
	if (!aplicada) {                       // sumar el bonus solo una vez
		short v = enemigo->getVelocidad() + (nivel - 1) * 3;
		if (v > 12) v = 15;                // tope para que no se salte celdas/paredes
		enemigo->setVelocidad(v);
		aplicada = true;
	}
	aleatoria.mover(enemigo, mapa);
}