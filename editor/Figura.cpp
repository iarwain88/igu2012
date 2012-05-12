#include <stdlib.h>
#include <GL/glut.h>
#include "Figura.h"

using namespace std;

Figura::Figura()
{
	rojo = 0.0;
	verde = 0.0;
	azul = 0.0;
}


void Figura::dibujar(bool dibuja) const
{
	glColor3f(rojo, verde, azul);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);

	for( unsigned int i = 0; i < total() ; ++i)
	{
		glVertex2f( _puntos[i].x, _puntos[i].y );
	}
		if(dibuja) glVertex2f(_proximoPunto.x, _proximoPunto.y);
	glEnd();
}

void Figura::borrarUltimoPunto()
{
	_puntos.pop_back();
}

void Figura::cambiarColor(float r, float v, float a)
{
	rojo = r;
	verde = v;
	azul = a;
}

void Figura::actualizaProximoPunto(const Vec2& punto)
{
	_proximoPunto = punto;
}

void Figura::fijarPunto()
{
	_puntos.push_back(_proximoPunto );
}

const Vec2& Figura::punto(unsigned int num) const
{
	return _puntos[num];
}

unsigned int Figura::total() const
{
	return _puntos.size();
}

// Guarda el perfil
void Figura::guardar(ostream& output )
{
	// Guardamos el numero de puntos
	output << _puntos.size() << std::endl;
	for(unsigned int i = 0; i < _puntos.size(); ++i)
	{
		output << _puntos[i].x << " " << _puntos[i].y << std::endl;
	}
}

// Carga un perfil
void Figura::cargar(istream& input )
{
	// vaciamos el contenido de la figura
	_puntos.clear();

	unsigned int total;
	input >> total;

	for(unsigned int i = 0; i < total; ++i)
	{
		float x, y;
		input >> x >> y;
		_puntos.push_back( Vec2(x,y) );
	}
}

void Figura::limpiarFigura()
{
	_puntos.clear();
}