/*	screenobject.cpp - implementation file for Fish class

Author: Ben Gilbert W0062289
CSC3406 Assignment 2

*/

#include "glut.h"
#include "utilities.h"
#include "screenobject.h"
#include "l3ds.h"

using namespace SweepAndPrune;


void SweepAndPrune::push(AABB* aabb1, AABB* aabb2) {
	Vector3* pos1 = aabb1->CreatePositionVector();
	Vector3* pos2 = aabb2->CreatePositionVector();
	Vector3 delta;
	delta.setDiff( *pos1, *pos2);
	if( aabb1->isMovable() ) {
		aabb1->Move(delta);
	} else {
		delta.flip();
		aabb2->Move(delta);
	}
	delete pos1;
	delete pos2;
}
