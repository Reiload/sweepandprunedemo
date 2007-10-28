/*
	Camera.cpp
	Camera class implementation file
	Provided by Penny de Byl
*/
#include "camera.h"
#include "glu.h"

void Camera :: setShape(float vAngle, float asp, float nr, float fr)
{
	viewAngle = vAngle;
	aspect = asp;
	nearDist = nr;
	farDist = fr;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearDist, farDist);
	glMatrixMode(GL_MODELVIEW);
}

void Camera :: setModelViewMatrix(void)
{ // load modelview matrix with existing camera values
	float m[16];
	Vector3 eVec(eye.GetX(), eye.GetY(), eye.GetZ()); // a vector version of eye 
	m[0] =  u.GetX(); m[4] =  u.GetY(); m[8]  =  u.GetZ();  m[12] = -eVec.dot(u);
	m[1] =  v.GetX(); m[5] =  v.GetY(); m[9]  =  v.GetZ();  m[13] = -eVec.dot(v);
	m[2] =  n.GetX(); m[6] =  n.GetY(); m[10] =  n.GetZ();  m[14] = -eVec.dot(n);
	m[3] =  0;   m[7] =  0;   m[11] =  0;    m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m); // load OpenGL’s modelview matrix
}

void Camera:: set(Vector3 Eye, Vector3 look, Vector3 up)
{	// create a modelview matrix and send it to OpenGL
	eye.set(Eye); // store the given eye position
	n.set(eye.GetX() - look.GetX(), eye.GetY() - look.GetY(), eye.GetZ() - look.GetZ()); // make n
	u.set(up.cross(n).GetX(), up.cross(n).GetY(), up.cross(n).GetZ()); // make u = up X n
	n.normalize(); u.normalize(); // make them unit length
	v.set(n.cross(u).GetX(), n.cross(u).GetY(), n.cross(u).GetZ());  // make v =  n X u
	setModelViewMatrix(); // tell OpenGL 
}

void Camera:: slide(float delU, float delV, float delN)
{
	eye.SetX( eye.GetX() + delU * u.GetX() + delV * v.GetX() + delN * n.GetX());
	eye.SetY( eye.GetY() + delU * u.GetY() + delV * v.GetY() + delN * n.GetY());
	eye.SetZ( eye.GetZ() + delU * u.GetZ() + delV * v.GetZ() + delN * n.GetZ());
	setModelViewMatrix();
}

void Camera:: roll(float angle)
{
	float cs=cos(PI/180.0 * angle);
	float sn=sin(PI/180.0 * angle);
	Vector3 t = u;
	u.set(cs*t.GetX() - sn*v.GetX(), cs*t.GetY() - sn*v.GetY(), cs*t.GetZ() - sn*v.GetZ());
	v.set(sn*t.GetX() + cs*v.GetX(), sn*t.GetY() + cs*v.GetY(), sn*t.GetZ() + cs*v.GetZ());
	setModelViewMatrix();
}

void Camera :: pitch (float angle)
{ // pitch the camera through angle degrees around U
	float cs = cos(3.14159265/180 * angle);
	float sn = sin(3.14159265/180 * angle);
	Vector3 t(v); // remember old v
	v.set(cs*t.GetX() - sn*n.GetX(), cs*t.GetY() - sn*n.GetY(), cs*t.GetZ() - sn*n.GetZ());
	n.set(sn*t.GetX() + cs*n.GetX(), sn*t.GetY() + cs*n.GetY(), sn*t.GetZ() + cs*n.GetZ());
	setModelViewMatrix();
}

void Camera :: yaw (float angle)
{ // yaw the camera through angle degrees around V
	float cs = cos(3.14159265/180 * angle);
	float sn = sin(3.14159265/180 * angle);
	Vector3 t(n); // remember old v
	n.set(cs*t.GetX() - sn*u.GetX(), cs*t.GetY() - sn*u.GetY(), cs*t.GetZ() - sn*u.GetZ());
	u.set(sn*t.GetX() + cs*u.GetX(), sn*t.GetY() + cs*u.GetY(), sn*t.GetZ() + cs*u.GetZ());
	setModelViewMatrix();
}