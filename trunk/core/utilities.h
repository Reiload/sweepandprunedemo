/*	utilities.h - implementation file for Vec and Exception classes

	Author: Ben Gilbert
	CSC3406 Assignment 1
	29th of March 2007

	Vec class based on CSC3406 Tutorial 3 vec structure
	Exception class based on CSC2402 Assignment 3 GameException Class
*/

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <cmath>
#include <cstdlib>
#include <string>
#include "glut.h"

using std::string;

namespace Sea {

	// Generates a random int between min and max
	inline int GetRandom(int min, int max) {
		return rand() % (max - min + 1) + min;
	}
}
#endif