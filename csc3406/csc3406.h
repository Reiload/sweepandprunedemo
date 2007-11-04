/* 
	csc3406.h - Header file Vector3 class and miscellaneous functions

	CSC3406 Assignment 2
	Ben Gilbert W0062289
	22 April 2007

	Based on file of the same name provided by Penny de Byl
*/


#ifndef __CSC3406_H__
#define __CSC3406_H__

#include <windows.h>
#include <cmath>
#include "gl.h"

const float PI = 3.14159265;

/*** Three dimensional vector class fir floats ***/
class Vector3{ 
public: 
	inline float GetX() const { return x; }
	inline float GetY() const { return y; }
	inline float GetZ() const { return z; }
	inline void SetX(float newX) { x = newX; }
	inline void SetY(float newY) { y = newY; }
	inline void SetZ(float newZ) { z = newZ; }
	inline void set(float dx, float dy, float dz){ x = dx; y = dy; z = dz;} 
	inline void set(Vector3& v){ x = v.x; y = v.y; z = v.z;}
	inline void flip(){x = -x; y = -y; z = -z;} // reverse this vector
	inline void setDiff(Vector3& a, Vector3& b)//set to difference a - b
	{ x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}
	void normalize()//adjust this vector to unit length
	{		
		double sizeSq = x * x + y * y + z * z;
		if(sizeSq < 0.0000001)
		{
			//cerr << "\nnormalize() sees vector (0,0,0)!";
			return; // does nothing to zero vectors;
		}
		float scaleFactor = 1.0/(float)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
	}
	Vector3(float xx, float yy, float zz){x = xx; y = yy; z = zz;}
	Vector3(const Vector3& v){x = v.x; y = v.y; z = v.z;}
	Vector3(){x = y = z = 0;} //default constructor
	inline Vector3 cross(Vector3 b) //return this cross b
	{
		Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
		return c;
	}
	inline float dot(Vector3 b) // return this dotted with b
	{return x * b.x + y * b.y + z * b.z;}
	inline double AngleNegY() {
		double angle = acos(-y) * 180/3.14; // From dot product with negative y axis
		return angle;
	}
	inline void scale(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}
	inline float getMagnitude() const {
		return sqrtf(x*x + y*y + z*z);
	}
private:
		float x,y,z;
};

struct WindowSetup
{
	unsigned int SCREENWIDTH;
	unsigned int SCREENHEIGHT;

	int WORLD_LEFT, WORLD_RIGHT, WORLD_TOP, WORLD_BOTTOM;
	int VIEW_LEFT, VIEW_RIGHT, VIEW_TOP, VIEW_BOTTOM;

	float ASPECT_RATIO;
};

class FileReadWriter
{

public:
	FileReadWriter(){}
	~FileReadWriter(){}
	void loadSetupFile(char *fileName, WindowSetup *settings);
	void writeLogFile(char *fileName, char *message);
	void captureScreen(int width, int height, char *filename);
	int loadTextures(char *filename);
};
#endif
