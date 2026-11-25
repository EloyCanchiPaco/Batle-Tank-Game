#include "EstadoGameOver.h"
#include "JuegoFacade.h"
#include "EstadoPartida.h"
#include "PanelHUD.h"
#include "FuentePixel.h"
#include "Sonido.h"
#include <string>

// Medidas de la escena del juego (mapa + panel + franja del titulo), en pixeles de pantalla
static const int ANCHO_MAPA = MAX_COLUMNAS * ANCHO_IMAGEN;
static const int ALTO_MAPA = MAX_FILAS * ALTO_IMAGEN;
static const int ANCHO_ESCENA = ANCHO_MAPA + CPanelHUD::ANCHO_PANEL + CPanelHUD::ANCHO_TITULO;

static const int TICKS_SUBIDA = 20;           // lo que tarda "GAME OVER" en subir (~2 s)
static const int PX_TITULO = 10;              // tamano de cada pixel de "GAME OVER"
static const int Y_TITULO = 230;              // donde se queda "GAME OVER"
static const int PX_TEXTO = 5;                // tamano de los pixeles de las opciones
static const int Y_PUNTAJE = 340;
static const int Y_OPCIONES = 450;            // y del texto de la primera opcion
static const int PASO = 90;                   // separacion entre opciones
static const int TAM_TANQUE = 64;
static const int SEPARACION = 24;             // entre el tanque y el texto

static const char* const OPCIONES[] = { "REINTENTAR", "MENU" };

EstadoGameOver::EstadoGameOver(ContextoJuego* contexto)
	: EstadoJuego(contexto), cursor(0), tick(0), tanqueY(0), moviendo(false) {
}

bool EstadoGameOver::listo() const {
	return tick >= TICKS_SUBIDA;
}

int EstadoGameOver::yObjetivoTanque() const {
	return Y_OPCIONES + (FuentePixel::ALTO * PX_TEXTO) / 2 - TAM_TANQUE / 2 + cursor * PASO;   // centrado en el texto
}

void EstadoGameOver::entrar() {
	cursor = 0;
	tick = 0;
	moviendo = false;
	tanqueY = yObjetivoTanque();

	// guarda el puntaje para el menu (I- y HI-)
	ctx->registrarPuntaje(EstadoPartida::getInstancia().getPuntuacion());

	// congela la ultima escena en un bitmap
	Recursos* r = ctx->getRecursos();
	Bitmap^ imagen = gcnew Bitmap(ANCHO_ESCENA, ALTO_MAPA);
	Graphics^ gi = Graphics::FromImage(imagen);
	gi->Clear(Color::Black);
	ctx->getJuego()->Dibujar(gi, r->piso, r->bloques, r->trofeo, r->tanques, r->bala, r->explosion, r->trofeoD);
	delete gi;
	foto = imagen;
}

void EstadoGameOver::salir() {
	Bitmap^ b = foto;
	if (b != nullptr) {
		delete b;
		foto = nullptr;
	}
}

void EstadoGameOver::actualizar() {
	tick++;

	int objetivo = yObjetivoTanque();
	int dy = objetivo - tanqueY;
	moviendo = (dy != 0);
	if (moviendo) {
		int velocidad = 45;
		tanqueY += (dy > 0) ? (dy < velocidad ? dy : velocidad) : (-dy < velocidad ? dy : -velocidad);
	}
}

void EstadoGameOver::aceptar() {
	if (cursor == 0) ctx->cambiarA(EEstado::jugando);      // REINTENTAR: mismo nivel (el nivel sigue guardado en el contexto)
	else             ctx->cambiarA(EEstado::menu);         // MENU
}

void EstadoGameOver::teclaPresionada(ETecla tecla) {
	if (!listo()) return;                                  // hasta que termine de subir el texto no se elige nada
	switch (tecla) {
	case ETecla::arriba:
	case ETecla::abajo:     cursor = 1 - cursor; break;
	case ETecla::aceptar:
	case ETecla::disparo:   aceptar(); break;              // (cambia de estado: va al final)
	case ETecla::volver:    ctx->cambiarA(EEstado::menu); break;
	default: break;
	}
}

void EstadoGameOver::dibujar(Graphics^ g, int ancho, int alto) {
	g->Clear(Color::Black);
	g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;
	g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
	g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;

	// 1) la ultima escena, oscurecida
	Bitmap^ imagen = foto;
	if (imagen != nullptr) g->DrawImage(imagen, 0, 0, ANCHO_ESCENA, ALTO_MAPA);
	SolidBrush^ velo = gcnew SolidBrush(Color::FromArgb(150, 0, 0, 0));
	g->FillRectangle(velo, 0, 0, ANCHO_ESCENA, ALTO_MAPA);
	delete velo;

	// 2) "GAME OVER" sube desde abajo hasta quedar centrado en el mapa
	SolidBrush^ rojo = gcnew SolidBrush(Color::FromArgb(181, 49, 32));      // rojo de Battle City
	int avance = (tick < TICKS_SUBIDA) ? tick : TICKS_SUBIDA;
	int yTitulo = ALTO_MAPA + (Y_TITULO - ALTO_MAPA) * avance / TICKS_SUBIDA;
	FuentePixel::texto(g, "GAME OVER", (ANCHO_MAPA - FuentePixel::ancho("GAME OVER", PX_TITULO)) / 2, yTitulo, PX_TITULO, rojo);
	delete rojo;

	if (!listo()) return;

	// 3) puntaje y opciones
	std::string ss = std::to_string(EstadoPartida::getInstancia().getPuntuacion());
	while (ss.size() < 6) ss = "0" + ss;
	std::string linea = "SCORE " + ss;
	FuentePixel::texto(g, linea.c_str(), (ANCHO_MAPA - FuentePixel::ancho(linea.c_str(), PX_TEXTO)) / 2, Y_PUNTAJE, PX_TEXTO, Brushes::White);

	int izquierda = (ANCHO_MAPA - (TAM_TANQUE + SEPARACION + FuentePixel::ancho(OPCIONES[0], PX_TEXTO))) / 2;
	int xTexto = izquierda + TAM_TANQUE + SEPARACION;
	for (int i = 0; i < 2; i++)
		FuentePixel::texto(g, OPCIONES[i], xTexto, Y_OPCIONES + i * PASO, PX_TEXTO, Brushes::White);

	// 4) el tanque del jugador (mirando a la derecha) marca la opcion elegida
	Bitmap^ bmpTanques = ctx->getRecursos()->tanques;
	int cuadro = (tick / 2) % 2;
	Rectangle origen = Rectangle(cuadro * ANCHO_IMAGEN, 1 * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
	Rectangle destino = Rectangle(izquierda, tanqueY, TAM_TANQUE, TAM_TANQUE);
	g->DrawImage(bmpTanques, destino, origen, GraphicsUnit::Pixel);

	// 5) ayuda
	const char* ayuda = "W-S ELEGIR   ENTER ACEPTAR";
	SolidBrush^ gris = gcnew SolidBrush(Color::FromArgb(150, 150, 150));
	FuentePixel::texto(g, ayuda, (ANCHO_MAPA - FuentePixel::ancho(ayuda, 3)) / 2, 720, 3, gris);
	delete gris;
}
