#include "Controladora.h"

CControladora::CControladora() {
	mapa = new CMapa();

}

void CControladora::Inicializar() {
	//mapa->nivel1();
	mapa->nivel2();
	jugador = new CJugador(1,10,4,3); ;
	enemigos = new CEnemigo(2, ETipoTanque::tipo1, 0, 7, 1);
};

void CControladora::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo) {
	mapa->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo);
};

//dibujar tanques
void CControladora::DibujarTanques(Graphics^ g, Bitmap^ bmpTanques) {
	jugador->dibujar(g, bmpTanques);
	enemigos->dibujar(g, bmpTanques);
};