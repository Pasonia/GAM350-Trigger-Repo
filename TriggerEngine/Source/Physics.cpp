/******************************************************************************/
/*!
\file Physics.cpp
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

#include "Physics.h"
#include "PhysicsManager.h"
#include <algorithm>
#include <iostream>

Physics::Physics(ObjectInstance* m_parent) : Component(m_parent), m_bGeometry(BoundShape()), m_Collided(false)
{
	SetComponentID(CT_PHYSICS);

	m_Collided = false;
	m_Falling = true;
	m_Dynamic = false;

	if (m_BoundType == Physics::BOUNDINGTYPE::SPHERE)
	{
		m_AngularYaw = (m_parent->GetVelocity().y / m_bGeometry.sphere_.radius) * g_dt;
		m_AngularRoll = (m_parent->GetVelocity().x / m_bGeometry.sphere_.radius) * g_dt;
		m_AngularPitch = (m_parent->GetVelocity().z / m_bGeometry.sphere_.radius) * g_dt;
	}		
	else
	{

	}

	m_Mass = 10.0f;
	m_Inertia = 10.0f;
	m_Resitution = 0.5f;
	m_LastSpeed = 0.0f;

	PhysicsManager_Inst->AddObject(m_parent, this);
}

void Physics::Init() {}

void Physics::Update() {}

void Physics::Free() {}

const BoundShape& Physics::GetBoundGeometry() const			{ return m_bGeometry; }
Physics::BOUNDINGTYPE Physics::GetBoundType() const			{ return m_BoundType; }
bool Physics::GetCollisionFlag() const						{ return m_Collided; }
bool Physics::GetPhysicsType() const						{ return m_Dynamic; }
bool Physics::GetFalling() const							{ return m_Falling; }

float Physics::GetMass() const								{ return m_Mass; }
float Physics::GetInertia() const							{ return m_Inertia; }
float Physics::GetAngularYaw() const						{ return m_AngularYaw; }
float Physics::GetAngularRoll() const						{ return m_AngularRoll; }
float Physics::GetAngularPitch() const						{ return m_AngularPitch; }
float Physics::GetResitution() const						{ return m_Resitution; }
float Physics::GetLastSpeed() const							{ return m_LastSpeed; }

void Physics::SetCollisionFlag(bool flag_)					{ m_Collided = flag_; }
void Physics::SetPhysicsType(bool dynamic_)					{ m_Dynamic = dynamic_; }
void Physics::SetFalling(bool flag_)						{ m_Falling = flag_; }
void Physics::SetAngularYaw(float aYaw_)					{ m_AngularYaw = aYaw_; }
void Physics::SetAngularRoll(float aRoll_)					{ m_AngularRoll = aRoll_; }
void Physics::SetAngularPitch(float aPitch_)				{ m_AngularPitch = aPitch_; }
void Physics::SetMass(float mass_)							{ m_Mass = mass_; }
void Physics::SetInertia(float inertia_)					{ m_Inertia = inertia_; }
void Physics::SetResitution(float res_)						{ m_Resitution = res_; }
void Physics::SetLastSpeed(float last_)						{ m_LastSpeed = last_; }

void Physics::SetBoundingGeometry(const BoundShape& bShape)
{
	//aabb
	m_bGeometry.aabb_.AABB[0] = bShape.aabb_.AABB[0];
	m_bGeometry.aabb_.AABB[1] = bShape.aabb_.AABB[1];
	m_bGeometry.aabb_.centre_ = bShape.aabb_.centre_;
	m_bGeometry.aabb_.length_ = bShape.aabb_.length_;
	//sphere
	m_bGeometry.sphere_.center = bShape.sphere_.center;
	m_bGeometry.sphere_.radius = bShape.sphere_.radius;
	//obb
	m_bGeometry.obb_.length    = bShape.obb_.length;
	m_bGeometry.obb_.center    = bShape.obb_.center;
	m_bGeometry.obb_.axis[0]   = bShape.obb_.axis[0];
	m_bGeometry.obb_.axis[1]   = bShape.obb_.axis[1];
	m_bGeometry.obb_.axis[2]   = bShape.obb_.axis[2];
	m_bGeometry.obb_.origin[0] = bShape.obb_.origin[0];
	m_bGeometry.obb_.origin[1] = bShape.obb_.origin[1];
	m_bGeometry.obb_.origin[2] = bShape.obb_.origin[2];
	m_bGeometry.obb_.diff      = bShape.obb_.diff;
}

void Physics::SetBoundType(int type_)
{
	if (type_ == 0)
		m_BoundType = BOUNDINGTYPE::SPHERE;
	else if (type_ ==1)
		m_BoundType = BOUNDINGTYPE::BOX;
	else if (type_ == 2)
		m_BoundType = BOUNDINGTYPE::OBB;
}