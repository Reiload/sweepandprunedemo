#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <list>
#include <iostream>
#include "screenobject.h"

namespace SweepAndPrune {
	class AABB;

	class Element{
	public:
		Element(AABB* ptaabb, Vector3 vec, bool isMax1) : ptAABB(ptaabb), coordinates(vec), isMax(isMax1) {
			InsertIntoList(this, isMax1);
		}
		Element* GetSmallerElementX() const { return smallerElements[0]; }
		Element* GetSmallerElementY() const { return smallerElements[1]; }
		Element* GetSmallerElementZ() const { return smallerElements[2]; }
		Element* GetGreaterElementX() const { return greaterElements[0]; }
		Element* GetGreaterElementY() const { return greaterElements[1]; }
		Element* GetGreaterElementZ() const { return greaterElements[2]; }
		float GetX() const { return coordinates.GetX(); }
		float GetY() const { return coordinates.GetY(); }
		float GetZ() const { return coordinates.GetZ(); }

		AABB* GetAABB() const { return ptAABB; }

		void SetSmallerElementX( Element* elem )  {  smallerElements[0] = elem ; }
		void SetSmallerElementY( Element* elem )  {  smallerElements[1] = elem ; }
		void SetSmallerElementZ( Element* elem )  {  smallerElements[2] = elem ; }
		void SetGreaterElementX( Element* elem )  {  greaterElements[0] = elem ; }
		void SetGreaterElementY( Element* elem )  {  greaterElements[1] = elem ; }
		void SetGreaterElementZ( Element* elem )  {  greaterElements[2] = elem ; }

		void  SetX( float  value )  {  coordinates.SetX(value); }
		void  SetY( float  value )  {  coordinates.SetY(value); }
		void  SetZ( float  value )  {  coordinates.SetZ(value); }

		inline bool GetIsMax() const {
			return isMax;
		}

static void InsertIntoList(Element* elem, bool isMax) {
			std::list<Element*>::iterator it = ElementListX().begin();
			if(isMax) {
				for (; it != ElementListX().end(); ) {
					if( elem->GetX() < (*it)->GetX() ) {
						ElementListX().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListX().end() ) {
						ElementListX().insert(it, elem);
						break;
					}
				}
				it = ElementListY().begin();
				for (; it != ElementListY().end(); ) {
					if( elem->GetY() < (*it)->GetY() ) {
						ElementListY().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListY().end() ) {
						ElementListY().insert(it, elem);
						break;
					}
				}
				it = ElementListZ().begin();
				for (; it != ElementListZ().end(); ) {
					if( elem->GetZ() < (*it)->GetZ() ) {
						ElementListZ().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListZ().end() ) {
						ElementListZ().insert(it, elem);
						break;
					}
				}
			} else {
				for (; it != ElementListX().end(); ) {
					if( elem->GetX() < (*it)->GetX() ) {
						ElementListX().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListX().end() ) {
						ElementListX().insert(it, elem);
						break;
					}
				}
				it = ElementListY().begin();
				for (; it != ElementListY().end(); ) {
					if( elem->GetY() < (*it)->GetY() ) {
						ElementListY().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListY().end() ) {
						ElementListY().insert(it, elem);
						break;
					}
				}
				it = ElementListZ().begin();
				for (; it != ElementListZ().end(); ) {
					if( elem->GetZ() < (*it)->GetZ() ) {
						ElementListZ().insert( it , elem); 
						break;
					}
					++it;
					if( it != ElementListZ().end() ) {
						ElementListZ().insert(it, elem);
						break;
					}
				}
			}
		}
		static void RemoveFromList(Element* elem) {
			ElementListX().remove(elem);
			ElementListY().remove(elem);
			ElementListZ().remove(elem);
		}

	private:
		AABB* ptAABB;
		Element* smallerElements[3];
		Element* greaterElements[3];
		Vector3 coordinates;
		bool isMax;
		static std::list<Element*>&  ElementListX() {	
			static std::list<Element*> ElementListX;	// static list of all Element Objects with respect to the x axis
			return ElementListX;
		}
		static std::list<Element*>&  ElementListY() {	
			static std::list<Element*> ElementListY;	// static list of all Element Objects with respect to the Y axis
			return ElementListY;
		}	
		static std::list<Element*>&  ElementListZ() {	
			static std::list<Element*> ElementListZ;	// static list of all Element Objects with respect to the Z axis
			return ElementListZ;
		}
		
	};


}
#endif