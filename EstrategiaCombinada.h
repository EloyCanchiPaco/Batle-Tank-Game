#pragma once
#include "Estrategia.h"

// Combina DOS estrategias: alterna entre ellas cada 'duracion' ticks.
// Para CEnemigo es una sola estrategia (cumple la interfaz Estrategia);
// por dentro delega en la que le toque (patron Strategy + Composite).
class EstrategiaCombinada : public Estrategia {
	Estrategia* primera;
	Estrategia* segunda;
	int duracion;              // ticks que dura cada estrategia antes de cambiar
	int contador = 0;
	bool usandoPrimera = true;
public:
	EstrategiaCombinada(Estrategia* primera, Estrategia* segunda, int duracion)
		: primera(primera), segunda(segunda), duracion(duracion) {
	}

	~EstrategiaCombinada() {      // es dueña de las dos
		delete primera;
		delete segunda;
	}

	void mover(CEnemigo* enemigo, CMapa* mapa) override {
		if (++contador >= duracion) {         // toca cambiar
			contador = 0;
			usandoPrimera = !usandoPrimera;
		}
		if (usandoPrimera) primera->mover(enemigo, mapa);
		else               segunda->mover(enemigo, mapa);
	}
};