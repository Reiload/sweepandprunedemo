/*	screenobject.h - Header file for AABB class and derived classes

Ben Gilbert W0062289
3 November 2007

CSC8490 Assignment 2

AABB - Axis Aligned Bounding Box is used to represent objects that appear on the screen for the 
purpose of collision detection and response
This implementation has been designed for the use of the sweep and prune collision algorithm

*/
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_

#include <list>
#include <utility>
#include <iostream>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "utilities.h"
#include "csc3406.h"
#include "l3ds.h"
#include "element.h"

/* Note: Element and AABB classes must be kept is separate files 
as they are defined in a circular manner */

namespace SweepAndPrune {	

	class Element;

	/* Overlap class is used by the AABB class to store overlap information 
	e.g which AABBs are overlaping and on how many axes */
	class Overlap {
	public:
		Overlap(AABB* p1, AABB* p2) : pAABB1(p1), pAABB2(p2), counter(0) {}
		/* MarkOverlap is called to record an overlap on an axis */
		bool MarkOverlap() {
			counter++;
			if(counter == 3) {			
				return true;
			} else {
				return false;
			}
		}
		/* IsCollision is call to determine if a collision has occurred */
		bool IsCollision() const {
			if(counter == 3) {
				return true;
			} else {
				return false;
			}
		}
		/* Two AABBs can overlap on a different axis but in a different order
		SamePair can be used to search for the same pair irrespective of order */
		bool SamePair(AABB* p1, AABB* p2) const {
			if( (p1 == pAABB1 && p2 == pAABB2) || ( p1 == pAABB2 && p2 == pAABB1)) {
				return true;
			} else {
				return false;
			}
		}
		/* 
		A function is used to access OverlapList because
		this guarantees that these static variables have been initialized
		*/
		static std::list< Overlap* >& OverlapList() {
			static std::list< Overlap* > OverlapList; // Overlap list is a list is used to record overlaps
			return OverlapList;
		}
		/* CleanOverlapList destroys contents of OverlapList */
		static void CleanOverlapList() {
			std::list< Overlap* >::iterator it = OverlapList().begin();
			for (; it != OverlapList().end(); ++it) {
				delete (*it);
			}
			OverlapList().clear();
		}
		/* UpdateList creates or updates an entry in the OverlapList for an Overlapping AABB pair */
		static void UpdateList(AABB* p1, AABB* p2) {
			std::list< Overlap* >::iterator it = OverlapList().begin();
			for (; it != OverlapList().end(); ++it) {
				if(	(*it)->SamePair(p1, p2)	) {
					(*it)->MarkOverlap();
					break;
				}
			}
			Overlap* temp = new Overlap(p1, p2);
			temp->MarkOverlap();
			OverlapList().push_back( temp );
		}
		inline AABB* GetFirstAABB() const {
			return pAABB1;
		}
		inline AABB* GetSecondAABB() const {
			return pAABB2;
		}
	private:
		AABB* pAABB1;
		AABB* pAABB2;
		unsigned counter; /* incremented each time an overlap occurs between two AABBs, 
						  if counter == 3 a collision has occured */
	};


	/* 
	AABB - Axis Aligned Bounding Box is used to represent objects that appear on the screen for the 
	purpose of collision detection and response
	*/
	class AABB{
	public:

		AABB(const Vector3 mins1, const Vector3 maxs1) : movable(false) {
			minElement = new Element(this, mins1, false); 
			maxElement = new Element(this, maxs1, true);
		}
		~AABB() {
			Element::RemoveFromList(minElement);
			Element::RemoveFromList(maxElement);
			delete minElement;
			delete maxElement;
		}
		inline Element* GetMinElement() const {
			return minElement;
		}
		inline Element* GetMaxElement() const {
			return maxElement;
		}
		/* Move translates an AABB's position an updates the element lists */
		inline void Move(const Vector3 delta) {
			directionOfTravel = delta;
			UpdateList(this, delta);
		}
		/* CreatePositionVector creates a vector at the centre of mass of the AABB */
		inline Vector3* CreatePositionVector() const {
			return new Vector3(
				(minElement->GetX() + maxElement->GetX())/2.0, 
				(minElement->GetY() + maxElement->GetY())/2.0, 
				(minElement->GetZ() + maxElement->GetZ())/2.0
				);
		}
		inline bool isMovable() const {
			return movable;
		}
		inline void SetMovable() {
			movable = true;
		}
		inline Vector3 GetDirectionOfTravel() const {
			return directionOfTravel;
		}
	protected:
		Element* minElement; // represents the minimum vertice
		Element* maxElement; // represents the maximum vectice
		bool movable; // true if the AABB is allowed to be moved
		Vector3 directionOfTravel; // direction that the AABB was moved in

		/* UpdateList moves both elements according to deltaPosition 
		and updates the element lists*/
		static void UpdateList(AABB* pAABB, const Vector3 deltaPosition) {
			Element* tempElement;
			tempElement = pAABB->GetMinElement();
			Element::RemoveFromList(tempElement);
			tempElement->SetX( tempElement->GetX() + deltaPosition.GetX() );
			tempElement->SetY( tempElement->GetY() + deltaPosition.GetY() );
			tempElement->SetZ( tempElement->GetZ() + deltaPosition.GetZ() );
			Element::InsertIntoList(tempElement, tempElement->GetIsMax());

			tempElement = pAABB->GetMaxElement();
			Element::RemoveFromList(tempElement);
			tempElement->SetX( tempElement->GetX() + deltaPosition.GetX() );
			tempElement->SetY( tempElement->GetY() + deltaPosition.GetY() );
			tempElement->SetZ( tempElement->GetZ() + deltaPosition.GetZ() );
			Element::InsertIntoList(tempElement, tempElement->GetIsMax());
			DetectCollisions();

		}
		/* DetectColllisions scans the element lists and searches for collisions
		according to the sweep and prune algorithm */
		static void DetectCollisions() {	
			Overlap::CleanOverlapList();
			/* minimum elements are placed in tempScanList when a scan of an axis is performed
			these elements are removed when the maximum element of the same object is encountered
			when a minimum element is to be inserted and there are other minimum elements in the list
			record that an overlap has occured on that axis between the AABB of the new element and all
			AABBs of the other minimum elements*/
			std::list< Element* > tempScanList; 
			std::list< Element* >::iterator tempListIt = tempScanList.begin();
			std::list< Element* >::iterator elementListIt = Element::ElementListX().begin();

			// scan the x axis
			for (; elementListIt != Element::ElementListX().end(); ++elementListIt) {	
				if( (*elementListIt)->GetIsMax()) { // true if element is a maximum
					tempScanList.remove( ((*elementListIt)->GetAABB())->GetMinElement() ); // remove corresponding minimum element
				} else { // true if element is a minimum
					tempListIt = tempScanList.begin();
					for(; tempListIt != tempScanList.end(); ++tempListIt) { 
						// mark overlaps between this elements AABB and the AABBs of the elements in the list
						Overlap::UpdateList( (*tempListIt)->GetAABB() , (*elementListIt)->GetAABB() );
					}
					tempScanList.push_back( (*elementListIt) );
				}
			}

			// scan the y axis
			elementListIt = Element::ElementListY().begin();
			for (; elementListIt != Element::ElementListY().end(); ++elementListIt) {
				if( (*elementListIt)->GetIsMax()) {
					tempScanList.remove( ((*elementListIt)->GetAABB())->GetMinElement() );
				} else {
					tempListIt = tempScanList.begin();
					for(; tempListIt != tempScanList.end(); ++tempListIt) {
						Overlap::UpdateList( (*tempListIt)->GetAABB() , (*elementListIt)->GetAABB() );
					}
					tempScanList.push_back( (*elementListIt) );
				}
			}

			// scan the z axis
			elementListIt = Element::ElementListZ().begin();
			for (; elementListIt != Element::ElementListZ().end(); ++elementListIt) {
				if( (*elementListIt)->GetIsMax()) {
					tempScanList.remove( ((*elementListIt)->GetAABB())->GetMinElement() );
				} else {
					tempListIt = tempScanList.begin();
					for(; tempListIt != tempScanList.end(); ++tempListIt) {
						Overlap::UpdateList( (*tempListIt)->GetAABB() , (*elementListIt)->GetAABB() );
					}
					tempScanList.push_back( (*elementListIt) );
				}
			}
		}

		/* Response applies pFunc to all colliding pairs */
		static void Response(void (*pFunc) (AABB* p1, AABB* p2) ) {
			std::list< std::pair < AABB*, AABB* >* > pairList;
			std::list< Overlap* > OverlapList2; 
			OverlapList2 = Overlap::OverlapList(); 
			/* necessary to create a copy of OverlapList as pFunc can change the contents of OverlapList*/
			std::list< Overlap* >::iterator it = OverlapList2.begin();
			for (; it != OverlapList2.end(); ++it) {
				if( (*it)->IsCollision() ) {
					AABB* aabb1 = (*it)->GetFirstAABB();
					AABB* aabb2 = (*it)->GetSecondAABB();
					(pFunc)(aabb1, aabb2);
				}
			}
		}
	};

	/* push a "response" function that allows the player to push the other AABB */
	void push(AABB* aabb1, AABB* aabb2);

	/* ToyBlocks class adds user interface functionality to the AABB class from which it is derived */
	class ToyBlocks : public AABB{
	public:
		ToyBlocks(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  AABB(mins1, maxs1), colour(col) {
		  }
		  /* MoveX moves ToyBlock in the x direction */
		  void MoveX(GLfloat x) {
			  Vector3 delta(x, 0, 0);
			  Move(delta);
			  AABB::Response( push );
		  }
		  /* MoveY moves ToyBlock in the x direction */
		  void MoveY(GLfloat y) {
			  Vector3 delta(0, y, 0);
			  Move(delta);
			  AABB::Response( push );
		  }
		  /* MoveZ moves ToyBlock in the x direction */
		  void MoveZ(GLfloat z) {
			  Vector3 delta(0, 0, z);
			  Move(delta);
			  AABB::Response( push );
		  }
		  /* Draw draws ToyBlock to the screen */
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
	private:
		const GLfloat* colour; // colour of the ToyBlocks
	};
}

#endif
