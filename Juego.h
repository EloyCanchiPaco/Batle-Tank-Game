#pragma once
#include "JuegoFacade.h"
#include "EstadoJuego.h"
#include <windows.h>
#pragma comment(lib, "user32.lib")
namespace Battle {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de Juego
	/// </summary>
	public ref class Juego : public System::Windows::Forms::Form
	{
	public:
		CJuegoFacade* juego = new CJuegoFacade();

		Bitmap^ bmpPiso = gcnew Bitmap("imagenes\\pisos.png");
		Bitmap^ bmpBloques = gcnew Bitmap("imagenes\\bloques.png");
		Bitmap^ bmpTrofeo = gcnew Bitmap("imagenes\\trofeo.png");
		Bitmap^ bmpTanques = gcnew Bitmap("imagenes\\tanques.png");
		Bitmap^ bmpBala = gcnew Bitmap("imagenes\\bala.png");
		Bitmap^ bmpExplosion = gcnew Bitmap("imagenes\\explosion.png");
		Bitmap^ bmpTrofeoD = gcnew Bitmap("imagenes\\trofeoD.png");
		Recursos* recursos = new Recursos();          // imagenes compartidas por todos los estados
		ContextoJuego* contexto = nullptr;           // PATRON STATE: menu / jugando / game over
		System::Diagnostics::Stopwatch^ reloj = System::Diagnostics::Stopwatch::StartNew();
		long long ultimoTick = 0;
		Juego(void)
		{
			InitializeComponent();
			bmpTanques->MakeTransparent(Color::Yellow);
			bmpExplosion->MakeTransparent(Color::Yellow);
			bmpTrofeo->MakeTransparent(Color::Black);
			recursos->piso = bmpPiso;
			recursos->bloques = bmpBloques;
			recursos->trofeo = bmpTrofeo;
			recursos->tanques = bmpTanques;
			recursos->bala = bmpBala;
			recursos->explosion = bmpExplosion;
			recursos->trofeoD = bmpTrofeoD;
			contexto = new ContextoJuego(juego, recursos);
			//
			//TODO: agregar c digo de constructor aqu 
			//
			//bmpPiso->MakeTransparent(bmpPiso->GetPixel(0, 0));
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se est n usando.
		/// </summary>
		/// 
		~Juego()
		{
			if (components)
			{
				delete components;
			}
			delete contexto;
			delete recursos;
		}
	private: System::Windows::Forms::Timer^ timer1;
	protected:

	private: System::ComponentModel::IContainer^ components;
	private:
		/// <summary>
		/// Variable del dise ador necesaria.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// M todo necesario para admitir el Dise ador. No se puede modificar
		/// el contenido de este m todo con el editor de c digo.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Juego::timer1_Tick);
			// 
			// Juego
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(387, 354);
			this->Name = L"Juego";
			this->Text = L"Juego";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &Juego::Juego_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Juego::Juego_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Juego::Juego_KeyUp);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		long long ahora = reloj->ElapsedMilliseconds;
		this->Text = String::Format("tick: {0} ms", ahora - ultimoTick);
		ultimoTick = ahora;

		contexto->actualizar();                    // el estado actual hace su logica

		Graphics^ g = this->CreateGraphics();
		BufferedGraphicsContext^ espacio = BufferedGraphicsManager::Current;
		BufferedGraphics^ buffer = espacio->Allocate(g, this->ClientRectangle);

		contexto->dibujar(buffer->Graphics, this->ClientRectangle.Width, this->ClientRectangle.Height);
		buffer->Render(g);

		delete buffer;
		delete g;
	}
	private: System::Void Juego_Load(System::Object^ sender, System::EventArgs^ e) {
		contexto->cambiarA(EEstado::menu);         // el juego arranca en el menu
	}
	private: System::Void Juego_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		ETecla tecla = Traducir(e->KeyCode);
		if (tecla != ETecla::ninguna) {
			contexto->teclaPresionada(tecla);
			e->Handled = true;
		}
	}
	private: System::Void Juego_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		ETecla tecla = Traducir(e->KeyCode);
		if (tecla != ETecla::ninguna) {
			contexto->teclaSoltada(tecla);
			e->Handled = true;
		}
	}

		   // Traduce la tecla de WinForms a una accion del juego (los estados no conocen WinForms)
	private: static ETecla Traducir(System::Windows::Forms::Keys tecla) {
		switch (tecla) {
		case Keys::W: case Keys::Up:      return ETecla::arriba;
		case Keys::S: case Keys::Down:    return ETecla::abajo;
		case Keys::A: case Keys::Left:    return ETecla::izquierda;
		case Keys::D: case Keys::Right:   return ETecla::derecha;
		case Keys::Space:                 return ETecla::disparo;
		case Keys::Enter:                 return ETecla::aceptar;
		case Keys::Escape: case Keys::Back: return ETecla::volver;
		default:                          return ETecla::ninguna;
		}
	}
	};
}
