/*	screenobject.h - Header file for Fish class
	
	Author: Ben Gilbert W0062289
	CSC3406 Assignment 2

	Fish Class represents an individual fish in the ocean scene

*/
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_

#include <list>
#include "utilities.h"
#include "csc3406.h"
#include "l3ds.h"
using std::list;

namespace Sea {	
	
	class Fish {
	public:
		// Constructor
		Fish(const Vector3 p, const Vector3 d, const GLfloat s, const float si, GLfloat* col) :
		pos(p), dir(d), speed(s), size(si), color(col) {
			dir.normalize();
			FishList().push_back(this);
		}
		// Destructor
		~Fish() {
			FishList().remove(this);
		}	
		// Get Position
		const Vector3& GetPos() const { return pos; }
		// Get Direction Vector
		const Vector3& GetDir() const { return dir; }
		// Get Speed
		GLfloat GetSpeed() const { return speed; }
		// Get Size
		float GetSize() const { return size; }
		// Get Position
		void SetPos(Vector3& p) { pos = p; }
		// Set Direction
		void Setdir(Vector3& v) { dir = v; }
		// Normalize Direction Vector
		void NormalizeDir() { dir.normalize();}
		// Set Fish Speed
		void SetSpeed(GLfloat s) { speed = s; }
		// Move fish
		void Move(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
		// Draw all fish
		static void DrawAll() {
			list<Fish*>::const_iterator it = FishList().begin();
			for (; it != FishList().end(); ++it) {
				(*it)->Draw();
			}
		}
		// Move all objects
		static void MoveAll(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
			list<Fish*>::const_iterator it = FishList().begin();
			for (; it != FishList().end(); ++it) {
				(*it)->Move(xMin, xMax, yMin, yMax, zMin, zMax);
			}
		}	
		// Load L3DS file
		static void File(const char *filename){
			Scene() = new L3DS();
			Scene()->LoadFile(filename);
		}
		void Draw(); 	
	protected:
		Vector3	pos; // Vector to position from origin
		Vector3 dir; // Vector showing direction of travel
		GLfloat speed; // How fast fish is moving
		float size; // Size
		GLfloat* color;  // Color
		// The following functions ensure that static variable are initialised when called
		static list<Fish*>&  FishList() {	
			static list<Fish*> FishList;	// static list of all ScreenObject objects
			return FishList;
		}
		static L3DS*& Scene() {
			static L3DS *scene;
			return scene;
		}
	};





}
#endif
	