/******************************************************************************/
/*!
\file ObjectInstance.h
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
#ifndef OBJINST_H_
#define OBJINST_H_

#include "TEMath.h"
#include "Model.h"
#include <vector>
#include <string>
#include "Component.h"
#include "ModelManager.h"

class ObjectInstance
{
public:
	ObjectInstance();
	ObjectInstance(const std::string& objectName, const std::string& textureName);

	void render(bool isDeferred, const int NUM, const Texture *tex);
	void render(bool isSkybox, float id,float STATE);
	void render();
	void renderShadow();

	void init();
	void free();
	~ObjectInstance();

	void renderAABB();
	void renderBS();
	void renderOBB();
	//Getters
	float GetRoll() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetScaleX() const;
	float GetScaleY() const;
	float GetScaleZ() const;
	const Vec3& GetPosition() const;
	const Vec3& GetVelocity() const;
	const Vec3& GetScale() const;
	const Mat4& GetTransform() const;
	
	int GetFlag() const;
	int GetType() const;
	bool CheckDelete() const;

	bool IsUsingAccuratePhysics();
	void SetUsingAccuratePhysics(bool use);
	void SetCollidedThisFrame(bool flag);
	bool HasCollidedThisFrame();

	const Model * GetModel() const;
	const int GetPtrModel() const;
	const int GetPtrTexture() const;
	Mat4 GetRotationMatrix();

	//Setters
	void SetRoll(float roll);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetPosition(const Vec3& pos);
	void SetVelocity(const Vec3& vel);
	void SetTransform(const Mat4& transform);
	void SetScale(float x, float y, float z);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);
	void SetType(int type);
	void SetFlag(int flag);
	void UnsetFlag(int flag);
	void SetDelete();
	void SetAnim(int anim);
	void UnsetAnim();
	void SetRender(bool render);
	void SetUV(Vec2 uv);

	void SetModel(const std::string& modelName);
	void SetTexture(const std::string& textureName);

	void AddComponent(Component* comp);
	Component * GetComponent(unsigned type);

	void Update();
	void UpdateTransform();
	Vec3 LastSpeed;
	float timer;
	bool  float_stat;
	bool  go_up;
private:
	Model * m_model;
	Texture * m_texture;
	int m_type;
	int m_flags; //flags
	int m_animtype;
	//rotations
	float m_roll;
	float m_yaw;
	float m_pitch;
	float m_timer;
	bool m_todelete;
	bool m_torender;
	bool m_useAccuratePhysics;
	bool m_hasCollidedThisFrame;
	Vec3 m_position;
	Vec3 m_velocity;
	Vec3 m_scale;
	Mat4 m_transform;
	Vec2 m_uv;
	std::vector<Component*> m_components;
};

#endif