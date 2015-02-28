#ifndef _FRUSTRUM_H_
#define _FRUSTRUM_H_
#include "Editor.h"
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Picking.h" 
#include "CameraController.h"
#include "Primitive.h"
#include "TriggerAssert.h"
#include <iostream>


class VFS
{
public:
	VFS(){};
	VFS(const Mat4& mat4);
	int AABBVsFrustum(const VFS& frustum_, const Vec3& aabb_min, const Vec3& aabb_max, int lastPlaneTested);
	int SphereVsFrustum(const VFS& frustum_, const Vec3& center, float radius, int lastPlaneTested);
	Vec4 plane_cont[6];
};



#endif