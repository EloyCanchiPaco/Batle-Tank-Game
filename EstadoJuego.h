#pragma once
#include <msclr/gcroot.h>
#include "Comun.h"

using namespace System::Drawing;

class CJuegoFacade;
class ContextoJuego;

// ===== PATRON STATE =====
//   EstadoJuego   : interfaz comun de todos los estados (menu, jugando, game over)
//   ContextoJuego : guarda el estado actual y cambia entre ellos. El Form (Juego.h)
//                   solo le habla al contexto, nunca sabe en que estado esta.
// Cada estado decide por si mismo que hace con el tick, el dibujo y las teclas,
// y pide el cambio con ctx->cambiarA(...).

// Teclas ya traducidas: los estados no dependen de WinForms
enum class ETecla { ninguna, arriba, abajo, izquierda, derecha, aceptar, disparo, volver };

// Los estados que existen
enum class EEstado { menu, jugando, gameOver };

// Las imagenes del juego, compartidas por todos los estados (las carga el Form una sola vez)
struct Recursos {
	msclr::gcroot<Bitmap^> piso, bloques, trofeo, tanques, bala, explosion, trofeoD;
};

// ---------------------------------------------------------------------------
// State (clase abstracta)
// ---------------------------------------------------------------------------
class EstadoJuego {
protected:
	ContextoJuego* ctx;
public:
	EstadoJuego(ContextoJuego* contexto) : ctx(contexto) {}
	virtual ~EstadoJuego() {}

	virtual void entrar() {}                                  // al pasar a este estado
	virtual void salir() {}                                   // al dejar este estado
	virtual void actualizar() = 0;                            // un tick de logica
	virtual void dibujar(Graphics^ g, int ancho, int alto) = 0;   // ancho/alto de la ventana
	virtual void teclaPresionada(ETecla tecla) {}
	virtual void teclaSoltada(ETecla tecla) {}
};

// ---------------------------------------------------------------------------
// Context
// ---------------------------------------------------------------------------
class ContextoJuego {
	EstadoJuego* estados[3];              // uno por cada EEstado (se crean una sola vez)
	EstadoJuego* actual;

	CJuegoFacade* juego;                  // no es dueno
	Recursos* recursos;                   // no es dueno

	// datos que se comparten entre estados
	int nivel;                            // nivel de la partida en curso (o la ultima)
	int ultimoPuntaje;
	int record;
public:
	ContextoJuego(CJuegoFacade* juego, Recursos* recursos);
	~ContextoJuego();

	void cambiarA(EEstado nuevo);

	// el Form reenvia todo al estado actual
	void actualizar();
	void dibujar(Graphics^ g, int ancho, int alto);
	void teclaPresionada(ETecla tecla);
	void teclaSoltada(ETecla tecla);

	CJuegoFacade* getJuego() { return juego; }
	Recursos* getRecursos() { return recursos; }

	int getNivel() { return nivel; }
	void setNivel(int n) { nivel = n; }
	int getUltimoPuntaje() { return ultimoPuntaje; }
	int getRecord() { return record; }
	void registrarPuntaje(int puntos);    // guarda el puntaje de la partida y actualiza el record
};

// ---------------------------------------------------------------------------
// Estado "jugando": envuelve el juego que ya existia (JuegoFacade)
// ---------------------------------------------------------------------------
class EstadoJugando : public EstadoJuego {
	bool arriba, abajo, izquierda, derecha;   // teclas de movimiento mantenidas
	int esperaFin;                            // ticks que se sigue mostrando la escena tras perder
public:
	EstadoJugando(ContextoJuego* contexto);

	void entrar() override;                   // crea la partida del nivel elegido
	void salir() override;
	void actualizar() override;
	void dibujar(Graphics^ g, int ancho, int alto) override;
	void teclaPresionada(ETecla tecla) override;
	void teclaSoltada(ETecla tecla) override;
};
