//-----------------------------------------------
//- Cabecera de Vec2:
//-      - Define un Vector de dos componentes 
//-----------------------------------------------

/* Estos defines permiten que la cabecera se pueda incluir
   varias veces en un mismo proyecto */
#ifndef __IGU_VEC2__
#define __IGU_VEC2__

#include <vector> // Cabecera estandar para vectores genéricos

struct Vec2
{
	// Constructor de Vec2
	Vec2(float x = 0.0f, float y = 0.0f);

	// Le suma a este vec2 otro vector dado (v)
	// ver nota [1]
	void suma(const Vec2 &v);

	// Le resta a este vec2 otro vector dado (v)
	// ver nota [1]
	void resta(const Vec2 &v);

	// Devuelve la longitud de un vector
	float longitud() const; // <- este const es importante ver nota[2]

	// Variables de un Vec2
	float x;
	float y;
};

// Definimos el tipo Vector de Vec2 :
typedef std::vector< Vec2 > VectorVec2;

/* Notas:
	[1] - Las funciones que reciban un Vec2 y *NO* deban modificar el estado
	(atributos de vec2) tienen que tener siempre la signatura de:
	                  -->    const Vec2 &parametro    <--
	De esta forma se pasa una referencia constante de Vec2 evitando copiar 
	memoria de forma innecesaria.


	[2] - *TODAS* Los métodos de Vec2 que *NO* modifiquen el estado de vec2 deben
	añadir el "const" al final, para indicar que este método no modifica el estado.
	Por ejemplo, obtener la longitud no requiere modificar los valores de x,y (estado
	de vec2). 

*/

#endif
