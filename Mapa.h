#pragma once
#include <iostream>
#include "Bloque.h"

using namespace System::Drawing;

class CMapa {
  CBloque* mapa[MAX_FILAS][MAX_COLUMNAS];
  int nivel;

  public:
	CMapa();
	void setNivel(int nivel);
	int getNivel();
	CBloque* getBloque(int i, int j);
	CBloque* (*CMapa::getMapa())[MAX_COLUMNAS];
	void nivel1();
	void nivel2();
	void DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo);
};

