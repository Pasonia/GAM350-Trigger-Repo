/******************************************************************************/
/*!
\file Factory.h
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

#ifndef FACTORY_H_
#define FACTORY_H_

#include "Component.h"
#include "JSONWrapper.h"
#include "ObjectInstance.h"
#include <map>

typedef Component*(*ArgsWidgetFunction)(void *, ObjectInstance*);
typedef Component*(*WidgetFunction)(const rapidjson::Value*, ObjectInstance*);
typedef Component*(*EmptyWidgetFunction)(ObjectInstance*);
//WidgetFunction is a pointer to a function that returns a pointer to a component and takes in a Document and an ObjectInstance

typedef std::map<unsigned, WidgetFunction> WidgetMap;
typedef std::map<unsigned, EmptyWidgetFunction> EmptyWidgetMap;
typedef std::map<unsigned, ArgsWidgetFunction> ArgsWidgetMap;

struct WidgetMaker
{
	WidgetFunction fn;
	unsigned type;
};
struct EmptyWidgetMaker
{
	EmptyWidgetFunction fn;
	unsigned type;
};
struct ArgsWidgetMaker
{
	ArgsWidgetFunction fn;
	unsigned type;
};

class Factory
{
public:
	static Factory * GetInstance();
	static void Destroy();
	Component * CreateComponent(unsigned type, const rapidjson::Value * obj, ObjectInstance * parent);
	Component * CreateComponentArgs(unsigned type, void * args, ObjectInstance * parent);
	Component * CreateEmptyComponent(unsigned type, ObjectInstance * parent);
	void RegisterWidgetMaker(WidgetFunction fn, unsigned type);
	void RegisterArgsWidgetMaker(ArgsWidgetFunction fn, unsigned type);
	void RegisterEmptyWidgetMaker(EmptyWidgetFunction fn, unsigned type);
private:
	Factory();
	~Factory();
	static Factory * m_instance;
	WidgetMap m_widgetmakers;
	EmptyWidgetMap m_emptywidgetmakers;
	ArgsWidgetMap m_argswidgetmakers;
};

#define FACTORY_INST Factory::GetInstance()

template <typename T>
T * CreateComponentFromFactory(unsigned type, const rapidjson::Value * obj, ObjectInstance * parent);

template <typename T>
T * CreateEmptyComponentFromFactory(unsigned type, ObjectInstance * parent);

#include "FactoryTemplate.inl"

#endif