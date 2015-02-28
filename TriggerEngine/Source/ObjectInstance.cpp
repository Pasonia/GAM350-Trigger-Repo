/******************************************************************************/
/*!
\file ObjectInstance.cpp
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

#include "ObjectInstance.h"
#include "System.h"
#include "Physics.h"
#include "PhysicsManager.h"

ObjectInstance::ObjectInstance() : m_flags(0), m_roll(0.f), m_pitch(0.f), m_yaw(0.f), m_transform(Mat4(1.0f))
{
	init();
	float_stat = false;
	go_up = false;
}

ObjectInstance::ObjectInstance(const std::string& objectName, const std::string& textureName)
{
	m_model = ModelManager_Inst->GetModelData(objectName);
	m_texture = TextureManager_Inst->GetTexture(textureName);
	init();
}

void ObjectInstance::init()
{
	m_type = 0;
	m_flags = 0;
	m_animtype = -1;
	m_roll = 0.f;
	m_pitch = 0.f;
	m_yaw = 0.f;
	m_timer = 0.f;
	m_todelete = false;
	m_torender = true;
	m_position = Vec3(0.f, 0.f, 1000.f);
	m_useAccuratePhysics = true;
	m_hasCollidedThisFrame = false;
	m_position = Vec3(0.f, 0.f, 0.f);
	m_velocity = Vec3(0.f, 0.f, 0.f);
	m_scale = Vec3(1.f, 1.f, 1.f);
	m_uv = Vec2(1.f, 1.f);
	UpdateTransform();
}

void ObjectInstance::Update()
{
	size_t size = m_components.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_components[i]->Update();
	}
	m_position += m_velocity * TIMER_INST->GetModifiedDeltaTime();
	if (m_animtype >= 0)
		m_timer += TIMER_INST->GetModifiedDeltaTime();
	UpdateTransform();
	m_hasCollidedThisFrame = false;
}

void ObjectInstance::free()
{
	m_model = 0;
	m_texture = 0;
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
		delete (*i);

	m_components.clear();
}

void ObjectInstance::renderShadow()
{
	if (m_torender && m_model)
	{
		m_model->RenderShadowMap(m_transform);
	}
}

void ObjectInstance::render(bool isDeferred, const int NUM, const Texture *tex)
{
	if (!isDeferred)
	{
		if (NUM == 0)
			m_model->ForwardRender(m_transform, m_texture, m_uv, 0, 0);
		else
			m_model->ForwardRender(m_transform, m_texture, m_uv, NUM, tex);
	}
}

void ObjectInstance::render()
{
	if (m_torender && m_model && m_texture)
	{
		m_model->Render(m_transform, m_texture, m_flags, m_animtype, m_timer, m_uv);
	}
}

void ObjectInstance::render(bool isSkybox, float id, float STATE)
{
	if (isSkybox)
	{
		m_model->RednerSkybox(m_transform, id, STATE);
	}
}

void ObjectInstance::renderOBB()
{
	if (m_torender && (m_flags & RENDER_OBB))
	{
		Physics * phy_comp = PhysicsManager_Inst->GetPhysics(this);
		if (phy_comp)
		{
			Vec3 tscale_ = phy_comp->GetBoundGeometry().obb_.length;

			Mat4 scale_matrix;
			scale_matrix = glm::scale(scale_matrix, Vec3(tscale_.x , tscale_.y , tscale_.z ));

			Mat4 rotation_matrix(1.0f);
			rotation_matrix[0][0] = phy_comp->GetBoundGeometry().obb_.axis[0].x;
			rotation_matrix[0][1] = phy_comp->GetBoundGeometry().obb_.axis[1].x;
			rotation_matrix[0][2] = phy_comp->GetBoundGeometry().obb_.axis[2].x;
			rotation_matrix[1][0] = phy_comp->GetBoundGeometry().obb_.axis[0].y;
			rotation_matrix[1][1] = phy_comp->GetBoundGeometry().obb_.axis[1].y;
			rotation_matrix[1][2] = phy_comp->GetBoundGeometry().obb_.axis[2].y;
			rotation_matrix[2][0] = phy_comp->GetBoundGeometry().obb_.axis[0].z;
			rotation_matrix[2][1] = phy_comp->GetBoundGeometry().obb_.axis[1].z;
			rotation_matrix[2][2] = phy_comp->GetBoundGeometry().obb_.axis[2].z;

			Vec3 pos = phy_comp->GetBoundGeometry().obb_.center;

			Mat4 translation_matrix = glm::translate(pos);

			Mat4 transform = translation_matrix * rotation_matrix;// *scale_matrix;

			ModelManager_Inst->RenderDebugAABB(transform, phy_comp->GetCollisionFlag());
			return;
		}
	}
}

void ObjectInstance::renderAABB()
{
	if (m_torender && (m_flags & RENDER_AABB))
	{
		Physics * phy_comp = PhysicsManager_Inst->GetPhysics(this);
		if (phy_comp)
		{
			//render AABB
			Vec3 tscale_ = phy_comp->GetBoundGeometry().aabb_.length_;

			Mat4 scale_matrix;
			scale_matrix = glm::scale(scale_matrix, Vec3(tscale_.x* 2, tscale_.y * 2, tscale_.z * 2));

			Vec3 pos = m_position;

			Mat4 translation_matrix = glm::translate(pos);

			Mat4 transform = translation_matrix * scale_matrix;

			ModelManager_Inst->RenderDebugAABB(transform, phy_comp->GetCollisionFlag());
		}
	}
}
void ObjectInstance::renderBS()
{
	if (m_torender && (m_flags & RENDER_BS))
	{
		Physics * phy_comp = PhysicsManager_Inst->GetPhysics(this);
		if (phy_comp)
		{
			//render sphere
			float scale = phy_comp->GetBoundGeometry().sphere_.radius;
			Vec3 pos = m_position;

			Mat4 scale_matrix = glm::scale(Vec3(scale));

			Mat4 translation_matrix = glm::translate(pos);

			Mat4 transform = translation_matrix * scale_matrix;

			ModelManager_Inst->RenderDebugBS(transform, phy_comp->GetCollisionFlag());
		}
	}
}
ObjectInstance::~ObjectInstance()
{
	free();
}

union PtrModel
{
	int ptr_num;
	Model * model_ptr;
	Texture * tex_ptr;
};

//Getters
float ObjectInstance::GetRoll() const						{ return m_roll; }
float ObjectInstance::GetYaw() const						{ return m_yaw; }
float ObjectInstance::GetPitch() const						{ return m_pitch; }
float ObjectInstance::GetScaleX() const						{ return m_scale.x; }
float ObjectInstance::GetScaleY() const						{ return m_scale.y; }
float ObjectInstance::GetScaleZ() const						{ return m_scale.z; }
const Vec3& ObjectInstance::GetPosition() const				{ return m_position; }
const Vec3& ObjectInstance::GetVelocity() const				{ return m_velocity; }
const Mat4& ObjectInstance::GetTransform() const			{ return m_transform; }
const Vec3& ObjectInstance::GetScale() const				{ return m_scale; }
int ObjectInstance::GetFlag() const							{ return m_flags;  }
bool ObjectInstance::CheckDelete() const					{ return m_todelete; }

bool ObjectInstance::IsUsingAccuratePhysics()				{ return m_useAccuratePhysics; }
bool ObjectInstance::HasCollidedThisFrame()					{ return m_hasCollidedThisFrame; }
const Model * ObjectInstance::GetModel() const				{ return m_model; }
const int ObjectInstance::GetPtrModel() const				{ PtrModel p; p.model_ptr = m_model; return p.ptr_num; }
const int ObjectInstance::GetPtrTexture() const				{ PtrModel p; p.tex_ptr = m_texture; return p.ptr_num; }
int ObjectInstance::GetType() const							{ return m_type; }

Mat4 ObjectInstance::GetRotationMatrix()
{
	return	
		glm::rotate(Mat4(1.0f), -m_yaw, Vec3(0.f, 1.f, 0.f)) *
		glm::rotate(Mat4(1.0f), m_roll, Vec3(0.f, 0.f, 1.f))*
		glm::rotate(Mat4(1.0f), m_pitch, Vec3(1.f, 0.f, 0.f));
}
//Setters
void ObjectInstance::SetRoll(float roll)					{ m_roll = roll; }
void ObjectInstance::SetYaw(float yaw)						{ m_yaw = yaw; }
void ObjectInstance::SetPitch(float pitch)					{ m_pitch = pitch; }
void ObjectInstance::SetPosition(const Vec3& pos)			{ m_position = pos; }
void ObjectInstance::SetVelocity(const Vec3& vel)			{ m_velocity = vel; }
void ObjectInstance::SetTransform(const Mat4& transform)	{ m_transform = transform; }
void ObjectInstance::SetScale(float x, float y, float z)	{ m_scale = Vec3(x, y, z); }
void ObjectInstance::SetScaleX(float x)						{ m_scale.x = x; }
void ObjectInstance::SetScaleY(float y)						{ m_scale.y = y; }
void ObjectInstance::SetScaleZ(float z)						{ m_scale.z = z; }
void ObjectInstance::SetType(int type)						{ m_type = type; }
void ObjectInstance::SetFlag(int flag = 0)					{ m_flags |= flag; }
void ObjectInstance::UnsetFlag(int flag)					{ m_flags &= ~flag; }
void ObjectInstance::SetDelete()							{ m_todelete = true; }
void ObjectInstance::SetRender(bool render)					{ m_torender = render; }
void ObjectInstance::SetUV(Vec2 uv)							{ m_uv = uv; }
void ObjectInstance::SetModel(const std::string& modelName)     { m_model = ModelManager_Inst->GetModelData(modelName); }
void ObjectInstance::SetTexture(const std::string& textureName) { m_texture = TextureManager_Inst->GetTexture(textureName); }
void ObjectInstance::AddComponent(Component* comp)          { m_components.push_back(comp); }
void ObjectInstance::SetUsingAccuratePhysics(bool use)      { m_useAccuratePhysics = use; }
void ObjectInstance::SetCollidedThisFrame(bool flag)					{ m_hasCollidedThisFrame = flag; }

Component * ObjectInstance::GetComponent(unsigned type)
{
	for (unsigned i = 0; i < m_components.size(); ++i)
	{
		if (m_components[i]->GetComponentID() == type)
			return m_components[i];
	}
	return 0;
}

void ObjectInstance::SetAnim(int anim)						{ m_animtype = anim; m_timer = 0;}
void ObjectInstance::UnsetAnim()							{ m_animtype = -1; m_timer = 0; }

void ObjectInstance::UpdateTransform()
{
	static Mat4 scale;
	static Mat4 rotation;
	static Mat4 translation;

	scale = Mat4(1.f);
	scale[0].x = m_scale.x;
	scale[1].y = m_scale.y;
	scale[2].z = m_scale.z;

	rotation = GetRotationMatrix();

	translation = glm::translate(m_position);

	m_transform = translation * rotation * scale;
}