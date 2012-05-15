//-----------------------------------------------
//- Cabecera de Figura:
//-      - Define una Figura y sus puntos
//-----------------------------------------------

/* Estos defines permiten que la cabecera se pueda incluir
   varias veces en un mismo proyecto */
#ifndef __IGU_FIGURA__
#define __IGU_FIGURA__

#include <iostream>

#include "Vec2.h"

class Figura
{
public:
	// Constructor (inicializar la figura)
	Figura();

	// Dibuja la figura con OpenGL
	void dibujar(bool dibuja) const;

	void borrarUltimoPunto();

	void cambiarColor(float r, float v, float a);

	// Actualiza proximo Punto
	void actualizaProximoPunto(const Vec2& nueva_posicion);

	// Añade el punto actual (_proximoPunto) a la figura
	void fijarPunto();

	// Devuelve el punto #num
	const Vec2& punto(unsigned int num) const;

	// Devuelve el numero de puntos
	unsigned int total() const;

	// Guarda el perfil
	void guardar( std::ostream& output );

	// Carga un perfil
	void cargar( std::istream& input );

	void limpiarFigura();

	VectorVec2 get_puntos();

private:
	Vec2 _proximoPunto; // Proximo punto que se añadirá
	VectorVec2 _puntos; // Vector de puntos de la figura
	float rojo, verde, azul;
};


#endif

