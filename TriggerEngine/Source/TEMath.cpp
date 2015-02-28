/******************************************************************************/
/*!
\file TEMath.cpp
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
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "TEMath.h"
#include "OpenGL.h"


void printVec(const Vec3& lhs)
{ std::cout << "x:" << lhs.x << " y:" << lhs.y << " z:" << lhs.z << std::endl; }

void copyAiMatrixToGLM(const aiMatrix4x4 *from, Mat4 &to)
{
	to[0][0] = (GLfloat)from->a1;
	to[1][0] = (GLfloat)from->a2;
	to[2][0] = (GLfloat)from->a3;
	to[3][0] = (GLfloat)from->a4;

	to[0][1] = (GLfloat)from->b1;
	to[1][1] = (GLfloat)from->b2;
	to[2][1] = (GLfloat)from->b3;
	to[3][1] = (GLfloat)from->b4;

	to[0][2] = (GLfloat)from->c1;
	to[1][2] = (GLfloat)from->c2;
	to[2][2] = (GLfloat)from->c3;
	to[3][2] = (GLfloat)from->c4;

	to[0][3] = (GLfloat)from->d1;
	to[1][3] = (GLfloat)from->d2;
	to[2][3] = (GLfloat)from->d3;
	to[3][3] = (GLfloat)from->d4;
}

void copyAiMatrix3ToGLM(const aiMatrix3x3 *from, Mat4 &to)
{
	to[0][0] = (GLfloat)from->a1;
	to[1][0] = (GLfloat)from->a2;
	to[2][0] = (GLfloat)from->a3;

	to[0][1] = (GLfloat)from->b1;
	to[1][1] = (GLfloat)from->b2;
	to[2][1] = (GLfloat)from->b3;

	to[0][2] = (GLfloat)from->c1;
	to[1][2] = (GLfloat)from->c2;
	to[2][2] = (GLfloat)from->c3;
}

Vec3 operator / (Vec3 lhs, float rhs)
{
	return Vec3(lhs.x / rhs, lhs.y/rhs, lhs.z/rhs);
}

Vec3 operator * (Vec3 lhs, float rhs)
{
	return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}