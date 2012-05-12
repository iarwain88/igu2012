//-----------------------------------------------
//- Cabecera de Rejilla:
//-      - Define metodos y atributos de la rejilla
//-----------------------------------------------

/* Estos defines permiten que la cabecera se pueda incluir
   varias veces en un mismo proyecto */
#ifndef __IGU_REJILLA__
#define __IGU_REJILLA__

#include "Vec2.h"

class Rejilla
{
public:
	// Constructor (inicializar los valores por defecto de Rejilla)
	Rejilla();

	// Dibuja la rejilla con OpenGL
	void dibujar(unsigned int ventana) const;
	void cambiarColor(float r, float v, float a);
	void cambiarResolucion(bool aumentar);
	void mostrarRejilla();

private:
	unsigned int _total;
	float rojo;
	float verde;
	float azul;
	bool rejillaMostrada;
	unsigned int resolucion;
};


#endif

