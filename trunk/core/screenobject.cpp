/*	screenobject.cpp - implementation file for Fish class

	Author: Ben Gilbert W0062289
	CSC3406 Assignment 2

*/
#include "glut.h"
#include "utilities.h"
#include "screenobject.h"
#include "l3ds.h"

using namespace Sea;

/*** Move Fish Object ***/
void Fish::Move(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {	
	int r = 0;
		while(true) {
			r = GetRandom(1,1000);
			// check to see if Fish object would be off screen
			// if r < 990 fish turns "randomly"
			if( pos.GetX() + dir.GetX() * speed > xMin  &&
				pos.GetX() + dir.GetX() * speed < xMax &&
				pos.GetY() + dir.GetY() * speed > yMin &&
				pos.GetY() + dir.GetY() * speed < yMax &&
				pos.GetZ() + dir.GetZ() * speed > zMin &&
				pos.GetZ() + dir.GetZ() * speed < zMax  && r < 990) {
				break;	
			}
			// if offscreen generate new velocity vector
			dir.SetX(GetRandom(-100,100));
			dir.SetY(GetRandom(-100,100));
			dir.SetZ(GetRandom(-100,100));
			dir.normalize();
		}
		// set new position
		pos.SetX(pos.GetX() + dir.GetX() * speed);
		pos.SetY(pos.GetY() + dir.GetY() * speed);
		pos.SetZ(pos.GetZ() + dir.GetZ() * speed);
}

/*** Draw Fish Object ***/
void Fish::Draw() 	{
			dir.normalize();
			GLfloat x = dir.GetX();
			GLfloat y = dir.GetY();
			GLfloat z = dir.GetZ();
			glPushMatrix();
				glTranslated(pos.GetX(), pos.GetY(), pos.GetZ());
				GLfloat angle = dir.AngleNegY(); // angle to turn fish by
				Vector3 cross = dir.cross(Vector3(0, -1, 0)); // normal to the plane of orientation
				glRotatef(-angle, cross.GetX(), cross.GetY(), cross.GetZ());
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
				glScalef(size,size,size);
				Scene()->DrawAll();
			glPopMatrix();			
}