/******************************************************************************/
/*!
\file Physics.h
\author Leung Kwan Yee, TEAM TRIGGER
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

#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "Component.h"
#include "ObjectInstance.h"

struct BoundingBox
{
	BoundingBox()
	{
		AABB[0] = Vec3();
		AABB[1] = Vec3();
		centre_ = Vec3();
		length_ = Vec3();
	}

	BoundingBox(Vec3& max, Vec3& min, Vec3& centre, Vec3& length)
	{
		AABB[0] = min;
		AABB[1] = max;
		centre_ = centre;
		length_ = length;
	};

	Vec3 AABB[2];
	Vec3 centre_, length_;
};

class Sphere 
{
public:
	Sphere() : center(Vec3()), radius(1.0f) {}
	Sphere(Vec3 center_, float radius_) : center(center_), radius(radius_) {}
	~Sphere() {}

	Vec3 center;
	float radius;
};

class OBBox
{
public:
	OBBox() : length(Vec3()), original_length(Vec3()), 
			  center(Vec3()), diff(Vec3()) {}
	~OBBox() {};

	Vec3 length;
	Vec3 original_length;
	Vec3 center;
	Vec3 axis[3];
	Vec3 origin[3];
	Vec3 diff;
};

struct BoundShape
{
	BoundShape() : aabb_(BoundingBox()), sphere_(Sphere()), obb_(OBBox()) {}

	BoundShape(const BoundingBox& bb, const Sphere& sphere, const OBBox& obb)
	{
		aabb_.AABB[0] = bb.AABB[0];
		aabb_.AABB[1] = bb.AABB[1];
		aabb_.centre_ = bb.centre_;
		aabb_.length_ = bb.length_;

		sphere_.center = sphere.center;
		sphere_.radius = sphere.radius;

		obb_.length = obb.length;
		obb_.center = obb.center;
		obb_.axis[0] = obb.axis[0];
		obb_.axis[1] = obb.axis[1];
		obb_.axis[2] = obb.axis[2];
		obb_.origin[0] = obb.origin[0];
		obb_.origin[1] = obb.origin[1];
		obb_.origin[2] = obb.origin[2];
		obb_.diff = obb.diff;
		obb_.original_length = obb.original_length;
	}

	BoundShape(const BoundShape& rhs)
	{
		aabb_.AABB[0] = rhs.aabb_.AABB[0];
		aabb_.AABB[1] = rhs.aabb_.AABB[1];
		aabb_.centre_ = rhs.aabb_.centre_;
		aabb_.length_ = rhs.aabb_.length_;

		sphere_.center = rhs.sphere_.center;
		sphere_.radius = rhs.sphere_.radius;

		obb_.length = rhs.obb_.length;
		obb_.center = rhs.obb_.center;
		obb_.axis[0] = rhs.obb_.axis[0];
		obb_.axis[1] = rhs.obb_.axis[1];
		obb_.axis[2] = rhs.obb_.axis[2];
		obb_.origin[0] = rhs.obb_.origin[0];
		obb_.origin[1] = rhs.obb_.origin[1];
		obb_.origin[2] = rhs.obb_.origin[2];
		obb_.diff = rhs.obb_.diff;
		obb_.original_length = rhs.obb_.original_length;
	}

	void SetBoundShape(const BoundingBox& bb, const Sphere& sphere, const OBBox& obb_left)
	{
		aabb_.AABB[0] = bb.AABB[0];
		aabb_.AABB[1] = bb.AABB[1];
		aabb_.centre_ = bb.centre_;
		aabb_.length_ = bb.length_;

		sphere_.center = sphere.center;
		sphere_.radius = sphere.radius;

		obb_.length    = obb_left.length;
		obb_.center    = obb_left.center;
		obb_.axis[0]   = obb_left.axis[0];
		obb_.axis[1]   = obb_left.axis[1];
		obb_.axis[2]   = obb_left.axis[2];
		obb_.origin[0] = obb_left.origin[0];
		obb_.origin[1] = obb_left.origin[1];
		obb_.origin[2] = obb_left.origin[2];
		obb_.diff      = obb_left.diff;
		obb_.original_length = obb_left.original_length;
	}

	BoundingBox aabb_;
	Sphere sphere_;
	OBBox    obb_;
};

class Plane
{
public:
	Plane(){};
	Plane(const Vec4& pl) :pl_(pl) {};
	Plane(float a, float b, float c, float d) { pl_ = Vec4(a, b, c, d); };
	Vec4 pl_;

	bool CollidedSphere(const Sphere& sph_)
	{
		//std::cout << pl_.x * sph_.center.x + pl_.y * sph_.center.y + pl_.z* sph_.center.z + pl_.w << std::endl;
		if (std::abs(pl_.x * sph_.center.x + pl_.y * sph_.center.y + pl_.z* sph_.center.z + pl_.w) <= sph_.radius)
			return true;
		else
			return false;
	}
};

class Physics : public Component
{
public:
	enum BOUNDINGTYPE
	{
		SPHERE = 0,
		BOX,
		OBB
    };

	Physics(ObjectInstance* m_parent);
	~Physics() {}

	void Init();
	void Update();
	void Free();

	const BoundShape& GetBoundGeometry() const;
	BOUNDINGTYPE  GetBoundType() const;
	bool GetCollisionFlag() const;
	bool GetPhysicsType() const;
	bool GetFalling() const;
	float GetMass() const;
	float GetInertia() const;
	float GetAngularYaw() const;
	float GetAngularRoll() const;
	float GetAngularPitch() const;
	float GetResitution() const;
	float GetLastSpeed() const;

	void SetBoundingGeometry(const BoundShape& bShape);
	void SetCollisionFlag(bool flag_);
	void SetPhysicsType(bool dynamic_);
	void SetFalling(bool flag_);
	void SetMass(float mass_);
	void SetInertia(float inertia_);
	void SetAngularYaw(float aYaw_);
	void SetAngularRoll(float aRoll_);
	void SetAngularPitch(float aPitch_);
	void SetResitution(float res_);
	void SetLastSpeed(float last_);
	void SetBoundType(int type_);

private:
	Physics();

    BoundShape m_bGeometry;
	BOUNDINGTYPE m_BoundType;
	
	bool m_Collided;
	bool m_Falling;
	bool m_Dynamic;

	float m_Mass;
	float m_Inertia;
	float m_AngularRoll;
	float m_AngularYaw;
	float m_AngularPitch;
	float m_Resitution;
	float m_LastSpeed;
};

#endif