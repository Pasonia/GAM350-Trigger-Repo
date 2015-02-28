/******************************************************************************/
/*!
\file ButtonManager.cpp
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

#include "ButtonManager.h"
#include"Audio.h"

ButtonManager* ButtonManager::m_instance = 0;

void ButtonManager::CreateButton(const std::string& name_, const std::string& trigger_, 
								 Vec3 position_, Vec3 scale_, void(*function_)(void*))
{
	m_buttonCont[name_] = Button(position_, scale_, function_);
	m_triggerCont[name_] = trigger_;

	ConstructBoundingGeometry(&m_buttonCont[name_]);
}

void ButtonManager::CreateButton(const std::string& name_, const std::string& trigger_,
								 Vec2 position_, Vec2 scale_, void(*function_)(void*))
{
	m_buttonCont[name_] = Button(position_, scale_, function_);
	m_triggerCont[name_] = trigger_;

	ConstructBoundingGeometry(&m_buttonCont[name_]);
}

void ButtonManager::ConstructBoundingGeometry(Button* button_)
{
	float width_ = button_->GetScale().x;
	float height_ = button_->GetScale().y;

	Vec3 position = button_->GetPosition();

	//Vec3 min_(position.x - width_, position.y - height_, 0.0f);
	//Vec3 max_(position.x + width_, position.y + height_, 0.0f);

	//std::cout << "[" << position.x << ", " << position.y << std::endl;

	Vec3 min_(position.x, position.y - height_, 0.0f);
	Vec3 max_(position.x + width_, position.y, 0.0f); 
	Vec3 length_ = 0.5f * (max_ - min_);

	button_->SetBoundGeometry(BoundShape(BoundingBox(max_, min_, position, length_), 
							  Sphere(), OBBox()));
}

void ButtonManager::ConstructBoundingGeometry(const Model* model_)
{
	// Constructing bounding aabb.
	BoundShape bGeometry_;

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
}

void ButtonManager::Update()
{
	ButtonContainer::iterator itr = m_buttonCont.begin();

	while (itr != m_buttonCont.end())
	{
		if (!itr->second.GetHiddenState())
			if (PointCollideAABB(itr->second.GetBoundingGeometry(), m_cursor))
			{
				if (!itr->second.GetHoverState())
				SOUNDMGR_INST->StartPlayingSound("Menuhover");
				itr->second.SetHoverState(true);
				
				if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
				{
					SOUNDMGR_INST->StartPlayingSound("Menuenter");
					itr->second.Update();
				}
			}
			else
				itr->second.SetHoverState(false);

		++itr;
	}
}

void ButtonManager::Render()
{
	ButtonContainer::iterator itr = m_buttonCont.begin();

	while (itr != m_buttonCont.end())
	{
		if (!itr->second.GetHiddenState())
		{
			Vec2 position_(itr->second.GetPosition().x, m_windowSize.y - itr->second.GetPosition().y);
			Vec2 scale_(itr->second.GetScale().x, itr->second.GetScale().y);
		
			IMAGE2D_Inst->Render(itr->first, position_, scale_, 0.0f, false);
		
			if (itr->second.GetHoverState())
				IMAGE2D_Inst->Render(m_triggerCont.find(itr->first)->second,
									 position_, scale_, 0.0f, false);
		}

		++itr;
	}
}

void ButtonManager::Free()
{
	m_buttonCont.clear();
	m_triggerCont.clear();
	Destroy();
}

void ButtonManager::Destroy()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

ButtonManager::ButtonManager() {}

ButtonManager::~ButtonManager() {}

bool ButtonManager::PointCollideAABB(const BoundShape& aBox, const Vec3& bPoint)
{
	if (bPoint.x > aBox.aabb_.AABB[0].x && bPoint.x < aBox.aabb_.AABB[1].x &&
		bPoint.y > aBox.aabb_.AABB[0].y && bPoint.y < aBox.aabb_.AABB[1].y)
		return true;

	return false;
}

ButtonManager* ButtonManager::GetInstance()
{
	m_instance = m_instance ? m_instance : new ButtonManager();

	return m_instance;
}

void ButtonManager::SetHiddenState(const std::string& name_)
{
	ButtonContainer::iterator itr = m_buttonCont.find(name_);

	if (itr != m_buttonCont.end())
		itr->second.SetHiddenState(true);
}

void ButtonManager::SetAppearState(const std::string& name_)
{
	ButtonContainer::iterator itr = m_buttonCont.find(name_);

	if (itr != m_buttonCont.end())
		itr->second.SetHiddenState(false);
}

void ButtonManager::SetAllHidden()
{
	ButtonContainer::iterator itr = m_buttonCont.begin();

	while (itr != m_buttonCont.end())
	{
		itr->second.SetHiddenState(true);

		++itr;
	}
}

void ButtonManager::SetCursorPosition(const Vec3& position_)		{ m_cursor = position_; }
void ButtonManager::SetWindowSize(const Vec3& position_)			{ m_windowSize = position_; }

bool ButtonManager::GetHiddenState(const std::string& name_) 
{
	ButtonContainer::iterator itr = m_buttonCont.find(name_);

	if (itr != m_buttonCont.end())
		return itr->second.GetHiddenState();
	else
		return false;
}