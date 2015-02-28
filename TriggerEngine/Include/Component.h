/******************************************************************************/
/*!
\file Component.h
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

#ifndef TRIGGERCOMPONENTS_H_
#define TRIGGERCOMPONENTS_H_

//For implementation of AABB, Physics, etc. inside of ObjectInstances.
//ObjectInstances will include a vector of Component*

class ObjectInstance;

class Component
{
public:
	enum COMPONENT_TYPE
	{
		CT_SAMPLE = 0,
		CT_PHYSICS = 1,
		CT_PLAYERCONTROL,
		CT_MOVINGOBSTACLE,
		CT_GAMETRACKER,
		CT_ROTATINGOBSTACLE,
		CT_TRIGGEREDOBSTACLE,
		CT_GRAVITYGATEOBSTACLE,
		CT_LASEROBSTACLE,
		CT_COLLECTABLE,
		CT_LIGHTORB,
		CT_TREE,
		CT_ENEMYCHASER
	};

	Component();
	Component(ObjectInstance * parent);
	virtual ~Component();
	const int& GetComponentID();
	void SetComponentID(int id);
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Free() = 0;
	ObjectInstance * GetParent();
protected:
	int m_componentID;
	ObjectInstance * m_parent;
};

#endif