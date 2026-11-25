#include "EstadoJuego.h"
#include "EstadoMenu.h"
#include "EstadoGameOver.h"
#include "JuegoFacade.h"
#include "Sonido.h"

// ===========================================================================
//  ContextoJuego
// ===========================================================================
ContextoJuego::ContextoJuego(CJuegoFacade* juego, Recursos* recursos)
	: actual(nullptr), juego(juego), recursos(recursos), nivel(1), ultimoPuntaje(0), record(20000) {
	estados[(int)EEstado::menu] = new EstadoMenu(this);
	estados[(int)EEstado::jugando] = new EstadoJugando(this);
	estados[(int)EEstado::gameOver] = new EstadoGameOver(this);
}

ContextoJuego::~ContextoJuego() {
	if (actual != nullptr) actual->salir();
	for (int i = 0; i < 3; i++) delete estados[i];
}

void ContextoJuego::cambiarA(EEstado nuevo) {
	if (actual != nullptr) actual->salir();
	actual = estados[(int)nuevo];
	actual->entrar();
}

void ContextoJuego::actualizar() {
	if (actual != nullptr) actual->actualizar();
}

void ContextoJuego::dibujar(Graphics^ g, int ancho, int alto) {
	if (actual != nullptr) actual->dibujar(g, ancho, alto);
}

void ContextoJuego::teclaPresionada(ETecla tecla) {
	if (actual != nullptr) actual->teclaPresionada(tecla);
}

void ContextoJuego::teclaSoltada(ETecla tecla) {
	if (actual != nullptr) actual->teclaSoltada(tecla);
}

void ContextoJuego::registrarPuntaje(int puntos) {
	ultimoPuntaje = puntos;
	if (puntos > record) record = puntos;
}

// ===========================================================================
//  EstadoJugando
// ===========================================================================
static const int TICKS_ANTES_DE_GAME_OVER = 15;     // ~1,5 s para ver la explosion del trofeo

EstadoJugando::EstadoJugando(ContextoJuego* contexto)
	: EstadoJuego(contexto), arriba(false), abajo(false), izquierda(false), derecha(false), esperaFin(0) {
}

void EstadoJugando::entrar() {
	arriba = abajo = izquierda = derecha = false;
	esperaFin = 0;
	ctx->getJuego()->Iniciar(ctx->getNivel());        // partida nueva (descarta la anterior)
}

void EstadoJugando::salir() {
	CSonido::getInstancia().musica(false);
	CSonido::getInstancia().motor(false);
}

void EstadoJugando::actualizar() {
	CJuegoFacade* juego = ctx->getJuego();

	if (!juego->HaPerdido()) {
		if (arriba)         juego->MoverJugador(EDireccion::arriba);
		else if (abajo)     juego->MoverJugador(EDireccion::abajo);
		else if (izquierda) juego->MoverJugador(EDireccion::izquierda);
		else if (derecha)   juego->MoverJugador(EDireccion::derecha);
		else                juego->DetenerJugador();
	}
	else {
		juego->DetenerJugador();
	}

	juego->MoverEnemigos();
	juego->ActualizarBalas();

	// perdio: se deja correr un momento la escena y luego pasa a GAME OVER
	if (juego->HaPerdido()) {
		if (++esperaFin >= TICKS_ANTES_DE_GAME_OVER)
			ctx->cambiarA(EEstado::gameOver);            // (siempre al final: el estado cambia)
	}
}

void EstadoJugando::dibujar(Graphics^ g, int ancho, int alto) {
	Recursos* r = ctx->getRecursos();
	g->Clear(Color::Black);
	ctx->getJuego()->Dibujar(g, r->piso, r->bloques, r->trofeo, r->tanques, r->bala, r->explosion, r->trofeoD);
}

void EstadoJugando::teclaPresionada(ETecla tecla) {
	switch (tecla) {
	case ETecla::arriba:    arriba = true;    break;
	case ETecla::abajo:     abajo = true;     break;
	case ETecla::izquierda: izquierda = true; break;
	case ETecla::derecha:   derecha = true;   break;
	case ETecla::disparo:
		if (!ctx->getJuego()->HaPerdido()) ctx->getJuego()->Disparar();
		break;
	case ETecla::volver:    ctx->cambiarA(EEstado::menu); break;   // salir al menu
	default: break;
	}
}

void EstadoJugando::teclaSoltada(ETecla tecla) {
	switch (tecla) {
	case ETecla::arriba:    arriba = false;    break;
	case ETecla::abajo:     abajo = false;     break;
	case ETecla::izquierda: izquierda = false; break;
	case ETecla::derecha:   derecha = false;   break;
	default: break;
	}
}
