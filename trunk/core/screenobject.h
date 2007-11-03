/*	screenobject.h - Header file for Fish class

Author: Ben Gilbert W0062289
CSC3406 Assignment 2

Fish Class represents an individual fish in the ocean scene

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



namespace SweepAndPrune {	

	class Element;

	class Overlap {
	public:
		Overlap(AABB* p1, AABB* p2) : pAABB1(p1), pAABB2(p2), counter(0) {}
		bool MarkOverlap() {
			
			counter++;
			if(counter == 3) {
			
				return true;
			} else {
				return false;
			}
		}
		bool IsCollision() const {
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
		static std::list< Overlap* >& OverlapList() {
			static std::list< Overlap* > OverlapList;
			return OverlapList;
		}
		static void CleanOverlapList() {
			std::list< Overlap* >::iterator it = OverlapList().begin();
			for (; it != OverlapList().end(); ++it) {
				delete (*it);
			}
			OverlapList().clear();
		}
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
		unsigned counter;
	};

	class AABB{
	public:

		AABB(const Vector3 mins1, const Vector3 maxs1) : movable(false) {
			minElement = new Element(this, mins1, false); 
			maxElement = new Element(this, maxs1, true);
		}
		inline Element* GetMinElement() const {
			return minElement;
		}
		inline Element* GetMaxElement() const {
			return maxElement;
		}
		inline void Move(const Vector3 delta) {
			directionOfTravel = delta;
			UpdateList(this, delta);
		}
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
		Element* minElement;
		Element* maxElement;
		bool movable;
		Vector3 directionOfTravel;

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
		static void DetectCollisions() {	
			Overlap::CleanOverlapList();
			std::list< Element* > tempScanList;
			std::list< Element* >::iterator it3 = tempScanList.begin();
			std::list< Element* >::iterator it2 = Element::ElementListX().begin();
			for (; it2 != Element::ElementListX().end(); ++it2) {
				
				if( (*it2)->GetIsMax()) {
					tempScanList.remove( ((*it2)->GetAABB())->GetMinElement() );
				} else {
					it3 = tempScanList.begin();
					for(; it3 != tempScanList.end(); ++it3) {
						Overlap::UpdateList( (*it3)->GetAABB() , (*it2)->GetAABB() );
					}
					tempScanList.push_back( (*it2) );

				}
			}
			it2 = Element::ElementListY().begin();
			for (; it2 != Element::ElementListY().end(); ++it2) {
				if( (*it2)->GetIsMax()) {
					
					tempScanList.remove( ((*it2)->GetAABB())->GetMinElement() );
				} else {
				
					it3 = tempScanList.begin();
					for(; it3 != tempScanList.end(); ++it3) {
						
						Overlap::UpdateList( (*it3)->GetAABB() , (*it2)->GetAABB() );
					}
					tempScanList.push_back( (*it2) );

				}
				
			}

			it2 = Element::ElementListZ().begin();
			for (; it2 != Element::ElementListZ().end(); ++it2) {
				if( (*it2)->GetIsMax()) {
					
					tempScanList.remove( ((*it2)->GetAABB())->GetMinElement() );
				} else {
					
					it3 = tempScanList.begin();
					for(; it3 != tempScanList.end(); ++it3) {
						
						Overlap::UpdateList( (*it3)->GetAABB() , (*it2)->GetAABB() );
					}
					tempScanList.push_back( (*it2) );

				}
				
			}


		}
		static void Response(void (*pFunc) (AABB* p1, AABB* p2) ) {
			std::list< void (*) (AABB*, AABB*) > functionList;
			std::list< std::pair < AABB*, AABB* >* > pairList;
			std::list< Overlap* >::iterator it = Overlap::OverlapList().begin();
			for (; it != Overlap::OverlapList().end(); ++it) {
				if( (*it)->IsCollision() ) {
					AABB* aabb1 = (*it)->GetFirstAABB();
					AABB* aabb2 = (*it)->GetSecondAABB();

					functionList.push_back(pFunc);
					pairList.push_back(new std::pair< AABB*, AABB* > (aabb1, aabb2) );
				}
			}
			if( functionList.empty() ){
				return;
			} else {
				std::list< void (*) (AABB*, AABB*) >::iterator itFunc = functionList.begin();
				AABB* temp1;
				AABB* temp2;
				for( ; itFunc != functionList.end(); ++itFunc ) {
					temp1 = (pairList.front())->first;
					temp2 = (pairList.front())->second;
					pairList.pop_front();
					(pFunc)(temp1, temp2);
				}
			}
		}
	};



	void push(AABB* aabb1, AABB* aabb2);

	enum DIRECTION_OF_TRAVEL { XNEG, XPOS, YNEG, YPOS, ZNEG, ZPOS, NONE }; // these are the directions the ToyBlocks can moving in



	class ToyBlocks : public AABB{
	public:
		ToyBlocks(const Vector3 mins1, const Vector3 maxs1, const GLfloat* col) : 
		  AABB(mins1, maxs1), colour(col), lastDirection(NONE) {
		  }
		  void MoveX(GLfloat x) {
			  Vector3 delta(x, 0, 0);
			  Move(delta);
			  AABB::Response( push );
		  }
		  void MoveY(GLfloat y) {
			  Vector3 delta(0, y, 0);
			  Move(delta);
			   AABB::Response( push );
		  }
		  void MoveZ(GLfloat z) {
			  Vector3 delta(0, 0, z);
			  Move(delta);
			   AABB::Response( push );
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
