#include "Controladora.h"
#include "EnemigoFactory.h"
CControladora::CControladora() : mapa(nullptr), jugador(nullptr), Enemigos(nullptr), trofeo(nullptr) {
	srand((unsigned)time(nullptr));
	std::cout << "Mapa: " << mapa << std::endl;
	if (mapa == nullptr) {
		mapa = new CMapa();
	}
	if (jugador == nullptr) {
		jugador = new CJugador(1, 12, 4, 3);
	}
	if (trofeo == nullptr) {
		trofeo = new CBloque(ETipoBloque::trofeo, 2);
	}
	Enemigos = new CColeccionEnemigos();
}
//
CControladora::~CControladora() {
	delete mapa;
	delete jugador;
	delete Enemigos;
	delete trofeo;
}

void CControladora::Inicializar() {

	mapa->nivel2(ETipoPiso::TierraClara); //TierraClara, tierraOscura, pastoLiso, pastoDetalles, pisoOscuro
	Enemigos->generarPorNivel(mapa, 12, 4, jugador);

}
void CControladora::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo) {
	mapa->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo);
};

//dibujar tanques
void CControladora::DibujarTanques(Graphics^ g, Bitmap^ bmpTanques) {
	jugador->dibujar(g, bmpTanques);
		Enemigos->dibujar(g, bmpTanques);
	
};
void CControladora::setDireccion(EDireccion direccion) {
	jugador->setDireccion(direccion);
}
void CControladora::moverJugador() {

	jugador->mover();
}
void CControladora::moverEnemigos() {
	for (int i = 0; i < Enemigos->getCantidad(); i++)
		Enemigos->getEnemigo(i)->mover(mapa);
}

