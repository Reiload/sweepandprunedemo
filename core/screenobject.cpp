/*	screenobject.cpp - implementation file for AABB class

Ben Gilbert W0062289
3 November 2007

CSC8490 Assignment 2

*/

#include "glut.h"
#include "utilities.h"
#include "screenobject.h"
#include "l3ds.h"


using namespace SweepAndPrune;

/* push a "response" function that allows the player to push the other AABB */
void SweepAndPrune::push(AABB* aabb1, AABB* aabb2) {
	Vector3* pos1 = aabb1->CreatePositionVector();
	Vector3* pos2 = aabb2->CreatePositionVector();
	Vector3 dir;
	if( aabb1->isMovable() ) {
		dir = aabb2->GetDirectionOfTravel();
		aabb1->Move(dir);
	} else {
		dir = aabb1->GetDirectionOfTravel();
		aabb2->Move(dir);
	}
	delete pos1;
	delete pos2;
}
