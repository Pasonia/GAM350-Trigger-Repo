/******************************************************************************/
/*!
\file Picking.cpp
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

#include "Picking.h"
#include "System.h"
float Picking::RayIntersectSphere(Vec3& mousePos, Sphere& sphere,int window_width, int window_H_eight)
{
	//normalized device coordinate
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = 1.0f;
	Vec3 ray_nds = Vec3(x, y, z);

	float scale_y = 0.1f * tan(PI_QUARTER*0.5f);
	
	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;

	ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;

	Vec4 sphere_center(sphere.center.x, sphere.center.y, sphere.center.z, 1.0f);
	
	sphere_center = System::GetInstance()->GetCamera()->GetViewMatrix()*sphere_center;
    Vec3 sphere_center_vec3 ( sphere_center.x , sphere_center.y , sphere_center.z );
	float length = sqrt(ray_nds.x *ray_nds.x + ray_nds.y *ray_nds.y + ray_nds.z *ray_nds.z);
	ray_nds /= length;
    Ray ray(Vec3(0,0,0), ray_nds);
    float m = glm::dot((sphere_center_vec3 - ray.pos), ray.dir);
	float m_square = m*m;
	Vec3 EC = sphere_center_vec3 - ray.pos;
	float EC_Square = glm::dot(EC,EC);
	float n_Square = EC_Square - m_square;
	if ( m < 0)
		return 0;
    if ((EC_Square - m_square)>sphere.radius * sphere.radius)
		return 0;
	return RayIntersectSphereComputeT(ray,sphere,n_Square,m);
}
bool Picking::RayIntersectAABB_general(Vec3 pos,float angle, BoundingBox bb)
{
	Ray ray(pos, Vec3(glm::cos(degreesToRadians(angle)), glm::sin(degreesToRadians(angle)), 0));
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (bb.AABB[ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;
	tmax = (bb.AABB[1 - ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;
	tymin = (bb.AABB[ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	tymax = (bb.AABB[1 - ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (bb.AABB[ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;
	tzmax = (bb.AABB[1 - ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ((tmin < FLT_MAX) && (tmax > 0));
}
bool Picking::RayIntersectAABB(Vec3& mousePos, BoundingBox bb, int window_width, int window_H_eight)
{
	//normalized device coordinate
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = -1.0f;
	Vec4 ray_nds = Vec4(x, y, z,1.0);
    float scale_y = 0.1f * tan(PI_QUARTER*0.5f);
	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;
    ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
  
	ray_nds = glm::inverse(System::GetInstance()->GetCamera()->GetViewMatrix())*ray_nds;


	Vec3 ray_eye(ray_nds.x - System::GetInstance()->GetCamera()->GetEye().x,
		         ray_nds.y - System::GetInstance()->GetCamera()->GetEye().y,
		         ray_nds.z - System::GetInstance()->GetCamera()->GetEye().z
		);

	float length = sqrt(ray_eye.x *ray_eye.x + ray_eye.y *ray_eye.y + ray_eye.z *ray_eye.z);
	ray_eye /= length;
	Ray ray(System::GetInstance()->GetCamera()->GetEye(), Vec3(ray_eye.x, ray_eye.y, ray_eye.z));
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (bb.AABB[ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;
	tmax = (bb.AABB[1 - ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;
	tymin = (bb.AABB[ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	tymax = (bb.AABB[1 - ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (bb.AABB[ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;
	tzmax = (bb.AABB[1 - ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ((tmin < FLT_MAX) && (tmax > 0));

}
bool check_axis(float start, float dir, float min, float max, float& tfirst, float& tlast)
{
	if (std::abs(dir) < 1.0E-8)
    {
		return (start < max && start > min);
	}
	float tmin = (min - start) / dir;
	float tmax = (max - start) / dir;
	if (tmin > tmax) std::swap(tmin, tmax);

	if (tmax < tfirst || tmin > tlast)
		return false;

	if (tmin > tfirst) 
		tfirst = tmin;
	if (tmax < tlast)  
		tlast = tmax;
	return true;
}

bool Picking::RayIntersectOBB(Vec3& mousePos, const OBBox& OBB, int window_width, int window_H_eight, float& t)
{
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = -1.0f;
	Vec4 ray_nds = Vec4(x, y, z, 1.0);
	float scale_y = 0.1f * tan(PI_QUARTER*0.5f);
	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;
	ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
    ray_nds = glm::inverse(System::GetInstance()->GetCamera()->GetViewMatrix())*ray_nds;
    Vec3 ray_eye(ray_nds.x - System::GetInstance()->GetCamera()->GetEye().x,
		ray_nds.y - System::GetInstance()->GetCamera()->GetEye().y,
		ray_nds.z - System::GetInstance()->GetCamera()->GetEye().z
		);
    float length = sqrt(ray_eye.x *ray_eye.x + ray_eye.y *ray_eye.y + ray_eye.z *ray_eye.z);
	ray_eye /= length;
	Ray ray(System::GetInstance()->GetCamera()->GetEye(), Vec3(ray_eye.x, ray_eye.y, ray_eye.z));
	Vec3 X_axis = glm::normalize(Vec3(OBB.axis[0].x, OBB.axis[1].x, OBB.axis[2].x));
	Vec3 Y_axis = glm::normalize(Vec3(OBB.axis[0].y, OBB.axis[1].y, OBB.axis[2].y));
	Vec3 Z_axis = glm::normalize(Vec3(OBB.axis[0].z, OBB.axis[1].z, OBB.axis[2].z));
	float tfirst = FLT_MIN, tlast = FLT_MAX;

	if (!check_axis(glm::dot(ray.pos, X_axis), glm::dot(ray.dir, X_axis), glm::dot(OBB.center, X_axis) - OBB.length.x, glm::dot(OBB.center, X_axis) + OBB.length.x, tfirst, tlast)) return false;
	if (!check_axis(glm::dot(ray.pos, Y_axis), glm::dot(ray.dir, Y_axis), glm::dot(OBB.center, Y_axis) - OBB.length.y, glm::dot(OBB.center, Y_axis) + OBB.length.y, tfirst, tlast)) return false;
	if (!check_axis(glm::dot(ray.pos, Z_axis), glm::dot(ray.dir, Z_axis), glm::dot(OBB.center, Z_axis) - OBB.length.z, glm::dot(OBB.center, Z_axis) + OBB.length.z, tfirst, tlast)) return false;
	t = tfirst;
	return true;
















}
Vec3 Picking::GetWorldCoordinate(Vec3& mousePos, int window_width, int window_H_eight)
{
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = -1.0f;
	Vec4 ray_nds(x, y, z, 1.0f);
	float scale_y = 0.1f * tan(PI_QUARTER*0.5f);
    float scale_x = (4.0f / 3.0f) * scale_y;
    ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
	ray_nds = glm::inverse(System::GetInstance()->GetCamera()->GetViewMatrix())*ray_nds;
	return Vec3(ray_nds.x, ray_nds.y, ray_nds.z);

}
Vec3 Picking::RayIntersectGrid(Vec3& mousePos, int window_width, int window_H_eight,Vec3& plane_normal , float offset)
{
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = -1.0f;
	Vec4 ray_nds(x, y, z, 1.0f);
	//convert to view frame
	float scale_y = 0.1f * tan(PI_QUARTER*0.5f);

	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;

	ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
	ray_nds = glm::inverse(System::GetInstance()->GetCamera()->GetViewMatrix())*ray_nds;
	Vec3 ray(ray_nds.x - System::GetInstance()->GetCamera()->GetEye().x,
		     ray_nds.y - System::GetInstance()->GetCamera()->GetEye().y,
			 ray_nds.z - System::GetInstance()->GetCamera()->GetEye().z
		);

	float length = sqrt(ray.x *ray.x + ray.y *ray.y + ray.z *ray.z);
	ray /= length;
	float t = -((glm::dot(System::GetInstance()->GetCamera()->GetEye(), plane_normal)-offset) / (glm::dot(ray, plane_normal)));
	
    return System::GetInstance()->GetCamera()->GetEye() + t * ray;
}
int Picking::ScenePickingCheck(std::vector<Sphere>& scene, Vec3& mousePos, int window_width, int window_H_eight)
{
	int obj_index = -1;
	//normalized device coordinate
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = 1.0f;
	float distance = FLT_MAX;
	Vec3 ray_nds = Vec3(x, y, z);
    float scale_y = 0.1f * tan(PI_QUARTER*0.5f);
	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;
    ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
	
	for (std::vector<Sphere>::iterator it = scene.begin(); it != scene.end(); ++it)
	{
		Vec4 sphere_center(it->center.x, it->center.y, it->center.z, 1.0f);
        sphere_center = System::GetInstance()->GetCamera()->GetViewMatrix()*sphere_center;
		Vec3 sphere_center_vec3(sphere_center.x, sphere_center.y, sphere_center.z);
		float length = sqrt(ray_nds.x *ray_nds.x + ray_nds.y *ray_nds.y + ray_nds.z *ray_nds.z);
		ray_nds /= length;
		Ray ray(Vec3(0, 0, 0), ray_nds);
		float m = glm::dot((sphere_center_vec3 - ray.pos), ray.dir);
		float m_square = m*m;
		Vec3 EC = sphere_center_vec3 - ray.pos;
		float EC_Square = glm::dot(EC, EC);
		float n_Square = EC_Square - m_square;
		if (m < 0)
			continue;
		//moving towards sphere, but will miss the sphere
		if ((EC_Square - m_square)> it->radius * it->radius)
			continue;
		float t = RayIntersectSphereComputeT(ray, *it, n_Square, m);
		if (t >= 0.f)
		{
			if (distance > t)
			{
				distance = t;
				obj_index = it - scene.begin();
			}
		}
    }
	return obj_index;
}
float Picking::RayIntersectSphereComputeT(Ray& ray, Sphere& sphere, float n_square, float m)
{
	float radius_square = sphere.radius*sphere.radius;
	float s = sqrt(radius_square - n_square);
	return m - s;
}
Vec3 Picking::GetRay(Vec3& mousePos, int window_width, int window_H_eight)
{
	float x = (2.0f * mousePos.x) / window_width - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y) / window_H_eight;
	float z = -1.0f;
	Vec4 ray_nds(x, y, z, 1.0f);
	//convert to view frame
	float scale_y = 0.1f * tan(PI_QUARTER*0.5f);

	float scale_x = ((float)window_width / (float)window_H_eight) * scale_y;

	ray_nds.x *= scale_x;
	ray_nds.y *= scale_y;
	ray_nds.z = -0.1f;
	ray_nds = glm::inverse(System::GetInstance()->GetCamera()->GetViewMatrix())*ray_nds;
	Vec3 ray(ray_nds.x - System::GetInstance()->GetCamera()->GetEye().x,
		ray_nds.y - System::GetInstance()->GetCamera()->GetEye().y,
		ray_nds.z - System::GetInstance()->GetCamera()->GetEye().z
		);

	float length = sqrt(ray.x *ray.x + ray.y *ray.y + ray.z *ray.z);
	ray /= length;
	return ray;



}