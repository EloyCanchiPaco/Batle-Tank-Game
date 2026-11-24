#include "PanelHUD.h"
#include "EstadoPartida.h"
#include <string>
#include <cstring>

using namespace System;
using namespace System::Drawing;

// ---------------------------------------------------------------------------
// Sprites en "pixel art": cada caracter es un pixel de la NES.
//   X = negro   R = rojo (o el color extra que se pase)   W = blanco   . o espacio = transparente
// ---------------------------------------------------------------------------

// Tanque enemigo pequeno (7x7), copiado del icono de la NES. R = el centro (se pinta con el color que se pase, aqui marron)
static const char* const ICONO_TANQUE[7] = {
	"X..X..X",
	"X.XXX.X",
	"XXXRXXX",
	"XXXRXXX",
	"X.XXX.X",
	"X..X..X",
	"..XXX.."
};

// Bandera de nivel (16x15), copiada de la NES. X = mastil negro, R = bandera (naranja)
static const char* const ICONO_BANDERA[15] = {
	"XX..............",
	"XXRR............",
	"XXRRRR..........",
	"XXRRRRRR........",
	"XXRRRRRRRR......",
	"XXRRRRRRRRRR....",
	"XXRRRRRRRRRRRR..",
	"XXRRRRRRRRRRRRRR",
	"XXRRRRRRRRRRRRRR",
	"XX..............",
	"XX..............",
	"XX..............",
	"XX..............",
	"XX..............",
	"XX.............."
};

// Fuente de numeros y las letras I y P (5x7). Indices: 0-9 = digitos, 10 = I, 11 = P, 12 = S, 13 = C, 14 = O, 15 = R, 16 = E, 17 = B, 18 = A, 19 = T, 20 = L, 21 = Y
static const char* const GLIFOS[22][7] = {
	{ " XXX ", "X   X", "X  XX", "X X X", "XX  X", "X   X", " XXX " },   // 0
	{ "  X  ", " XX  ", "  X  ", "  X  ", "  X  ", "  X  ", " XXX " },   // 1
	{ " XXX ", "X   X", "    X", "   X ", "  X  ", " X   ", "XXXXX" },   // 2
	{ " XXX ", "X   X", "    X", "  XX ", "    X", "X   X", " XXX " },   // 3
	{ "   X ", "  XX ", " X X ", "X  X ", "XXXXX", "   X ", "   X " },   // 4
	{ "XXXXX", "X    ", "XXXX ", "    X", "    X", "X   X", " XXX " },   // 5
	{ " XXX ", "X    ", "X    ", "XXXX ", "X   X", "X   X", " XXX " },   // 6
	{ "XXXXX", "    X", "   X ", "  X  ", " X   ", " X   ", " X   " },   // 7
	{ " XXX ", "X   X", "X   X", " XXX ", "X   X", "X   X", " XXX " },   // 8
	{ " XXX ", "X   X", "X   X", " XXXX", "    X", "    X", " XXX " },   // 9
	{ "XXXXX", "  X  ", "  X  ", "  X  ", "  X  ", "  X  ", "XXXXX" },   // I
	{ "XXXX ", "X   X", "X   X", "XXXX ", "X    ", "X    ", "X    " },   // P
	{ " XXX ", "X   X", "X    ", " XXX ", "    X", "X   X", " XXX " },   // S
	{ " XXX ", "X   X", "X    ", "X    ", "X    ", "X   X", " XXX " },   // C
	{ " XXX ", "X   X", "X   X", "X   X", "X   X", "X   X", " XXX " },   // O
	{ "XXXX ", "X   X", "X   X", "XXXX ", "X X  ", "X  X ", "X   X" },   // R
	{ "XXXXX", "X    ", "X    ", "XXXX ", "X    ", "X    ", "XXXXX" },   // E
	{ "XXXX ", "X   X", "X   X", "XXXX ", "X   X", "X   X", "XXXX " },   // B
	{ " XXX ", "X   X", "X   X", "XXXXX", "X   X", "X   X", "X   X" },   // A
	{ "XXXXX", "  X  ", "  X  ", "  X  ", "  X  ", "  X  ", "  X  " },   // T
	{ "X    ", "X    ", "X    ", "X    ", "X    ", "X    ", "XXXXX" },   // L
	{ "X   X", "X   X", " X X ", "  X  ", "  X  ", "  X  ", "  X  " }    // Y
};

// Dibuja un sprite. Junta los pixeles iguales de una fila para dibujar menos rectangulos.
static void pintar(Graphics^ g, const char* const* filas, int nFilas, int x, int y, int escala, Brush^ rojo) {
	for (int f = 0; f < nFilas; f++) {
		const char* s = filas[f];
		int n = (int)strlen(s);
		int c = 0;
		while (c < n) {
			char ch = s[c];
			if (ch == '.' || ch == ' ') { c++; continue; }
			int fin = c;
			while (fin < n && s[fin] == ch) fin++;
			Brush^ b = Brushes::Black;
			if (ch == 'R') b = rojo;
			else if (ch == 'W') b = Brushes::White;
			g->FillRectangle(b, x + c * escala, y + f * escala, (fin - c) * escala, escala);
			c = fin;
		}
	}
}

// Posicion de una letra o digito dentro de GLIFOS (-1 si no existe)
static int indiceGlifo(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	switch (c) {
	case 'I': return 10;  case 'P': return 11;  case 'S': return 12;  case 'C': return 13;
	case 'O': return 14;  case 'R': return 15;  case 'E': return 16;  case 'B': return 17;
	case 'A': return 18;  case 'T': return 19;  case 'L': return 20;  case 'Y': return 21;
	}
	return -1;
}

// Dibuja un texto formado por digitos y letras de GLIFOS (en negro)
static void texto(Graphics^ g, const char* t, int x, int y, int escala, Brush^ rojo) {
	for (int i = 0; t[i] != '\0'; i++) {
		int idx = indiceGlifo(t[i]);
		if (idx >= 0) pintar(g, GLIFOS[idx], 7, x, y, escala, rojo);
		x += 6 * escala;                       // 5 columnas + 1 de separacion
	}
}

// Dibuja UNA letra de GLIFOS con el color que se quiera (para el titulo).
// 'grueso' engorda cada trazo esa cantidad de pixeles (0 = grosor normal).
static void letraColor(Graphics^ g, char c, int x, int y, int escala, int grueso, Brush^ color) {
	int idx = indiceGlifo(c);
	if (idx < 0) return;
	for (int f = 0; f < 7; f++) {
		const char* s = GLIFOS[idx][f];
		int n = (int)strlen(s);
		int col = 0;
		while (col < n) {
			if (s[col] != 'X') { col++; continue; }
			int fin = col;
			while (fin < n && s[fin] == 'X') fin++;
			g->FillRectangle(color, x + col * escala, y + f * escala, (fin - col) * escala + grueso, escala + grueso);
			col = fin;
		}
	}
}

// ---------------------------------------------------------------------------

CPanelHUD::CPanelHUD() {
	actualizar();                         // toma los valores actuales al crearse
}

void CPanelHUD::actualizar() {
	EstadoPartida& e = EstadoPartida::getInstancia();
	puntuacion = e.getPuntuacion();
	nivel = e.getNivel();
	vidas = e.getVidas();
	enemigos = e.getEnemigosRestantes();
}

void CPanelHUD::dibujar(Graphics^ g) {
	const int X = MAX_COLUMNAS * ANCHO_IMAGEN;       // el panel empieza donde termina el mapa
	const int ALTO = MAX_FILAS * ALTO_IMAGEN;
	const int ESC = 5;                               // 1 pixel de la NES = 5 de pantalla
	const int MARGEN = 30;

	SolidBrush^ gris = gcnew SolidBrush(Color::FromArgb(109, 109, 109));   // gris del panel NES
	SolidBrush^ rojo = gcnew SolidBrush(Color::FromArgb(161, 102, 0));      // bandera (naranja de la NES)
	SolidBrush^ marron = gcnew SolidBrush(Color::FromArgb(121, 40, 0));     // centro del tanque

	g->FillRectangle(gris, X, 0, ANCHO_PANEL, ALTO);

	// ---- Enemigos restantes: iconos en filas horizontales ----
	const int TAM_ICONO = 7 * ESC;                   // 35 px
	const int PASO = TAM_ICONO + ESC;                // 40 px: 1 pixel de la NES entre iconos
	int porFila = (ANCHO_PANEL - 2 * MARGEN) / PASO; // 8 por fila
	if (porFila < 1) porFila = 1;
	for (int i = 0; i < enemigos; i++) {
		int fila = i / porFila;
		int col = i % porFila;
		pintar(g, ICONO_TANQUE, 7, X + MARGEN + col * PASO, 30 + fila * PASO, ESC, marron);
	}

	int x = X + MARGEN;

	// ---- Vidas del jugador: "IP" y debajo el icono con el numero ----
	texto(g, "IP", x, 300, ESC, rojo);
	pintar(g, ICONO_TANQUE, 7, x, 345, ESC, marron);
	std::string sv = std::to_string(vidas);
	texto(g, sv.c_str(), x + TAM_ICONO + 15, 347, ESC, rojo);

	// ---- Score: la palabra arriba y el puntaje debajo ----
	texto(g, "SCORE", x, 440, 4, rojo);
	std::string ss = std::to_string(puntuacion);
	while (ss.size() < 6) ss = "0" + ss;
	texto(g, ss.c_str(), x, 476, 4, rojo);

	// ---- Bandera y numero de nivel ----
	pintar(g, ICONO_BANDERA, 15, x, 650, ESC, rojo);
	std::string sn = std::to_string(nivel);
	texto(g, sn.c_str(), x + 10 * ESC, 650 + 16 * ESC, ESC, rojo);   // el numero va debajo, a la derecha del mastil

	// ---- Titulo en dos columnas, letra bajo letra: BATTLE | CITY (CITY centrada en vertical) ----
	const int XT = X + ANCHO_PANEL;
	g->FillRectangle(Brushes::Black, XT, 0, ANCHO_TITULO, ALTO);
	SolidBrush^ colTitulo = gcnew SolidBrush(Color::FromArgb(181, 49, 32));   // rojo de Battle City

	const char* col1 = "BATTLE";
	const char* col2 = "CITY";
	const int ESC_T = 9;                                   // tamano de cada letra
	const int GRUESO = ESC_T / 3;                          // trazos mas gruesos
	const int PASO_T = 110;                                // distancia vertical entre letras
	const int SEP = 20;                                    // separacion entre las dos columnas
	int anchoLetra = 5 * ESC_T + GRUESO;
	int altoLetra = 7 * ESC_T + GRUESO;
	int x1 = XT + (ANCHO_TITULO - (2 * anchoLetra + SEP)) / 2;
	int x2 = x1 + anchoLetra + SEP;
	int y0 = (ALTO - (5 * PASO_T + altoLetra)) / 2;        // BATTLE centrado en vertical

	for (int i = 0; col1[i] != '\0'; i++)
		letraColor(g, col1[i], x1, y0 + i * PASO_T, ESC_T, GRUESO, colTitulo);
	for (int i = 0; col2[i] != '\0'; i++)
		letraColor(g, col2[i], x2, y0 + (i + 1) * PASO_T, ESC_T, GRUESO, colTitulo);   // empieza una fila mas abajo

	delete colTitulo;
	delete gris;
	delete rojo;
	delete marron;
}