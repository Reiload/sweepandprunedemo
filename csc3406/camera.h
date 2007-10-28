#include "csc3406.h"

class Camera
{
	private:
		Vector3 eye;
		Vector3 u,v,n;
		double viewAngle, aspect, nearDist, farDist; // view volume shape
		void setModelViewMatrix(); // tell OpenGL where the camera is

	public:
		Camera(void){}; // default constructor
		void set(Vector3 eye, Vector3 look, Vector3 up); // like gluLookAt()
		void roll(float angle); // roll it
		void pitch(float angle); // increase pitch
		void yaw(float angle); // yaw it
		void slide(float delU, float delV, float delN); // slide it
		void rotate (Vector3 axis, float angle);
		void setShape(float vAng, float asp, float nearD, float farD);
};