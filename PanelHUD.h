#pragma once
#include "Observador.h"
#include "Comun.h"
using namespace System::Drawing;

// Panel de datos a la derecha del mapa. Es OBSERVADOR de EstadoPartida:
// cuando algo cambia, recibe actualizar() y guarda los valores para dibujarlos.
class CPanelHUD : public Observador {
	int puntuacion = 0;
	int nivel = 1;
	int vidas = 0;
	int enemigos = 0;
public:
	static const int ANCHO_PANEL = 400;
	static const int ANCHO_TITULO = 170;     // franja negra con el titulo, a la derecha del panel

	CPanelHUD();
	void actualizar() override;          // lo llama EstadoPartida al cambiar algo
	void dibujar(Graphics^ g);
};