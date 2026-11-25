#include "EstrategiaVelocidad.h"
#include "Enemigo.h"
#include "Mapa.h"

// velocidades validas: dividen a 64, asi el tanque siempre queda alineado con las celdas
static short velocidadValida(int v) {
	const short validas[] = { 4, 8, 8, 16 };
	short resultado = validas[0];
	for (short x : validas)
		if (x <= v) resultado = x;        // la mayor valida que no pase de v
	return resultado;
}

void EstrategiaVelocidad::mover(CEnemigo* enemigo, CMapa* mapa) {
	if (!aplicada) {                       // sumar el bonus solo una vez
		int v = enemigo->getVelocidad() + (nivel - 1) * 3;
		if (v > 8) v = 8;                // tope para que no se salte celdas/paredes
		enemigo->setVelocidad(velocidadValida(v));
		aplicada = true;
	}
	aleatoria.mover(enemigo, mapa);
}