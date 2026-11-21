#pragma once
#include "Estrategia.h"

// Par de estrategias. Dos usos:
//  - duracion = 0 (por defecto): NO alterna. El par solo sirve para repartir las dos
//    estrategias entre los enemigos (ColeccionEnemigos saca una con extraer()).
//  - duracion > 0: alterna entre ellas cada 'duracion' ticks (patron Strategy + Composite).
class EstrategiaCombinada : public Estrategia {
	Estrategia* primera;
	Estrategia* segunda;
	int duracion;
	int contador = 0;
	bool usandoPrimera = true;
public:
	EstrategiaCombinada(Estrategia* primera, Estrategia* segunda, int duracion = 0)
		: primera(primera), segunda(segunda), duracion(duracion) {
	}

	~EstrategiaCombinada() {      // es dueña de las dos (delete nullptr no hace nada)
		delete primera;
		delete segunda;
	}

	// Saca una de las dos (0 = primera, 1 = segunda). Quien la recibe pasa a ser su dueño.
	Estrategia* extraer(int cual) {
		Estrategia*& ref = (cual == 0) ? primera : segunda;
		Estrategia* tmp = ref;
		ref = nullptr;
		return tmp;
	}

	void mover(CEnemigo* enemigo, CMapa* mapa) override {
		if (duracion > 0 && ++contador >= duracion) {
			contador = 0;
			usandoPrimera = !usandoPrimera;
		}
		Estrategia* actual = usandoPrimera ? primera : segunda;
		if (actual) actual->mover(enemigo, mapa);
	}
};