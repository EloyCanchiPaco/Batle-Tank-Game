#include "Mapa.h"

CMapa::CMapa() {
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
    this->tipoPiso = PisoDeseado;
    int tmpNivel1[MAX_FILAS][MAX_COLUMNAS] = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1,
      1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1,
      1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
      1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
      1, 1, 2, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1,
      1, 1, 3, 1, 1, 1, 2, 1, 1, 2, 1, 3, 1,
      1, 1, 2, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 2, 2, 1, 3, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 9, 2, 1, 1, 1, 1, 1
    };

    std::cout << "Inicializando nivel 1\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel1[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            if (tmpNivel1[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel1[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel1[i][j] == 9) {
                mapa[i][j] = new CBloque(ETipoBloque::trofeo, 1);
            }
        }
    }
}

void CMapa::nivel2(ETipoPiso PisoDeseado) {
    this->tipoPiso = PisoDeseado;
    int tmpNivel2[MAX_FILAS][MAX_COLUMNAS] = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1,
      1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1,
      1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1,
      1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1,
      1, 5, 1, 1, 2, 1, 1, 1, 2, 1, 1, 5, 1,
      1, 5, 1, 1, 1, 2, 1, 2, 1, 1, 1, 5, 1,
      1, 5, 1, 1, 1, 1, 2, 1, 1, 1, 1, 5, 1,
      1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1,
      1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 9, 2, 1, 1, 1, 1, 1
    };

    std::cout << "Inicializando nivel 2\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel2[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            if (tmpNivel2[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel2[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
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
//nivel 4
void CMapa::nivel4(ETipoPiso PisoDeseado) {
	this->tipoPiso = PisoDeseado;
    int tmpNivel4[MAX_FILAS][MAX_COLUMNAS] = {
      1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1,
      4, 4, 1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 4,
      4, 1, 1, 1, 2, 2, 1, 2, 2, 2, 2, 1, 3,
      3, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
      5, 1, 2, 2, 1, 3, 1, 3, 1, 2, 1, 5, 5,
      1, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 1, 1,
      1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1,
      1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 1, 4,
      4, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 4, 4,
      3, 4, 1, 1, 1, 2, 9, 2, 1, 1, 4, 4, 3
    };

    std::cout << "Inicializando nivel 4\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel4[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            if (tmpNivel4[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel4[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel4[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, -1);
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
	this->tipoPiso = PisoDeseado;
    int tmpNivel5[MAX_FILAS][MAX_COLUMNAS] = {
      2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1,
      1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1,
      5, 5, 5, 2, 2, 5, 5, 5, 1, 3, 1, 1, 5,
      5, 5, 5, 2, 2, 5, 5, 5, 1, 3, 1, 1, 5,
      1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1,
      2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3,
      1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 1,
      1, 1, 2, 1, 1, 2, 9, 2, 1, 2, 1, 2, 1

    };

    std::cout << "Inicializando nivel 5\n";
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            if (tmpNivel5[i][j] == 1) {
                mapa[i][j] = new CBloque(ETipoBloque::piso, 0);
            }
            if (tmpNivel5[i][j] == 2) {
                mapa[i][j] = new CBloque(ETipoBloque::destruible, 2);
            }
            if (tmpNivel5[i][j] == 3) {
                mapa[i][j] = new CBloque(ETipoBloque::noDestruible, -1);
            }
            if (tmpNivel5[i][j] == 4) {
                mapa[i][j] = new CBloque(ETipoBloque::agua, -1);
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
                Rectangle rectDestruible = Rectangle(1 * ANCHO_IMAGEN, 0 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
                g->DrawImage(bmpBloques, zoom, rectDestruible, GraphicsUnit::Pixel);
            }
            if (getBloque(i, j)->getTipo() == ETipoBloque::noDestruible) {
                Rectangle rectNoDestruible = Rectangle(0 * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
                Rectangle zoom = Rectangle(X, Y, ANCHO_IMAGEN * 1.0, ALTO_IMAGEN * 1.0);
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
