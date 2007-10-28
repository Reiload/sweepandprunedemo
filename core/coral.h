/* coral.h - header file for coral class
		
	CSC3406 Assignment 3
	Author: Ben Gilbert W0062289
	27 May 2007

	Create foreground coral for aquarium scene.  Coral is generated using L-System rules

*/

#ifndef __CORAL_H__
#define __CORAL_H__

#include "utilities.h"
using namespace Sea;

namespace Plant {

	/*** Return Angle +/- random tolerance ***/
	float angle(float base, float tolerance) {
		return GetRandom(base-tolerance, base + tolerance);
	}

	/*** Three dimensional point - Provided by Penny de Byl ***/
	class Point3 {
	public:
		float x, y, z;
		Point3() {x = y = z = 0.0f;} //constructor 1
		Point3(float xx, float yy, float zz) {x=xx; y=yy; z=zz;} //constructor 2
		void set(float xx, float yy, float zz) {x=xx; y=yy; z=zz;}
		float getX() {return x;}
		float getY() {return y;}
		float getZ() {return z;}
		void draw(void) {
			glBegin(GL_POINTS); //draw this point
			glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
			glEnd();
		}

	};

	/*** Coral Class ***/
	class Coral {
	public:	
		Coral(char* _atom, char* _fstr, char* _xstr, char* _ystr, float _bw, float _bl, 
			  GLfloat* _ambient) {
			atom = _atom;	
			fstr = _fstr;
			xstr = _xstr;
			ystr = _ystr;
			qobj = gluNewQuadric();
			bw = _bw;
			bl = _bl;
			ambient = _ambient;
		} 
		void Compile(int order); // Compile structure to OpenGL display list
		void Draw();
	private:
		void DrawBranch();
		// L-System Strings
		char* atom;
		char* fstr;
		char* xstr;
		char* ystr;

		GLUquadricObj *qobj; 
		float bw; // branch width
		float bl; // branch length
		GLfloat* ambient;
		void ProduceString(char *st,int order);
		GLuint LTREEDL;
	};

	/*** Process L-System Strings ***/
	void Coral::ProduceString(char *st,int order) {	
		while(*st != '\0') {
			switch(*st) {
			case '[': // push matrix and lighten color and decrease size
				glPushMatrix();
				ambient[0] = ambient[0] * 1.2;
				ambient[1] = ambient[1] * 1.2;
				ambient[2] = ambient[2] * 1.2;
				bw /= 1.2;
				bl /= 1.2;
				glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, ambient);
				break;
			case ']': // pop matrix and restore color and size
				glPopMatrix();	
				ambient[0] = ambient[0] / 1.2;
				ambient[1] = ambient[1] / 1.2;
				ambient[2] = ambient[2] / 1.2;
				bw *= 1.2;
				bl *= 1.2;
				glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, ambient);
				break;
			case '+': // rotate around z axis in clockwise direction
				glTranslated(-bw/2.0, -bw/2.0, 0);
				glRotated(-angle(22,5), 0, 0, 1);
				glTranslated(bw/2.0,bw/2.0,0);
				break;
			case '-': // rotate around z axis in anti-clockwise direction
				glTranslated(-bw/2.0, -bw/2.0, 0);
				glRotated(angle(22,5), 0, 0, 1);
				glTranslated(bw/2.0,bw/2.0,0);
				break;
			case 'F': // Draw branch in forward direction
				if(order > 0)
					ProduceString(fstr, order -1);
				else {
					DrawBranch();
					glTranslated(0, bl, 0.0);
				}
				break;
			case 'X': // Substring
				if(order > 0) {
					ProduceString(xstr, order -1);
					break;
				}
				break;
			case 'Y': // Substring
				if(order > 0) {
					ProduceString(ystr, order -1);
					break;
				}

			case '>': // rotate around x axis in clockwise direction
				glTranslated(-bw/2.0, -bw/2.0,0);
				glRotated(-angle(22,5),1,0,0);
				glTranslated(bw/2.0, bw/2.0,0); 
				break;
			case '<': // rotate around x axis in anti-clockwise direction
				glTranslated(-bw/2.0, -bw/2.0,0);
				glRotated(angle(22,5),1,0,0);
				//glRotated(22,1,0,0);
				glTranslated(bw/2.0, bw/2.0,0); 
				break;
			case '}': // rotate around y axis in clockwise direction
				glTranslated(-bw/2.0, -bw/2.0,0);
				glRotated(-angle(22,5),0,1,0);
				//glRotated(-22,0,1,0);
				glTranslated(bw/2.0, bw/2.0,0); 
				break;
			case '{': // rotate around y axis in anti-clockwise direction
				glTranslated(-bw/2.0, -bw/2.0,0);
				glRotated(angle(22,5),0,1,0);
				//glRotated(22,0,1,0);
				glTranslated(bw/2.0, bw/2.0,0); 
				break;
			}
			st++;
		}
	}

	/*** Draw branch - a simple cylinder ***/
	void Coral::DrawBranch() {
		glPushMatrix();
		glRotated(-90,1,0,0);
		gluCylinder(qobj, bw, bw, bl, 15, 5);
		glPopMatrix();
	}

	/*** Compile Coral Structure to OpenGL display list ***/
	void Coral::Compile(int order) {
		LTREEDL = glGenLists(1);
		glNewList(LTREEDL, GL_COMPILE);
			glMaterialfv(GL_FRONT,GL_AMBIENT, ambient);
			ProduceString(atom, order);
		glEndList();
	}

	/*** Draw Coral structure to screen ***/
	void Coral::Draw() {
		glCallList(LTREEDL);
	}

}
#endif