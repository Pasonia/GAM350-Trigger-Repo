/******************************************************************************/
/*!
\file		  TriggerVector3D.cpp
\author 	Lew Yunan, Joel
\par    	email: yunanjoel.lew\@digipen.edu
\date   	July 08, 2013
\brief

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "TriggerVector3D.h"
#include "math.h"

float myEpsilon =  0.0001f;

Vector3D::Vector3D()
{
	x = y = z = 0;
}

Vector3D::Vector3D(const Vector3D &rhs)
{
	*this = rhs;
}

Vector3D::Vector3D(float x_, float y_, float z_ = 0) : x(x_), y(y_), z(z_) {};

Vector3D& Vector3D::operator=(const Vector3D &rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;

	return *this;
};

Vector3D& Vector3D::operator+=(const Vector3D &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector3D Vector3D::operator+(const Vector3D &rhs)
{
	Vector3D tmp(*this);
	//using add assign
	tmp += rhs;

	return tmp;
}

Vector3D& Vector3D::operator-=(const Vector3D &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vector3D Vector3D::operator-(const Vector3D &rhs)
{
	Vector3D tmp(*this);
	//using minus assign
	tmp -= rhs;

	return tmp;
}

Vector3D& Vector3D::operator*=(const Vector3D &rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;

	return *this;
}

Vector3D Vector3D::operator*(const Vector3D &rhs)
{
	Vector3D tmp(*this);
	//using add assign
	tmp *= rhs;

	return tmp;
}

bool Vector3D::operator==(const Vector3D& rhs) const
{
	//testing with epsilon
	return(
		((rhs.x - myEpsilon < x) && (x < rhs.x + myEpsilon)) &&
		((rhs.y - myEpsilon < y) && (y < rhs.y + myEpsilon)) &&
		((rhs.z - myEpsilon < z) && (z < rhs.z + myEpsilon)) 
		);
}

bool Vector3D::operator!=(const Vector3D& rhs) const
{
	//not of equality
	return(!(*this == rhs));
}

float Vector3D::Distance(const Vector3D& rhs)
{
	//2D distance
	return sqrt((rhs.x - x)*(rhs.x - x) + (rhs.y - y)*(rhs.y - y));

}

float Vector3D::Length()
{
	return sqrt(x*x + y*y + z*z);
}

float Vector3D::DotProduct(const Vector3D &rhs)
{
	return(x*rhs.x + y*rhs.y + z*rhs.z);
}

Vector3D Vector3D::CrossProduct(const Vector3D &rhs)
{
	float nx = (y*rhs.z) - (z*rhs.y);
	float ny = (z*rhs.y) - (x*rhs.z);
	float nz = (x*rhs.y) - (y*rhs.x);

	return Vector3D(nx, ny, nz);
}

Vector3D Vector3D::Normalize()
{
	float length;
	if(Length() == 0)
	{
		length = 0;
	}
	else
		length = 1/Length();

	float nx = x*length;
	float ny = y*length;
	float nz = z*length;

	return Vector3D(nx, ny, nz);
}

