#pragma once
#include <vector>
#include "Enemigo.h"
#include "Mapa.h"
#include "Colision.h"

class CColeccionEnemigos {
    std::vector<CEnemigo*> enemigos;

    // ---- oleadas: enemigos que faltan por aparecer ----
    CMapa* mapa = nullptr;
    CTanque* jugador = nullptr;
    int filaJugador = 0, columnaJugador = 0;
    int nivel = 1;
    ETipoTanque tipoNivel = tipo1;
    int pendientes = 0;          // faltan por aparecer
    int maxEnPantalla = 5;       // maximo de enemigos vivos a la vez
    int espera = 0;              // ticks hasta el proximo que aparece
    int idSiguiente = 0;         // los ids no se repiten aunque mueran enemigos
    int ultimaVariante = 1;      // para desempatar el reparto del par de estrategias

    // cadena de colisiones que reciben los enemigos al aparecer
    Colision* cadena = nullptr;
    ContextoColision* contexto = nullptr;

    bool celdaOcupada(int fila, int columna);
    bool buscarCelda(int& fila, int& columna);
    bool aparecerUno();
public:
    ~CColeccionEnemigos();
    void Agregar(CEnemigo* Enemigo);
    void EliminarPorId(short id);
    CEnemigo* getEnemigo(int indice);
    int getCantidad();                       // enemigos vivos en pantalla
    int getRestantes();                      // vivos + los que faltan por aparecer
    void dibujar(Graphics^ g, Bitmap^ bmpTanques);

    void setColision(Colision* cadena, ContextoColision* contexto);
    void iniciarNivel(CMapa* mapa, int filaJugador, int columnaJugador, CTanque* jugador,int total);
    void actualizarGeneracion();
};