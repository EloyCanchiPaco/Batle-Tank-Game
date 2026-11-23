#pragma once
#include "JuegoFacade.h"
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
		bool teclaArriba = false, teclaAbajo = false, teclaIzquierda = false, teclaDerecha = false;

		Juego(void)
		{
			InitializeComponent();
			bmpTanques->MakeTransparent(Color::Yellow);
			bmpExplosion->MakeTransparent(Color::Yellow);
			bmpTrofeo->MakeTransparent(Color::Black);
			//
			//TODO: agregar código de constructor aquí
			//
			//bmpPiso->MakeTransparent(bmpPiso->GetPixel(0, 0));
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		/// 
		~Juego()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^ timer1;
	protected:

	private: System::ComponentModel::IContainer^ components;
	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
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
		if (teclaArriba)    juego->MoverJugador(EDireccion::arriba);
		else if (teclaAbajo)     juego->MoverJugador(EDireccion::abajo);
		else if (teclaIzquierda) juego->MoverJugador(EDireccion::izquierda);
		else if (teclaDerecha)   juego->MoverJugador(EDireccion::derecha);
		else juego->DetenerJugador();

		juego->MoverEnemigos();
		juego->ActualizarBalas();
		Graphics^ g = this->CreateGraphics();
		BufferedGraphicsContext^ espacio = BufferedGraphicsManager::Current;
		BufferedGraphics^ buffer = espacio->Allocate(g, this->ClientRectangle);


		juego->Dibujar(buffer->Graphics, bmpPiso, bmpBloques, bmpTrofeo, bmpTanques, bmpBala, bmpExplosion, bmpTrofeoD);
		buffer->Render(g);

		delete buffer, espacio, g;
	}
	private: System::Void Juego_Load(System::Object^ sender, System::EventArgs^ e) {
		juego->Iniciar();
	}
	private: System::Void Juego_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {

		switch (e->KeyCode) {
		case Keys::W:
			std::cout << "Tecla arriba" << std::endl;
			teclaArriba = true;
			break;
		case Keys::S:
			std::cout << "Tecla abajo" << std::endl;
			teclaAbajo = true;

			break;
		case Keys::A:
			std::cout << "Tecla izquierda" << std::endl;
			teclaIzquierda = true;
			break;
		case Keys::D:
			std::cout << "Tecla derecha" << std::endl;
			teclaDerecha = true;
			break;
		case Keys::Space:
			std::cout << "1: tecla espacio" << std::endl;
			juego->Disparar();
			break;
		default:
			break;
		}
	}
	private: System::Void Juego_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		switch (e->KeyCode) {
		case Keys::W:    teclaArriba = false;    break;
		case Keys::S:  teclaAbajo = false;     break;
		case Keys::A:  teclaIzquierda = false; break;
		case Keys::D: teclaDerecha = false;   break;
		default: break;
		}
	}
	};
}