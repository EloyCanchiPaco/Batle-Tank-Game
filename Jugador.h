#pragma once
#include <string>
#include "Tanque.h"

class CJugador: public CTanque {
	std::string nombre;
	int puntuacion;
	int imagenX = 0; // Para animación de movimiento
public:
	void setNombre(std::string nombre);
	void setPuntuacion(int puntuacion);
	std::string getNombre();
	int getPuntuacion();

	void dibujar(Graphics^ g, Bitmap^ bmpTanques);
	CJugador(short id, int fila, int columna, int vidas);
	void mover();
	void animar();

};

