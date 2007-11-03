/* main.cpp 

	CSC3406 Assignment 3
	Author: Ben Gilbert W0062289
	4 June 2007

	Aquarium scene with reflection and shadows

*/

#pragma warning(disable: 4068)	//ignore warning given by MSVis about some opengl settings

#include <windows.h>
#include <math.h>
#include <iostream>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "camera.h"
#include "shadow.h"
#include "l3ds.h"
#include "screenobject.h"
#include "utilities.h"


using namespace SweepAndPrune;


//used for glut init
//needed but don't really do anything!
int argc = 1;
char *argv = "";

Camera cam;

FileReadWriter fileHandler;
WindowSetup settings;

/*** Global Variables ***/
GLfloat colors1[4] = {0.8, 0.0, 0.0, 1.0};
ToyBlocks ToyBlocks1(Vector3(1.0, 1.0, 1.0), Vector3(2.0, 2.0, 2.0), colors1);


GLfloat colors2[4] = {0.8, 0.8, 0.0, 1.0};
ToyBlocks ToyBlocks2(Vector3(-2.0, 1.0, 1.0), Vector3(-1.0, 2.0, 2.0), colors2);

const GLfloat INC = 0.2;

// Window Dimensions
int windowWidth, windowHeight;

// Aquarium boundaries
const GLfloat XMIN = -6.0;
const GLfloat XMAX = 6.0;
const GLfloat ZMIN = -4.0;
const GLfloat ZMAX = 4.0;
const GLfloat YMIN = 0.0;
const GLfloat YMAX = 4.0;

// Camera position
const GLfloat CAM_X = 0.0;
const GLfloat CAM_Y = 5.0;
const GLfloat CAM_Z = 8.0;

// Direction of camera 
const GLfloat LOOK_X = 0.0;
const GLfloat LOOK_Y = 2.0;
const GLfloat LOOK_Z = 0.0;

// Aquarium floor OpenGL variables
const GLfloat FLOOR_MATERIAL_SHININESS[] = {90.0f};
GLfloat FLOOR_MATERIAL_AMBIENT[] = {0.0, 0.8, 0.8, 0.8f};
GLfloat FLOOR_COLOR[] = {0.0f, 1.0f, 1.0f, 0.8f};
GLfloat FLOOR_NORMAL[] = {0.0f, 1.0f, 0.0f};



/*** Draw Aquarium Floor ***/
void drawFloor(void) {
	glPushMatrix();
		glColor4fv(FLOOR_COLOR);
		glMaterialfv(GL_FRONT,GL_AMBIENT, FLOOR_MATERIAL_AMBIENT);
		glNormal3fv(FLOOR_NORMAL);
		glMaterialfv(GL_FRONT, GL_SHININESS, FLOOR_MATERIAL_SHININESS);
		glBegin(GL_QUADS);
			glVertex3f(XMAX, YMIN, ZMIN);
			glVertex3f(XMIN, YMIN, ZMIN);
			glVertex3f(XMIN, YMIN, ZMAX);
			glVertex3f(XMAX, YMIN, ZMAX);
		glEnd();
	glPopMatrix();
}

/*** Handle Window Reshaping ***/
void myReshape(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w/h, 1.0, 20.0);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);  
}

/*** Draw Aquarium Scene ***/
void render(void)
{
	GLdouble P[5][3];
	GLdouble matrix[4][4];
	GLfloat shadowMat[4][4];
	GLfloat Lf[4];
	GLfloat Lc[4];

	// Lighting
	Lc[0] = 1.0;
	Lc[1] = 1.0;
	Lc[2] = 1.0;
	Lc[3] = 1.0;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, &Lc[0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &Lc[0]);
	glEnable(GL_LIGHT0);

	GLfloat on[4] = { 0.92, 0.92, 0.92, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, on);

	Lf[0] = L[0];
	Lf[1] = L[1];
	Lf[2] = L[2];
	Lf[3] = L[3];

	glLightfv(GL_LIGHT0, GL_POSITION, &Lf[0]);

	// Fog
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG); {
		GLfloat fogColor[4] = {0.1, 0.1, 1.0, 1.0};
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, 0.1);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 1.0);
		glFogf(GL_FOG_END, 5.0);
	}
	
	
	glClearColor(0.1,0.1,1.0,0.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	computeShadowVolume(L, O, M, N, Pbg, P);
	constructShadowVolumeMatrixd(P, matrix);
	shadowMatrix(shadowMat, Pg, Lf);

	// Draw Reflections (mirror images of foreground coral and fish)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CLIP_PLANE0);
	double eqr[] = { 0.0, -1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqr);
	glPushMatrix();
		ToyBlocks1.Draw();
		ToyBlocks2.Draw();
	glPopMatrix();
	
	glDisable(GL_CLIP_PLANE0);
	
	// Draw ToyBlockss
	ToyBlocks1.Draw();
	ToyBlocks2.Draw();
	// Draw semi-transparent floor to screne and stencil buffer
	glDisable(GL_LIGHTING); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x2, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	drawFloor();
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	// Draw foreground coral and fish projected on to floor
	// Also draw to stencil buffer
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
		glMultMatrixf((GLfloat *) shadowMat);
		glStencilFunc(GL_EQUAL, 0x3, 0x2);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		ToyBlocks1.Draw();
		ToyBlocks2.Draw();
	//	drawObjects();
	glPopMatrix();

	// Redraw floor where projected foreground objects were drawn
	// in the previous section, floor drawn with lights disabled
	glStencilFunc(GL_EQUAL, 0x3, 0x3);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDisable(GL_LIGHT0);
	drawFloor();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	
	
}

/*** Draw Aquarium Scene and swap buffers ***/
void myDisplay(void) {
	render();
	glFlush();
	glutSwapBuffers();
}


void mySpecialKeyboardFunc(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT)
	{
		
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		
	}
	else if(key == GLUT_KEY_UP)
	{
			ToyBlocks1.MoveY(INC);
			ToyBlocks1.SetDirectionOfTravel(YPOS);
			
	}
	else if(key == GLUT_KEY_DOWN)
	{
			ToyBlocks1.MoveY(-INC);
			ToyBlocks1.SetDirectionOfTravel(YNEG);
			
	}
	glutPostRedisplay();
	
}

/*** Handle Keyboard ***/
void myKeyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
		case 'w' : 
			ToyBlocks1.MoveZ(-INC);
			ToyBlocks1.SetDirectionOfTravel(ZNEG);
			break;
		case 'W' :
			ToyBlocks1.MoveZ(-INC);
			ToyBlocks1.SetDirectionOfTravel(ZNEG);
			break;
		case 's' : 
			ToyBlocks1.MoveZ(INC);
			ToyBlocks1.SetDirectionOfTravel(ZPOS);
			break;
		case 'S' :
			ToyBlocks1.MoveZ(INC);
			ToyBlocks1.SetDirectionOfTravel(ZPOS);
			break;
		case 'a' :
			ToyBlocks1.MoveX(-INC);
			ToyBlocks1.SetDirectionOfTravel(XNEG);
			break;
		case 'A' :
			ToyBlocks1.MoveX(-INC);
			ToyBlocks1.SetDirectionOfTravel(XNEG);
			break;
		case 'd' :
			ToyBlocks1.MoveX(INC);
			ToyBlocks1.SetDirectionOfTravel(XPOS);
			break;
		case 'D' :
			ToyBlocks1.MoveX(INC);
			ToyBlocks1.SetDirectionOfTravel(XPOS);
			break;
	case 27:
		exit(0);
		break;
	case 'p':
		fileHandler.captureScreen(settings.SCREENWIDTH, settings.SCREENHEIGHT,"capture.bmp");
		break;
	case 'P':
		fileHandler.captureScreen(settings.SCREENWIDTH, settings.SCREENHEIGHT,"capture.bmp");
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

/*** Handle Program Idle ***/
void myIdle() {
	glutPostRedisplay();
}

/*** Main ***/
int main()
{
	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	//load window setups from ini file
	fileHandler.loadSetupFile("setup.ini", &settings);
	glutInitWindowSize(settings.SCREENWIDTH, settings.SCREENHEIGHT);

	glutInitWindowPosition(100, 100);
	glutCreateWindow("Playing with my blocks");

	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialKeyboardFunc);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);	

	// Calculate the aspect ratio of the window
	cam.setShape(60.0f,(GLfloat)640/(GLfloat)480, 1.0f, 20.0f);
	cam.set(Vector3(CAM_X, CAM_Y, CAM_Z),Vector3(LOOK_X, LOOK_Y, LOOK_Z),Vector3(0,1,0));
	
	glEnable(GL_DEPTH_TEST);
	glPointSize(4.0);

	/* Determine the plane equation of the ground plane from three points. */
	findPlane(Pbg, Sbg, Tbg, Rbg);
	findPlane(Pg, Sg, Tg, Rg);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* Scale the orientations for the cut-out. */
	M[0] = 0.3;
	M[1] = 0.0;
	M[2] = 0.0;

	N[0] = 0.0;
	N[1] = 0.0;
	N[2] = 0.3;

	
	ToyBlocks2.SetMovable();
	//// Create 3 Fish objects
	//Fish::File("Fish.3ds");
	//new Fish(Vector3(1.0, 1.1, 1.0), Vector3(1, 0.01, 0.01), 0.05, 0.1, colors1);
	//new Fish(Vector3(1.0, 1.1, -1.0), Vector3(0.1, 0.1, 0.1), 0.05, 0.04, colors2);
	//new Fish(Vector3(-1.0, 1.5, 0.0), Vector3(0.1, 0.1, 0.1), 0.01, 0.3, colors3);
	
	
	glutMainLoop();
	return 0;
}