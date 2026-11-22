#pragma once
#include <vector>
#include <algorithm>

// ===== PATRON OBSERVER =====
// Observador: quien quiere enterarse cuando algo cambia (ej. el panel de datos).
class Observador {
public:
	virtual ~Observador() {}
	virtual void actualizar() = 0;
};

// Sujeto: quien avisa. Guarda la lista de observadores y los notifica.
class Sujeto {
	std::vector<Observador*> observadores;     // no es dueño de ellos
public:
	virtual ~Sujeto() {}
	void agregarObservador(Observador* o) { observadores.push_back(o); }
	void quitarObservador(Observador* o) {
		observadores.erase(std::remove(observadores.begin(), observadores.end(), o), observadores.end());
	}
protected:
	void notificar() {
		for (Observador* o : observadores) o->actualizar();
	}
};