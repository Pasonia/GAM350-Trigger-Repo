/******************************************************************************/
/*!
\file PhysicsManager.h
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

#ifndef _PHYSICSMANAGER_H_
#define _PHYSICSMANAGER_H_

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <gmm/gmm.h>
#include "Audio.h"
#include "TEMath.h"
#include "Physics.h"
#include "ObjectMap.h"
#include "ObjectManager.h"

class Model;
class Plane;
struct BoundShape;
class ObjectInstance;

typedef std::map<ObjectInstance*, Physics*> PhysicsContainer;
typedef std::map<Model*, BoundShape> MContainer;
typedef std::map<std::string, Model*> ModelNameContainer;
typedef std::vector<ObjectInstance*> UpdatableContainer;

class PhysicsManager
{
public:
	void RegisterModel(Model* model_);	
	void AddObject(ObjectInstance* id, Physics* physics_component);	
	void RegisterObject(ObjectInstance* object_);

	void SetPlayer(ObjectInstance* player_ = 0);
	void SetGravityRadius(float radius);
	
	static PhysicsManager* GetInstance();
	UpdatableContainer& GetUpdatables();
	float GetGravityRadius();
	bool GetPlayerCollided();
	Physics* GetPhysics(ObjectInstance* id);

	void UnregisterObject(ObjectInstance* object_);
	void ClearUpdatables();

	void FloatObject(ObjectInstance* object_,float time);
	void GravityInverse();	
	
	void Update();
	void EditorUpdate();
	void Free();
	static void Destroy();

	Vec3 Gravity_dir;
	std::vector<Plane> Plane_cont;

private:
	struct Manifold
	{
		float m_pDistance = 0.0f;
		Vec3 m_cNormal = Vec3();
	};

	PhysicsManager();
	PhysicsManager(const PhysicsManager&);
	PhysicsManager& operator=(const PhysicsManager&);
	~PhysicsManager();

	void ConstructBoundingGeometry(Model* model_);
	void ConstructBoundingGeometry(ObjectInstance* object_, Physics* physic_);
	void ConstructOBB(ObjectInstance* object_, Physics* physic_);
	
	void UpdatePlayerGeometry();
	void UpdateBoundingGeometry();

	bool CollisionDetector(Physics* object_A, Physics* object_B, Manifold& manifold_);
	
	bool SphereCollideSphere(const BoundShape& aSphere, const BoundShape& bSphere, Manifold& manifold_);
	bool BoxCollideBox(const BoundShape& aBox, const BoundShape& bBox, Manifold& manifold_);
	bool SphereCollideBox(const BoundShape& aSphere, const BoundShape& bBox, Manifold& manifold_);
	bool SphereCollideOBB(const BoundShape& aSphere, const BoundShape& bObb, Manifold& manifold_);
	
	bool CollisionPointPlane(Vec3& Point, Vec3& PlaneCenter, Vec3& PlaneNormal);
	
	void CollisionResponse(ObjectInstance* object_A, ObjectInstance* object_B, Manifold& manifold_);
	
	static PhysicsManager* m_instance;

	UpdatableContainer m_updatables;
	PhysicsContainer m_physicsCont;
	MContainer m_modelCont;
	ModelNameContainer m_nameCont;
	
	ObjectInstance* m_player;
	
	float m_gravity_radius;
	bool m_playerCollided;
};

#define PhysicsManager_Inst PhysicsManager::GetInstance()

#endif