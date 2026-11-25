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

	CSonido::getInstancia().iniciar();
}
//destructor
CControladora::~CControladora() {
	// (el sonido NO se cierra aqui: el menu tambien lo usa y CSonido se cierra solo al terminar el programa)
	EstadoPartida::getInstancia().quitarObservador(hud);
	delete hud;
	delete Enemigos;
	delete jugador;
	delete trofeo;
	delete balas;
	delete cadenaJugador;       // borra tambien los eslabones siguientes
	delete cadenaEnemigos;
	delete mapa;
}

void CControladora::Inicializar(int nivel) {
	derrota = false;
		derrota = false;

	// un piso distinto por nivel (posicion 0 = nivel 1, ... posicion 4 = nivel 5)
	ETipoPiso pisos[] = {
		ETipoPiso::TierraClara,     // nivel 1
		ETipoPiso::tierraOscura,    // nivel 2
		ETipoPiso::pastoLiso,       // nivel 3
		ETipoPiso::pastoDetalles,   // nivel 4
		ETipoPiso::pisoOscuro       // nivel 5
	};
	int indice = (nivel >= 1 && nivel <= 5) ? nivel - 1 : 0;   // evita salirse de la tabla

	mapa->cargarNivel(nivel, pisos[indice]);
	Enemigos->setColision(cadenaEnemigos, &contexto);
	Enemigos->iniciarNivel(mapa, 12, 4, jugador);
	EstadoPartida::getInstancia().nuevaPartida(mapa->getNivel(), jugador->getVidas(), Enemigos->getRestantes());
	CSonido::getInstancia().musica(true);

}
void CControladora::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo, Bitmap^ bmpTrofeoD) {
	mapa->DibujarMapa(g, bmpPiso, bmpBloques, bmpTrofeo, bmpTrofeoD);
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
		CSonido::getInstancia().motor(true);
	}
	else {
		CSonido::getInstancia().motor(false);    // choco contra algo
	}
}

// Ninguna tecla presionada: silenciar el motor
void CControladora::detenerJugador() {
	CSonido::getInstancia().motor(false);
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
	recogerDisparos();
	int antes = Enemigos->getRestantes();
	balas->actualizar(mapa, jugador, Enemigos);
	reproducirSonidos();
	actualizarEstado(antes);
}

void CControladora::recogerDisparos() {
	jugador->actualizarDisparo();
	if (CBala* b = jugador->sacarBala(true, 5)) balas->Agregar(b);

	for (int i = 0; i < Enemigos->getCantidad(); i++) {
		CEnemigo* e = Enemigos->getEnemigo(i);
		e->actualizarDisparo();
		if (CBala* be = e->sacarBala(false, 20)) balas->Agregar(be);
	}
}

void CControladora::reproducirSonidos() {
	CSonido& sonido = CSonido::getInstancia();
	for (EEventoBala ev : balas->getEventos()) {
		switch (ev) {
		case EEventoBala::disparoJugador:
		case EEventoBala::disparoEnemigo:    sonido.disparo();   break;
		case EEventoBala::ladrilloGolpeado:
		case EEventoBala::enemigoGolpeado:
		case EEventoBala::jugadorGolpeado:   sonido.golpe();     break;
		case EEventoBala::enemigoDestruido:  sonido.explosion(); break;
		case EEventoBala::trofeoDestruido:   sonido.explosion(); derrota = true; break;   // sin trofeo se pierde
		}
	}
	balas->getEventos().clear();
}

void CControladora::actualizarEstado(int enemigosAntes) {
	EstadoPartida& estado = EstadoPartida::getInstancia();
	int muertos = enemigosAntes - Enemigos->getRestantes();
	if (muertos > 0) estado.sumarPuntos(muertos * 100);
	estado.setEnemigosRestantes(Enemigos->getRestantes());
	estado.setVidas(jugador->getVidas());
	if (jugador->getVidas() <= 0) derrota = true;      // sin vidas se pierde
}

bool CControladora::haPerdido() {
	return derrota;
}

void CControladora::DibujarBalas(Graphics^ g, Bitmap^ bmpBala, Bitmap^ bmpExplosion) {
	balas->dibujar(g, bmpBala, bmpExplosion);
}
void CControladora::DibujarHUD(Graphics^ g) {
	hud->dibujar(g);
}




//arquirectura tipo monolito ->juego 
//practica final,no es codigo ,hacer diagrama separando las clases especificas ,diagrama completo con las capaz despectivas (presentacion y negicion) 2 layer 
//practica  7:Diagrama de clases completo 
// incluir m-LAy o MVC
//examen final juego con 6 patrones de dise o