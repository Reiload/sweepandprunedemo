/*
element.h - header file for element class

Ben Gilbert W0062289
3 November 2007

CSC8490 Assignment 2

An element object defines a maximum or minimum vertices of an axis aligned bounding box
This implementation has been designed for the use of the sweep and prune collision algorithm
*/

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <list>
#include "screenobject.h" 

/* Note: Element and AABB classes must be kept is separate files 
as they are defined in a circular manner */

namespace SweepAndPrune {

	class AABB;

	/* An element object defines a maximum or minimum vertice of an axis aligned bounding box */
	class Element{
	public:
		Element(AABB* ptaabb, Vector3 vec, bool isMax1) : 
		  ptAABB(ptaabb), coordinates(vec), isMax(isMax1) {
			  InsertIntoList(this, isMax1);
		  }
		  ~Element() {
			  RemoveFromList(this);	
		  }
		  inline float GetX() const { return coordinates.GetX(); }
		  inline float GetY() const { return coordinates.GetY(); }
		  inline float GetZ() const { return coordinates.GetZ(); }

		  inline AABB* GetAABB() const { return ptAABB; }

		  inline void  SetX( float  value )  {  coordinates.SetX(value); }
		  inline void  SetY( float  value )  {  coordinates.SetY(value); }
		  inline void  SetZ( float  value )  {  coordinates.SetZ(value); }

		  inline bool GetIsMax() const {
			  return isMax;
		  }
		  /* InsertIntoList inserts an Element object into three static lists (one for each axis)
		  Elements are inserted such that the ElementLists are ordered 		  
		  These lists are scanned to detect collisions */
		  static void InsertIntoList(Element* elem, bool isMax) {
			  if( ElementListX().empty() ) { // true if first element created or last element remaining
				  ElementListX().push_back(elem);
				  ElementListY().push_back(elem);
				  ElementListZ().push_back(elem);
				  return;
			  }
			  std::list<Element*>::iterator it = ElementListX().begin();
			  for (; it != ElementListX().end(); ) {
				  if( elem->GetX() < (*it)->GetX() ) {
					  ElementListX().insert( it , elem); 
					  break;
				  }
				  ++it;
				  if( it == ElementListX().end() ) {
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
				  if( it == ElementListY().end() ) {
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
				  if( it == ElementListZ().end() ) {
					  ElementListZ().insert(it, elem);
					  break;
				  }
			  }
		  }
		  static void RemoveFromList(Element* elem) {
			  ElementListX().remove(elem);
			  ElementListY().remove(elem);
			  ElementListZ().remove(elem);
		  }
		  /* 
			 A function is used to access ElementListX, ElementListY, ElementListZ because
			 this guarantees that these static variables have been initialize
		  */
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
	private:
		AABB* ptAABB; // The axis aligned bounding box associated with this element
		Vector3 coordinates; // Coordinates of the element
		bool isMax; // True if element defines the maximum vertices of the AABB
	};


}
#endif