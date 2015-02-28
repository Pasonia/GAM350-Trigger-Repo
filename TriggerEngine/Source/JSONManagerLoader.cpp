/******************************************************************************/
/*!
\file JSONManagerLoader.cpp
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

#include "JSONManagerLoader.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "TriggerAssert.h"
#include "PhysicsManager.h"
#include "ObjectMap.h"
#include "Audio.h"
#include "GUI2D.h"
//#include "FolderManager.h"

#include "Physics.h"
#include "Factory.h"

void LoadModelsFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["Models"].IsArray(), "Error obtaining array of models");

	const rapidjson::Value& _array = doc["Models"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["name"].IsString(), "Error reading name");
		ASSERT(_array[i]["path"].IsString(), "Error reading path");

		ModelManager_Inst->CreateModel(_array[i]["path"].GetString(), _array[i]["name"].GetString());
		Model * model = ModelManager_Inst->GetModelData(_array[i]["name"].GetString());
		if (model)
		{
			PhysicsManager_Inst->RegisterModel(model);
			OBJMAP_INST->AddObjectMap(_array[i]["name"].GetString(), model);
		}

		if (_array[i].HasMember("track"))
		{
			const rapidjson::Value& _tracks = _array[i]["track"];
			for (rapidjson::SizeType j = 0; j < _tracks.Size(); ++j)
			{
				ASSERT(_tracks[j]["start"].IsDouble(), "Error reading start time for this track");
				ASSERT(_tracks[j]["end"].IsDouble(), "Error reading end time for this track");
				model->AddAnimationTrackTimer(static_cast<float>(_tracks[j]["start"].GetDouble()),
											  static_cast<float>(_tracks[j]["end"].GetDouble()));
			}
		}
	}
}
void LoadTexturesFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["Textures"].IsArray(), "Error obtaining array of textures");

	const rapidjson::Value& _array = doc["Textures"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["name"].IsString(), "Error reading name");
		ASSERT(_array[i]["path"].IsString(), "Error reading path");

		TextureManager_Inst->CreateTexture(_array[i]["path"].GetString(), _array[i]["name"].GetString());
		OBJMAP_INST->AddTextureMap(_array[i]["name"].GetString());
	}
}

void LoadUIElementsFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["UIElements"].IsArray(), "Error obtaining array of UI Elements");

	const rapidjson::Value& _array = doc["UIElements"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["name"].IsString(), "Error reading name");
		ASSERT(_array[i]["path"].IsString(), "Error reading path");
		ASSERT(_array[i]["sizeX"].IsInt(), "Error reading size on the X");
		ASSERT(_array[i]["sizeY"].IsInt(), "Error reading size on the Y");
		ASSERT(_array[i]["indexX"].IsInt(), "Error reading index on the X");
		ASSERT(_array[i]["indexY"].IsInt(), "Error reading index on the X");
		IMAGE2D_Inst->Load2DGUIImage(_array[i]["path"].GetString()	, _array[i]["name"].GetString(),
									 _array[i]["sizeX"].GetInt()	, _array[i]["sizeY"].GetInt(),
									 _array[i]["indexX"].GetInt()	, _array[i]["indexY"].GetInt());
	}
}

void LoadObjectInstancesFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["ObjectInstances"].IsArray(), "Error obtaining array of object instances");

	const rapidjson::Value& _array = doc["ObjectInstances"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["model"].IsString(), "Error reading model name");
		ASSERT(_array[i]["texture"].IsString(), "Error reading texture name");

		ObjectInstance * objInst = ObjectManager::GetInstance().CreateObjectInstance(_array[i]["model"].GetString(), _array[i]["texture"].GetString());
		
		if (_array[i].HasMember("type"))
		{
			int type = _array[i]["type"].GetInt();
			objInst->SetType(type);
		}
		//read for position
		ASSERT(_array[i]["position"].IsObject(), "Error reading position object");
		ASSERT(_array[i]["position"]["x"].IsDouble(), "Error reading position x");
		ASSERT(_array[i]["position"]["y"].IsDouble(), "Error reading position y");
		ASSERT(_array[i]["position"]["z"].IsDouble(), "Error reading position z");

		Vec3 translate = Vec3(_array[i]["position"]["x"].GetDouble(),
							  _array[i]["position"]["y"].GetDouble(),
							  _array[i]["position"]["z"].GetDouble());
		objInst->SetPosition(translate);

		//read for rotation
		ASSERT(_array[i]["rotation"].IsObject(), "Error reading rotation object");
		ASSERT(_array[i]["rotation"]["yaw"].IsDouble(), "Error reading rotation yaw");
		ASSERT(_array[i]["rotation"]["pitch"].IsDouble(), "Error reading rotation pitch");
		ASSERT(_array[i]["rotation"]["roll"].IsDouble(), "Error reading rotation roll");

		objInst->SetYaw(static_cast<float>(_array[i]["rotation"]["yaw"].GetDouble()));
		objInst->SetPitch(static_cast<float>(_array[i]["rotation"]["pitch"].GetDouble()));
		objInst->SetRoll(static_cast<float>(_array[i]["rotation"]["roll"].GetDouble()));

		//read for scale
		ASSERT(_array[i]["scale"].IsObject(), "Error reading scale object");
		ASSERT(_array[i]["scale"]["x"].IsDouble(), "Error reading scale x");
		ASSERT(_array[i]["scale"]["y"].IsDouble(), "Error reading scale y");
		ASSERT(_array[i]["scale"]["z"].IsDouble(), "Error reading scale z");

		Vec3 scale = Vec3(_array[i]["scale"]["x"].GetDouble(),
						  _array[i]["scale"]["y"].GetDouble(),
						  _array[i]["scale"]["z"].GetDouble());
		objInst->SetScale(scale.x,scale.y,scale.z);

		if (_array[i].HasMember("render"))
		{
			ASSERT(_array[i]["render"]["wireframe"].IsBool(), "Error reading render wireframe flag");
			ASSERT(_array[i]["render"]["aabb"].IsBool(), "Error reading render aabb flag");
			ASSERT(_array[i]["render"]["highlight"].IsBool(), "Error reading render highlight flag");
			ASSERT(_array[i]["render"]["bs"].IsBool(), "Error reading render bounding sphere flag");

			int flags = 0;
			if (_array[i]["render"]["wireframe"].GetBool())
			{
				flags += RENDER_WIREFRAME;
			}
			if (_array[i]["render"]["aabb"].GetBool())
			{
				flags += RENDER_AABB;
			}
			if (_array[i]["render"]["highlight"].GetBool())
			{
				flags += RENDER_HIGHLIGHT;
			}
			if (_array[i]["render"]["bs"].GetBool())
			{
				flags += RENDER_BS;
			}
			objInst->SetFlag(flags);
		}
		objInst->UpdateTransform();
		if (_array[i].HasMember("components"))
		{
			const rapidjson::Value& _comparray = _array[i]["components"];
			for (rapidjson::SizeType k = 0; k < _comparray.Size(); ++k)
			{
				ASSERT(_array[i]["components"][k]["type"].IsString(), "Error reading scale object");
				std::string str = _array[i]["components"][k]["type"].GetString();
				Component * comp = 0;
				const rapidjson::Value& obj = _array[i]["components"][k];
				if (str.compare("physics") == 0)
				{
					/*
					Physics * physics_comp = new Physics(objInst);
					objInst->AddComponent(physics_comp);
					ASSERT(_array[i]["components"][k]["mass"].IsDouble(), "Error reading physics mass");
					physics_comp->SetMass(static_cast<float>(_array[i]["components"][k]["mass"].GetDouble()));
					ASSERT(_array[i]["components"][k]["restitution"].IsDouble(), "Error reading physics restitution");
					physics_comp->SetResitution(static_cast<float>(_array[i]["components"][k]["restitution"].GetDouble()));
					ASSERT(_array[i]["components"][k]["dynamic"].IsBool(), "Error reading physics dynamic flag");
					physics_comp->SetPhysicsType(_array[i]["components"][k]["dynamic"].GetBool());
					ASSERT(_array[i]["components"][k]["falling"].IsBool(), "Error reading physics falling flag");
					physics_comp->SetFalling(_array[i]["components"][k]["falling"].GetBool());
					ASSERT(_array[i]["components"][k]["boundingtype"].IsInt(), "Error reading physics object type flag");
					physics_comp->SetBoundType(_array[i]["components"][k]["boundingtype"].GetInt());
					*/
					comp = FACTORY_INST->CreateComponent(Component::CT_PHYSICS, &obj, objInst);
				}
				if (str.compare("movingobstacle") == 0)
				{
					comp = FACTORY_INST->CreateComponent(Component::CT_MOVINGOBSTACLE, &obj, objInst);
				}
				if (comp != 0)
				{
					objInst->AddComponent(comp);
				}
			}
		}
		if (_array[i].HasMember("customUVscale"))
		{
			ASSERT(_array[i]["customUVscale"]["x"].IsDouble(), "Error reading scale object");
			ASSERT(_array[i]["customUVscale"]["y"].IsDouble(), "Error reading scale object");
			objInst->SetUV(Vec2(static_cast<float>(_array[i]["customUVscale"]["x"].IsDouble()),
						        static_cast<float>(_array[i]["customUVscale"]["y"].IsDouble())));
		}
		else
		{
			objInst->SetUV(Vec2(objInst->GetScaleX(), objInst->GetScaleY()));
		}
	}
}

void LoadAudiofilesFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["Audio"].IsArray(), "Error obtaining array of object instances");
	const rapidjson::Value& _array = doc["Audio"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["name"].IsString(), "Error reading path name");
		ASSERT(_array[i]["path"].IsString(), "Error reading path name");
		ASSERT(_array[i]["type"].IsString(), "Error reading path name");

		std::string name = _array[i]["name"].GetString();
		std::string filepath = _array[i]["path"].GetString();
		std::string type = _array[i]["type"].GetString();

		if (type.compare("BGM") == 0)
		{
			SOUNDMGR_INST->LoadSound(name, filepath, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, true);
		}
		if (type.compare("SFX") == 0)
		{
			SOUNDMGR_INST->LoadSound(name, filepath, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
		}
	}
}
/*
void LoadPathNamesFromJSON(const std::string& filename)
{
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["PathMap"].IsArray(), "Error obtaining array of object instances");
	const rapidjson::Value& _array = doc["PathMap"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		ASSERT(_array[i]["resource"].IsString(), "Error reading resource name");
		ASSERT(_array[i]["path"].IsString(), "Error reading path name");

		FOLDERMANAGER_INST->AddMap(_array[i]["resource"].GetString(), _array[i]["path"].GetString());
	}
}*/