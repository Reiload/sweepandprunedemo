/*	screenobject.h - Header file for Fish class

Author: Ben Gilbert W0062289
CSC3406 Assignment 2

Fish Class represents an individual fish in the ocean scene

*/
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_


#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "utilities.h"
#include "csc3406.h"
#include "l3ds.h"


namespace SweepAndPrune {	

	enum DIR { XNEG, XPOS, YNEG, YPOS, ZNEG, ZPOS, NONE };

	class Box{
	public:
		Box(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  mins(mins1), maxs(maxs1), colour(col), lastDir(NONE) {
		  }
		  void MoveX(GLfloat x) {
			  mins.SetX(mins.GetX() + x);
			  maxs.SetX(maxs.GetX() + x);
		  }
		  void MoveY(GLfloat y) {
			  mins.SetY(mins.GetY() + y);
			  maxs.SetY(maxs.GetY() + y);
		  }
		  void MoveZ(GLfloat z) {
			  mins.SetZ(mins.GetZ() + z);
			  maxs.SetZ(maxs.GetZ() + z);
		  }
		  void Draw(){
			  GLfloat minX = mins.GetX();
			  GLfloat minY = mins.GetY();
			  GLfloat minZ = mins.GetZ();
			  GLfloat maxX = maxs.GetX();
			  GLfloat maxY = maxs.GetY();
			  GLfloat maxZ = maxs.GetZ();
			  glPushMatrix();
			  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colour);
			  glBegin(GL_QUADS);
			  glVertex3f(minX, minY, minZ);
			  glVertex3f(maxX, minY, minZ);
			  glVertex3f(maxX, maxY, minZ);
			  glVertex3f(minX, maxY, minZ);

			  glVertex3f(minX, minY, maxZ);
			  glVertex3f(maxX, minY, maxZ);
			  glVertex3f(maxX, maxY, maxZ);
			  glVertex3f(minX, maxY, maxZ);

			  glVertex3f(minX, minY, minZ);
			  glVertex3f(minX, minY, maxZ);
			  glVertex3f(minX, maxY, maxZ);
			  glVertex3f(minX, maxY, minZ);

			  glVertex3f(maxX, minY, minZ);
			  glVertex3f(maxX, minY, maxZ);
			  glVertex3f(maxX, maxY, maxZ);
			  glVertex3f(maxX, maxY, minZ);

			  glVertex3f(minX, minY, minZ);
			  glVertex3f(minX, minY, maxZ);
			  glVertex3f(maxX, minY, maxZ);
			  glVertex3f(maxX, minY, minZ);

			  glVertex3f(minX, maxY, minZ);
			  glVertex3f(minX, maxY, maxZ);
			  glVertex3f(maxX, maxY, maxZ);
			  glVertex3f(maxX, maxY, minZ);

			  glEnd();
			  glPopMatrix();
		  }
		  void SetDir(DIR dir) {lastDir = dir;}
		  DIR GetDir() const {return lastDir;}
	private:
		DIR lastDir; // last direction of movement
		Vector3 mins; // minimum co-ordinates of the box
		Vector3 maxs; // maximum co-ordinates of the box
		const GLfloat* colour; // colour of the box

	};

	struct Elem {
		Elem *pLeft[3];  // Pointers to the previous linked list element (one for each axis)
		Elem *pRight[3]; // Pointers to the next linked list element (one for each axis)
		float value[3];  // All min or all max coordinate values (one for each axis)
		int   minmax:1;  // All min values or all max values?
	};

	struct AABB {
		Elem min;        // Element containing the three minimum interval values
		Elem max;        // Element containing the three maximum interval values
		Box *pObj;    // Pointer to the actual object contained in the AABB
	};
}

#endif
