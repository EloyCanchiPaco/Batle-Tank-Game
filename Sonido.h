#pragma once

// Estructura con los detalles de Windows (hilo, cola, dispositivos de audio).
// Solo se define en Sonido.cpp, asi este header no depende de windows.h
struct DatosSonido;

// Singleton: un unico objeto de sonido para todo el juego.
// Uso:  CSonido::getInstancia().disparo();
class CSonido {
public:
	static CSonido& getInstancia();

	void iniciar();                        // abre los .wav (se puede llamar varias veces)
	void cerrar();                         // libera los sonidos

	// efectos puntuales
	void disparo();                        // se creo una bala
	void explosion();                      // un enemigo o el trofeo destruido
	void golpe();                          // impacto de una bala

	// sonidos continuos
	void motor(bool moviendose);           // mientras el jugador se mueve
	void musica(bool sonar);               // musica de fondo en bucle (durante la partida)
	void musicaMenu(bool sonar);           // musica del menu (inicio.wav) en bucle

	// volumen: 0.0 a 1.0 (para el menu)
	void  setVolumenMusica(float v);
	void  setVolumenEfectos(float v);      // efectos y motor
	float getVolumenMusica() const;
	float getVolumenEfectos() const;

private:
	CSonido();                             // constructor privado: nadie crea otros objetos
	~CSonido();
	CSonido(const CSonido&) = delete;      // no se puede copiar
	CSonido& operator=(const CSonido&) = delete;

	DatosSonido* d;
};