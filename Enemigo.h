#pragma once
#include "Tanque.h"

enum ETipoTanque {
	tipo1=0,
	tipo2=4,
	tipo3=8,
	tipo4=12,
	tipo5=16
};

class CEnemigo: public CTanque {
	ETipoTanque tipo;
public:
	CEnemigo(short id,short tipo, int fila, int columna, int vidas);
	void setTipo(ETipoTanque tipo);
	ETipoTanque getTipo();
	void dibujar(Graphics^ g, Bitmap^ bmpTanques);
};
