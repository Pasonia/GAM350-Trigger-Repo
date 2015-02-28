/******************************************************************************/
/*!
\file Factory.cpp
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
#include "Factory.h"

Factory * Factory::m_instance = 0;

Factory * Factory::GetInstance()
{
	m_instance = m_instance ? m_instance : new Factory();
	return m_instance;
}
void Factory::Destroy()
{
	if (m_instance)
	{
		delete m_instance;
	}
}

Factory::Factory()
{
}
Factory::~Factory()
{
}
Component * Factory::CreateComponent(unsigned type, const rapidjson::Value * obj, ObjectInstance * parent)
{
	if (m_widgetmakers.find(type) == m_widgetmakers.end())
		return 0;
	return m_widgetmakers[type](obj, parent);
}
Component * Factory::CreateComponentArgs(unsigned type, void * args, ObjectInstance * parent)
{
	if (m_argswidgetmakers.find(type) == m_argswidgetmakers.end())
		return 0;
	return m_argswidgetmakers[type](args, parent);
}
Component * Factory::CreateEmptyComponent(unsigned type, ObjectInstance * parent)
{
	if (m_emptywidgetmakers.find(type) == m_emptywidgetmakers.end())
		return 0;
	return m_emptywidgetmakers[type](parent);
}
void Factory::RegisterWidgetMaker(WidgetFunction fn, unsigned type)
{
	if (m_widgetmakers.find(type) == m_widgetmakers.end())
	{
		m_widgetmakers[type] = fn;
	}
}
void Factory::RegisterEmptyWidgetMaker(EmptyWidgetFunction fn, unsigned type)
{
	if (m_emptywidgetmakers.find(type) == m_emptywidgetmakers.end())
	{
		m_emptywidgetmakers[type] = fn;
	}
}

void Factory::RegisterArgsWidgetMaker(ArgsWidgetFunction fn, unsigned type)
{
	if (m_argswidgetmakers.find(type) == m_argswidgetmakers.end())
	{
		m_argswidgetmakers[type] = fn;
	}
}
