#pragma once
#include "Tanque.h"
#include "Estrategia.h"
#include "EstrategiaAleatoria.h"
#include "EstrategiaJugador.h"
#include "EstrategiaTrofeo.h"
#include "EstrategiaVelocidad.h"
enum ETipoTanque {
	tipo1=0,
	tipo2=4,
	tipo3=8,
	tipo4=12,
	tipo5=16
};

class CEnemigo: public CTanque {
	ETipoTanque tipo;
	//por aqui se deveria empezar a implementar el patron estrategia
	// Solo UNO estara asignado a la vez; los demas quedan en nullptr.
	// //
	//EstrategiaJugador* ajugador = nullptr;
	EstrategiaAleatoria* aleatorio = nullptr;
	//EstrategiaTrofeo* atrofeo = nullptr;
	//EstrategiaVelocidad* mayorVelocidad = nullptr;
private:
	int imagenX = 0;
public:
	CEnemigo(short id,short tipo, int fila, int columna, int vidas);
	~CEnemigo();
	void setTipo(ETipoTanque tipo);
	ETipoTanque getTipo();
	void dibujar(Graphics^ g, Bitmap^ bmpTanques);
	void animar();
	//
	void setEstrategia(Estrategia* estrategia);
	//void setVelocidad(int velocidad);
	void mover();
	//
	void mover(CMapa* mapa);            // delega en la estrategia
	// utilidades para las estrategias
	bool puedeAvanzar(CMapa* mapa);
	void alinearACelda();
};
