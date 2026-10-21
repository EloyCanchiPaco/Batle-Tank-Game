#include "Mapa.h"

CMapa::CMapa() {
	nivel = 1;
	tipoPiso = ETipoPiso::TierraClara;
};

void CMapa::setNivel(int nivel) {
    this->nivel = nivel;
}

int CMapa::getNivel() {
    return this->nivel;
}

CBloque* CMapa::getBloque(int i, int j) {
    return mapa[i][j];
}

CBloque* (*CMapa::getMapa())[MAX_COLUMNAS] {
    return this->mapa;
}

void CMapa::nivel1(ETipoPiso PisoDeseado) {
	this->nivel = 1;
    this->tipoPiso = PisoDeseado;
    int tmpNivel1[MAX_FILAS][MAX_COLUMNAS] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1,
        1, 2, 1, 2, 1, 2, 3, 2, 1, 1, 2, 1, 2, 1,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        3, 1, 2, 2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 3,
        1, 1, 1, 1, 1,22, 1,22, 1, 1, 1, 1, 1, 1,
        1, 2, 1, 2, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1,
        1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1,
        1, 1, 1, 1, 1,22,22,22, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 2, 9, 2, 1, 1, 1, 1, 1, 1
    };

    std::cout << "Inicializando nivel 1\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel1[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            // 2 es un bloque destruible
            if (tmpNivel1[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel1[i][j] == 21) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadArriba);
            }
            if (tmpNivel1[i][j] == 22) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel1[i][j] == 23) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel1[i][j] == 24) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadIzquierda);
            }
            //bloque no destruible
            if (tmpNivel1[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel1[i][j] == 31) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadArriba);
            }
            if (tmpNivel1[i][j] == 32) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel1[i][j] == 33) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadIzquierda);
            }
            if (tmpNivel1[i][j] == 34) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel1[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, 0);
            }
            if (tmpNivel1[i][j] == 5) {
                mapa[i][j] = new CBloque(ETipoBloque::arbol, 0);
            }
            if (tmpNivel1[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }

        }
    }
}

void CMapa::nivel2(ETipoPiso PisoDeseado) {
	this->nivel = 2;
    this->tipoPiso = PisoDeseado;
    int tmpNivel2[MAX_FILAS][MAX_COLUMNAS] = {
     1,  1,  1,  1,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,
    32,  1, 22,  1,  2,  1,  1,  1, 31, 31,  3,  1,  1,  1,
     3,  1,  2,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,
     2,  1,  2,  2,  2,  1,  2,  2,  1,  4,  4,  1,  4,  1,
    21,  1,  1,  1, 21,  1,  1,  1,  1,  4,  1,  1,  1,  1,
     1,  1, 22,  1,  4,  4,  1,  4,  4,  4,  1,  2,  2,  1,
     2,  2,  1,  1,  4,  2,  1,  2, 24,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  4,  1,  1,  1,  1,  1, 34, 33,  1,  1,
     4,  4,  4,  1,  4,  1,  3,  1,  2,  1, 34,  1,  1,  1,
     1,  1,  1, 22, 22,  1,  1,  1,  1,  1, 34,  2,  2,  1,
     1,  1,  1,  1,  2, 21, 21, 21,  2, 22,  1,  1,  1,  1,
     2,  2, 21,  1,  1, 22, 22, 22,  1, 21,  2,  1,  1,  1,
    21,  1,  1,  1,  1, 2,   9,  2,  1,  1,  1,  1,  1,  1
    };

    std::cout << "Inicializando nivel 2\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel2[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
			// 2 es un bloque destruible
            if (tmpNivel2[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel2[i][j] == 21) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadArriba);
            }
            if (tmpNivel2[i][j] == 22) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel2[i][j] == 23) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel2[i][j] == 24) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadIzquierda);
            }
			//bloque no destruible
            if (tmpNivel2[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel2[i][j] == 31) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadArriba);
            }
            if (tmpNivel2[i][j] == 32) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel2[i][j] == 33) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadIzquierda);
            }
            if (tmpNivel2[i][j] == 34) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel2[i][j] == 4) {
				mapa[i][j] = new CBloque(ETipoBloque::agua, 0);
            }
            if (tmpNivel2[i][j] == 5) {
                mapa[i][j] = new CBloque(ETipoBloque::arbol, 0);
            }
            if (tmpNivel2[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }


        }
    }
}
//nivel 3
void CMapa::nivel3(ETipoPiso PisoDeseado) {
	this->nivel = 3;
    this->tipoPiso = PisoDeseado;
    int tmpNivel3[MAX_FILAS][MAX_COLUMNAS] = {
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     5,  5,  1,  1, 22,  2,  2,  2, 22,  1,  1,  5,  5,  1,
     5,  1,  1, 23,  2,  2,  2,  2,  2, 24,  1,  1,  5,  1,
     1,  1,  1,  2,  2,  5,  2,  5,  2,  2,  1,  1,  1,  1,
     1,  1,  1,  2,  5,  5,  2,  5,  5,  2,  1,  1,  1,  1,
     5,  1,  1,  2,  2,  2,  2,  2,  2,  2,  1,  1,  5,  1,
     5,  5,  1,  1,  2,  5,  2,  5,  2,  1,  1,  5,  5,  1,
     4,  4,  4,  1,  2,  2,  2,  2,  2,  1,  4,  4,  4,  1,
     1,  1,  1,  1, 23, 23, 23, 23, 23,  1,  1,  1,  1,  1,
     1,  1,  1,  1, 24, 24, 24, 24, 24,  1,  1,  1,  1,  1,
    34, 34, 34,  1,  1,  1,  1,  1,  1,  1, 33, 33, 33,  1,
    24, 24, 24,  1,  1, 22, 22, 22,  1,  1, 23, 23, 23,  1,
    33, 33, 33, 34,  1,  2,  9,  2,  1, 33, 34, 34, 34,  1
    };

    std::cout << "Inicializando nivel 3\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel3[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            // 2 es un bloque destruible
            if (tmpNivel3[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel3[i][j] == 21) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadArriba);
            }
            if (tmpNivel3[i][j] == 22) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel3[i][j] == 23) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel3[i][j] == 24) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadIzquierda);
            }
            //bloque no destruible
            if (tmpNivel3[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel3[i][j] == 31) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadArriba);
            }
            if (tmpNivel3[i][j] == 32) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel3[i][j] == 33) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadIzquierda);
            }
            if (tmpNivel3[i][j] == 34) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel3[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, 0);
            }
            if (tmpNivel3[i][j] == 5) {
                mapa[i][j] = new CBloque(ETipoBloque::arbol, 0);
            }
            if (tmpNivel3[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }
        }
    }
}

//nivel 4
void CMapa::nivel4(ETipoPiso PisoDeseado) {
	this->nivel = 4;
	this->tipoPiso = PisoDeseado;
    int tmpNivel4[MAX_FILAS][MAX_COLUMNAS] = {
     1,  5,  5,  1,  1,  1,  1,  1,  1,  1,  1,  5,  1,  1,
     5,  5,  1,  1, 22,  2,  2, 22, 22,  1,  1,  1,  5,  1,
     5,  1,  1, 23,  2,  2,  2,  2,  2,  2, 22,  1, 31,  1,
    31,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2, 24,  1,  1,
     1,  1, 23, 21,  1,  1,  1, 21,  2,  2,  1, 24,  1,  1,
     4,  1, 23,  1, 33,  1, 33,  1,  2, 24,  1,  1,  1,  1,
     1,  1,  2,  1, 22, 22,  1,  1,  2, 24,  1,  4,  4,  1,
     1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,
     1, 23,  2,  2,  2,  2,  2,  2,  2,  2, 24,  1,  1,  1,
     1, 21, 21,  2,  2,  2,  2,  2,  2, 21, 21,  1,  1,  1,
     1,  2,  2, 22,  1,  2,  2, 21, 22,  2,  2,  1,  5,  1,
     5,  1, 21, 21,  1, 22, 22, 22, 21, 21,  1,  5,  5,  1,
     3,  5,  1,  1,  1,  2,  9,  2,  1,  1,  5,  5,  3,  1
    };

    std::cout << "Inicializando nivel 4\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel4[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            // 2 es un bloque destruible
            if (tmpNivel4[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel4[i][j] == 21) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadArriba);
            }
            if (tmpNivel4[i][j] == 22) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel4[i][j] == 23) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel4[i][j] == 24) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadIzquierda);
            }
            //bloque no destruible
            if (tmpNivel4[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel4[i][j] == 31) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadArriba);
            }
            if (tmpNivel4[i][j] == 32) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel4[i][j] == 33) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadIzquierda);
            }
            if (tmpNivel4[i][j] == 34) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel4[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, 0);
            }
            if (tmpNivel4[i][j] == 5) {
                mapa[i][j] = new CBloque(ETipoBloque::arbol, 0);
            }
            if (tmpNivel4[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }
        }
    }
}
// nivel 5
void CMapa::nivel5(ETipoPiso PisoDeseado) {
	this->nivel = 5;
	this->tipoPiso = PisoDeseado;
    int tmpNivel5[MAX_FILAS][MAX_COLUMNAS] = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1,
      1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1,
      4, 4, 4, 2, 2, 5, 5, 5, 1, 3, 1, 1, 5, 1,
      4, 4, 4, 2, 2, 5, 5, 5, 1, 3, 1, 1, 5, 1,
      1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1,
      2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1,
      2, 2, 2, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1,
      1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 1, 1,
      1, 1, 2, 1, 1, 2, 9, 2, 1, 2, 1, 2, 1, 1
    };

    std::cout << "Inicializando nivel 5\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel5[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            // 2 es un bloque destruible
            if (tmpNivel5[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel5[i][j] == 21) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadArriba);
            }
            if (tmpNivel5[i][j] == 22) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel5[i][j] == 23) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel5[i][j] == 24) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2, EMitadBloque::mitadIzquierda);
            }
            //bloque no destruible
            if (tmpNivel5[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel5[i][j] == 31) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadArriba);
            }
            if (tmpNivel5[i][j] == 32) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadAbajo);
            }
            if (tmpNivel5[i][j] == 33) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadIzquierda);
            }
            if (tmpNivel5[i][j] == 34) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1, EMitadBloque::mitadDerecha);
            }
            if (tmpNivel5[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, 0);
            }
            if (tmpNivel5[i][j] == 5) {
                mapa[i][j] = new CBloque(ETipoBloque::arbol, 0);
            }
            if (tmpNivel5[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }
        }
    }
}



void CMapa::DibujarMapa(Graphics^ g, Bitmap^ bmpPiso, Bitmap^ bmpBloques, Bitmap^ bmpTrofeo ) {
    int X = 0, Y = 0;
    int columna = this->tipoPiso % 2;
	int fila = this->tipoPiso / 2;

    for (int i = 0; i < MAX_FILAS; i++) {
        X = 0;

        for (int j = 0; j < MAX_COLUMNAS; j++) {
            CBloque* b = getBloque(i, j);

            if (getBloque(i, j)->getTipo() == ETipoBloque::piso) {
                Rectangle rectPiso = Rectangle(columna * ANCHO_IMAGEN, fila * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                g->DrawImage(bmpPiso, zoom, rectPiso, GraphicsUnit::Pixel);
            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::agua) {
                Rectangle rectAgua = Rectangle(2 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                g->DrawImage(bmpBloques, zoom, rectAgua, GraphicsUnit::Pixel);
            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::arbol) {
                Rectangle rectArbol = Rectangle(1 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                g->DrawImage(bmpBloques, zoom, rectArbol, GraphicsUnit::Pixel);

            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::destruible) {

                EMitadBloque mitad = b->getMitad();

                if (mitad != EMitadBloque::mitadCompleto) {
                    // piso debajo, para la parte libre
                    Rectangle rectPiso = Rectangle(columna * ANCHO_IMAGEN, fila * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                    Rectangle zoomPiso = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                    g->DrawImage(bmpPiso, zoomPiso, rectPiso, GraphicsUnit::Pixel);
                }

                // por defecto, el bloque completo
                Rectangle rectDestruible = Rectangle(1 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);

                // si es una mitad, se reemplazan los dos rectángulos por la versión recortada
                if (mitad == EMitadBloque::mitadArriba) {
                    rectDestruible = Rectangle(1 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                    zoom = Rectangle(X, Y, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                }
                if (mitad == EMitadBloque::mitadAbajo) {
                    rectDestruible = Rectangle(1 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN + ALTO_IMAGEN / 2, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                    zoom = Rectangle(X, Y + ALTO_IMAGEN / 2, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                }
                if (mitad == EMitadBloque::mitadIzquierda) {
                    rectDestruible = Rectangle(1 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                    zoom = Rectangle(X, Y, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                }
                if (mitad == EMitadBloque::mitadDerecha) {
                    rectDestruible = Rectangle(1 * ANCHO_IMAGEN + ANCHO_IMAGEN / 2, 0 * ALTO_IMAGEN, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                    zoom = Rectangle(X + ANCHO_IMAGEN / 2, Y, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                }

                g->DrawImage(bmpBloques, zoom, rectDestruible, GraphicsUnit::Pixel);
            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::noDestruible) {
                EMitadBloque mitad = b->getMitad();

                if (mitad != EMitadBloque::mitadCompleto) {
                    // piso debajo, para la parte libre
                    Rectangle rectPiso = Rectangle(columna * ANCHO_IMAGEN, fila * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                    Rectangle zoomPiso = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                    g->DrawImage(bmpPiso, zoomPiso, rectPiso, GraphicsUnit::Pixel);
                }

                // por defecto, el bloque completo
                Rectangle rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);

                if (mitad == EMitadBloque::mitadArriba) {
                    rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                    zoom = Rectangle(X, Y, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                }
                if (mitad == EMitadBloque::mitadAbajo) {
                    rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN + ALTO_IMAGEN / 2, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                    zoom = Rectangle(X, Y + ALTO_IMAGEN / 2, ANCHO_IMAGEN, ALTO_IMAGEN / 2);
                }
                if (mitad == EMitadBloque::mitadIzquierda) {
                    rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                    zoom = Rectangle(X, Y, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                }
                if (mitad == EMitadBloque::mitadDerecha) {
                    rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN + ANCHO_IMAGEN / 2, 1 * ALTO_IMAGEN, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                    zoom = Rectangle(X + ANCHO_IMAGEN / 2, Y, ANCHO_IMAGEN / 2, ALTO_IMAGEN);
                }

                g->DrawImage(bmpBloques, zoom, rectNoDestruible, GraphicsUnit::Pixel);
            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::trofeo) {
                Rectangle rectTrofeo = Rectangle(0 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                g->DrawImage(bmpTrofeo, zoom, rectTrofeo, GraphicsUnit::Pixel);
            }

            X += ANCHO_IMAGEN;
        }
        Y += ALTO_IMAGEN;
    }
}
