/******************************************************************************/
/*!
\file FactoryController.cpp
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for FactoryController functions factory class.
*/
/******************************************************************************/
#include "FactoryController.h"
#include "Physics.h"
#include "MovingObstacle.h"
#include "RotatingObstacle.h"
#include "TriggeredObstacle.h"
#include "GravityGateObstacle.h"
#include "LaserObstacle.h"
#include "TreeComponent.h"
#include "Collectables.h"
#include "TriggerAssert.h"
#include "LightOrbs.h"
#include "Enemy_Chaser.h"

Component * PhysicsComponentCreator(const rapidjson::Value * obj, ObjectInstance * parent)
{
	Physics * physics_comp = new Physics(parent);
	ASSERT((*obj)["mass"].IsDouble(), "Error reading physics mass");
	physics_comp->SetMass(static_cast<float>((*obj)["mass"].GetDouble()));
	ASSERT((*obj)["restitution"].IsDouble(), "Error reading physics restitution");
	physics_comp->SetResitution(static_cast<float>((*obj)["restitution"].GetDouble()));
	ASSERT((*obj)["dynamic"].IsBool(), "Error reading physics dynamic flag");
	physics_comp->SetPhysicsType((*obj)["dynamic"].GetBool());
	ASSERT((*obj)["falling"].IsBool(), "Error reading physics falling flag");
	physics_comp->SetFalling((*obj)["falling"].GetBool());
	ASSERT((*obj)["boundingtype"].IsInt(), "Error reading physics object type flag");
	physics_comp->SetBoundType((*obj)["boundingtype"].GetInt());

	return physics_comp;
}

Component * EmptyPhysicsComponentCreator(ObjectInstance * parent)
{
	return new Physics(parent);
}

Component * ArgsCollectableCreator(void * args, ObjectInstance * parent)
{
	ObjectInstance * playerInst = reinterpret_cast<ObjectInstance*>(args);
	Collectable * collectable = new Collectable(parent);
	collectable->SetPlayerInstance(playerInst);
	return collectable;
}

Component * ArgsTriggeredObstacleCreator(void * args, ObjectInstance * parent)
{
	ObjectInstance * playerInst = reinterpret_cast<ObjectInstance*>(args);
	TriggeredObstacle * trigger = new TriggeredObstacle(parent);
	trigger->RegisterPlayer(playerInst);
	return trigger;
}


Component * ArgsLightOrbsCreator(void * args, ObjectInstance * parent)
{
	ObjectInstance * playerInst = reinterpret_cast<ObjectInstance*>(args);
	LightOrbs* orb = new LightOrbs(parent);
	orb->SetPlayerInstance(playerInst);
	return orb;
}

Component * MovingObstacleComponentCreator(const rapidjson::Value * obj, ObjectInstance * parent)
{
	MovingObstacle * moving_comp = new MovingObstacle(parent);
	ASSERT((*obj)["checkpoints"].IsArray(), "Error reading moving obstacle checkpoints");
	const rapidjson::Value& moving_comp_checkpoints = (*obj)["checkpoints"];

	for (rapidjson::SizeType k = 0; k < moving_comp_checkpoints.Size(); ++k)
	{
		ASSERT(moving_comp_checkpoints[k]["x"].IsDouble(), "Error reading moving obstacle checkpoint %d's x component", static_cast<unsigned>(k));
		ASSERT(moving_comp_checkpoints[k]["y"].IsDouble(), "Error reading moving obstacle checkpoint %d's y component", static_cast<unsigned>(k));
		ASSERT(moving_comp_checkpoints[k]["z"].IsDouble(), "Error reading moving obstacle checkpoint %d's z component", static_cast<unsigned>(k));
		ASSERT(moving_comp_checkpoints[k]["time"].IsDouble(), "Error reading moving obstacle checkpoint %d's time component", static_cast<unsigned>(k));
		Vec3 pos(static_cast<float>(moving_comp_checkpoints[k]["x"].GetDouble()),
				static_cast<float>(moving_comp_checkpoints[k]["y"].GetDouble()),
				static_cast<float>(moving_comp_checkpoints[k]["z"].GetDouble()));
		moving_comp->AddCheckpoint(pos, static_cast<float>(moving_comp_checkpoints[k]["time"].GetDouble()));
	}

	return moving_comp;
}

Component * EmptyMovingObstacleComponentCreator(ObjectInstance * parent)
{
	return new MovingObstacle(parent);
}

Component * EmptyRotatingObstacleComponentCreator(ObjectInstance * parent)
{
	return new RotatingObstacle(parent);
}
Component * EmptyTriggeredObstacleComponentCreator(ObjectInstance * parent)
{
	return new TriggeredObstacle(parent);
}

Component * EmptyLaserObstacleComponentCreator(ObjectInstance * parent)
{
	return new LaserObstacle(parent);
}

Component * EmptyGravityGateObstacleComponentCreator(ObjectInstance * parent)
{
	return new GravityGateObstacle(parent);
}

Component * EmptyCollectableComponentCreator(ObjectInstance * parent)
{
	return new Collectable(parent);
}

Component * EmptyTreeComponentCreator(ObjectInstance * parent)
{
	return new Tree(parent);
}

Component * EmptyLightOrbsComponentCreator(ObjectInstance * parent)
{
	return new LightOrbs(parent);
}
Component * EmptyEnemyChaserComponentCreator(ObjectInstance * parent)
{
	return new EnemyChaser(parent);
}

/////////////////////////////////////////////////////////////////////////////////////////

//Sample Component implementation for making specific widgets.

#include "SampleComponent.h"

//1. Widget with no arguments. Straightforward.

Component * EmptySampleComponentCreator(ObjectInstance * parent)
{
	return new SampleComponent(parent);
}

//2. Widget with arguments. Also pretty straightforward.

//The only thing that might be iffy is the arguments.
//I highly recommend that you make structs in the Components' headers that can
//contain ALL related arguments. That way, you can access the arguments here too.

Component * ArgsSampleComponentCreator(void * args, ObjectInstance * parent)
{
	SampleComponent * sample = new SampleComponent(parent);
	int value = *(reinterpret_cast<int*>(args));
	sample->SetValue(value);
	return sample;
}

//3. Widget with arguments taken from JSON file. Less straightforward but should be
//easy to understand nevertheless.

Component * SampleComponentCreator(const rapidjson::Value * obj, ObjectInstance * parent)
{
	SampleComponent * sample = new SampleComponent(parent);
	
	ASSERT((*obj)["value"].IsInt(), "Error reading sample int");
	sample->SetValue((*obj)["value"].GetInt());

	return sample;
}

void RegisterFactoryFunctions()
{
	FACTORY_INST->RegisterWidgetMaker(PhysicsComponentCreator, Component::CT_PHYSICS);
	FACTORY_INST->RegisterWidgetMaker(MovingObstacleComponentCreator, Component::CT_MOVINGOBSTACLE);
	FACTORY_INST->RegisterArgsWidgetMaker(ArgsCollectableCreator, Component::CT_COLLECTABLE);
	FACTORY_INST->RegisterArgsWidgetMaker(ArgsTriggeredObstacleCreator, Component::CT_TRIGGEREDOBSTACLE);
	FACTORY_INST->RegisterArgsWidgetMaker(ArgsLightOrbsCreator, Component::CT_LIGHTORB);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyPhysicsComponentCreator, Component::CT_PHYSICS);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyMovingObstacleComponentCreator, Component::CT_MOVINGOBSTACLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyRotatingObstacleComponentCreator, Component::CT_ROTATINGOBSTACLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyTriggeredObstacleComponentCreator, Component::CT_TRIGGEREDOBSTACLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyLaserObstacleComponentCreator, Component::CT_LASEROBSTACLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyGravityGateObstacleComponentCreator, Component::CT_GRAVITYGATEOBSTACLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyCollectableComponentCreator, Component::CT_COLLECTABLE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyLightOrbsComponentCreator, Component::CT_LIGHTORB);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyTreeComponentCreator, Component::CT_TREE);
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptyEnemyChaserComponentCreator, Component::CT_ENEMYCHASER);
	
	//for sample component:
	FACTORY_INST->RegisterEmptyWidgetMaker(EmptySampleComponentCreator, Component::CT_SAMPLE);
	FACTORY_INST->RegisterArgsWidgetMaker(ArgsSampleComponentCreator, Component::CT_SAMPLE);
	FACTORY_INST->RegisterWidgetMaker(SampleComponentCreator, Component::CT_SAMPLE);
}

//The BETTER implementation would be that this FactoryController simply includes all the
//header files of all components that should be created, and the separate headers have
//their own argument implementations inside their own cpp, but then making factory functions
//over there can make things doubly confusing, so I elected to make this the central repository.
//If you want to debate this implementation or you want to change it, it can be done quickly.