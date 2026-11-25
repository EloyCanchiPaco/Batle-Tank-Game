#include "EstadoMenu.h"
#include "JuegoFacade.h"
#include "FuentePixel.h"
#include "Sonido.h"
#include <string>
#include <cstring>

// ---------------------------------------------------------------------------
// Medidas del lienzo virtual (en "pixeles de NES"). Todo se multiplica por k al dibujar.
// ---------------------------------------------------------------------------
static const int LIENZO_ANCHO = 256;
static const int LIENZO_ALTO = 240;
static const int X_TANQUE = 70;         // columna del tanque cursor
static const int X_TEXTO = 92;          // columna del texto de las opciones
static const int PASO = 16;             // separacion vertical entre opciones (= alto del tanque)
static const int TAM_TANQUE = 16;
static const int CELDA = 5;             // lado de cada ladrillo del titulo
static const int Y_BATTLE = 28;         // fila del titulo: BATTLE

// Las opciones de cada pagina
static const char* const OPC_PRINCIPAL[] = { "JUGAR", "NIVELES", "OPCIONES", "CREDITOS" };
static const char* const OPC_NIVELES[] = { "NIVEL 1", "NIVEL 2", "NIVEL 3", "NIVEL 4", "NIVEL 5", "VOLVER" };
static const char* const OPC_OPCIONES[] = { "MUSICA", "EFECTOS", "VOLVER" };
static const char* const OPC_CREDITOS[] = { "VOLVER" };

// Integrantes (Integrantes.txt)
static const char* const INTEGRANTES[] = { "CANCHI PACO ELOY", "MARISCAL TOLA JAEL", "GONZALES TOLABA ERIK ALBERTO" };

EstadoMenu::EstadoMenu(ContextoJuego* contexto)
	: EstadoJuego(contexto), pagina(Pagina::principal), cursor(0), tanqueY(0), tick(0),
	dirTanque(EDireccion::derecha), moviendo(false), k(1), ox(0), oy(0) {
}

// ===========================================================================
//  Tablas de cada pagina
// ===========================================================================
int EstadoMenu::cantidad() const {
	switch (pagina) {
	case Pagina::principal: return 4;
	case Pagina::niveles:   return 6;
	case Pagina::opciones:  return 3;
	default:                return 1;
	}
}

const char* EstadoMenu::etiqueta(int i) const {
	switch (pagina) {
	case Pagina::principal: return OPC_PRINCIPAL[i];
	case Pagina::niveles:   return OPC_NIVELES[i];
	case Pagina::opciones:  return OPC_OPCIONES[i];
	default:                return OPC_CREDITOS[i];
	}
}

int EstadoMenu::yLista() const {
	return (pagina == Pagina::creditos) ? 196 : 124;      // en creditos, el VOLVER va abajo del texto
}

int EstadoMenu::yObjetivoTanque() const {
	return yLista() - 4 + cursor * PASO;                  // el tanque (16) queda centrado en la fila del texto (7)
}

// ===========================================================================
//  Ciclo de vida del estado
// ===========================================================================
void EstadoMenu::entrar() {
	CSonido::getInstancia().iniciar();        // el menu tambien tiene sonido (opciones)
	CSonido::getInstancia().musica(false);       // la musica de la partida no suena en el menu
	CSonido::getInstancia().musicaMenu(true);    // suena inicio.wav

	pagina = Pagina::principal;
	cursor = 0;
	tick = 0;
	moviendo = false;
	dirTanque = EDireccion::derecha;
	tanqueY = yObjetivoTanque();

	// el ladrillo destruible (columna 1, fila 0 de bloques.png) como bitmap propio,
	// asi al escalarlo no se mezcla con los bloques vecinos de la imagen
	Bitmap^ bloques = ctx->getRecursos()->bloques;
	ladrillo = bloques->Clone(Rectangle(1 * ANCHO_IMAGEN, 0, ANCHO_IMAGEN, ALTO_IMAGEN),
		System::Drawing::Imaging::PixelFormat::Format32bppArgb);
}

void EstadoMenu::salir() {
	CSonido::getInstancia().musicaMenu(false);
	Bitmap^ b = ladrillo;
	if (b != nullptr) {
		delete b;
		ladrillo = nullptr;
	}
}

void EstadoMenu::actualizar() {
	tick++;

	// el tanque se desliza hasta la opcion elegida, mirando arriba o abajo mientras avanza
	int objetivo = yObjetivoTanque();
	int dy = objetivo - tanqueY;
	moviendo = (dy != 0);
	if (moviendo) {
		int velocidad = (dy > 32 || dy < -32) ? 16 : 8;
		int paso = (dy > 0) ? (dy < velocidad ? dy : velocidad) : (-dy < velocidad ? dy : -velocidad);
		tanqueY += paso;
		dirTanque = (dy > 0) ? EDireccion::abajo : EDireccion::arriba;
	}
	else {
		dirTanque = EDireccion::derecha;
	}
}

// ===========================================================================
//  Acciones
// ===========================================================================
void EstadoMenu::irA(Pagina nueva, int cursorNuevo) {
	pagina = nueva;
	cursor = cursorNuevo;
}

void EstadoMenu::aceptar() {
	switch (pagina) {
	case Pagina::principal:
		if (cursor == 0) { ctx->setNivel(1); ctx->cambiarA(EEstado::jugando); }   // (al final: el estado cambia)
		else if (cursor == 1) irA(Pagina::niveles, 0);
		else if (cursor == 2) irA(Pagina::opciones, 0);
		else                  irA(Pagina::creditos, 0);
		break;
	case Pagina::niveles:
		if (cursor < 5) { ctx->setNivel(cursor + 1); ctx->cambiarA(EEstado::jugando); }
		else volver();
		break;
	case Pagina::opciones:
		if (cursor == 2) volver();
		break;
	case Pagina::creditos:
		volver();
		break;
	}
}

void EstadoMenu::volver() {
	if (pagina == Pagina::principal) return;
	// vuelve al menu principal, con el tanque en la opcion que abrio esta pagina
	int indice = (pagina == Pagina::niveles) ? 1 : (pagina == Pagina::opciones) ? 2 : 3;
	irA(Pagina::principal, indice);
}

// Sube o baja un paso (10 %) el volumen de la opcion elegida
static float moverVolumen(float volumen, int delta) {
	int n = (int)(volumen * 10.0f + 0.5f) + delta;
	if (n < 0) n = 0;
	if (n > 10) n = 10;
	return n / 10.0f;
}

void EstadoMenu::ajustar(int delta) {
	if (pagina != Pagina::opciones) return;
	CSonido& sonido = CSonido::getInstancia();
	if (cursor == 0) {
		sonido.setVolumenMusica(moverVolumen(sonido.getVolumenMusica(), delta));
	}
	else if (cursor == 1) {
		sonido.setVolumenEfectos(moverVolumen(sonido.getVolumenEfectos(), delta));
		sonido.golpe();                                   // se escucha el nuevo volumen
	}
}

void EstadoMenu::teclaPresionada(ETecla tecla) {
	int n = cantidad();
	switch (tecla) {
	case ETecla::arriba:    cursor = (cursor + n - 1) % n; break;
	case ETecla::abajo:     cursor = (cursor + 1) % n;     break;
	case ETecla::izquierda: ajustar(-1); break;
	case ETecla::derecha:   ajustar(+1); break;
	case ETecla::aceptar:
	case ETecla::disparo:   aceptar();   break;           // (puede cambiar de estado: va al final)
	case ETecla::volver:    volver();    break;
	default: break;
	}
}

// ===========================================================================
//  Dibujo
// ===========================================================================
void EstadoMenu::dibujar(Graphics^ g, int ancho, int alto) {
	// factor entero para que el pixel art quede nitido, y lienzo centrado
	k = ancho / LIENZO_ANCHO;
	if (alto / LIENZO_ALTO < k) k = alto / LIENZO_ALTO;
	if (k < 1) k = 1;
	ox = (ancho - LIENZO_ANCHO * k) / 2;
	oy = (alto - LIENZO_ALTO * k) / 2;

	g->Clear(Color::Black);
	g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;
	g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;

	dibujarCabecera(g);
	dibujarTitulo(g);
	dibujarPagina(g);
	dibujarTanque(g);
	dibujarAyuda(g);
}

// "I-    00  HI- 20000"
void EstadoMenu::dibujarCabecera(Graphics^ g) {
	int ultimo = ctx->getUltimoPuntaje();
	std::string sp = (ultimo == 0) ? "00" : std::to_string(ultimo);
	std::string sr = std::to_string(ctx->getRecord());

	FuentePixel::texto(g, "I-", X(24), Y(8), k, Brushes::White);
	FuentePixel::texto(g, sp.c_str(), X(100) - FuentePixel::ancho(sp.c_str(), k), Y(8), k, Brushes::White);
	FuentePixel::texto(g, "HI-", X(132), Y(8), k, Brushes::White);
	FuentePixel::texto(g, sr.c_str(), X(232) - FuentePixel::ancho(sr.c_str(), k), Y(8), k, Brushes::White);
}


void EstadoMenu::dibujarTitulo(Graphics^ g) {
	const int FILAS = 11;
	const int COLS = 35;
	static const int matriz[FILAS][COLS] = {
		// B A T T L E
		{ 1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1 },
		{ 1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0 },
		{ 1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,1,0 },
		{ 1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0 },
		{ 1,1,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		// C I T Y
		{ 0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0 }
	};

	g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;   // el ladrillo se reduce
	Bitmap^ b = ladrillo;                                  // recortado en entrar()

	int x0 = (LIENZO_ANCHO - COLS * CELDA) / 2;            // titulo centrado en el lienzo
	for (int f = 0; f < FILAS; f++) {
		for (int c = 0; c < COLS; c++) {
			if (matriz[f][c] != 1) continue;
			Rectangle destino = Rectangle(X(x0 + c * CELDA), Y(Y_BATTLE + f * CELDA), CELDA * k, CELDA * k);
			g->DrawImage(b, destino);
		}
	}
}
void EstadoMenu::dibujarVolumen(Graphics^ g, int y, float volumen) {
	int llenos = (int)(volumen * 10.0f + 0.5f);
	SolidBrush^ apagado = gcnew SolidBrush(Color::FromArgb(70, 70, 70));
	for (int i = 0; i < 10; i++)
		g->FillRectangle(i < llenos ? Brushes::White : (Brush^)apagado, X(150 + i * 5), Y(y), 4 * k, FuentePixel::ALTO * k);
	delete apagado;
}

void EstadoMenu::dibujarPagina(Graphics^ g) {
	int yl = yLista();

	// las opciones
	for (int i = 0; i < cantidad(); i++)
		FuentePixel::texto(g, etiqueta(i), X(X_TEXTO), Y(yl + i * PASO), k, Brushes::White);

	if (pagina == Pagina::opciones) {
		dibujarVolumen(g, yl, CSonido::getInstancia().getVolumenMusica());
		dibujarVolumen(g, yl + PASO, CSonido::getInstancia().getVolumenEfectos());
	}
	else if (pagina == Pagina::creditos) {
		SolidBrush^ gris = gcnew SolidBrush(Color::FromArgb(150, 150, 150));
		for (int i = 0; i < 3; i++) {
			int anchoTexto = (int)strlen(INTEGRANTES[i]) * FuentePixel::AVANCE - 1;
			FuentePixel::texto(g, INTEGRANTES[i], X((LIENZO_ANCHO - anchoTexto) / 2), Y(124 + i * PASO), k, Brushes::White);
		}
		const char* grupo = "ING SISTEMAS - G3";
		int anchoGrupo = (int)strlen(grupo) * FuentePixel::AVANCE - 1;
		FuentePixel::texto(g, grupo, X((LIENZO_ANCHO - anchoGrupo) / 2), Y(124 + 3 * PASO + 4), k, gris);
		delete gris;
	}
}

// El tanque del jugador (tanques.png: fila = direccion, columna = cuadro de las orugas)
void EstadoMenu::dibujarTanque(Graphics^ g) {
	Bitmap^ bmpTanques = ctx->getRecursos()->tanques;

	int fila = 1;                                            // derecha
	if (dirTanque == EDireccion::arriba) fila = 0;
	else if (dirTanque == EDireccion::abajo) fila = 2;

	int cuadro = moviendo ? (tick % 2) : ((tick / 2) % 2);   // las orugas se animan siempre
	Rectangle origen = Rectangle(cuadro * ANCHO_IMAGEN, fila * ALTO_IMAGEN, ANCHO_IMAGEN, ALTO_IMAGEN);
	Rectangle destino = Rectangle(X(X_TANQUE), Y(tanqueY), TAM_TANQUE * k, TAM_TANQUE * k);

	g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
	g->DrawImage(bmpTanques, destino, origen, GraphicsUnit::Pixel);
}

void EstadoMenu::dibujarAyuda(Graphics^ g) {
	const char* ayuda = "W-S ELEGIR   ENTER ACEPTAR";
	if (pagina == Pagina::opciones) ayuda = "W-S ELEGIR   A-D CAMBIAR";
	else if (pagina == Pagina::creditos) ayuda = "ENTER VOLVER";

	int anchoTexto = (int)strlen(ayuda) * FuentePixel::AVANCE - 1;
	SolidBrush^ gris = gcnew SolidBrush(Color::FromArgb(150, 150, 150));
	FuentePixel::texto(g, ayuda, X((LIENZO_ANCHO - anchoTexto) / 2), Y(226), k, gris);
	delete gris;
}