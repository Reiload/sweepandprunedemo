/*	screenobject.h - Header file for Fish class
	
	Author: Ben Gilbert W0062289
	CSC3406 Assignment 2

	Fish Class represents an individual fish in the ocean scene

*/
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_

//#include <list>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "utilities.h"
#include "csc3406.h"
#include "l3ds.h"
//using std::list;

class Vector3;



namespace SweepAndPrune {	
	class Box{
	public:
		Box(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  mins(mins1), maxs(maxs1), colour(col) {}
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
	private:
		Vector3 mins; // minimum co-ordinates of the box
		Vector3 maxs; // maximum co-ordinates of the box
		const GLfloat* colour; // colour of the box
	};
}
#endif
	