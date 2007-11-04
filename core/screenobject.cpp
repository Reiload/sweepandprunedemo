/*	screenobject.cpp - implementation file for AABB class

Ben Gilbert W0062289
3 November 2007

CSC8490 Assignment 2

*/

#include "glut.h"
#include "csc3406.h"
#include "screenobject.h"
#include "l3ds.h"


using namespace SweepAndPrune;

/* push a "response" function that allows the player to push the other AABB */
void SweepAndPrune::push(AABB* aabb1, AABB* aabb2) {
	/*Vector3* pos1 = aabb1->CreatePositionVector();
	Vector3* pos2 = aabb2->CreatePositionVector();
	Vector3 delta;
	delta.setDiff(*pos1,*pos2);
	if( (0.9 < delta.getMagnitude()) && ( delta.getMagnitude() < 1.1 ) ) {
		return;
	}*/
	Vector3 dir;
				GLfloat minX ;
				GLfloat minY ;
				GLfloat minZ ;
				GLfloat maxX ;
				GLfloat maxY ;
				GLfloat maxZ ;

	if( aabb1->isMovable() ) {
		dir = aabb2->GetDirectionOfTravel();
		//dir.scale(1.001);
				minX = (aabb1->GetMinElement())->GetX();
				minY = (aabb1->GetMinElement())->GetY();
				minZ = (aabb1->GetMinElement())->GetZ();
				maxX = (aabb1->GetMaxElement())->GetX();
				maxY = (aabb1->GetMaxElement())->GetY();
				 maxZ = (aabb1->GetMaxElement())->GetZ();
				 std::cout << "Push Block1 pre move: (" << minX << ", " << minY << ", " << minZ << ", "
				  << ") ( " << maxX << ", " << maxY << ", " << maxZ << ") AABB" <<  aabb1 << std::endl; 
				std::cout << "Push pAABB " << aabb1 << " minElement* " << aabb1->GetMinElement() << std::endl;
				 aabb1->Move(dir);
				 minX = aabb1->GetMinElement()->GetX();
				 minY = aabb1->GetMinElement()->GetY();
				 minZ = aabb1->GetMinElement()->GetZ();
				 maxX = aabb1->GetMaxElement()->GetX();
				 maxY = aabb1->GetMaxElement()->GetY();
				 maxZ = aabb1->GetMaxElement()->GetZ();
				 std::cout << "Push Block1 post move: (" << minX << ", " << minY << ", " << minZ << ", "
				  << ") ( " << maxX << ", " << maxY << ", " << maxZ  << ") AABB" <<  aabb1 << std::endl; 
	} else {
		GLfloat minX = aabb2->GetMinElement()->GetX();
				minX = (aabb2->GetMinElement())->GetX();
				minY = (aabb2->GetMinElement())->GetY();
			minZ = (aabb2->GetMinElement())->GetZ();
				maxX = (aabb2->GetMaxElement())->GetX();
				maxY = (aabb2->GetMaxElement())->GetY();
				maxZ = (aabb2->GetMaxElement())->GetZ();
				 std::cout << "Push Block2 pre move: (" << minX << ", " << minY << ", " << minZ << ", "
				  << ") ( " << maxX << ", " << maxY << ", " << maxZ  << ") AABB" <<  aabb2 << std::endl; 
		dir = aabb1->GetDirectionOfTravel();
		//dir.scale(1.001);
		std::cout << " Push pAABB " << aabb2 << " minElement* " << aabb2->GetMinElement() << std::endl;
		aabb2->Move(dir);
					minX = aabb2->GetMinElement()->GetX();
				 minY = aabb2->GetMinElement()->GetY();
				 minZ = aabb2->GetMinElement()->GetZ();
				 maxX = aabb2->GetMaxElement()->GetX();
				 maxY = aabb2->GetMaxElement()->GetY();
				 maxZ = aabb2->GetMaxElement()->GetZ();
				 std::cout << "Push Block2 post move: (" << minX << ", " << minY << ", " << minZ << ", "
				  << ") ( " << maxX << ", " << maxY << ", " << maxZ  << ") AABB" <<  aabb2 << std::endl; 
	}
	delete pos1;
	delete pos2;
	glutPostRedisplay();
}
