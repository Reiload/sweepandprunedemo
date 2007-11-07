/*	screenobject.cpp - implementation file for AABB class

Ben Gilbert W0062289
3 November 2007

CSC8490 Assignment 2

*/

#include "glut.h"
#include "screenobject.h"
#include "l3ds.h"


using namespace SweepAndPrune;

/* push a "response" function that allows the player to push the other AABB */
void SweepAndPrune::push(AABB* aabb1, AABB* aabb2) {
	
	Vector3 dir;
	if( aabb1->isMovable() ) {
		dir = aabb2->GetDirectionOfTravel();
		dir.scale(1.01); // always maintain a small gap by pushing the other AABB a bit further than dir
		aabb1->Move(dir);
	} else {
		dir = aabb1->GetDirectionOfTravel();
		dir.scale(1.01);
		aabb2->Move(dir);
	}

}
