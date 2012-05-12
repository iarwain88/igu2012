#include "Vec2.h"
#include <cmath> // Librerias matem�ticas

// Constructor
Vec2::Vec2(float _x, float _y) : 
	x(_x), // Inicializamos x
	y(_y)  // Inicializamos y
{
}

// M�todo Suma
void Vec2::suma(const Vec2 &v)
{
	x = x + v.x;
	y = y + v.y;
}

// M�todo Resta
void Vec2::resta(const Vec2 &v)
{
	x = x - v.x;
	y = y - v.y;
}

// Devuelve la longitud del vector
// (raiz cuadrada de las componentes al cuadrado)
float Vec2::longitud() const
{
	return sqrtf(x*x + y*y);
}