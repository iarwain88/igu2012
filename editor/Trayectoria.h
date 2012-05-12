//-----------------------------------------------
//- Cabecera de Trayectoria:
//-      - Define una Trayectoria y sus puntos
//-----------------------------------------------

/* Estos defines permiten que la cabecera se pueda incluir
   varias veces en un mismo proyecto */
#ifndef __IGU_Trayectoria__
#define __IGU_Trayectoria__

#include <iostream>

#include "Vec2.h"

class Trayectoria {

public:
	// Constructor (inicializar la Trayectoria)
	Trayectoria();

	// Dibuja la Trayectoria con OpenGL
	void dibujar(bool dibuja) const;

	void borrarUltimoPunto();

	void cambiarColor(float r, float v, float a);

	// Actualiza proximo Punto
	void actualizaProximoPunto(const Vec2& nueva_posicion);

	// Añade el punto actual (_proximoPunto) a la Trayectoria
	void fijarPunto();

	// Devuelve el punto #num
	const Vec2& punto(unsigned int num) const;

	// Devuelve el numero de puntos
	unsigned int total() const;

	// Guarda el perfil
	void guardar( std::ostream& output );

	// Carga un perfil
	void cargar( std::istream& input );

	void limpiarTrayectoria();

private:


	//vectores para representar la trayectoria
	Vec2 _ppTrayectoria;  //proximo punto trayec
	VectorVec2 _pTrayectoria;//puntos trayectoria
	float rojo;
	float verde;
	float azul;

};


#endif

