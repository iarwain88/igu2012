#include <stdlib.h>
#include <gl/glut.h>
#include <gl/glui.h>
#include <iostream>
#include <fstream>
#include "Figura.h"
#include "Rejilla.h"
#include "Trayectoria.h"

using namespace std;

int fondoRojo = 1.0;
int fondoVerde = 1.0;
int fondoAzul = 1.0;
int   TamVentana = 400;
int   TamVentanaX = 400;
int   TamVentanaY = 600;
int   TamSubVentana = 200;
float AT = 400; 
float AF = 400;// tamano default
int   main_window;
int	  idCentral;
int	  idSub1;
int	  idSub2;
int   idv3d; //id ventana dibujo en 3D
int idSub23d;
int idSub13d;
int id3dCentral;
bool  dibuja = 0;
bool  drawFigure=1;
bool rotationON = false;

Figura  figura; //ventana ischierda
Trayectoria  trayectoria; //dibujo de trayectoria
Rejilla rejilla;  //para dibujar rejillas
Figura figuras[10]; //para guardar figuras de extrusion

//glui variables
int  lighting;
char *file;
float sphere_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float torus_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
float zoomZ = 0;
float zoom = 0;
GLUI_EditText *edittext;



float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

/** These are the live variables passed into GLUI ***/
int   obj_type = 1;
int   segments = 10;
int   segments2 = 10;

float scale = 1;
int   show_sphere=1;
int   show_torus=1;
int   show_axes = 1;
int   show_text = 1;
int show_teapot = 0;

/********** User IDs for callbacks ********/
#define CARGAR_ID            304


void dibujarTitulo (char *t)
{
  unsigned int i;
  for (i = 0; i < strlen(t); i++)
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, t[i]);
}


void cargar3d()
{

	for (int i = 0; i < 10; i++)
	{
		figuras[i].limpiarFigura();
	}
	
trayectoria.limpiarTrayectoria(); figura.limpiarFigura();


 cout << "nombre fichero: " << edittext->get_text() <<endl;

	ifstream fichero(edittext->get_text());
	if (fichero.is_open() == true)
	{
		fichero >> AT;
		trayectoria.cargar(fichero);
		fichero >> AF;
		figura.cargar(fichero);	
	}
	fichero.close();
}	



void extrusion()
{
//----------------------------------------------------cargamos la trayectoria y la figura del fichero-------------------------------------------------------//

	//cargar3d();

//----------------------------------------------------guardamos el numero de elementos de figura y trayectoria-------------------------------------------------------//

	int n = figura.total();
	int w = trayectoria.total();
		
//----------------------------------------------------copiamos la figura principal para la base de traslación-------------------------------------------------------//
	
	for (int i = 0; i < n; i++)
	{
		figuras[0].guardarPunto(figura.get_puntos().at(i));	
	}

//----------------------------------------------------copiamos puntos de la trayectoria para poder trasladar la figura-------------------------------------------------------//

	VectorVec2 trayectoriaAux = trayectoria.get_pTrayectoria();
	
//----------------------------------------------------traslación de figuras-------------------------------------------------------//

	for (int h = 0; h < w-1; h++)
	{
		for (int i = 0; i < n; i++)
		{
			figuras[h+1].guardarPunto(Vec2(figuras[0].get_puntos().at(i).x + trayectoriaAux.at(h+1).x, figuras[h].get_puntos().at(i).y));
		}		
	}

//----------------------------------------------------dibujar la extrusion-------------------------------------------------------//
	
	for (int i = 0; i < w; i++)
	{
		figuras[i].dibujar3D(trayectoriaAux,i);
	}

	for (int j = 0; j < n; j++)
	{
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < w; i++)
		{
			glVertex3f(figuras[i].get_puntos().at(j).x, trayectoriaAux.at(i).y, -figuras[i].get_puntos().at(j).y);
			//cout << figuras[i].get_puntos().at(j).x << " " << trayectoriaAux.at(i).y << " " << figuras[i].get_puntos().at(j).y <<endl;
		}
		glEnd();
	}

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


void myGlutMotion(int x, int y )
{
	
	if(rotationON){
	rotationX += (float) (y - last_y);
	rotationY += (float) (x - last_x);

	last_x = x;
	last_y = y;
	}
	else{

		if(y>last_y)
		zoomZ=zoomZ-0.05;
		else
			zoomZ=zoomZ+0.05;
		last_y = y;
		
		
	}
	

	glutSetWindow(id3dCentral);
	glutPostRedisplay(); 
	/*glutSetWindow(idv3d);
	glutPostRedisplay(); */
 cout << "myGlutMotion " << endl;
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );

	xy_aspect = (float)tw / (float)th;
 cout << "myGlutReshape " << endl;
	glutPostRedisplay();
}



void draw_axes( float scale )
{
	glPushMatrix();
	glScalef( 1, 1, 1 );

	glBegin( GL_LINES );

	glColor3f( 1.0, 0.0, 0.0 );
	glVertex3f( .8f, 0.05f, 0.0 );  glVertex3f( 1.0, 0.25f, 0.0 ); /* Letter X */
	glVertex3f( 0.8f, .25f, 0.0 );  glVertex3f( 1.0, 0.05f, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 1.0, 0.0, 0.0 ); /* X axis      */

	glColor3f( 0.0, 1.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 1.0, 0.0 ); /* Y axis      */

	glColor3f( 0.0, 0.0, 1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 1.0 ); /* Z axis    */
	glEnd();

	glPopMatrix();

}


void myGlutDisplay( void )
{
	glClearColor( .9f, .9f, .9f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(60.0f,xy_aspect,0.1,15.0);
	//glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );


	//gluPerspective (90, 1, 1.0, 1000.0);
	//gluOrtho2D(0,1,0,1);
	//gluLookAt(400.0,400.0,400.0,0.0,0.0,0.0,0.0,1.0,0.0);	


	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();


	glTranslatef( 0, 0.0, -2.6f );
	glTranslatef( 0.0, 0.0, zoomZ );
		if (show_teapot && show_sphere)
		glTranslatef( -0.7, 0.0,0);
		
	glRotatef( rotationY, 0.0, 1.0, 0.0 );
	glRotatef( rotationX, 1.0, 0.0, 0.0 );


	if ( show_axes ){
		draw_axes(.52f);
		glPushMatrix();
	}	
	glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	//glMultMatrixf( view_rotate );


	glScalef( scale, scale, scale );




	/*** Now we render object, using the variables 'obj_type', 'segments', and
	'wireframe'.  These are _live_ variables, which are transparently 
	updated by GLUI ***/

	//glPushMatrix();

	
	
	
//glTranslatef( -.5, 0.0, 0.0 );
	glMultMatrixf( sphere_rotate );
	
	if ( show_sphere)
	glutWireSphere( .4, segments, segments );
        
	glScalef( 0.002, 0.002, 0.002 );
	extrusion();
	    glScalef( 1, 1, 1 );
		
	
	if(show_teapot && show_sphere)
	  glTranslatef(1, 0.0, 0.0 );
	
	if ( show_teapot)		
		glutWireTeapot(300);

	glScalef( 0.002, 0.002, 0.002 );
	extrusion();
	    glScalef( 1, 1, 1 );

	//if ( show_axes )
	//	draw_axes(.52f);
	

	
//	glTranslatef( .5, 0.0, 0.0 );
//	glMultMatrixf( torus_rotate );
	//if ( show_torus )
	//	glutWireTorus( .15,.3,16,segments );

	if ( show_axes ){		
		glPopMatrix();
	}

	//glPopMatrix();

	
	glutSwapBuffers(); 
}





void centralRedisplay (int w, int h)
	{
	glutReshapeWindow(TamVentana, TamVentana);
	}


void redisplay1 (int w, int h)
	{

	}

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


void subDisplay2inv(void)
{

	glClear (GL_COLOR_BUFFER_BIT); 
	glClearColor (fondoRojo,fondoVerde,fondoAzul, 1.0);

	if(!drawFigure)//mostramos trayectoria
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


void redisplay3d(int w, int h){
	glutReshapeWindow(700, 700);
	
}
void v3Ddisplay(void){
	
	glutReshapeFunc (redisplay3d);
	// selecciona el color de borrado 
	glClearColor (1.0, 1.0, 1.0, 1.0);
	
	glScalef( 0.002, 0.002, 0.002 );
	extrusion();
	    glScalef( 1, 1, 1 );

}


void inicializaSub2 (void) 
	{

	// selecciona el color de borrado 
	glClearColor (1.0, 1.0, 1.0, 1.0);

	
	//funcion para determinar que hacer si el raton
	// esta en la subventana 1 y hay una pinchada 
	
	

	}





void inicializaSub3d (void) 
	{

	// selecciona el color de borrado 
	glClearColor (1.0, 1.0, 1.0, 1.0);

	}





void inicializa3D(void){
	
	
	glClearColor (0.0, 0.0, 1.0, 1.0);
}


void inicializa3dCentral(void){
	{

	//glMatrixMode (GL_PROJECTION);   
	//glLoadIdentity();  
	//gluPerspective (90, 1, 1.0, 420.0);
	////gluOrtho2D(0,1,0,1);
	//gluLookAt(100.0,100.0,100.0,0.0,0.0,0.0,0.0,1.0,0.0);	
	//
		
	}
}

void refreshSubventanas3d(){

				glutSetWindow(idSub13d);
				glutPostRedisplay();
				glutSetWindow(idSub23d);
				glutPostRedisplay();
}


/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control )
{
	if ( control == CARGAR_ID )
	{
		cargar3d();
		refreshSubventanas3d();
	
	}
}
/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
	/* According to the GLUT specification, the current window is 
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != id3dCentral ) 
		glutSetWindow(id3dCentral);  

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
	application  */
 //cout << "myGlutIdle " << endl;
	glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
	if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
		last_x = x;
		last_y = y;
		rotationON = 1;
		
	}

if ( button ==GLUT_RIGHT_BUTTON && button_state == GLUT_DOWN ) {
		rotationON = false;
		last_y = y;

		
	}
}




void myGlutKeyboard(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case 27: 
	case 'q':
		exit(0);
		break;
	};

	glutPostRedisplay();
}




void main(int argc, char** argv)
{
	// Inicializa la librer�a auxiliar GLUT
	glutInit(&argc, argv);

	// Inicializa el modo de visualizaci�n
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);


	// Indica la posici�n inicial (xmin,ymin) 
	glutInitWindowPosition (200, 50);



glutInitWindowSize (700, 700); 


	//------Ventana con dibujo en 3D-------
	idv3d = glutCreateWindow("dibujo en 3D");
	glutDisplayFunc(v3Ddisplay);
	glutReshapeFunc(redisplay3d);
	//glutMotionFunc( myGlutMotion );
	inicializa3D();

	//------subventana 3D Central-------
	id3dCentral = glutCreateSubWindow(idv3d,0,0,577,620);
   
	glutDisplayFunc( myGlutDisplay );

	GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
	
	GLUI_Master.set_glutSpecialFunc( NULL );

	glutMotionFunc( myGlutMotion );
		GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
	
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
		inicializa3dCentral();
	

	 

	idSub23d = glutCreateSubWindow(id3dCentral, 0,0,100,100);
	inicializaSub3d();
	glutDisplayFunc(subDisplay2);

	idSub13d = glutCreateSubWindow(id3dCentral, 100,0,100,100);
	inicializaSub3d();
glutDisplayFunc(subDisplay2inv);


	
// GLUI -------------------------------------------------------------
	
	/*
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
	GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
	glutMotionFunc( myGlutMotion );*/
	//

	
//menu derecha
	 GLUI *glui = GLUI_Master.create_glui_subwindow( idv3d,
		 GLUI_SUBWINDOW_RIGHT );
	 new GLUI_StaticText( glui, "Menu de control" );
	 new GLUI_Separator( glui );
	
	
	edittext =
     glui->add_edittext( "Text:", GLUI_EDITTEXT_TEXT,file);

	edittext->set_text("tio");
	edittext->set_h(20);
	edittext->set_w(50);
	

	/**** Link windows to GLUI, and register idle callback ******/

	glui->set_main_gfx_window( idv3d );


	//cargar fichero button
	new GLUI_Button( glui, "Cargar", CARGAR_ID, control_cb);
new GLUI_Separator( glui );
	glui->add_checkbox( "tetera", &show_teapot);
	glui->add_checkbox( "esfera", &show_sphere);
new GLUI_Separator( glui );
	new GLUI_StaticText( glui, "Escala" );
	GLUI_Scrollbar *sb;
	sb = new GLUI_Scrollbar( glui, "Escala",GLUI_SCROLL_HORIZONTAL,&scale);
	sb->set_float_limits(0.2f,4.0f);
	sb->set_w(100);
	//menu abajo
	
	GLUI *glui2 = GLUI_Master.create_glui_subwindow( idv3d,
    GLUI_SUBWINDOW_BOTTOM );
	//GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objetos", view_rotate );
	//view_rot->set_spin( 1.0 );
	//new GLUI_Column( glui2, false );
	GLUI_Rotation *sph_rot = new GLUI_Rotation(glui2, "Rotacion", sphere_rotate );
	sph_rot->set_spin( .98 );
	new GLUI_Column( glui2, false );
	/*GLUI_Rotation *tor_rot = new GLUI_Rotation(glui2, "Toro", torus_rotate );
	tor_rot->set_spin( .98 );
	new GLUI_Column( glui2, false );*/
	GLUI_Translation *trans_x = 
		new GLUI_Translation(glui2, "Objetos X", GLUI_TRANSLATION_X, obj_pos );
	trans_x->set_speed( .005 );
	new GLUI_Column( glui2, false );
	GLUI_Translation *trans_y = 
		new GLUI_Translation( glui2, "Objetos Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
	trans_y->set_speed( .005 );
	new GLUI_Column( glui2, false );
	GLUI_Translation *trans_z = 
		new GLUI_Translation( glui2, "Objetos Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
	trans_z->set_speed( .005 );

	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );

		
// END GLUI -------------------------------------------------------------
	

	glutMainLoop();
}




