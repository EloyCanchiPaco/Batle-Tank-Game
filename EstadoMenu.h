#pragma once
#include "EstadoJuego.h"

// ===========================================================================
//  EstadoMenu: UN solo estado que contiene todas las paginas del menu.
//  Pagina actual (enum) + tablas de opciones: no hace falta una clase por pagina.
//
//     principal : JUGAR / NIVELES / OPCIONES / CREDITOS
//     niveles   : NIVEL 1..5 / VOLVER          (elegir un nivel empieza la partida)
//     opciones  : MUSICA / EFECTOS / VOLVER    (izquierda-derecha cambian el volumen)
//     creditos  : integrantes / VOLVER
//
//  El titulo se arma con el ladrillo destruible de bloques.png y el tanque del
//  jugador (tanques.png) es el cursor que se mueve entre las opciones.
// ===========================================================================
class EstadoMenu : public EstadoJuego {
	enum class Pagina { principal, niveles, opciones, creditos };

	Pagina pagina;
	int cursor;                         // opcion elegida dentro de la pagina
	int tanqueY;                        // posicion del tanque (en pixeles virtuales)
	int tick;                           // cuenta ticks: anima las orugas
	EDireccion dirTanque;               // hacia donde mira el tanque
	bool moviendo;

	msclr::gcroot<Bitmap^> ladrillo;    // el ladrillo recortado de bloques.png

	// el menu se dibuja en un lienzo virtual de 256x240 que se escala en enteros y se centra
	int k, ox, oy;                      // factor, origen X e Y del lienzo en la ventana
	int X(int v) const { return ox + v * k; }
	int Y(int v) const { return oy + v * k; }

	// ---- tablas de cada pagina ----
	int cantidad() const;               // cuantas opciones tiene la pagina
	const char* etiqueta(int i) const;  // texto de la opcion i
	int yLista() const;                 // y de la primera opcion
	int yObjetivoTanque() const;        // donde debe estar el tanque para la opcion elegida

	// ---- acciones ----
	void irA(Pagina nueva, int cursorNuevo);
	void aceptar();
	void volver();
	void ajustar(int delta);            // izquierda/derecha (solo en opciones)

	// ---- dibujo ----
	void dibujarCabecera(Graphics^ g);
	void dibujarTitulo(Graphics^ g);
	void dibujarPagina(Graphics^ g);
	void dibujarVolumen(Graphics^ g, int y, float volumen);
	void dibujarTanque(Graphics^ g);
	void dibujarAyuda(Graphics^ g);
public:
	EstadoMenu(ContextoJuego* contexto);

	void entrar() override;
	void salir() override;
	void actualizar() override;
	void dibujar(Graphics^ g, int ancho, int alto) override;
	void teclaPresionada(ETecla tecla) override;
};
