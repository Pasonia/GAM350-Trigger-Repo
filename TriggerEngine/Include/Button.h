/******************************************************************************/
/*!
\file Button.h
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

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

#include "Physics.h"
#include "ObjectInstance.h"

class Button
{
public:	
	Button() : m_Box(BoundShape()), m_Position(Vec3()), 
			   m_Scale(Vec3()), m_Hidden(false), m_Clicked(false), 
			   m_Function(NULL) {}
	Button(Vec3 position_, Vec3 scale_, void(*function_)(void*));
	Button(Vec2 position_, Vec2 scale_, void(*function_)(void*));
	~Button();

	void SetBoundGeometry(BoundShape bGeometry_);
	void SetClickState(const bool state_);
	void SetHoverState(const bool state_);
	void SetCallBack(void(*function_)(void*));
	void SetHiddenState(bool state_);

	BoundShape GetBoundingGeometry() const;
	Vec3 GetPosition() const;
	Vec3 GetScale() const;
	bool GetClickState() const;
	bool GetHoverState() const;
	bool GetHiddenState() const;

	void Update();

private:
	BoundShape m_Box;
	Vec3 m_Position;
	Vec3 m_Scale;
	bool m_Hidden;
	bool m_Clicked;
	bool m_Hovered;
	void(*m_Function)(void*);
};

#endif

