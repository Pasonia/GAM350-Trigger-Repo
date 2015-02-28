/******************************************************************************/
/*!
\file TEMath.h
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
#define GLM_FORCE_RADIANS
#include "Assert.h"
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/ext.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "assimp\Importer.hpp" 

#define g_mt TIMER_INST->GetModifiedDeltaTime()
#define g_dt TIMER_INST->GetDeltaTime()

#define PI 3.1415926535897932384626f
#define PI_QUARTER  PI/4.f
#define PI_H_ALF  PI/2.f

#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0f)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0f / PI)


typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::mat4 Mat4;
typedef glm::mat3 Mat3;

Vec3 operator / (Vec3 lhs, float rhs);
Vec3 operator * (Vec3 lhs, float rhs);

void printVec(const Vec3& lhs);
void copyAiMatrixToGLM(const aiMatrix4x4 *from, Mat4 &to);
void copyAiMatrix3ToGLM(const aiMatrix3x3 *from, Mat4 &to);
//
//Mat3 Mat3x3Identity()
//{
//	return mat3(1.0f);
//}
//
//Mat4 Mat4x4Identity()
//{
//	return mat4(1.0f);
//}
