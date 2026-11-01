#include "EstrategiaAleatoria.h"
#include "Enemigo.h"
#include "Mapa.h"
#include <cstdlib>

void EstrategiaAleatoria::mover(CEnemigo* enemigo, CMapa* mapa) {
	// Cambia de direccion al azar (1 de cada 20 ticks) o si esta bloqueado
	if (enemigo->getDireccion() == EDireccion::ninguno ||
		rand() % 20 == 0 || !enemigo->puedeAvanzar(mapa)) {
		enemigo->alinearACelda();   // evita quedar "atorado" en pasillos de 1 celda
		enemigo->setDireccion((EDireccion)(1 + rand() % 4));
	}
	if (enemigo->puedeAvanzar(mapa))
		enemigo->CTanque::mover();  // movimiento base de CTanque (usa direccion y velocidad)
}