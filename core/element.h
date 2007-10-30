#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "screenobject.h"

namespace SweepAndPrune {
class AABB;

class Element{
	public:
		Element(AABB* ptaabb, Vector3 vec) : ptAABB(ptaabb), coordinates(vec) {}
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

	private:
		AABB* ptAABB;
		Element* smallerElements[3];
		Element* greaterElements[3];
		Vector3 coordinates;
	};


}
#endif