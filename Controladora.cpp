#include "Controladora.h"
#include "EnemigoFactory.h"
CControladora::CControladora() : mapa(nullptr), jugador(nullptr), Enemigos(nullptr), trofeo(nullptr) {
	srand((unsigned)time(nullptr));
	std::cout << "Mapa: " << mapa << std::endl;
	if (mapa == nullptr) {
		mapa = new CMapa();
	}
	if (jugador == nullptr) {
		jugador = new CJugador(1, 4, 10, 3);
	}
	if (trofeo == nullptr) {
		trofeo = new CBloque(ETipoBloque::trofeo, 2);
	}

}
//
CControladora::~CControladora() {
	delete mapa;
	delete jugador;
	delete Enemigos;
}

void CControladora::Inicializar() {
	
	mapa->nivel3(ETipoPiso::pisoOscuro); //TierraClara, tierraOscura, pastoLiso, pastoDetalles, pisoOscuro
	jugador = new CJugador(1, 12, 4, 3); //id, fila de aparicion, columna de aparicion, vidas
	Enemigos = new CColeccionEnemigos();
	// crear enemigos con for deacuredo al nivel
	ETipoTanque tipos[] = { tipo1, tipo2, tipo3, tipo4, tipo5 };
	int nivel = mapa->getNivel();
	if (nivel < 1) nivel = 1;            // protección si el nivel no se asignó
	if (nivel > 5) nivel = 5;
	ETipoTanque tipoNivel = tipos[nivel - 1];
	int cantidades[] = {4, 5, 6, 7, 8};
	int cantidad = cantidades[nivel - 1];

	for (int i = 0; i < cantidad; i++) {
		int fila = -1, columna = -1;

		for (int intento = 0; intento < 500; intento++) {   // límite de intentos
			int f = rand() % MAX_FILAS;
			int c = rand() % MAX_COLUMNAS;
			CBloque* b = mapa->getBloque(f, c);

			if (b != nullptr && b->getTipo() == ETipoBloque::piso) {
				fila = f;
				columna = c;
				break;
			}
		}

		if (fila == -1) continue;   // no se encontró celda libre, se omite este enemigo

		Enemigos->Agregar(CEnemigoFactory::CrearEnemigo(tipoNivel, i, fila, columna));
	}
};

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

//hacer mover los enemigos con animacion,poner la posicion rand del enemigo en coleccion de enemigos 
//practica 5 aplcicar un patron de diseño estructural (bringe,facde,proxy,decorator, flyweight),forzar aplicacion
