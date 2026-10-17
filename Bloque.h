#pragma once
#include "Comun.h"

class CBloque {
	int posX;
	int posY;
	ETipoBloque tipo;
	short durabilidad;
	EMitadBloque mitad;


  public:
	CBloque(ETipoBloque tipo, int dureza, EMitadBloque mitad = EMitadBloque::mitadCompleto);
	~CBloque();

	ETipoBloque getTipo();
	void setDurabilidad(short durabilidad);
	short getDurabilidad();
	EMitadBloque getMitad();
	void setMitad(EMitadBloque mitad);


};
