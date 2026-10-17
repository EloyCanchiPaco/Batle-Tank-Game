#pragma once
#define MAX_FILAS 13
#define MAX_COLUMNAS 14

#define ANCHO_IMAGEN 64
#define ALTO_IMAGEN 64

enum ETipoBloque {
	piso = 1,
	destruible,
	noDestruible,
	agua,
	arbol,
	trofeo
};
enum EMitadBloque {
	mitadCompleto = 0,
	mitadArriba,
	mitadAbajo,
	mitadIzquierda,
	mitadDerecha
};
//creamos una enumeracion para cambiar de piso 
enum ETipoPiso {
	tierraOscura=0,
	TierraClara =1,
	pastoLiso=2,
	pastoDetalles=3,
	pisoOscuro=4,
};

enum EDireccion {
	ninguno,
	arriba,
	abajo,
	izquierda,
	derecha
};

class CComun {

};

