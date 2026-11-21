#pragma once
#include "Comun.h"
#include "Bala.h"
class Colision;
struct ContextoColision;
using namespace System::Drawing;
class CTanque {
	short id;
	short vidas;
	EDireccion direccion;
	short velocidad;
	int posX;
	int posY;
	short powerups[99];
	bool quiereDisparar = false;         // NUEVO (miembros privados)
	int enfriamiento = 0;
	Colision* colision = nullptr;            // cadena de colisiones (no es dueno)
	ContextoColision* contexto = nullptr;
public:
	CTanque(short id, int posX, int posY, int vidas);
	~CTanque();
	void setId(short id);
	void setVidas(short vidas);
	void setDireccion(EDireccion direccion);
	void setVelocidad(short velocidad);
	void setX(int posX);
	void setY(int posY);
	void mover();
	short getId();
	short getVidas();
	EDireccion getDireccion();
	short getVelocidad();
	int getX();
	int getY();
	void disparar();                     
	CBala* sacarBala(bool delJugador, int espera);
	void actualizarDisparo();           
	void setColision(Colision* cadena, ContextoColision* ctx);
	bool puedeAvanzar();                     // consulta la cadena
	void alinearACelda();
	bool centrarParaAvanzar(EDireccion d);
	bool puedeDisparar() { return enfriamiento == 0; }
	ContextoColision* getContexto() { return contexto; }
};

