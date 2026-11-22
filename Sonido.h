#pragma once

class Sonido {
public:
	static void iniciar();                 // abre los .wav (se puede llamar varias veces)
	static void cerrar();                  // libera los sonidos

	static void disparo();                 // el jugador dispara
	static void explosion();               // un enemigo destruido
	static void golpe();                   // el jugador recibe un impacto
	static void inicioNivel();             // empieza el nivel
	static void motor(bool moviendose);    // sonido continuo mientras el jugador se mueve
};
