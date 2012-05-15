#include <stdlib.h>
#include <GL/glut.h>
#include "Trayectoria.h"

using namespace std;


Trayectoria::Trayectoria()
{	
	_pTrayectoria.push_back(Vec2(0.0,0.0));
	rojo = 0.0f;
	verde = 0.0f;
	azul = 0.0f;	
}

void Trayectoria::dibujar(bool dibuja) const
{
	glColor3f(rojo, verde, azul);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for( unsigned int i = 0; i < total() ; ++i)
	{
		glVertex2f( _pTrayectoria[i].x, _pTrayectoria[i].y );
	}
	//glVertex2f(_ppTrayectoria.x, _ppTrayectoria.y);
	//glEnd();
	
	if(dibuja) glVertex2f(_ppTrayectoria.x, _ppTrayectoria.y);
	glEnd();
}

void Trayectoria::borrarUltimoPunto()
{
	_pTrayectoria.size() > 1 ? _pTrayectoria.pop_back() : cout << "no pasa nada! ;)" ;
}

void Trayectoria::cambiarColor(float r, float v, float a)
{
	rojo = r;
	verde = v;
	azul = a;
}

void Trayectoria::actualizaProximoPunto(const Vec2& punto)
{
	_ppTrayectoria = punto;
}

void Trayectoria::fijarPunto()
{
	 if (_pTrayectoria.back().y < _ppTrayectoria.y)
		_pTrayectoria.push_back(_ppTrayectoria);
}

const Vec2& Trayectoria::punto(unsigned int num) const
{
	return _pTrayectoria[num];
}

unsigned int Trayectoria::total() const
{
	return _pTrayectoria.size();
}

// Guarda el perfil
void Trayectoria::guardar(ostream& output)
{
	// Guardamos el numero de puntos
	output << _pTrayectoria.size() << std::endl;
	for(unsigned int i = 0; i < _pTrayectoria.size(); ++i)
	{
		output << _pTrayectoria[i].x << " " << _pTrayectoria[i].y <<endl;
	}
}

// Carga un perfil
void Trayectoria::cargar(istream& input)
{
	// vaciamos el contenido de la figura
	_pTrayectoria.clear();

	unsigned int total;
	input >> total;

	for(unsigned int i = 0; i < total; ++i)
	{
		float x, y;
		input >> x >> y;
		_pTrayectoria.push_back( Vec2(x,y) );
	}
}

VectorVec2 Trayectoria::get_pTrayectoria()
{
	return _pTrayectoria;
}

void Trayectoria::limpiarTrayectoria()
{
	_pTrayectoria.clear();
}