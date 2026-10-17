#include "Bloque.h"

CBloque::CBloque(ETipoBloque tipo, int durabilidad, EMitadBloque mitad) {
	this->tipo = tipo;
	this->durabilidad = durabilidad;
	this->mitad = mitad;
}

CBloque::~CBloque() {}

ETipoBloque CBloque::getTipo() {
	return this->tipo;
}


void CBloque::setDurabilidad(short durabilidad) {
	this->durabilidad = durabilidad;
}

short CBloque::getDurabilidad() {
	return this->durabilidad;
}
EMitadBloque CBloque::getMitad() {
	return this->mitad;
}
void CBloque::setMitad(EMitadBloque mitad) {
	this->mitad = mitad;
}


