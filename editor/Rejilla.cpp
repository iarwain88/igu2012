#include <stdlib.h>
#include <GL/glut.h>
#include "Rejilla.h"

//sebastian
Rejilla::Rejilla()
{
	rojo = .9f;
	verde = .65f;
	azul = .65f;
	rejillaMostrada = 1;
	resolucion = 10;
}

//sebastian
void Rejilla::dibujar(unsigned int ventana) const
{
	if (rejillaMostrada)
	{
		const float sep = ventana/resolucion; // separacion entre rejillas
		glColor3f(rojo, verde, azul);
		glLineWidth(1.0);
		glBegin(GL_LINES);
		for( unsigned int i = 0; i < resolucion; ++i)
		{
			// horizontal
			glVertex2f(0, i*sep);
			glVertex2f(ventana, i*sep);
			// vertical
			glVertex2f(i*sep,0);
			glVertex2f(i*sep, ventana);
		}
		glEnd();
	}
	else return;
}

void Rejilla::mostrarRejilla()
{
	rejillaMostrada ? (rejillaMostrada = 0) : (rejillaMostrada = 1);
}

void Rejilla::cambiarResolucion(bool aumentar)
{
	aumentar ? ++resolucion : --resolucion;
}

void Rejilla::cambiarColor(float r, float v, float a)
{
	rojo = r;
	verde = v;
	azul = a;
	resolucion;
}
