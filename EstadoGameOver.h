#pragma once
#include "EstadoJuego.h"

// ===========================================================================
//  EstadoGameOver: se llega aqui cuando el jugador se queda sin vidas o
//  destruyen el trofeo. Muestra la ultima escena oscurecida, el texto
//  "GAME OVER" sube desde abajo y luego aparecen REINTENTAR / MENU,
//  que se eligen moviendo el tanque.
// ===========================================================================
class EstadoGameOver : public EstadoJuego {
	int cursor;                          // 0 = REINTENTAR, 1 = MENU
	int tick;                            // ticks desde que empezo
	int tanqueY;                         // posicion del tanque (pixeles de pantalla)
	bool moviendo;
	msclr::gcroot<Bitmap^> foto;         // la ultima escena, congelada

	bool listo() const;                  // ya termino de subir el texto
	int yObjetivoTanque() const;
	void aceptar();
public:
	EstadoGameOver(ContextoJuego* contexto);

	void entrar() override;
	void salir() override;
	void actualizar() override;
	void dibujar(Graphics^ g, int ancho, int alto) override;
	void teclaPresionada(ETecla tecla) override;
};
