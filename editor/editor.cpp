#include <stdlib.h>
#include <gl/glut.h>
#include <gl/glui.h>
#include <iostream>
#include <fstream>
#include "Figura.h"
#include "Rejilla.h"
#include "Trayectoria.h"




// *************************************************************************
//				         ESTOY TRABAJANDO YO
//	  SI QUIERES TRABAJAR SOBRE EL CODIGO DIMELO ANTES DE MODIFICAR

//**************************************************************************
//1. cambiar tambien color de titulos al cambiar el color de fondo
//2. anadir eleccion de ficheros



using namespace std;


GLUI *glui;
GLUI_RadioGroup *radio;
GLUI_EditText   *filetext, *wtajtext, *wfigtext;


char  fichero[128];
char fnombre[20];

int   TamVentana = 400;
int   TamVentanaX = 400;
int   TamVentanaY = 600;
int   TamSubVentana = 200;
float AT = 400; 
float AF = 400;// tamano default
int	  idPrincipal;
int	  idCentral;
int	  idSub1;
int	  idSub2;
bool  dibuja = 0;
bool  drawFigure=1;
int fondoRojo = 1.0;
int fondoVerde = 1.0;
int fondoAzul = 1.0;

Figura  figura; //ventana ischierda
Trayectoria  trayectoria; //dibujo de trayectoria
Rejilla rejilla;  //para dibujar rejillas

void actualizarVentanas() //actualizar ventanas cetral y ventana dos al cambiar opciones
{
	glutSetWindow(idCentral);
	glutPostRedisplay(); 
	glutSetWindow(idSub2);
	glutPostRedisplay();
    glutSetWindow(idCentral);
}
void dibujarTitulo (char *t)
{
  unsigned int i;
  for (i = 0; i < strlen(t); i++)
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, t[i]);
}
void guardar(int b) 
{
	ofstream fichero(fnombre);
	fichero << AT;
	fichero << endl;
	trayectoria.guardar(fichero);
	fichero << AF;
	fichero << endl;
	figura.guardar(fichero);
	fichero.close();
}

void cargar(int a)
{
	ifstream fichero(fnombre);
	if (fichero.is_open() == true)
	{
		fichero >> AT;
		trayectoria.cargar(fichero);
		fichero >> AF;
		figura.cargar(fichero);	
	}
	fichero.close();
}	



void click_raton(int button, int state, int x, int y)
{

	if(dibuja){ 
		switch (button) {
		case GLUT_LEFT_BUTTON:

			if (state==GLUT_UP){

				if(drawFigure)
					figura.fijarPunto();
				else
					trayectoria.fijarPunto();

			glutPostRedisplay();

			}
			break;
		}
	}
}
void mover_raton(int x, int y)
{
	y = TamVentana - y;
	if(dibuja)
	{
		if(drawFigure)
		{
			figura.actualizaProximoPunto(Vec2(x*AF/TamVentana-AF/2.0, y*AF/TamVentana-AF/2.0));
		}
		else
		{
			trayectoria.actualizaProximoPunto(Vec2(x*AT/TamVentana-AT/2.0, y*AT/TamVentana));
		}
	}
	glutPostRedisplay();
}
void dibujarEjesT(bool esTrayectoria)
{		
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		//eje Y
		esTrayectoria ? glColor3f(0, 1, 0) : glColor3f(0, 0, 1);
		glVertex2f(0,-AT/3);
		glVertex2f(0,AT/3);
		//eje X
		glColor3f(1, 0, 0);
		glVertex2f(-AT/3,0.5);
		glVertex2f(AT/3,0.5);
	glEnd();
}

void dibujarEjesF(bool esTrayectoria)
{		
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		//eje Y
		esTrayectoria ? glColor3f(0, 1, 0) : glColor3f(0, 0, 1);
		glVertex2f(0,-AF/3);
		glVertex2f(0,AF/3);
		//eje X
		glColor3f(1, 0, 0);
		glVertex2f(-AF/3,0);
		glVertex2f(AF/3,0);
	glEnd();
}
void teclado(unsigned char tecla, int x, int y)
{
	switch (tecla)
	{
	case 27: // tecla escape
		//guardarFigura(fichero); 
		//guardarTrayectoria(fichero); 
		break;

	case 112: // tecla p (parar)
		dibuja = 0; 
		//guardarTrayectoria(fichero); 
		break;	


	case 101: // tecla e (empiezar)
		dibuja = 1; 
		//guardarTrayectoria(fichero); 
		break;	
	}
}
void display(void)
{
	glutSetWindow(idPrincipal);
	//glClearColor(fondoRojo,fondoVerde,fondoAzul,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	glutSwapBuffers();
}
void centralDisplay(void)
{
	glutSetWindow(idCentral);
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor(fondoRojo,fondoVerde,fondoAzul,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(!drawFigure)//mostramos trayectoria
	{	
		gluOrtho2D(-AT/2,AT/2,0,AT);

		//dibujar rejillas 
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(-AT/2,0,0);
		//rejilla.dibujar(TamVentana);
		rejilla.dibujar(AT);
		glPopMatrix();

		//dibujar ejes
		glPushMatrix();
		glScalef(1,2.5,1);
		dibujarEjesT(1);
		glPopMatrix();

		//dibujar titulito "TRAYECTORIA"
		glColor3f(.0, .0, .0);
		glRasterPos2f (AT/-2.5, AT/1.1);
		dibujarTitulo("TRAYECTORIA");

		//dibujar nombres ejes
		glColor3f(.0, .0, .0);
		glRasterPos2f (AT/4, AT/40);
		dibujarTitulo("X");
					
		glRasterPos2f (AT/40, AT/1.3);
		dibujarTitulo("Y");

		//dibujar trayectoria
		trayectoria.dibujar(dibuja);
	}

	else//mostramos la figura
	{       
		gluOrtho2D(-AF/2.0,AF/2.0,-AF/2,AF/2);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//dibujar rejillas
		glPushMatrix();
		glTranslatef(-AF/2,-AF/2,0);
		//rejilla.dibujar(TamVentana);
		rejilla.dibujar(AF);
		glPopMatrix();
		
		// Dibujar ejes
		dibujarEjesF(0);

		glColor3f(.0, .0, .0);
		glRasterPos2f (AF/-2.5, AF/2.5);
		dibujarTitulo("FIGURA");
	

		//dibujar nombres ejes
		glColor3f(.0, .0, .0);
		glRasterPos2f (AF/4, AF/40);
		dibujarTitulo("X");

		glRasterPos2f (AF/40, AF/4);
		dibujarTitulo("Z");

		//dubujar figura
		figura.dibujar(dibuja);
	}

	glFlush();
	glutSwapBuffers();
}
void centralRedisplay (int w, int h)
	{
	glutReshapeWindow(TamVentana, TamVentana);
	}
void redisplay (int w, int h)
	{
	glutReshapeWindow(TamVentanaX, TamVentanaY);
	glViewport(0,0,400,400);
	}
void subDisplay1(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	//Dibujar nombres
	glColor3f(.0, .0, .0);
	glRasterPos2f (32, 91);
	dibujarTitulo("SEBASTIAN MATYSIAK");
	glRasterPos2f (32, 105);
	dibujarTitulo("MATTEO MASCIOTTA");
	
	//Dibujamos un cuadradito alrededor de los nombres
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(10);
	glBegin(GL_LINES);
		glVertex2f(20.0,20.0); 
		glVertex2f(180.0,20.0);
	
		glVertex2f(20.0,20.0); 
		glVertex2f(20.0,180.0);

		glVertex2f(180.0,20.0); 
		glVertex2f(180.0,180.0);

		glVertex2f(180.0,180.0); 
		glVertex2f(20.0,180.0);
	glEnd();

	glutSwapBuffers();
}
void redisplay1 (int w, int h)
	{

	}
///-/-/-/-/----end ventana 1---/-/-/-/-/



//**************ventana 2*******************
void subDisplay2(void)
{

	glClear (GL_COLOR_BUFFER_BIT); 
	glClearColor (fondoRojo,fondoVerde,fondoAzul, 1.0);

	if(drawFigure)//mostramos trayectoria
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-AT/2,AT/2,0,AT);

		//dibujar rejillas
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(-AT/2,0,0);
		//rejilla.dibujar(TamVentana);
		rejilla.dibujar(AT);
		glPopMatrix();

		//dibujar ejes
		glPushMatrix();
		glScalef(1,2.5,1);
		dibujarEjesT(1);
		glPopMatrix();

		//dibujar nombres ejes
		glColor3f(.0, .0, .0);
		glRasterPos2f (AT/4, AT/40);
		dibujarTitulo("X");
					
		glRasterPos2f (AT/40, AT/1.3);
		dibujarTitulo("Y");

		//dibujar titulito "TRAYECTORIA"
		glColor3f(.0, .0, .0);
		glRasterPos2f (AT/-2.5, AT/1.1);
		dibujarTitulo("TRAYECTORIA");

		//dibujar trayectoria
		trayectoria.dibujar(dibuja);
	}

	else//mostramos la figura
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-AF/2.0,AF/2.0,-AF/2,AF/2);

		//dibujar rejillas
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(-AF/2,-AF/2,0);
		//rejilla.dibujar(TamVentana);
		rejilla.dibujar(AF);
		glPopMatrix();

		// Dibujar ejes
		dibujarEjesF(0);

		glColor3f(.0, .0, .0);
		glRasterPos2f (AF/-2.5, AF/2.5);
		dibujarTitulo("FIGURA");

		//dibujar nombres ejes
		glColor3f(.0, .0, .0);
		glRasterPos2f (AF/4, AF/40);
		dibujarTitulo("X");

		glRasterPos2f (AF/40, AF/4);
		dibujarTitulo("Z");

		//dibujar figura
		figura.dibujar(dibuja);
	}

	glFlush();
	glutSwapBuffers();
}
void redisplay2 (int w, int h)
	{
	glutReshapeWindow(TamSubVentana, TamSubVentana);
	}
///-/-/-/------end ventana 2----/-/-/-/-/-/-/-/-/-/



// to get the mouse click on the subwindow2 to switch the windows each other
//TODO -- reshape the size of the 2 draws

void click_raton_sub2(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state==GLUT_UP)
			{
				drawFigure=!drawFigure;
				// to refresh both windows (sub1 y principal)
				//if not, refreshing only on mouse over window
				glutPostRedisplay();
				glutSetWindow(idCentral);
				glutPostRedisplay();
			}
			break;
	}
}
void inicializa (void) 
	{
	glutSetWindow(idPrincipal);

	glutMouseFunc (click_raton);
	glutPassiveMotionFunc(mover_raton);
	glutReshapeFunc (redisplay);
	glutKeyboardFunc(teclado);
}
void inicializaCentral(void) 
	{

	// selecciona el color de borrado 
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glViewport(0,0,400,400);
	

	// inicializa los valores de la vista 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glutReshapeFunc (redisplay1);

	glutReshapeFunc (centralRedisplay);
	glutMouseFunc (click_raton);
	glutPassiveMotionFunc(mover_raton);

	glutKeyboardFunc(teclado);


	}
void inicializaSub1 (void) 
	{

	// selecciona el color de borrado 
	glClearColor (0.0, 1.0, 1.0, 0.0);

	// inicializa los valores de la vista 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, TamSubVentana, 0,TamSubVentana);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutReshapeFunc (redisplay1);
	}
void inicializaSub2 (void) 
	{

	// selecciona el color de borrado 
	glClearColor (1.0, 1.0, 1.0, 1.0);

	
	//funcion para determinar que hacer si el raton
	// esta en la subventana 1 y hay una pinchada 
	glutMouseFunc(click_raton_sub2);
	glutReshapeFunc (redisplay2);

	}
void inicializarMenu(int a) 
{
	switch(a)
	{
		case 0: cargar(1); actualizarVentanas(); break;
		case 1: guardar(1); break;
		case 2: trayectoria.limpiarTrayectoria(); figura.limpiarFigura(); actualizarVentanas(); break;
		case 3: rejilla.mostrarRejilla(); actualizarVentanas(); break;
	}
}

void cambiarColorRejilla(int a)
{
	switch(a)
	{
		case 0: rejilla.cambiarColor(.0,.0,.0); 
				actualizarVentanas();
				break;

		case 1: rejilla.cambiarColor(.9,.65,.65); 
				actualizarVentanas();
				break;
	}
}
void cambiarColorFondo(int a)
{
	switch(a)
	{
		case 0:  fondoRojo = .0; fondoVerde = .0; fondoAzul = .0; 
			
			glutSetWindow(idCentral);
			glClear (GL_COLOR_BUFFER_BIT);
		
			glClearColor(fondoRojo,fondoVerde,fondoAzul,1.0);
			glutPostRedisplay();
			glutSetWindow(idSub2);


			glClear (GL_COLOR_BUFFER_BIT);
			glClearColor(fondoRojo,fondoVerde,fondoAzul,1.0);
			glutPostRedisplay();

			 break;
		case 1:  fondoRojo = 1.0; fondoVerde = 1.0; fondoAzul = 1.0;
				glutSetWindow(idCentral);
			glClear (GL_COLOR_BUFFER_BIT);
		
			glClearColor(fondoRojo,fondoVerde,fondoAzul,1.0);
			glutPostRedisplay();
			glutSetWindow(idSub2);
			glClear (GL_COLOR_BUFFER_BIT);
			glClearColor(fondoRojo,fondoVerde,fondoAzul,1.0);
			glutPostRedisplay();
			
			break;
	}
}
void cambiarColorFigura(int a)
{
	switch(a)
	{
		case 0:  figura.cambiarColor(0.0,0.0,0.0); actualizarVentanas(); break;
		case 1:  figura.cambiarColor(0.0,1.0,1.0); actualizarVentanas(); break;
	}
}
void cambiarColorTrayectoria(int a)
{
	switch(a)
	{
		case 0:  trayectoria.cambiarColor(1.0,1.0,1.0); actualizarVentanas(); break;
		case 1:  trayectoria.cambiarColor(0.0,1.0,1.0); actualizarVentanas(); break;
	}
}
void cambiarResRejilla(int a)
{
	switch(a)
	{
		case 0:  rejilla.cambiarResolucion(1); actualizarVentanas(); break;
		case 1:  rejilla.cambiarResolucion(0); actualizarVentanas(); break;
	}
}
void borrarUltimoPunto(int a)
{
	switch(a)
	{
		case 0:  trayectoria.borrarUltimoPunto(); actualizarVentanas(); break;
		case 1:  figura.borrarUltimoPunto(); actualizarVentanas(); break;
	}
}
void cambiarColor(int a) {}
void crearMenu()
{
	int menuColorRejilla = glutCreateMenu(cambiarColorRejilla);
	glutAddMenuEntry("Negro", 0);
	glutAddMenuEntry("Default", 1);

	int menuColorFondo = glutCreateMenu(cambiarColorFondo);
	glutAddMenuEntry("Negro", 0);
	glutAddMenuEntry("Blanco", 1);

	int menuColorFigura = glutCreateMenu(cambiarColorFigura);
	glutAddMenuEntry("Negro", 0);
	glutAddMenuEntry("Magenta", 1);

	int menuColorTrayectoria = glutCreateMenu(cambiarColorTrayectoria);
	glutAddMenuEntry("Negro", 0);
	glutAddMenuEntry("Magenta", 1);

	int menuCambiarColor = glutCreateMenu(cambiarColor);
	glutAddSubMenu("Rejilla", menuColorRejilla);
	glutAddSubMenu("Fondo", menuColorFondo);
	glutAddSubMenu("Figura", menuColorFigura);
	glutAddSubMenu("Trayectoria", menuColorTrayectoria);

	int menuCambiarResRejilla = glutCreateMenu(cambiarResRejilla);
	glutAddMenuEntry("Aumentar", 0);
	glutAddMenuEntry("Disminuir", 1);

	int menuBorrarPunto = glutCreateMenu(borrarUltimoPunto);
	glutAddMenuEntry("Trayectoria", 0);
	glutAddMenuEntry("Figura", 1);

	int menuPrincipal = glutCreateMenu(inicializarMenu);
	glutAddMenuEntry("Visualizar rejilla", 3);
	glutAddSubMenu("Cambiar res rejilla", menuCambiarResRejilla);
	glutAddSubMenu("Cambiar color", menuCambiarColor);
	glutAddSubMenu("Borrar el ultimo punto", menuBorrarPunto);
	glutAddMenuEntry("Cargar fichero", 0);
	glutAddMenuEntry("Guardar fichero", 1);
	glutAddMenuEntry("Limpiar todo", 2);
		
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void main(int argc, char** argv)
{
	// Inicializa la librer�a auxiliar GLUT
	glutInit(&argc, argv);

	// Inicializa el modo de visualizaci�n
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);


	// Indica el tama�o de la ventana (ancho,alto)
	glutInitWindowSize (TamVentanaX, TamVentanaY); 

	// Indica la posici�n inicial (xmin,ymin) 
	glutInitWindowPosition (100, 100);

	cout << "Pon el nombre del fichero: ";
	cin >> fnombre;

	long temp;
	cout << "Pon la anchura de la ventana de trayectoria, 0 para valor de default: " ;
	cin >> temp;
	if (temp==0){
		cout << "Cargado valor por defecto: 400 " <<endl;}
	else 
		AT = temp;

		cout << "Pon la anchura de la ventana de figura, 0 para valor de default: ";
	cin >> temp;
	if (temp==0){
		cout << "Cargado valor por defecto: 400 " <<endl;}
	else 
		AF = temp;


	// Abre la ventana con el t�tulo indicado
	idPrincipal = glutCreateWindow ("Editor de figuras - IGU 2012");

	// Indica cual es la funci�n de dibujo
	glutDisplayFunc(display); 

	//------subventanaIschierda-------
	idSub1 = glutCreateSubWindow(idPrincipal, 0,0,TamSubVentana,TamSubVentana);
	inicializaSub1();
	glutDisplayFunc(subDisplay1);

	//------subventanaDerecha-------
	idSub2 = glutCreateSubWindow(idPrincipal, 200,0,TamSubVentana,TamSubVentana);
	inicializaSub2();
	glutDisplayFunc(subDisplay2);



	//------subventanaCentral-------
	idCentral = glutCreateSubWindow(idPrincipal,0,200,400,400);
	inicializaCentral();
	glutDisplayFunc(centralDisplay);

	// Inicializar valores
	inicializa ();

	//inicializar menu
	glutSetWindow(idCentral);
	crearMenu();

	// Comienza el bucle de dibujo y proceso de eventos.
	glutMainLoop();
}




