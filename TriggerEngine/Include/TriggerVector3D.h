/******************************************************************************/
/*!
\file TriggerVector3D.cpp
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for TriggerVector3D class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef TRIGGERVEC3_H_
#define TRIGGERVEC3_H_



class Vector3D{
	public:
		float x, y, z;
		
		Vector3D();									//Default constructor for Vec3
		Vector3D(const Vector3D& v);				//Copy constructor
		Vector3D(float x, float y, float z);				//None Default constructor
		Vector3D& operator=(const Vector3D& rhs);	//Assignment operator
		float GetX() {return x;}						//Get x,y,z values
		float GetY() {return y;}
		float GetZ() {return z;}
    void Set(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_; y = y_; z = z_;};
		Vector3D operator +(const Vector3D& rhs);	//add operator
		Vector3D& operator+=(const Vector3D& rhs);	//add assign
		Vector3D operator -(const Vector3D& rhs);	//minus
		Vector3D& operator-=(const Vector3D& rhs);	//minus assign
		Vector3D operator *(const Vector3D& rhs);	//scale
		Vector3D& operator*=(const Vector3D& rhs);	//scale assign
		bool operator==(const Vector3D& rhs) const;	//equals to
		bool operator!=(const Vector3D& rhs) const; //not equals to
		float Distance(const Vector3D& rhs);		//Vector distance
		float Length();								//Vector length
		float DotProduct(const Vector3D& rhs);		//Dot product
		Vector3D CrossProduct(const Vector3D& rhs);	//Cross product
		Vector3D Normalize();						//Normalize
};

#endif