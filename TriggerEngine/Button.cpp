#include "Button.h"

Button::Button(Vec3 position_, Vec3 scale_, void(*function_)())
{
	m_Box.SetBoundShape(BoundingBox(), Sphere(), OBBox());
	m_Position = position_; m_Scale = scale_;
	m_Clicked = false;
	m_Function = function_;
}

Button::Button(Vec2 position_, Vec2 scale_, void(*function_)())
{
	m_Box.SetBoundShape(BoundingBox(), Sphere(), OBBox());
	m_Position = Vec3(position_.x, position_.y, 0.0f);
	m_Scale = Vec3(scale_.x, scale_.y, 0.0f);
	m_Clicked = false;
	m_Function = function_;
}

Button::~Button() {}

void Button::Update()
{
	m_Function();
}

void Button::SetBoundGeometry(BoundShape bGeometry_)		{ m_Box.SetBoundShape(bGeometry_.aabb_, bGeometry_.sphere_, bGeometry_.obb_); }
void Button::SetClickState(const bool state_)				{ m_Clicked = state_; }
void Button::SetHoverState(const bool state_)				{ m_Hovered = state_; }
void Button::SetCallBack(void(*function_)())				{ m_Function = function_; }

BoundShape Button::GetBoundingGeometry() const				{ return m_Box; }
Vec3 Button::GetPosition() const							{ return m_Position; }
Vec3 Button::GetScale() const								{ return m_Scale; }
bool Button::GetClickState() const							{ return m_Clicked; }
bool Button::GetHoverState() const							{ return m_Hovered; }