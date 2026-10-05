#pragma once
#include <iostream>
#include "Bloque.h"

using namespace System::Drawing;

class CMapa {
  CBloque* mapa[MAX_FILAS][MAX_COLUMNAS];
  int nivel;
  ETipoPiso tipoPiso;

  public:
	CMapa();
	void setNivel(int nivel);	
	int getNivel();
	CBloque* getBloque(int i, int j);
	CBloque* (*CMapa::getMapa())[MAX_COLUMNAS];
	void nivel1(ETipoPiso PisoDeseado);
	void nivel2(ETipoPiso PisoDeseado);
	void nivel3(ETipoPiso PisoDeseado);
	void nivel4(ETipoPiso PisoDeseado);
	void nivel5(ETipoPiso PisoDeseado);	
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo );
};

