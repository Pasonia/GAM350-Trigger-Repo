#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

#include "Physics.h"
#include "ObjectInstance.h"

class Button
{
public:	
	Button() : m_Box(BoundShape()), m_Position(Vec3()), 
			   m_Scale(Vec3()), m_Clicked(false), m_Function(NULL) {}
	Button(Vec3 position_, Vec3 scale_, void(*function_)());
	Button(Vec2 position_, Vec2 scale_, void(*function_)());
	~Button();

	void SetBoundGeometry(BoundShape bGeometry_);
	void SetClickState(const bool state_);
	void SetHoverState(const bool state_);
	void SetCallBack(void(*function_)());

	BoundShape GetBoundingGeometry() const;
	Vec3 GetPosition() const;
	Vec3 GetScale() const;
	bool GetClickState() const;
	bool GetHoverState() const;

	void Update();

private:
	BoundShape m_Box;
	Vec3 m_Position;
	Vec3 m_Scale;
	bool m_Clicked;
	bool m_Hovered;
	void(*m_Function)();
};

#endif

