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
#include "element.h"



namespace SweepAndPrune {	

	class Element;

	class AABB{
	public:
		AABB(const Vector3 mins1, const Vector3 maxs1) {
			minElement = new Element(this, mins1); 
			maxElement = new Element(this, maxs1);
		}
	protected:
		Element* minElement;
		Element* maxElement;
	};

	

	enum DIRECTION_OF_TRAVEL { XNEG, XPOS, YNEG, YPOS, ZNEG, ZPOS, NONE }; // these are the directions the ToyBlocks can moving in

	class ToyBlocks : public AABB{
	public:
		ToyBlocks(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  AABB(mins1, maxs1), colour(col), lastDirection(NONE) {
		  }
		  void MoveX(GLfloat x) {
			  minElement->SetX(minElement->GetX() + x);
			  maxElement->SetX(maxElement->GetX() + x);
		  }
		  void MoveY(GLfloat y) {
			  minElement->SetY(minElement->GetY() + y);
			  maxElement->SetY(maxElement->GetY() + y);
		  }
		  void MoveZ(GLfloat z) {
			  minElement->SetZ(minElement->GetZ() + z);
			  maxElement->SetZ(maxElement->GetZ() + z);
		  }
		  void Draw(){
			  GLfloat minX = minElement->GetX();
			  GLfloat minY = minElement->GetY();
			  GLfloat minZ = minElement->GetZ();
			  GLfloat maxX = maxElement->GetX();
			  GLfloat maxY = maxElement->GetY();
			  GLfloat maxZ = maxElement->GetZ();
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
		  void SetDirectionOfTravel(DIRECTION_OF_TRAVEL dir) {lastDirection = dir;}
		  DIRECTION_OF_TRAVEL GetDirectionOfTravel() const {return lastDirection;}
	private:
		DIRECTION_OF_TRAVEL lastDirection; // records the direction that the cube is try to move in	
		const GLfloat* colour; // colour of the ToyBlocks
	};


}

#endif
