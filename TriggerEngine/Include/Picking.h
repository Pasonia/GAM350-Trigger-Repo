/******************************************************************************/
/*!
\file Picking.h
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
#ifndef _PICKING_H_
#define _PICKING_H_

#include "TEMath.h"
#include "Physics.h"
#include <vector>

class Ray
{
public:
	Ray(){};
	Ray(Vec3 pos_, Vec3 dir_) : pos(pos_), dir(dir_)
	{
		inv_dir = Vec3(1 / dir.x, 1 / dir.y, 1 / dir.z);
		sign[0] = (inv_dir.x < 0);
		sign[1] = (inv_dir.y < 0);
		sign[2] = (inv_dir.z < 0);
	};
	~Ray(){};

	Vec3 pos;
	Vec3 dir;
	Vec3 inv_dir;
	int sign[3];

};

namespace Picking
{
	//check may intersect or not
	//return 0 means no intersection
	//return non-zero means got intersected, and the intersected time is returned.
	float  RayIntersectSphere(Vec3& mousePos, Sphere& sphere, int window_width, int window_H_eight);
	//return the time of intersection
	float RayIntersectSphereComputeT(Ray& ray, Sphere& sphere,float n_square,float m);

	int ScenePickingCheck(std::vector<Sphere>& scene, Vec3& mousePos, int window_width, int window_H_eight);
	//the grid is forever on the X_Y plane
	Vec3 RayIntersectGrid(Vec3& mousePos, int window_width, int window_H_eight,Vec3& plane_normal,float offset = 0);

	bool RayIntersectAABB(Vec3& mousePos, BoundingBox bb, int window_width, int window_H_eight);

	Vec3 GetWorldCoordinate(Vec3& mousePos, int window_width, int window_H_eight);

	Vec3 GetRay(Vec3& mousePos, int window_width, int window_H_eight);

	bool RayIntersectAABB_general(Vec3 Pos,float angle,BoundingBox bb);

	bool RayIntersectOBB(Vec3& mousePos, const OBBox& OBB, int window_width, int window_H_eight, float& t);

}





#endif


