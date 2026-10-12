#include "Controladora.h"
#include "EnemigoFactory.h"
CControladora::CControladora() {
	mapa = new CMapa();
	jugador = nullptr;
	Enemigos = nullptr; 
	if (trofeo == NULL) {
		trofeo = new CBloque(ETipoBloque::trofeo,2);
	}

}
//
CControladora::~CControladora() {
	delete mapa;
	delete jugador;
	delete Enemigos;
}

void CControladora::Inicializar() {
	//mapa->nivel1();
	mapa->nivel4(ETipoPiso::pastoDetalles); //TierraClara, tierraOscura, pastoLiso, pastoDetalles, pisoOscuro
	jugador = new CJugador(1, 10, 4, 3); //id, fila de aparicion, columna de aparicion, vidas
	Enemigos = new CColeccionEnemigos();
	Enemigos->Agregar(CEnemigoFactory::CrearEnemigo(ETipoTanque::tipo1, 2, 0, 1));   // fila 0, columna 1
	Enemigos->Agregar(CEnemigoFactory::CrearEnemigo(ETipoTanque::tipo2, 3, 0, 7));  //id, tipo, fila de aparicion, columna de aparicion, vidas

};

void CControladora::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo) {
	mapa->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo);
};

//dibujar tanques
void CControladora::DibujarTanques(Graphics^ g, Bitmap^ bmpTanques) {
	jugador->dibujar(g, bmpTanques);
		Enemigos->dibujar(g, bmpTanques);
	
};