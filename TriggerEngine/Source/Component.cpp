/******************************************************************************/
/*!
\file Component.cpp
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
#include "Component.h"

Component::Component(ObjectInstance * parent)
: m_parent(parent), m_componentID(0)
{
}

Component::~Component()
{
}

const int& Component::GetComponentID()
{
	return m_componentID;
}

ObjectInstance * Component::GetParent()
{
	return m_parent;
}

void Component::SetComponentID(int id) { m_componentID = id; }