#include "Controladora.h"
#include "EnemigoFactory.h"
#include "Sonido.h"

CControladora::CControladora() : mapa(nullptr), jugador(nullptr), Enemigos(nullptr), trofeo(nullptr),
balas(nullptr), cadenaJugador(nullptr), cadenaEnemigos(nullptr), hud(nullptr) {
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
	balas = new CColeccionBalas();
	jugador->setDireccion(EDireccion::arriba);        // para poder disparar antes de moverse

	// ---- Cadena de colisiones ----
	// 1) el contexto: lo que los eslabones necesitan conocer
	contexto.mapa = mapa;
	contexto.jugador = jugador;
	contexto.enemigos = Enemigos;

	// 2) cadena de los enemigos: Limites -> Bloques -> Tanques
	cadenaEnemigos = new ColisionLimites();
	cadenaEnemigos->setSiguiente(new ColisionBloques(false))->setSiguiente(new ColisionTanques());

	// 3) cadena del jugador: igual, pero puede pisar el trofeo
	cadenaJugador = new ColisionLimites();
	cadenaJugador->setSiguiente(new ColisionBloques(true, 2))->setSiguiente(new ColisionTanques());

	// 4) el jugador usa su cadena
	jugador->setColision(cadenaJugador, &contexto);
	hud = new CPanelHUD();
	EstadoPartida::getInstancia().agregarObservador(hud);   // el panel se suscribe

	Sonido::iniciar();
}
//
CControladora::~CControladora() {
	Sonido::cerrar();
	EstadoPartida::getInstancia().quitarObservador(hud);
	delete hud;
	delete mapa;
	delete jugador;
	delete Enemigos;
	delete trofeo;
	delete balas;
	delete cadenaJugador;       // borra tambien los eslabones siguientes
	delete cadenaEnemigos;
}

void CControladora::Inicializar() {

	mapa->nivel2(ETipoPiso::TierraClara); //TierraClara, tierraOscura, pastoLiso, pastoDetalles, pisoOscuro
	Enemigos->setColision(cadenaEnemigos, &contexto);
	Enemigos->iniciarNivel(mapa, 12, 4, jugador);
	EstadoPartida::getInstancia().nuevaPartida(mapa->getNivel(), jugador->getVidas(), Enemigos->getRestantes());
	Sonido::inicioNivel;

}
void CControladora::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo, Bitmap^ bmpTrofeoD) {
	mapa->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo,bmpTrofeoD);
};

//dibujar tanques
void CControladora::DibujarTanques(Graphics^ g, Bitmap^ bmpTanques) {
	jugador->dibujar(g, bmpTanques);
	Enemigos->dibujar(g, bmpTanques);
};

void CControladora::setDireccion(EDireccion nueva) {
	jugador->setDireccion(nueva);
}

void CControladora::moverJugador() {
	if (jugador->puedeAvanzar()) { // pregunta a la cadena de colisiones
		jugador->mover();
		jugador->animar();
		Sonido::motor(true);
	}
	else {
		Sonido::motor(false);   // choco contra algo
	}
}

// Ninguna tecla presionada: silenciar el motor
void CControladora::detenerJugador() {
	Sonido::motor(false);
}

void CControladora::moverEnemigos() {
	Enemigos->actualizarGeneracion();
	for (int i = 0; i < Enemigos->getCantidad(); i++)
		Enemigos->getEnemigo(i)->mover(mapa);
}

//funcion para disparar del jugador
void CControladora::jugadorDispara() {
	jugador->disparar();
}

void CControladora::actualizarBalas() {
	// 1) recoger los disparos pedidos
	jugador->actualizarDisparo();
	CBala* b = jugador->sacarBala(true, 5.5);
	if (b) { balas->Agregar(b); Sonido::disparo(); }

	for (int i = 0; i < Enemigos->getCantidad(); i++) {
		CEnemigo* e = Enemigos->getEnemigo(i);
		e->actualizarDisparo();
		CBala* be = e->sacarBala(false, 20);
		if (be) { balas->Agregar(be); Sonido::disparo(); }
	}
	// 2) mover las balas y resolver choques
	int antes = Enemigos->getRestantes();
	int vidasAntes = jugador->getVidas();
	balas->actualizar(mapa, jugador, Enemigos);

	// 3) avisar al estado de la partida (solo notifica si algo cambio)
	// 3) avisar al estado de la partida y a los sonidos
	EstadoPartida& estado = EstadoPartida::getInstancia();
	int muertos = antes - Enemigos->getRestantes();
	if (muertos > 0) {
		estado.sumarPuntos(muertos * 100);
		             
	}
	if (jugador->getVidas() < vidasAntes) Sonido::golpe();   
	estado.setEnemigosRestantes(Enemigos->getRestantes());
	estado.setVidas(jugador->getVidas());
}

void CControladora::DibujarBalas(Graphics^ g, Bitmap^ bmpBala, Bitmap^ bmpExplosion) {
	balas->dibujar(g, bmpBala,bmpExplosion);
}
void CControladora::DibujarHUD(Graphics^ g) {
	hud->dibujar(g);
}




//arquirectura tipo monolito ->juego 
//practica final,no es codigo ,hacer diagrama separando las clases especificas ,diagrama completo con las capaz despectivas (presentacion y negicion) 2 layer 
//practica  7:Diagrama de clases completo 
// incluir m-LAy o MVC
//examen final juego con 6 patrones de diseño