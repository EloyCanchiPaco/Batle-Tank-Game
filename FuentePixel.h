#pragma once
#include <cstring>
#include <cctype>
using namespace System::Drawing;

// ---------------------------------------------------------------------------
// Fuente "pixel art" 5x7 (solo cabecera, para no agregar un .cpp).
// La usan el menu y el game over. Cada 'X' es un pixel; cualquier otro caracter, vacio.
// Caracteres disponibles: 0-9, A-Z, '-' y '.'   (el espacio solo avanza)
// ---------------------------------------------------------------------------
class FuentePixel {
public:
	static const int ANCHO = 5;      // columnas de cada letra
	static const int ALTO = 7;       // filas de cada letra
	static const int AVANCE = 6;     // columnas + 1 de separacion

	// Las 7 filas (de 5 caracteres) de una letra, o nullptr si no existe
	static const char* const* glifo(char c) {
		static const char ORDEN[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-.";
		static const char* const GLIFOS[][7] = {
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
			{ " XXX ", "X   X", "X   X", "XXXXX", "X   X", "X   X", "X   X" },   // A
			{ "XXXX ", "X   X", "X   X", "XXXX ", "X   X", "X   X", "XXXX " },   // B
			{ " XXX ", "X   X", "X    ", "X    ", "X    ", "X   X", " XXX " },   // C
			{ "XXXX ", "X   X", "X   X", "X   X", "X   X", "X   X", "XXXX " },   // D
			{ "XXXXX", "X    ", "X    ", "XXXX ", "X    ", "X    ", "XXXXX" },   // E
			{ "XXXXX", "X    ", "X    ", "XXXX ", "X    ", "X    ", "X    " },   // F
			{ " XXX ", "X   X", "X    ", "X XXX", "X   X", "X   X", " XXX " },   // G
			{ "X   X", "X   X", "X   X", "XXXXX", "X   X", "X   X", "X   X" },   // H
			{ "XXXXX", "  X  ", "  X  ", "  X  ", "  X  ", "  X  ", "XXXXX" },   // I
			{ "    X", "    X", "    X", "    X", "X   X", "X   X", " XXX " },   // J
			{ "X   X", "X  X ", "X X  ", "XX   ", "X X  ", "X  X ", "X   X" },   // K
			{ "X    ", "X    ", "X    ", "X    ", "X    ", "X    ", "XXXXX" },   // L
			{ "X   X", "XX XX", "X X X", "X X X", "X   X", "X   X", "X   X" },   // M
			{ "X   X", "XX  X", "X X X", "X  XX", "X   X", "X   X", "X   X" },   // N
			{ " XXX ", "X   X", "X   X", "X   X", "X   X", "X   X", " XXX " },   // O
			{ "XXXX ", "X   X", "X   X", "XXXX ", "X    ", "X    ", "X    " },   // P
			{ " XXX ", "X   X", "X   X", "X   X", "X X X", "X  X ", " XX X" },   // Q
			{ "XXXX ", "X   X", "X   X", "XXXX ", "X X  ", "X  X ", "X   X" },   // R
			{ " XXX ", "X   X", "X    ", " XXX ", "    X", "X   X", " XXX " },   // S
			{ "XXXXX", "  X  ", "  X  ", "  X  ", "  X  ", "  X  ", "  X  " },   // T
			{ "X   X", "X   X", "X   X", "X   X", "X   X", "X   X", " XXX " },   // U
			{ "X   X", "X   X", "X   X", "X   X", "X   X", " X X ", "  X  " },   // V
			{ "X   X", "X   X", "X   X", "X X X", "X X X", "XX XX", "X   X" },   // W
			{ "X   X", "X   X", " X X ", "  X  ", " X X ", "X   X", "X   X" },   // X
			{ "X   X", "X   X", " X X ", "  X  ", "  X  ", "  X  ", "  X  " },   // Y
			{ "XXXXX", "    X", "   X ", "  X  ", " X   ", "X    ", "XXXXX" },   // Z
			{ "     ", "     ", "     ", "XXXXX", "     ", "     ", "     " },   // -
			{ "     ", "     ", "     ", "     ", "     ", "  X  ", "  X  " }    // .
		};
		if (c == '\0') return nullptr;
		c = (char)toupper((unsigned char)c);
		const char* p = strchr(ORDEN, c);
		return p ? GLIFOS[p - ORDEN] : nullptr;
	}

	// Ancho en pantalla de un texto, donde cada pixel de la fuente mide 'px' pixeles
	static int ancho(const char* t, int px) {
		int n = (int)strlen(t);
		return n == 0 ? 0 : (n * AVANCE - 1) * px;
	}

	// Dibuja un texto con la esquina superior izquierda en (x, y)
	static void texto(Graphics^ g, const char* t, int x, int y, int px, Brush^ color) {
		for (int i = 0; t[i] != '\0'; i++) {
			const char* const* gl = glifo(t[i]);
			if (gl != nullptr) {
				for (int f = 0; f < ALTO; f++) {
					int c = 0;
					while (c < ANCHO) {                        // junta los pixeles seguidos en un rectangulo
						if (gl[f][c] != 'X') { c++; continue; }
						int fin = c;
						while (fin < ANCHO && gl[f][fin] == 'X') fin++;
						g->FillRectangle(color, x + c * px, y + f * px, (fin - c) * px, px);
						c = fin;
					}
				}
			}
			x += AVANCE * px;
		}
	}
};
