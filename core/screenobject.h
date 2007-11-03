/*	screenobject.h - Header file for Fish class

Author: Ben Gilbert W0062289
CSC3406 Assignment 2

Fish Class represents an individual fish in the ocean scene

*/
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_

#include <list>
#include <utility>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "utilities.h"
#include "csc3406.h"
#include "l3ds.h"
#include "element.h"



namespace SweepAndPrune {	

	class Element;

	class Overlap {
		Overlap(AABB* p1, AABB* p2) : pAABB1(p1), pAABB2(p2), counter(0) {}
		bool MarkOverlap() {
			counter++;
			if(counter == 3) {
				return true;
			} else {
				return false;
			}
		}
		bool IsOverlap() const {
			if(counter == 3) {
				return true;
			} else {
				return false;
			}
		}
		bool SamePair(AABB* p1, AABB* p2) const {
			if( (p1 == pAABB1 && p2 == pAABB2) || ( p1 == pAABB2 && p2 == pAABB1)) {
				return true;
			} else {
				return false;
			}
		}
	private:
		AABB* pAABB1;
		AABB* pAABB2;
		unsigned counter;
	};

	class AABB{
	public:
		
		AABB(const Vector3 mins1, const Vector3 maxs1) {
			minElement = new Element(this, mins1, false); 
			maxElement = new Element(this, maxs1, true);
		}
		inline Element* GetMinElement() const {
			return minElement;
		}
		inline Element* GetMaxElement() const {
			return maxElement;
		}
		inline void Move(const Vector3 newPosition) {
			UpdateList(this, newPosition);
		}
	protected:
		Element* minElement;
		Element* maxElement;
		static std::list< std::pair<AABB*, AABB*>* >& CollisionPairs() {
			static std::list< std::pair<AABB*, AABB*>* > CollisionPairs;
			return CollisionPairs;
		}
		static void AddCollisionPair(AABB* pAABB1, AABB* pAABB2) {
			CollisionPairs().push_back( new std::pair< AABB*, AABB* > (pAABB1, pAABB2 ) );
		}
		static void UpdateList(AABB* pAABB, const Vector3 newPosition) {
			Element* tempElement;
			tempElement = pAABB->GetMinElement();
			Element::RemoveFromList(tempElement);
			tempElement->SetX( tempElement->GetX() + newPosition.GetX() );
			tempElement->SetY( tempElement->GetY() + newPosition.GetY() );
			tempElement->SetZ( tempElement->GetZ() + newPosition.GetZ() );
			Element::InsertIntoList(tempElement, tempElement->GetIsMax());

			tempElement = pAABB->GetMaxElement();
			Element::RemoveFromList(tempElement);
			tempElement->SetX( tempElement->GetX() + newPosition.GetX() );
			tempElement->SetY( tempElement->GetY() + newPosition.GetY() );
			tempElement->SetZ( tempElement->GetZ() + newPosition.GetZ() );
			Element::InsertIntoList(tempElement, tempElement->GetIsMax());

		}
	/*	bool isOverlapping(AABB* pAABB1, AABB* pAABB2) {
			float minX1 = pAABB1->minElement->GetX();
			float minY1 = pAABB1->minElement->GetY();
			float minZ1 = pAABB1->minElement->GetZ();

			float maxX1 = pAABB1->maxElement->GetX();
			float maxY1 = pAABB1->maxElement->GetY();
			float maxZ1 = pAABB1->maxElement->GetZ();
			
			float minX2 = pAABB2->minElement->GetX();
			float minY2 = pAABB2->minElement->GetY();
			float minZ2 = pAABB2->minElement->GetZ();

			float maxX2 = pAABB2->maxElement->GetX();
			float maxY2 = pAABB2->maxElement->GetY();
			float maxZ2 = pAABB2->maxElement->GetZ();

			bool xOverlap = false;
			bool yOverlap = false;
			bool zOverlap = false;

			if( (minX1 < minX2) && (maxX1 > minX2) ) { 
				xOverlap = true;
			}
			if( (minX2 < minX1) && ( maxX2 > minX1) ) {
				xOverlap = true;
			}
			if( (minX1 < minX2) && ( maxX1 < minX2 ) {
				xOverlap = true;
			}
			if( (minX2 < minX1) && ( maxX2 < minX1 ) {
				xOverlap = true;
			}

			if( (minY1 < minY2) && (maxY1 > minY2) ) { 
				yOverlap = true;
			}
			if( (minY2 < minY1) && ( maxY2 > minY1) ) {
				yOverlap = true;
			}
			if( (minY1 < minY2) && ( maxY1 < minY2 ) {
				yOverlap = true;
			}
			if( (minY2 < minY1) && ( maxY2 < minY1 ) {
				yOverlap = true;
			}

			if( (minZ1 < minZ2) && (maxZ1 > minZ2) ) { 
				zOverlap = true;
			}
			if( (minZ2 < minZ1) && ( maxZ2 > minZ1) ) {
				zOverlap = true;
			}
			if( (minZ1 < minZ2) && ( maxZ1 < minZ2 ) {
				zOverlap = true;
			}
			if( (minZ2 < minZ1) && ( maxZ2 < minZ1 ) {
				zOverlap = true;
			}

		}*/
	};

	

	
	

	enum DIRECTION_OF_TRAVEL { XNEG, XPOS, YNEG, YPOS, ZNEG, ZPOS, NONE }; // these are the directions the ToyBlocks can moving in

	class ToyBlocks : public AABB{
	public:
		ToyBlocks(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  AABB(mins1, maxs1), colour(col), lastDirection(NONE) {
		  }
		  void MoveX(GLfloat x) {
			  Vector3 delta(x, 0, 0);
			  Move(delta);
		  }
		  void MoveY(GLfloat y) {
				Vector3 delta(0, y, 0);
				Move(delta);
		  }
		  void MoveZ(GLfloat z) {
				Vector3 delta(0, 0, z);
				Move(delta);
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
