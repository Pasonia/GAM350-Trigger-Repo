/******************************************************************************/
/*!
\file PhysicsManager.cpp
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

#include "PhysicsManager.h"

#define g_dt TIMER_INST->GetDeltaTime()

float gravity = 9.8f;

PhysicsManager* PhysicsManager::m_instance = 0;

void PhysicsManager::RegisterModel(Model* model_)
{
	std::string fileDir = "../Resources/BoundingVertices/";
	std::string name = OBJMAP_INST->GetMappedObjectName(OBJMAP_INST->GetModelIndex(model_));
	
	fileDir += name;
	fileDir += "_bound.txt";

	std::ifstream file(fileDir.c_str());

	//if (file.is_open()) // Read bounding vertices data from existing file.
	//{
	//	std::string value;

	//	BoundShape bGeometry;

	//	while (file.good)
	//	{
	//		std::getline(file, value);

	//		std::istringstream iss(value);

	//		iss >> bGeometry.aabb_.AABB[0].x >> bGeometry.aabb_.AABB[0].y >> bGeometry.aabb_.AABB[0].z;

	//	}
	//}
	//else // Reconstruct bounding vertices and write the data to a file.
	{
		ConstructBoundingGeometry(model_);


	}


}

void PhysicsManager::AddObject(ObjectInstance* id, Physics* physics_)
{
	ConstructBoundingGeometry(id, physics_);
	
	m_physicsCont[id] = physics_;
}

void PhysicsManager::RegisterObject(ObjectInstance* object_)
{
	m_updatables.push_back(object_);
}

void PhysicsManager::UnregisterObject(ObjectInstance* object_)
{
	UpdatableContainer::iterator itr = std::remove(m_updatables.begin(), m_updatables.end(), object_);
	
	m_updatables.erase(itr, m_updatables.end());
}

UpdatableContainer& PhysicsManager::GetUpdatables() 
{
	return m_updatables;
}

void PhysicsManager::ClearUpdatables()
{
	m_updatables.clear();
}

Physics* PhysicsManager::GetPhysics(ObjectInstance* iD)
{
	auto itr = m_physicsCont.find(iD);

	if (itr != m_physicsCont.end())
		return itr->second;

	return NULL;
}

PhysicsManager* PhysicsManager::GetInstance()
{
	m_instance = m_instance ? m_instance : new PhysicsManager();
	
	return m_instance;
}

void PhysicsManager::SetPlayer(ObjectInstance* player_)
{ 
	player_ ? m_player = player_ : m_player = OBJECTMANAGER_INST[0];
}

void PhysicsManager::Update()
{
	UpdateBoundingGeometry();

	m_playerCollided = false;

	// Remove once OBJECTMANAGER_INST[0] is fixed.
	PhysicsContainer::iterator player_ = m_physicsCont.find(m_player);

	//PhysicsContainer::iterator player_ = m_physicsCont.find(OBJECTMANAGER_INST[0]);

	Physics* dragon_ = NULL; Physics* ball_ = NULL;

	Vec3 Player_position;

	if (player_ != m_physicsCont.end())
	{
		dragon_ = GetPhysics(player_->first);
		dragon_->SetCollisionFlag(false);
		Player_position = (player_->first)->GetPosition();		
	}
	
	if (m_updatables.size() > 0)
	{		
		std::vector<ObjectInstance*>::iterator itr = m_updatables.begin();
       
		// Update physical attributes.
		while (itr != m_updatables.end())
		{
			Physics* physics_A = GetPhysics(*itr);

			if (physics_A != dragon_ && physics_A != ball_)
			{
				// Update rotational motion.
				if (physics_A->GetBoundType() == Physics::BOUNDINGTYPE::SPHERE)
				{
					float roll_ = physics_A->GetAngularRoll();
					float yaw_ = physics_A->GetAngularYaw();
					float pitch_ = physics_A->GetAngularPitch();

					Vec3 velocity_ = (*itr)->GetVelocity();

					//(*itr)->SetRoll((roll_ += (velocity_.x / physics_A->GetBoundGeometry().sphere_.radius)) * g_dt);
					//(*itr)->SetYaw((yaw_ += (velocity_.y / physics_A->GetBoundGeometry().sphere_.radius)) * g_dt);
					//(*itr)->SetPitch((pitch_ += (velocity_.z / physics_A->GetBoundGeometry().sphere_.radius)) * g_dt);

					physics_A->SetAngularRoll(roll_);
					physics_A->SetAngularYaw(yaw_);
					physics_A->SetAngularPitch(pitch_);
				}
				else
				{

				} // End of rotational motion update.

				//// Update gravity.
				//if (player_->first != (*itr))
				//{
				//	if (physics_A->GetPhysicsType())
				//	{
				//		//if (physics_A->GetFalling())
				//	//	{
				//		if (glm::length((*itr)->GetPosition() - Player_position) > m_gravity_radius)
				//		{
				//			Vec3 gravity_temp(0, -1.0f, 0);
				//			Vec3 CurrVel((*itr)->GetVelocity() + gravity * g_dt * 20.0f  * gravity_temp);
				//			(*itr)->SetVelocity(CurrVel);

				//		}
				//		else
				//		{

				//			Vec3 CurrVel((*itr)->GetVelocity() + gravity * g_dt * 20.0f  * Gravity_dir);
				//			(*itr)->SetVelocity(CurrVel);
				//		}
				//		

				//			for (std::vector<Plane>::iterator itp = Plane_cont.begin(); itp != Plane_cont.end(); ++itp)
				//			{

				//				if (itp->CollidedSphere((physics_A->GetBoundGeometry()).sphere_))
				//				{
				//					//std::cout << std::abs(glm::length((*itr)->LastSpeed) - glm::length((*itr)->GetVelocity())) << std::endl;
				//					
				//					if (std::abs(glm::length((*itr)->LastSpeed) - glm::length((*itr)->GetVelocity())) < 1.0f)
				//					{
				//						(*itr)->SetVelocity(Vec3(0, 0, 0));
				//						physics_A->SetFalling(false);
				//						continue;
				//					}
				//					(*itr)->LastSpeed = (*itr)->GetVelocity();
				//					//float diff = std::abs((*itr)->GetPosition().y - (physics_A->GetBoundGeometry()).sphere_.radius);
				//					float temp = itp->pl_.x * (*itr)->GetVelocity().x +
				//						itp->pl_.y * (*itr)->GetVelocity().y +
				//						itp->pl_.z * (*itr)->GetVelocity().z;
				//					temp *= 2;

				//					Vec3 BoundedVel((*itr)->GetVelocity() - temp * Vec3(itp->pl_.x, itp->pl_.y, itp->pl_.z));
				//					//if ((*itr)->GetPosition().y < 0)
				//					//{
				//					//(*itr)->SetPosition((*itr)->GetPosition() + (diff + FLT_EPSILON)*Vec3(itp->pl_.x, itp->pl_.y, itp->pl_.z));
				//					(*itr)->SetPosition((*itr)->GetPosition() + 0.1f*Vec3(itp->pl_.x, itp->pl_.y, itp->pl_.z));
				//					//}

				//					(*itr)->SetVelocity(0.9f * BoundedVel);
				//				}
				//				//else
				//				//{
				//				//	physics_A->SetCollisionFlag(false);
				//				//}
				//			}
				//		//}
				//	}
				//}// End of gravity update.

				Manifold manifold_;

				// Collision check between player and obstacles.
				if (CollisionDetector(dragon_, physics_A, manifold_))
				{
					SOUNDMGR_INST->StartPlayingSound("HitObstacle");
					
					m_playerCollided = true;

					(*itr)->SetCollidedThisFrame(true);

					// Player's collision response.
					CollisionResponse(player_->first, *itr, manifold_); 
				}
			} // End of collision check between player and obstacles.
			else if (physics_A == ball_)
			{
				// Check collision with ground.


			} // End of collision check between hovering sphere and ground.

			std::vector<ObjectInstance*>::iterator itr_ = m_updatables.begin();

			// Collision check between obstacles.
			//while (itr_ != m_updatables.end())
			//{
			//	if (itr != itr_)
			//	{
			//		Physics* physics_B = GetPhysics(*itr_);

			//		//if (physics_A->GetPhysicsType() == false && physics_B->GetPhysicsType() == false)
			//		//{
			//	//		continue;
			//	//	}
			//		/*std::cout <<"A: " << physics_A->GetPhysicsType() << std::endl;
			//		std::cout <<"B: " <<physics_B->GetPhysicsType() << std::endl;*/

			//		// Check collision if neither both are static objects.
			//		if (physics_A->GetPhysicsType() || physics_B->GetPhysicsType())
			//		{		
			//			if (CollisionDetector(physics_A, physics_B, manifold_))
			//				CollisionResponse(*itr, *itr_, manifold_);
			//		}
			//	}
			//	
			//	++itr_;
			//}  End of obstacles to obstacles collision check.
			
			++itr;
		} // End of physical attributes updates.
	} // End of updatables.
}

void PhysicsManager::EditorUpdate()
{
	PhysicsContainer::iterator itr = m_physicsCont.begin();

	while (itr != m_physicsCont.end())
	{
		MContainer::iterator model_itr = m_modelCont.find(const_cast<Model*>(itr->first->GetModel()));

		if (model_itr == m_modelCont.end())
			return;

		Physics* physics_ = GetPhysics(itr->first);

		BoundShape newGeometry;

		// Update bounding box.
		if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::BOX)
		{
			float xLength = model_itr->second.aabb_.length_.x;
			float yLength = model_itr->second.aabb_.length_.y;
			float zLength = model_itr->second.aabb_.length_.z;

			// Scale bounding box length.
			xLength *= itr->first->GetScaleX();
			yLength *= itr->first->GetScaleY();
			zLength *= itr->first->GetScaleZ();

			Vec3 tmpLength(xLength, yLength, zLength);

			Mat4 rMatrix = glm::transpose(itr->first->GetRotationMatrix());

			float x_len = std::fabs(rMatrix[0][0]) * tmpLength.x +
						  std::fabs(rMatrix[0][1]) * tmpLength.y +
						  std::fabs(rMatrix[0][2]) * tmpLength.z;
			float y_len = std::fabs(rMatrix[1][0]) * tmpLength.x +
						  std::fabs(rMatrix[1][1]) * tmpLength.y +
						  std::fabs(rMatrix[1][2]) * tmpLength.z;
			float z_len = std::fabs(rMatrix[2][0]) * tmpLength.x +
						  std::fabs(rMatrix[2][1]) * tmpLength.y +
						  std::fabs(rMatrix[2][2]) * tmpLength.z;

			Vec3 newLength(x_len, y_len, z_len);

			Vec3 min_ = model_itr->second.aabb_.centre_ +
						itr->first->GetPosition() - newLength;
			Vec3 max_ = model_itr->second.aabb_.centre_ +
						itr->first->GetPosition() + newLength;
			Vec3 centre_ = min_ + newLength;

			newGeometry.SetBoundShape(BoundingBox(max_, min_, centre_, newLength), Sphere(), OBBox());
		}
		else if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::SPHERE)// Update bounding sphere.
		{
			float scale_ = itr->first->GetScaleX() > itr->first->GetScaleY() ?
						   itr->first->GetScaleX() : itr->first->GetScaleY();
			scale_ = itr->first->GetScaleY() > itr->first->GetScaleZ() ?
					 itr->first->GetScaleY() : itr->first->GetScaleZ();

			float radius_ = model_itr->second.sphere_.radius * scale_;

			Vec3 sCentre_ = model_itr->second.sphere_.center + itr->first->GetPosition();

			newGeometry.SetBoundShape(BoundingBox(), Sphere(sCentre_, radius_), OBBox());
		}
		else if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::OBB)
		{
			OBBox temp_obb;
			Vec3 obb_center, obb_axis_zero, obb_axis_one, obb_axis_two;
			Vec4 temp;
			temp = Vec4(model_itr->second.obb_.diff.x, model_itr->second.obb_.diff.y, model_itr->second.obb_.diff.z, 1);
			temp = itr->first->GetTransform()*temp;

			obb_center = Vec3(temp.x, temp.y, temp.z);

			temp = Vec4(model_itr->second.obb_.origin[0].x, model_itr->second.obb_.origin[1].x, model_itr->second.obb_.origin[2].x, 0);
			temp = itr->first->GetTransform()*temp;

			obb_axis_zero.x = temp.x; obb_axis_one.x = temp.y; obb_axis_two.x = temp.z;

			temp = Vec4(model_itr->second.obb_.origin[0].y, model_itr->second.obb_.origin[1].y, model_itr->second.obb_.origin[2].y, 0);
			temp = itr->first->GetTransform()*temp;

			obb_axis_zero.y = temp.x; obb_axis_one.y = temp.y; obb_axis_two.y = temp.z;

			temp = Vec4(model_itr->second.obb_.origin[0].z, model_itr->second.obb_.origin[1].z, model_itr->second.obb_.origin[2].z, 0);
			temp = itr->first->GetTransform()*temp;

			obb_axis_zero.z = temp.x; obb_axis_one.z = temp.y; obb_axis_two.z = temp.z;
			temp_obb.length = model_itr->second.obb_.length;
			temp_obb.center = obb_center;
			temp_obb.axis[0] = obb_axis_zero;
			temp_obb.axis[1] = obb_axis_one;
			temp_obb.axis[2] = obb_axis_two;
			temp_obb.origin[0] = model_itr->second.obb_.origin[0];
			temp_obb.origin[1] = model_itr->second.obb_.origin[1];
			temp_obb.origin[2] = model_itr->second.obb_.origin[2];
			temp_obb.diff = model_itr->second.obb_.diff;
			temp_obb.original_length = model_itr->second.obb_.original_length;

			newGeometry.SetBoundShape(BoundingBox(), Sphere(), temp_obb);
		}
		GetPhysics(itr->first)->SetBoundingGeometry(newGeometry);

		++itr;
	} // End of update loop.
}

void PhysicsManager::Free()
{
	m_physicsCont.clear();
	m_updatables.clear();
}

void PhysicsManager::Destroy()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void PhysicsManager::GravityInverse()
{
	PhysicsContainer::iterator player_ = m_physicsCont.find(OBJECTMANAGER_INST[0]);

	if (player_ == m_physicsCont.end())
		return;

	player_->second->SetCollisionFlag(false);

	PhysicsContainer::iterator itr = m_physicsCont.begin();

	// Check for collision.
	while (itr != m_physicsCont.end())
	{
		if (player_ != itr)
			itr->second->SetFalling(true);

		++itr;
	}
}

PhysicsManager::PhysicsManager()
{
	Gravity_dir = Vec3(0,-1,0);
}

PhysicsManager::~PhysicsManager()
{
	Free();
}

void PhysicsManager::ConstructBoundingGeometry(Model* model_)
{
	BoundShape bGeometry_;

	// Constructing bounding aabb.
	const float BIGNUMBER = 10000.f;

	bGeometry_.aabb_.AABB[0].x = bGeometry_.aabb_.AABB[0].y = bGeometry_.aabb_.AABB[0].z = BIGNUMBER;
	bGeometry_.aabb_.AABB[1].x = bGeometry_.aabb_.AABB[1].y = bGeometry_.aabb_.AABB[1].z = -BIGNUMBER;

	for (unsigned int i = 0; i < model_->vertexCount; ++i)
	{
		bGeometry_.aabb_.AABB[0].x = std::min(model_->mVertexData[i].Pos.x, bGeometry_.aabb_.AABB[0].x);
		bGeometry_.aabb_.AABB[0].y = std::min(model_->mVertexData[i].Pos.y, bGeometry_.aabb_.AABB[0].y);
		bGeometry_.aabb_.AABB[0].z = std::min(model_->mVertexData[i].Pos.z, bGeometry_.aabb_.AABB[0].z);

		bGeometry_.aabb_.AABB[1].x = std::max(model_->mVertexData[i].Pos.x, bGeometry_.aabb_.AABB[1].x);
		bGeometry_.aabb_.AABB[1].y = std::max(model_->mVertexData[i].Pos.y, bGeometry_.aabb_.AABB[1].y);
		bGeometry_.aabb_.AABB[1].z = std::max(model_->mVertexData[i].Pos.z, bGeometry_.aabb_.AABB[1].z);
	}

	Vec3 tmp = bGeometry_.aabb_.AABB[1] - bGeometry_.aabb_.AABB[0];

	bGeometry_.aabb_.length_ = Vec3(tmp.x / 2.0f, tmp.y / 2.0f, tmp.z / 2.0f);
	bGeometry_.aabb_.centre_ = Vec3();

	// Constructing bounding sphere.
	Vec3 xmin, xmax, ymin, ymax, zmin, zmax, dia1, dia2, cen;

	xmin.x = ymin.y = zmin.z = BIGNUMBER;
	xmax.x = ymax.y = zmax.z = -BIGNUMBER;

	for (unsigned int i = 0; i < model_->vertexCount; ++i)
	{
		Vec3 tmp = Vec3(model_->mVertexData[i].Pos.x,
						model_->mVertexData[i].Pos.y,
						model_->mVertexData[i].Pos.z);

		if (model_->mVertexData[i].Pos.x < xmin.x)
			xmin = tmp;
		if (model_->mVertexData[i].Pos.x > xmax.x)
			xmax = tmp;

		if (model_->mVertexData[i].Pos.y < ymin.y)
			ymin = tmp;
		if (model_->mVertexData[i].Pos.y > ymax.y)
			ymax = tmp;

		if (model_->mVertexData[i].Pos.z < zmin.z)
			zmin = tmp;
		if (model_->mVertexData[i].Pos.z > zmax.z)
			zmax = tmp;
	}

	float dx, dy, dz, xspan, yspan, zspan, maxspan, rad, rad_sq, old_to_new, old_to_p_sq, old_to_p;

	dx = xmax.x - xmin.x;
	dy = xmax.y - xmin.y;
	dz = xmax.z - xmin.z;
	xspan = dx*dx + dy*dy + dz*dz;

	dx = ymax.x - ymin.x;
	dy = ymax.y - ymin.y;
	dz = ymax.z - ymin.z;
	yspan = dx*dx + dy*dy + dz*dz;

	dx = zmax.x - zmin.x;
	dy = zmax.y - zmin.y;
	dz = zmax.z - zmin.z;
	zspan = dx*dx + dy*dy + dz*dz;

	dia1 = xmin; dia2 = xmax;
	maxspan = xspan;

	cen.x = (dia1.x + dia2.x) / 2.0f;
	cen.y = (dia1.y + dia2.y) / 2.0f;
	cen.z = (dia1.z + dia2.z) / 2.0f;

	dx = dia2.x - cen.x;
	dy = dia2.y - cen.y;
	dz = dia2.z - cen.z;
	rad_sq = dx*dx + dy*dy + dz*dz;
	rad = sqrt(rad_sq);

	for (unsigned int i = 0; i < model_->vertexCount; ++i)
	{
		dx = model_->mVertexData[i].Pos.x - cen.x;
		dy = model_->mVertexData[i].Pos.y - cen.y;
		dz = model_->mVertexData[i].Pos.z - cen.z;
		old_to_p_sq = dx*dx + dy*dy + dz*dz;

		if (old_to_p_sq > rad_sq)
		{
			old_to_p = sqrt(old_to_p_sq);

			float oldRadius = rad;
			rad = (rad + old_to_p) / 2.0f;
			rad_sq = rad*rad;
			old_to_new = old_to_p - rad;

			Vec3 tmp = Vec3(model_->mVertexData[i].Pos.x,
				model_->mVertexData[i].Pos.y,
				model_->mVertexData[i].Pos.z);

			cen += glm::normalize(tmp - cen) * (rad - oldRadius);
		}
	}

    bGeometry_.sphere_.center = cen;
	bGeometry_.sphere_.radius = rad;
	
	// Construct obb.
	Vec3 mu(0.0f, 0.0f, 0.0f), mui;
	gmm::dense_matrix<double> C(3, 3);
	double cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;
	
	for (unsigned i = 0; i < model_->mVertexData.size(); ++i)
	{
		Vec4 p = model_->mVertexData[i].Pos;
		cxx += p.x*p.x - mu.x*mu.x;
		cxy += p.x*p.y - mu.x*mu.y;
		cxz += p.x*p.z - mu.x*mu.z;
		cyy += p.y*p.y - mu.y*mu.y;
		cyz += p.y*p.z - mu.y*mu.z;
		czz += p.z*p.z - mu.z*mu.z;
	}
	
	gmm::dense_matrix<double> Ccovariance(3, 3);
	
	Ccovariance(0, 0) = cxx; Ccovariance(0, 1) = cxy; Ccovariance(0, 2) = cxz;
	Ccovariance(1, 0) = cxy; Ccovariance(1, 1) = cyy; Ccovariance(1, 2) = cyz;
	Ccovariance(2, 0) = cxz; Ccovariance(1, 2) = cyz; Ccovariance(2, 2) = czz;
	
	gmm::dense_matrix<double> eigvec(3, 3);
	std::vector<double> eigval(3);
	
	gmm::symmetric_qr_algorithm(Ccovariance, eigval, eigvec);

	Vec4 r(eigvec(0, 0), eigvec(1, 0), eigvec(2, 0), 0);
	Vec4 u(eigvec(0, 1), eigvec(1, 1), eigvec(2, 1), 0);
	Vec4 f(eigvec(0, 2), eigvec(1, 2), eigvec(2, 2), 0);
	
	bGeometry_.obb_.axis[0].x = r.x; bGeometry_.obb_.axis[0].y = u.x; bGeometry_.obb_.axis[0].z = f.x;
	bGeometry_.obb_.axis[1].x = r.y; bGeometry_.obb_.axis[1].y = u.y; bGeometry_.obb_.axis[1].z = f.y;
	bGeometry_.obb_.axis[2].x = r.z; bGeometry_.obb_.axis[2].y = u.z; bGeometry_.obb_.axis[2].z = f.z;

	Vec3 minim(FLT_MAX, FLT_MAX, FLT_MAX), maxim(FLT_MIN, FLT_MIN, FLT_MIN);
	
	for (unsigned i = 0; i<model_->mIndexData.size(); ++i)
	{
		Vec3 p_prime(glm::dot(r, ((model_->mVertexData)[(model_->mIndexData)[i]]).Pos), glm::dot(u, (model_->mVertexData)[(model_->mIndexData)[i]].Pos), glm::dot(f, (model_->mVertexData)[(model_->mIndexData)[i]].Pos));
		minim.x = std::min(minim.x, p_prime.x);
		minim.y = std::min(minim.y, p_prime.y);
		minim.z = std::min(minim.z, p_prime.z);
		maxim.x = std::max(maxim.x, p_prime.x);
		maxim.y = std::max(maxim.y, p_prime.y);
		maxim.z = std::max(maxim.z, p_prime.z);
	}

	Vec3 center = (maxim + minim) * 0.5f;

	bGeometry_.obb_.center = Vec3(glm::dot(bGeometry_.obb_.axis[0], center), glm::dot(bGeometry_.obb_.axis[1], center), glm::dot(bGeometry_.obb_.axis[2], center));
	bGeometry_.obb_.length = (maxim - minim) * 0.5f;
	bGeometry_.obb_.original_length = (maxim - minim) * 0.5f;

	float max = FLT_MIN;

	bGeometry_.obb_.diff = bGeometry_.obb_.center;
	bGeometry_.obb_.origin[0] = bGeometry_.obb_.axis[0];
	bGeometry_.obb_.origin[1] = bGeometry_.obb_.axis[1];
	bGeometry_.obb_.origin[2] = bGeometry_.obb_.axis[2];

	m_modelCont[model_] = bGeometry_;
}

void PhysicsManager::ConstructBoundingGeometry(ObjectInstance* object_, Physics* physic_)
{
	MContainer::iterator itr = m_modelCont.find(const_cast<Model*>(object_->GetModel()));

	if (itr == m_modelCont.end())
		return;

	Vec4 aabbMin, aabbMax, aabbCentre, aabbLength, sphereCentre;
	Vec3 obb_center, obb_axis_zero, obb_axis_one, obb_axis_two;

	Vec4 tmp(itr->second.aabb_.AABB[0].x, itr->second.aabb_.AABB[0].y, itr->second.aabb_.AABB[0].z, 1.0f);
	aabbMin = object_->GetTransform() * tmp;

	Vec4 tmp2(itr->second.aabb_.AABB[1].x, itr->second.aabb_.AABB[1].y, itr->second.aabb_.AABB[1].z, 1.0f);
	tmp2.x = itr->second.aabb_.AABB[1].x;
	tmp2.y = itr->second.aabb_.AABB[1].y;
	tmp2.z = itr->second.aabb_.AABB[1].z;
	aabbMax = object_->GetTransform() * tmp2;

	Vec4 tmp3(itr->second.aabb_.centre_.x, itr->second.aabb_.centre_.y, itr->second.aabb_.centre_.z, 1.0f);
	tmp3.x = itr->second.aabb_.centre_.x;
	tmp3.y = itr->second.aabb_.centre_.y;
	tmp3.z = itr->second.aabb_.centre_.z;
	aabbCentre = object_->GetTransform() * tmp3;

	Vec4 tmp4(itr->second.aabb_.length_.x, itr->second.aabb_.length_.y, itr->second.aabb_.length_.z, 0.0f);
	tmp4.x = itr->second.aabb_.length_.x;
	tmp4.y = itr->second.aabb_.length_.y;
	tmp4.z = itr->second.aabb_.length_.z;
	aabbLength = object_->GetTransform() * tmp4;

	Vec4 tmpSphere(itr->second.sphere_.center.x, itr->second.sphere_.center.y, itr->second.sphere_.center.z, 1.0f);

	sphereCentre = object_->GetTransform() * tmpSphere;

	float xMat = object_->GetTransform()[0][0];
	float yMat = object_->GetTransform()[1][1];
	float zMat = object_->GetTransform()[2][2];

	float scale = xMat  > yMat ? xMat : yMat;
	scale = yMat > zMat ? yMat  : zMat;
	//obb update
	//Vec4 temp;
	//temp = Vec4(itr->second.obb_.diff.x, itr->second.obb_.diff.y, itr->second.obb_.diff.z, 1);
	//temp = object_->GetTransform()*temp;

	//obb_center = Vec3(temp.x, temp.y, temp.z);

	//temp = Vec4(itr->second.obb_.origin[0].x, itr->second.obb_.origin[1].x, itr->second.obb_.origin[2].x, 0);
	//temp = object_->GetTransform()*temp;

	//obb_axis_zero.x = temp.x; obb_axis_one.x = temp.y; obb_axis_two.x = temp.z;

	//temp = Vec4(itr->second.obb_.origin[0].y, itr->second.obb_.origin[1].y, itr->second.obb_.origin[2].y, 0);
	//temp = object_->GetTransform()*temp;

	//obb_axis_zero.y = temp.x; obb_axis_one.y = temp.y; obb_axis_two.y = temp.z;

	//temp = Vec4(itr->second.obb_.origin[0].z, itr->second.obb_.origin[1].z, itr->second.obb_.origin[2].z, 0);
	//temp = object_->GetTransform()*temp;

	//obb_axis_zero.z = temp.x; obb_axis_one.z = temp.y; obb_axis_two.z = temp.z;
	BoundShape bGeometry;

	bGeometry.aabb_.AABB[0].x = aabbMin.x;
	bGeometry.aabb_.AABB[0].y = aabbMin.y;
	bGeometry.aabb_.AABB[0].z = aabbMin.z;

	bGeometry.aabb_.AABB[1].x = aabbMax.x;
	bGeometry.aabb_.AABB[1].y = aabbMax.y;
	bGeometry.aabb_.AABB[1].z = aabbMax.z;

	bGeometry.aabb_.length_.x = aabbLength.x;
	bGeometry.aabb_.length_.y = aabbLength.y;
	bGeometry.aabb_.length_.z = aabbLength.z;

	bGeometry.sphere_.center.x = sphereCentre.x;
	bGeometry.sphere_.center.y = sphereCentre.y;
	bGeometry.sphere_.center.z = sphereCentre.z;

	bGeometry.sphere_.radius *= scale;

	// Oriented bounding box.
	bGeometry.obb_.length    = itr->second.obb_.length;
	bGeometry.obb_.center    = itr->second.obb_.center;
	bGeometry.obb_.axis[0]   = itr->second.obb_.axis[0];
	bGeometry.obb_.axis[1]   = itr->second.obb_.axis[1];
	bGeometry.obb_.axis[2]   = itr->second.obb_.axis[2];
	bGeometry.obb_.origin[0] = itr->second.obb_.origin[0];
	bGeometry.obb_.origin[1] = itr->second.obb_.origin[1];
	bGeometry.obb_.origin[2] = itr->second.obb_.origin[2];
	bGeometry.obb_.diff      = itr->second.obb_.diff;
	bGeometry.obb_.original_length = itr->second.obb_.original_length;


    physic_->SetBoundingGeometry(bGeometry);
	m_physicsCont[object_] = physic_;
}

void PhysicsManager::UpdatePlayerGeometry()
{
	// Remove once OBJECTMANAGER_INST[0] is fixed.
	PhysicsContainer::iterator player_ = m_physicsCont.find(m_player);

	//PhysicsContainer::iterator player_ = m_physicsCont.find(OBJECTMANAGER_INST[0]);
	
	if (player_ == m_physicsCont.end())
		return;

	MContainer::iterator model_itr = m_modelCont.find(const_cast<Model*>(player_->first->GetModel()));

	if (model_itr == m_modelCont.end())
		return;

	// Update bounding box.
	float xLength = model_itr->second.aabb_.length_.x;
	float yLength = model_itr->second.aabb_.length_.y;
	float zLength = model_itr->second.aabb_.length_.z;

	// Scale bounding box length.
	xLength *= player_->first->GetScaleX();
	yLength *= player_->first->GetScaleY();
	zLength *= player_->first->GetScaleZ();

	Vec3 tmpLength(xLength, yLength, zLength);

	Mat4 rMatrix = glm::transpose(player_->first->GetRotationMatrix());

	float x_len = std::fabs(rMatrix[0][0]) * tmpLength.x +
				  std::fabs(rMatrix[0][1]) * tmpLength.y +
				  std::fabs(rMatrix[0][2]) * tmpLength.z;
	float y_len = std::fabs(rMatrix[1][0]) * tmpLength.x +
				  std::fabs(rMatrix[1][1]) * tmpLength.y +
				  std::fabs(rMatrix[1][2]) * tmpLength.z;
	float z_len = std::fabs(rMatrix[2][0]) * tmpLength.x +
				  std::fabs(rMatrix[2][1]) * tmpLength.y +
				  std::fabs(rMatrix[2][2]) * tmpLength.z;

	Vec3 newLength(x_len, y_len, z_len);

	Vec3 min_ = model_itr->second.aabb_.centre_ +
				player_->first->GetPosition() - newLength;
	Vec3 max_ = model_itr->second.aabb_.centre_ +
				player_->first->GetPosition() + newLength;
	Vec3 centre_ = min_ + newLength;
	float scale_ = player_->first->GetScaleX() >  player_->first->GetScaleY() ?
				   player_->first->GetScaleX() : player_->first->GetScaleY();
	scale_ = player_->first->GetScaleY() >  player_->first->GetScaleZ() ?
			 player_->first->GetScaleY() : player_->first->GetScaleZ();

	float radius_ = model_itr->second.sphere_.radius * scale_ * 0.5f;

	Vec3 sCentre_ = model_itr->second.sphere_.center + player_->first->GetPosition();
	
	BoundShape newGeometry(BoundingBox(max_, min_, centre_, newLength), Sphere(sCentre_, radius_), OBBox());

	player_->second->SetBoundingGeometry(newGeometry);
}

void PhysicsManager::UpdateBoundingGeometry()
{
	if (m_player)
		UpdatePlayerGeometry();

	if (m_updatables.size() > 0)
	{
		std::vector<ObjectInstance*>::iterator itr = m_updatables.begin();

		while (itr != m_updatables.end())
		{
			MContainer::iterator model_itr = m_modelCont.find(const_cast<Model*>((*itr)->GetModel()));

			if (model_itr == m_modelCont.end())
				return;

			Physics* physics_ = GetPhysics(*itr);

			BoundShape newGeometry;

			// Update bounding box.
			if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::BOX)
			{
				float xLength = model_itr->second.aabb_.length_.x;
				float yLength = model_itr->second.aabb_.length_.y;
				float zLength = model_itr->second.aabb_.length_.z;

				// Scale bounding box length.
				xLength *= (*itr)->GetScaleX();
				yLength *= (*itr)->GetScaleY();
				zLength *= (*itr)->GetScaleZ();

				Vec3 tmpLength(xLength, yLength, zLength);

				Mat4 rMatrix = glm::transpose((*itr)->GetRotationMatrix());

				float x_len = std::fabs(rMatrix[0][0]) * tmpLength.x +
							  std::fabs(rMatrix[0][1]) * tmpLength.y +
							  std::fabs(rMatrix[0][2]) * tmpLength.z;
				float y_len = std::fabs(rMatrix[1][0]) * tmpLength.x +
							  std::fabs(rMatrix[1][1]) * tmpLength.y +
							  std::fabs(rMatrix[1][2]) * tmpLength.z;
				float z_len = std::fabs(rMatrix[2][0]) * tmpLength.x +
							  std::fabs(rMatrix[2][1]) * tmpLength.y +
							  std::fabs(rMatrix[2][2]) * tmpLength.z;

				Vec3 newLength(x_len, y_len, z_len);

				Vec3 min_ = model_itr->second.aabb_.centre_ +
							(*itr)->GetPosition() - newLength;
				Vec3 max_ = model_itr->second.aabb_.centre_ +
							(*itr)->GetPosition() + newLength;
				Vec3 centre_ = min_ + newLength;

				newGeometry.SetBoundShape(BoundingBox(max_, min_, centre_, newLength), Sphere(), OBBox());
			}
			else if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::SPHERE) // Update bounding sphere.
			{
				float scale_ = (*itr)->GetScaleX() > (*itr)->GetScaleY() ?
							   (*itr)->GetScaleX() : (*itr)->GetScaleY();
				scale_ = (*itr)->GetScaleY() > (*itr)->GetScaleZ() ?
						 (*itr)->GetScaleY() : (*itr)->GetScaleZ();

				float radius_ = model_itr->second.sphere_.radius * scale_;

				Vec3 sCentre_ = model_itr->second.sphere_.center + (*itr)->GetPosition();

				newGeometry.SetBoundShape(BoundingBox(), Sphere(sCentre_, radius_), OBBox());
			}
			else if (physics_->GetBoundType() == Physics::BOUNDINGTYPE::OBB)
			{
				OBBox temp_obb;
				Vec3 obb_center, obb_axis_zero, obb_axis_one, obb_axis_two , obb_length;
				Vec4 temp;
				temp = Vec4(model_itr->second.obb_.diff.x, model_itr->second.obb_.diff.y, model_itr->second.obb_.diff.z, 1);
				temp = (*itr)->GetTransform()*temp;

				obb_center = Vec3(temp.x, temp.y, temp.z);

				temp = Vec4(model_itr->second.obb_.origin[0].x, model_itr->second.obb_.origin[1].x, model_itr->second.obb_.origin[2].x, 0);
				temp = (*itr)->GetTransform()*temp;

				obb_axis_zero.x = temp.x; obb_axis_one.x = temp.y; obb_axis_two.x = temp.z;

				temp = Vec4(model_itr->second.obb_.origin[0].y, model_itr->second.obb_.origin[1].y, model_itr->second.obb_.origin[2].y, 0);
				temp = (*itr)->GetTransform()*temp;

				obb_axis_zero.y = temp.x; obb_axis_one.y = temp.y; obb_axis_two.y = temp.z;

				temp = Vec4(model_itr->second.obb_.origin[0].z, model_itr->second.obb_.origin[1].z, model_itr->second.obb_.origin[2].z, 0);
				temp = (*itr)->GetTransform()*temp;

				obb_axis_zero.z = temp.x; obb_axis_one.z = temp.y; obb_axis_two.z = temp.z;


				obb_length = model_itr->second.obb_.original_length;
				obb_length.x *= (*itr)->GetScaleX();
				obb_length.y *= (*itr)->GetScaleY();
				obb_length.z *= (*itr)->GetScaleZ();

				temp_obb.length = obb_length;
				temp_obb.center =  obb_center;
				temp_obb.axis[0] = obb_axis_zero;
				temp_obb.axis[1] = obb_axis_one;
				temp_obb.axis[2] = obb_axis_two;
				temp_obb.origin[0] = model_itr->second.obb_.origin[0];
				temp_obb.origin[1] = model_itr->second.obb_.origin[1];
				temp_obb.origin[2] = model_itr->second.obb_.origin[2];
				temp_obb.diff = model_itr->second.obb_.diff;

				newGeometry.SetBoundShape(BoundingBox(), Sphere(), temp_obb);
			}

			GetPhysics((*itr))->SetBoundingGeometry(newGeometry);

			++itr;
		} // End of update loop.
	} // End of updatables.
}

bool PhysicsManager::CollisionDetector(Physics* object_A, Physics* object_B, Manifold& manifold_)
{
	
	Physics::BOUNDINGTYPE type_A = object_A->GetBoundType();
	Physics::BOUNDINGTYPE type_B = object_B->GetBoundType();

	// Sphere vs sphere.
	if (type_A == Physics::BOUNDINGTYPE::SPHERE && 
		type_B == Physics::BOUNDINGTYPE::SPHERE)
	{
		if (SphereCollideSphere(object_A->GetBoundGeometry(), object_B->GetBoundGeometry(), manifold_))
		{
			object_A->SetCollisionFlag(true);
			object_B->SetCollisionFlag(true);

			return true;
		}
		else
			object_B->SetCollisionFlag(false);
	}
	else if (type_A == Physics::BOUNDINGTYPE::BOX &&
			 type_B == Physics::BOUNDINGTYPE::BOX) // Box vs box.
	{
		if (BoxCollideBox(object_A->GetBoundGeometry(), object_B->GetBoundGeometry(), manifold_))
		{
			object_A->SetCollisionFlag(true);
			object_B->SetCollisionFlag(true);

			return true;
		}
		else
			object_B->SetCollisionFlag(false);
	}
	else if ((type_A == Physics::BOUNDINGTYPE::OBB  &&  type_B == Physics::BOUNDINGTYPE::SPHERE)
		|| (type_A == Physics::BOUNDINGTYPE::SPHERE &&  type_B == Physics::BOUNDINGTYPE::OBB))// Sphere vs box OR box vs sphere.
	{
		if ( type_A == Physics::BOUNDINGTYPE::OBB)
		{
			if (SphereCollideOBB(object_B->GetBoundGeometry(), object_A->GetBoundGeometry(), manifold_))
			{
				object_A->SetCollisionFlag(true);
				object_B->SetCollisionFlag(true);

				return true;
			}
			else
				object_B->SetCollisionFlag(false);

		}
		else
		{
			if (SphereCollideOBB(object_A->GetBoundGeometry(), object_B->GetBoundGeometry(), manifold_))
			{
				object_A->SetCollisionFlag(true);
				object_B->SetCollisionFlag(true);

				return true;
			}
			else
				object_B->SetCollisionFlag(false);

		}


	}
	else if ((type_A == Physics::BOUNDINGTYPE::BOX  &&  type_B == Physics::BOUNDINGTYPE::SPHERE) 
		|| (type_A == Physics::BOUNDINGTYPE::SPHERE &&  type_B == Physics::BOUNDINGTYPE::BOX))// Sphere vs box OR box vs sphere.
	{
		if (type_A == Physics::BOUNDINGTYPE::SPHERE)
		{
			if (SphereCollideBox(object_A->GetBoundGeometry(), object_B->GetBoundGeometry(), manifold_))
			{
				object_A->SetCollisionFlag(true);
				object_B->SetCollisionFlag(true);

				return true;
			}
			else
				object_B->SetCollisionFlag(false);
		}
		else
		{
			if (SphereCollideBox(object_B->GetBoundGeometry(), object_A->GetBoundGeometry(), manifold_))
			{
				object_A->SetCollisionFlag(true);
				object_B->SetCollisionFlag(true);

				return true;
			}
			else
				object_B->SetCollisionFlag(false);
		}
	}

	return false;
}

bool PhysicsManager::SphereCollideSphere(const BoundShape& aSphere, const BoundShape& bSphere, Manifold& manifold_)
{
	Vec3 AB(aSphere.sphere_.center - bSphere.sphere_.center);

	float vecLength = static_cast<float>(glm::length(AB));
	float radiuSum = aSphere.sphere_.radius + bSphere.sphere_.radius;

	// Update manifold.
	if (vecLength <= radiuSum)
	{
		manifold_.m_pDistance = bSphere.sphere_.radius - (vecLength - aSphere.sphere_.radius);
		manifold_.m_cNormal = (1.0f / vecLength) * AB;

		return true;
	}
		
	return false;
}

bool PhysicsManager::BoxCollideBox(const BoundShape& aBox, const BoundShape& bBox, Manifold& manifold_)
{
	Vec3 Amax = aBox.aabb_.AABB[1];
	Vec3 Amin = aBox.aabb_.AABB[0];
	Vec3 Bmax = bBox.aabb_.AABB[1];
	Vec3 Bmin = bBox.aabb_.AABB[0];

	if (Amax.x < Bmin.x || Bmax.x < Amin.x ||
		Amax.y < Bmin.y || Bmax.y < Amin.y ||
		Amax.z < Bmin.z || Bmax.z < Amin.z)
		return false;

	// Update manifold.
	Vec3 AB(aBox.aabb_.centre_ - bBox.aabb_.centre_);

	float vecLength = static_cast<float>(glm::length(AB));

	manifold_.m_pDistance = vecLength;
	manifold_.m_cNormal = (1.0f / vecLength) * AB;

	return true;
}

bool PhysicsManager::SphereCollideOBB(const BoundShape& aSphere, const BoundShape& bObb, Manifold& manifold_)
{
	Vec3 sphere_center = aSphere.sphere_.center;
	float sphere_radius = aSphere.sphere_.radius;
	Vec3 X_axis, Y_axis, Z_axis , obb_center, obb_length;
	X_axis.x = bObb.obb_.axis[0].x; Y_axis.x = bObb.obb_.axis[0].y;  Z_axis.x = bObb.obb_.axis[0].z;
	X_axis.y = bObb.obb_.axis[1].x; Y_axis.y = bObb.obb_.axis[1].y;  Z_axis.y = bObb.obb_.axis[1].z;
	X_axis.z = bObb.obb_.axis[2].x; Y_axis.z = bObb.obb_.axis[2].y;  Z_axis.z = bObb.obb_.axis[2].z;
	X_axis = glm::normalize(X_axis);
	Y_axis = glm::normalize(Y_axis);
	Z_axis = glm::normalize(Z_axis);
	obb_center = bObb.obb_.center;
	obb_length = bObb.obb_.length;

	Vec3 centerRelPointToOBB = sphere_center - obb_center;

	Vec3 closestPoint = obb_center;

	float dist = centerRelPointToOBB.x * X_axis.x + centerRelPointToOBB.y * X_axis.y + centerRelPointToOBB.z * X_axis.z;

	if (dist > obb_length.x ) {
		dist = obb_length.x ;
	}
	if (dist < -obb_length.x) {
		dist = -obb_length.x ;
	}

	closestPoint = closestPoint + X_axis * dist;

	dist = centerRelPointToOBB.x * Y_axis.x + centerRelPointToOBB.y * Y_axis.y + centerRelPointToOBB.z * Y_axis.z;

	if (dist > obb_length.y ) {
		dist = obb_length.y ;
	}
	if (dist < -obb_length.y) {
		dist = -obb_length.y ;
	}

	closestPoint = closestPoint + Y_axis * dist;

	dist = centerRelPointToOBB.x * Z_axis.x + centerRelPointToOBB.y * Z_axis.y + centerRelPointToOBB.z * Z_axis.z;

	if (dist > obb_length.z ) {
		dist = obb_length.z;
	}
	if (dist < -obb_length.z ) {
		dist = -obb_length.z ;
	}

	closestPoint = closestPoint + Z_axis * dist;
	

	Vec3 delta = closestPoint - sphere_center;
	float distance = glm::length(delta);
	if (distance < sphere_radius)
		return true;
	else
		return false;
}

bool PhysicsManager::SphereCollideBox(const BoundShape& aSphere, const BoundShape& bBox, Manifold& manifold_)
{
	// Calculate closest point on box to sphere.
	Vec3 point_; float coordinate = 0.0f;

	coordinate = aSphere.sphere_.center.x;
	coordinate = glm::max(coordinate, bBox.aabb_.AABB[0].x);
	coordinate = glm::min(coordinate, bBox.aabb_.AABB[1].x);
	point_.x = coordinate;

	coordinate = aSphere.sphere_.center.y;
	coordinate = glm::max(coordinate, bBox.aabb_.AABB[0].y);
	coordinate = glm::min(coordinate, bBox.aabb_.AABB[1].y);
	point_.y = coordinate;

	coordinate = aSphere.sphere_.center.z;
	coordinate = glm::max(coordinate, bBox.aabb_.AABB[0].z);
	coordinate = glm::min(coordinate, bBox.aabb_.AABB[1].z);
	point_.z = coordinate;

	// Calculate vector from point_ to sphere centre.
	Vec3 vector_ = point_ - aSphere.sphere_.center;

	float vecLength = static_cast<float>(glm::length(vector_));

	if (vecLength < aSphere.sphere_.radius)
	{
		manifold_.m_pDistance = (aSphere.sphere_.radius - vecLength);

		if (vecLength > 0.0f)
			manifold_.m_cNormal = (1.0f / vecLength) * vector_;
		
		return true;
	}
	
	return false;
}

bool PhysicsManager::CollisionPointPlane(Vec3& Point, Vec3& PlaneCenter, Vec3& PlaneNormal)
{
	Vec3 temp(Point.x - PlaneCenter.x, Point.y - PlaneCenter.y, Point.z - PlaneCenter.z);

	float dotProduct = glm::dot(temp, PlaneNormal);

	if (dotProduct > 0) // In front of the plane.
		return false;
	else if (dotProduct == 0) // On the plane.
		return true;

	// Behind the plane.
	return false;
}

void PhysicsManager::CollisionResponse(ObjectInstance* object_A, ObjectInstance* object_B, Manifold& manifold_)
{
	Physics* physics_A = GetPhysics(object_A);
	Physics* physics_B = GetPhysics(object_B);

	Vec3 Vab(object_A->GetVelocity() - object_B->GetVelocity());

	// Vab in the direction of collision normal.
	float Vab_N = (Vab.x * manifold_.m_cNormal.x) + (Vab.y * manifold_.m_cNormal.y) + (Vab.z * manifold_.m_cNormal.z);

	//if (physics_A->GetBoundType() == physics_B->GetBoundType())
		if (Vab_N > 0.0f) // Moving away from each other.
			return;

	float elasticity = glm::min(physics_A->GetResitution(), physics_B->GetResitution());

	float j = -(1.0f + elasticity) * Vab_N;
	j /= ((1.0f / physics_A->GetMass()) + (1.0f / physics_B->GetMass()));

	Vec3 impulse(j * manifold_.m_cNormal), velocity_;

	if (object_A->IsUsingAccuratePhysics())
	{
		if (physics_A->GetPhysicsType()) // A is dynamic - update A.
		{
			velocity_ = object_A->GetVelocity() + (impulse / physics_A->GetMass());

			// B is static - bounce A.
			if (!physics_B->GetPhysicsType())
				object_A->SetVelocity(-velocity_);		
			else // B is dynamic - update B.
			{
				object_A->SetVelocity(velocity_);

				if (object_B->IsUsingAccuratePhysics())
				{
					velocity_ = object_B->GetVelocity() - (impulse / physics_B->GetMass());
					object_B->SetVelocity(velocity_);
		
					// Update change in angular velocity of B.
					physics_B->SetAngularRoll(physics_A->GetInertia() + physics_B->GetAngularRoll());
					physics_B->SetAngularYaw(physics_A->GetInertia() + physics_B->GetAngularYaw());
					physics_B->SetAngularPitch(physics_A->GetInertia() + physics_B->GetAngularPitch());
				}
			}

			// Update change in angular velocity of A.
				physics_A->SetAngularRoll(physics_B->GetInertia() - physics_A->GetAngularRoll());
				physics_A->SetAngularYaw(physics_B->GetInertia() - physics_A->GetAngularYaw());
				physics_A->SetAngularPitch(physics_B->GetInertia() - physics_A->GetAngularPitch());
		}
	}

	if (object_B->IsUsingAccuratePhysics())
	{
		// B is dynamic - bounce B.
		if (physics_B->GetPhysicsType())
		{
			object_B->SetPosition(object_B->GetPosition() - (manifold_.m_cNormal * manifold_.m_pDistance) * 2.0f);
			
			velocity_ = object_B->GetVelocity() - (impulse / physics_B->GetMass()) ;
			object_B->SetVelocity(velocity_);

			// Update change in angular velocity of B.
			physics_B->SetAngularRoll(physics_A->GetInertia() + physics_B->GetAngularRoll());
			physics_B->SetAngularYaw(physics_A->GetInertia() + physics_B->GetAngularYaw());
			physics_B->SetAngularPitch(physics_A->GetInertia() + physics_B->GetAngularPitch());
		}
	}
}

void PhysicsManager::FloatObject(ObjectInstance* object_, float time)
{

}

void PhysicsManager::SetGravityRadius(float radius)
{
	m_gravity_radius = radius;
}

float PhysicsManager::GetGravityRadius()
{
	return m_gravity_radius;
}

bool PhysicsManager::GetPlayerCollided()
{
	return m_playerCollided;
}
