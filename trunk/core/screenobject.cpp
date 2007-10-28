/*	screenobject.cpp - implementation file for Fish class

Author: Ben Gilbert W0062289
CSC3406 Assignment 2

*/
#include "glut.h"
#include "utilities.h"
#include "screenobject.h"
#include "l3ds.h"

using namespace SweepAndPrune;




Elem *gListHead[3];



bool AABBOverlap(AABB* first, AABB* second) {return false;} // need to fix this
void AddCollisionPair(AABB* first, AABB* second) {}// need to fix this
bool HasCollisionPair(AABB* first, AABB* second) {return false;} // need to fix this
void DeleteCollisionPair(AABB* first, AABB* second) {}// need to fix this

AABB *GetAABB(Elem *pElem)
{
	return (AABB *)(pElem->minmax ? (pElem - 1) : pElem);
}



enum {
	MIN_ELEM = 0, // Indicates AABB minx, miny, or minz element
	MAX_ELEM = 1  // Indicates AABB maxx, maxy, or maxz element
};

const float FLT_MAX = 1000;

void InitializeList() {
	// Initialize the lists, with start and end sentinels
	AABB *pSentinel = new AABB;
	for (int i = 0; i < 3; i++) {
		pSentinel->min.pLeft[i] = NULL; // not strictly needed
		pSentinel->min.pRight[i] = &pSentinel->max;
		pSentinel->max.pLeft[i] = &pSentinel->min;
		pSentinel->max.pRight[i] = NULL; // not strictly needed
		pSentinel->min.value[i] = -FLT_MAX;
		pSentinel->max.value[i] = FLT_MAX;
		gListHead[i] = &pSentinel->min;
	}
	// Note backwardness of initializing these two
	pSentinel->min.minmax = MAX_ELEM;
	pSentinel->max.minmax = MIN_ELEM;
}


void InsertAABBIntoList(AABB *pAABB)
{
	// For all three axes
	for (int i = 0; i < 3; i++) {
		// Search from start of list
		Elem *pElem = gListHead[i];

		// Insert min cell at position where pElem points to first larger element.
		// Assumes large sentinel value guards from falling off end of list
		while (pElem->value[i] < pAABB->min.value[i])
			pElem = pElem->pRight[i];
		pAABB->min.pLeft[i] = pElem->pLeft[i];
		pAABB->min.pRight[i] = pElem;
		pElem->pLeft[i]->pRight[i] = &pAABB->min;
		pElem->pLeft[i] = &pAABB->min;

		// Insert max cell in the same way. Can continue searching from last
		// position as list is sorted. Also assumes sentinel value present
		while (pElem->value[i] < pAABB->max.value[i])
			pElem = pElem->pRight[i];
		pAABB->max.pLeft[i] = pElem->pLeft[i];
		pAABB->max.pRight[i] = pElem;
		pElem->pLeft[i]->pRight[i] = &pAABB->max;
		pElem->pLeft[i] = &pAABB->max;
	}

	// Now scan through list and add overlap pairs for all objects that
	// this AABB intersects. This pair tracking could be incorporated into
	// the loops above, but is not done here to simplify the code
	for (Elem *pElem = gListHead[0]; ; ) {
		if (pElem->minmax == MIN_ELEM) {
			if (pElem->value[0] > pAABB->max.value[0])
				break;
			if (AABBOverlap(pAABB, GetAABB(pElem)))
				AddCollisionPair(pAABB, GetAABB(pElem));
		} else if (pElem->value[0] < pAABB->min.value[0])
			break;
	}
}

void MoveElement(int i, Elem *pElem, Elem *pDest)
{
	// Unlink element...
	pElem->pLeft[i]->pRight[i] = pElem->pRight[i];
	pElem->pRight[i]->pLeft[i] = pElem->pLeft[i];
	// ...and relink it _after_ the destination element
	pElem->pLeft[i] = pDest;
	pElem->pRight[i] = pDest->pRight[i];
	pDest->pRight[i]->pLeft[i] = pElem;
	pDest->pRight[i] = pElem;
}

// This updating code assumes all other elements of list are sorted
void UpdateAABBPosition(AABB *pAABB)
{
	// For all three axes
	for (int i = 0; i < 3; i++) {
		Elem *pMin = &pAABB->min, *pMax = &pAABB->max, *t;

		// Try to move min element to the left. Move the roaming pointer t left
		// for as long as it points to elem with value larger than pMin's. While
		// doing so, keep track of the update status of any AABBs passed over
		for (t = pMin->pLeft[i]; pMin->value[i] < t->value[i]; t = t->pLeft[i])
			if (t->minmax == MAX_ELEM)
				if (AABBOverlap(pAABB, GetAABB(t)))
					if (!HasCollisionPair(pAABB, GetAABB(t)))
						AddCollisionPair(pAABB, GetAABB(t));
		// If t moves from its original position, move pMin into new place
		if (t != pMin->pLeft[i])
			MoveElement(i, pMin, t);

		// Similarly to above, try to move max element to the right
		for (t = pMax->pRight[i]; pMax->value[i] > t->value[i]; t = t->pRight[i])
			if (t->minmax == MIN_ELEM)
				if (AABBOverlap(pAABB, GetAABB(t)))
					if (!HasCollisionPair(pAABB, GetAABB(t)))
						AddCollisionPair(pAABB, GetAABB(t));
		if (t != pMax->pRight[i])
			MoveElement(i, pMax, t->pLeft[i]);

		// Similarly to above, try to move min element to the right
		for (t = pMin->pRight[i]; pMin->value[i] > t->value[i]; t = t->pRight[i])
			if (t->minmax == MAX_ELEM)
				if (HasCollisionPair(pAABB, GetAABB(t)))
					DeleteCollisionPair(pAABB, GetAABB(t));
		if (t != pMin->pRight[i])
			MoveElement(i, pMin, t->pLeft[i]);

		// Similarly to above, try to move max element to the left
		for (t = pMax->pLeft[i]; pMax->value[i] < t->value[i]; t = t->pLeft[i])
			if (t->minmax == MIN_ELEM)
				if (HasCollisionPair(pAABB, GetAABB(t)))
					DeleteCollisionPair(pAABB, GetAABB(t));
		if (t != pMax->pLeft[i])
			MoveElement(i, pMax, t);

	}
}
