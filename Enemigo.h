#pragma once
#include "Tanque.h"
#include "Estrategia.h"
#include "EstrategiaAleatoria.h"
#include "EstrategiaJugador.h"
#include "EstrategiaTrofeo.h"
#include "EstrategiaVelocidad.h"
#include "EstrategiaCombinada.h"
enum ETipoTanque {
	tipo1 = 0,
	tipo2 = 4,
	tipo3 = 8,
	tipo4 = 12,
	tipo5 = 16
};

class CEnemigo : public CTanque {
	ETipoTanque tipo;
	// Solo UNO estara asignado a la vez; los demas quedan en nullptr.
	EstrategiaJugador* ajugador = nullptr;
	EstrategiaAleatoria* aleatorio = nullptr;
	EstrategiaTrofeo* atrofeo = nullptr;
	EstrategiaVelocidad* mayorVelocidad = nullptr;
	EstrategiaCombinada* combinada = nullptr;
private:
	int imagenX = 0;
	int variante = -1;   // -1: sin variante; 0 o 1: cual mitad del par le toco
public:
	CEnemigo(short id, short tipo, int fila, int columna, int vidas);
	~CEnemigo();
	void setTipo(ETipoTanque tipo);
	ETipoTanque getTipo();
	void dibujar(Graphics^ g, Bitmap^ bmpTanques);
	void animar();
	//
	void setEstrategia(Estrategia* estrategia);
	EstrategiaCombinada* getCombinada() { return combinada; }
	int  getVariante() { return variante; }
	void setVariante(int v) { variante = v; }
	//
	void mover(CMapa* mapa);            // delega en la estrategia
};