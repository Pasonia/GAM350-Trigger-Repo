/******************************************************************************/
/*!
\file LevelEditor.cpp
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for LevelEditor tool class.
*/
/******************************************************************************/
#include "LevelEditor.h"
#include "TriggerAssert.h"
#include "ObjectManager.h"
//#include "PhysicsManager.h"
#include "Physics.h"
#include "Factory.h"
#include "MovingObstacle.h"
#include "RotatingObstacle.h"
#include "TriggeredObstacle.h"
#include "LaserObstacle.h"
#include "Collectables.h"
#include "GravityGateObstacle.h"

//LevelObject Definitions

LevelObject::LevelObject(int i, int j, int k, char model_type, char texture_type, float scaleX, float scaleY, float scaleZ) :
m_i(i), m_j(j), m_k(k), m_type(model_type), m_texture_type(texture_type), m_scale(scaleX, scaleY, scaleZ), m_instancecreated(false), m_iscollectable(false), m_collectbonustime(0.f), m_collect_type(0), m_collect_score(1000.f)
{
}
LevelObject::LevelObject(LevelObject&& obj) :
m_i(obj.m_i), m_j(obj.m_j), m_k(obj.m_k), m_type(obj.m_type), m_texture_type(obj.m_texture_type), m_scale(obj.m_scale),
m_instance(obj.m_instance), m_instancecreated(obj.m_instancecreated), m_tempcheckpoint(obj.m_tempcheckpoint), m_temprotations(obj.m_temprotations),
m_bounding_type(obj.m_bounding_type), m_gVector(obj.m_gVector), m_gOffset(obj.m_gOffset), m_lSpeed(obj.m_lSpeed), m_angles(obj.m_angles),
m_triggerdist(obj.m_triggerdist), m_temptriggerinfo(obj.m_temptriggerinfo), m_iscollectable(obj.m_iscollectable), m_collectbonustime(obj.m_collectbonustime), m_collect_type(obj.m_collect_type), m_collect_score(obj.m_collect_score)
{
	obj.m_instance = 0;
}
LevelObject::~LevelObject()
{
	if (m_instance && m_instancecreated)
		m_instance->SetDelete();
}
void LevelObject::GetObjectCoordinates(int& i, int&j, int&k)
{
	i = m_i;
	j = m_j;
	k = m_k;
}
char LevelObject::GetModelType() { return m_type; }
char LevelObject::GetTextureType() { return m_texture_type; }
char LevelObject::GetBoundingType() { return m_bounding_type; }
ObjectInstance* LevelObject::GetInstance() { return m_instance; }
void LevelObject::UpdateCheckpoints(Vec3 pos)
{
	MovingObstacle * move_comp = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	move_comp->RemoveAllCheckpoints();
	for (unsigned i = 0; i < m_tempcheckpoint.size(); ++i)
	{
		move_comp->AddCheckpoint(pos + m_tempcheckpoint[i].m_pos, m_tempcheckpoint[i].m_timetoarrive);
	}
}
void LevelObject::UpdateRotations()
{
	RotatingObstacle * rot_comp = reinterpret_cast<RotatingObstacle*>(m_instance->GetComponent(Component::CT_ROTATINGOBSTACLE));
	rot_comp->RemoveAllRotations();
	for (unsigned i = 0; i < m_temprotations.size(); ++i)
	{
		rot_comp->AddRotation(m_temprotations[i].m_pos, m_temprotations[i].m_timetoarrive);
	}
}

void LevelObject::UpdateGravityGate()
{
	GravityGateObstacle * grav_comp = reinterpret_cast<GravityGateObstacle*>(m_instance->GetComponent(Component::CT_GRAVITYGATEOBSTACLE));
	grav_comp->SetOffset(m_gOffset);
	grav_comp->SetVector(m_gVector);
}

void LevelObject::UpdateLaser()
{
	LaserObstacle * laser_comp = reinterpret_cast<LaserObstacle*>(m_instance->GetComponent(Component::CT_LASEROBSTACLE));
	laser_comp->RemoveAllWaypoints();
	for (unsigned i = 0; i < m_angles.size(); ++i)
	{
		laser_comp->AddAngleWaypoint(m_angles[i]);
	}
}
void LevelObject::OffsetCheckpoints(int i, int j, int k)
{
	Vec3 shift(0.f);
	if (i != m_i)
	{
		shift.x += GRIDSIZE * (i - m_i);
	}
	if (j != m_j)
	{
		shift.y += GRIDSIZE * (j - m_j);
	}
	if (k != m_k)
	{
		shift.z += GRIDSIZE * (m_k - k);
	}
	if (shift.length() != 0.f)
	{
		MovingObstacle * move_comp = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
		move_comp->OffsetCheckpoint(shift);
	}
}
void LevelObject::CreateInstance(int rowlength)
{
	m_instance = OBJECTMANAGER_INST.CreateObjectInstance(
		OBJMAP_INST->GetMappedObjectName(m_type),
		OBJMAP_INST->GetMappedTextureName(m_texture_type));
	Vec3 pos = GetGridToWorldWithScale(m_i, m_j, m_k, rowlength, m_scale);
	m_instance->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_instance->SetPosition(pos);
	//obj.GetInstance()->SetFlag(RENDER_AABB | RENDER_BS);
	//m_instance->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, m_instance));
	m_instance->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_MOVINGOBSTACLE, m_instance));
	m_instance->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_ROTATINGOBSTACLE, m_instance));
	m_instance->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TRIGGEREDOBSTACLE, m_instance));
	m_instance->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_COLLECTABLE, m_instance));
	m_instance->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_instance->SetUV(Vec2(1.f,1.f));
	m_instancecreated = true;
	if (m_tempcheckpoint.size() > 0)
		UpdateCheckpoints(pos);
	if (m_temprotations.size() > 0)
		UpdateRotations();
	if (m_temptriggerinfo.size() > 0)
		UpdateTriggerInformation();
	UpdateCollectable();
}
void LevelObject::SetInstance(ObjectInstance * inst, int rowlength)
{
	m_instance = inst;
	m_instance->SetModel(OBJMAP_INST->GetMappedObjectName(m_type));
	m_instance->SetTexture(OBJMAP_INST->GetMappedTextureName(m_texture_type));
	m_instance->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_instance->SetPosition(GetGridToWorldWithScale(m_i, m_j, m_k, rowlength, m_scale));
	m_instance->SetUV(Vec2(m_scale.x/10.f, m_scale.y/10.f));
	MovingObstacle * move_comp = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	move_comp->RemoveAllCheckpoints();
	for (unsigned i = 0; i < m_tempcheckpoint.size(); ++i)
	{
		move_comp->AddCheckpoint(m_tempcheckpoint[i].m_pos, m_tempcheckpoint[i].m_timetoarrive);
	}
	RotatingObstacle * rot_comp = dynamic_cast<RotatingObstacle*>(m_instance->GetComponent(Component::CT_ROTATINGOBSTACLE));
	rot_comp->RemoveAllRotations();
	for (unsigned i = 0; i < m_temprotations.size(); ++i)
	{
		rot_comp->AddRotation(m_temprotations[i].m_pos, m_temprotations[i].m_timetoarrive);
	}
	TriggeredObstacle * trig_comp = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	trig_comp->RemoveAllInformation();
	trig_comp->SetTriggerDistance(m_triggerdist); 
	for (unsigned i = 0; i < m_temptriggerinfo.size(); ++i)
	{
		trig_comp->AddInformation(m_temptriggerinfo[i]);
	}
	Physics * phy_comp = reinterpret_cast<Physics*>(m_instance->GetComponent(Component::CT_PHYSICS));
	phy_comp->SetBoundType(m_bounding_type);
	Collectable * col_comp = dynamic_cast<Collectable*>(m_instance->GetComponent(Component::CT_COLLECTABLE));
	col_comp->SetActive(m_iscollectable); 
	col_comp->SetBonusTime(m_collectbonustime);
	col_comp->SetType(m_collect_type);
	col_comp->SetScore(m_collect_score);
}
void LevelObject::SetModelType(char model)
{
	m_type = model;
	m_instance->SetModel(OBJMAP_INST->GetMappedObjectName(model));
}
void LevelObject::SetgVector(Vec3 gVector)
{
	m_gVector = gVector;
}
Vec3 LevelObject::GetgVector()
{
	return m_gVector;
}
void LevelObject::SetgOffset(float gOffset)
{
	m_gOffset = gOffset;
}
float LevelObject::GetgOffset()
{
	return m_gOffset;
}
float LevelObject::GetlSpeed()
{
	return m_lSpeed;
}
Vec3 LevelObject::GetScale()
{
	return m_scale;
}
void LevelObject::SetlSpeed(float speed)
{
	m_lSpeed = speed;
}
std::vector<Vec2>& LevelObject::GetAngles()
{
	return m_angles;
}
void LevelObject::AddAngle(const Vec2& angle)
{
	m_angles.push_back(angle);
}

void LevelObject::SetTextureType(char texture)
{
	m_texture_type = texture;
	m_instance->SetTexture(OBJMAP_INST->GetMappedTextureName(texture));
}
void LevelObject::SetBoundingType(char bound)
{
	m_bounding_type = bound;
}
void LevelObject::SetScale(const Vec3& scale)
{
	m_scale = scale;
	m_instance->SetScale(scale.x, scale.y, scale.z);
	TriggeredObstacle * trig_comp = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	trig_comp->SetHalfScale(m_scale);
}
void LevelObject::CopyCheckpoints(const LevelObject& source)
{
	m_tempcheckpoint = source.m_tempcheckpoint;
}
void LevelObject::SyncCheckpoints(const LevelObject& source)
{
	MovingObstacle* sourcemove = reinterpret_cast<MovingObstacle*>(source.m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	MovingObstacle* move = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	if (sourcemove && move)
		move->Synchronize(*sourcemove);
}
void LevelObject::SyncRotations(const LevelObject& source)
{
	RotatingObstacle* sourcerot = reinterpret_cast<RotatingObstacle*>(source.m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	RotatingObstacle* rot = reinterpret_cast<RotatingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
	if (sourcerot && rot)
		rot->Synchronize(*sourcerot);
}
void LevelObject::ResetMovement()
{
	if (m_instance && m_tempcheckpoint.size() > 0)
	{
		MovingObstacle* move = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
		if (move)
			move->ResetMovement();
	}
}
void LevelObject::AddNewCheckpoint(Vec3 pos, float time)
{
	m_tempcheckpoint.push_back(TempCheckpoint(pos, time));
	if (m_instance)
	{
		MovingObstacle * move_comp = reinterpret_cast<MovingObstacle*>(m_instance->GetComponent(Component::CT_MOVINGOBSTACLE));
		if (move_comp)
		{
			move_comp->AddCheckpoint(pos, time);
		}
	}
}
void LevelObject::RemoveCheckpoint(unsigned index)
{
	m_tempcheckpoint.erase(m_tempcheckpoint.begin() + index);
}
void LevelObject::SetObjectCoordinates(int i, int j, int k)
{
	m_i = i;
	m_j = j;
	m_k = k;
}

std::vector<TempCheckpoint>& LevelObject::GetCheckPoints()
{
	return m_tempcheckpoint;
}
std::vector<TempCheckpoint>& LevelObject::GetRotations()
{
	return m_temprotations;
}
std::vector<Vec3>& LevelObject::GetTriggerInformation()
{
	return m_temptriggerinfo;
}
bool LevelObject::GetCollectable()
{
	return m_iscollectable;
}
float LevelObject::GetCollectBonusTime()
{
	return m_collectbonustime;
}
float LevelObject::GetCollectScore()
{
	return m_collect_score;
}
int LevelObject::GetCollectType()
{
	return m_collect_type;
}

void LevelObject::AddNewRotation(Vec3 pos, float time)
{
	m_temprotations.push_back(TempCheckpoint(pos, time));
	if (m_instance)
	{
		RotatingObstacle * rot_comp = reinterpret_cast<RotatingObstacle*>(m_instance->GetComponent(Component::CT_ROTATINGOBSTACLE));
		if (rot_comp)
		{
			rot_comp->AddRotation(pos, time);
		}
	}
}
void LevelObject::RemoveRotation(unsigned index)
{

	m_temprotations.erase(m_temprotations.begin() + index);
}
void LevelObject::CopyRotations(const LevelObject& source)
{
	m_temprotations = source.m_temprotations;
}
void LevelObject::SetTriggerDistance(float distance)
{
	m_triggerdist = distance;
}
float LevelObject::GetTriggerDistance()
{
	return m_triggerdist;
}
void LevelObject::AddTriggerInformation(Vec3 information)
{
	m_temptriggerinfo.push_back(information);
}
void LevelObject::RemoveTriggerInformation(unsigned index)
{
	m_temptriggerinfo.erase(m_temptriggerinfo.begin() + index);
}
void LevelObject::CopyTriggerInformation(const LevelObject& source)
{
	m_temptriggerinfo = source.m_temptriggerinfo;
	m_triggerdist = source.m_triggerdist;
}
void LevelObject::SyncTriggerInformation(const LevelObject& source)
{
	TriggeredObstacle* sourcetrig = dynamic_cast<TriggeredObstacle*>(source.m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	TriggeredObstacle* trig = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	if (sourcetrig && trig)
		trig->Synchronize(*sourcetrig);
}
void LevelObject::UpdateTriggerInformation()
{
	TriggeredObstacle * trig_comp = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	trig_comp->RemoveAllInformation();
	for (unsigned i = 0; i < m_temptriggerinfo.size(); ++i)
	{
		trig_comp->AddInformation(m_temptriggerinfo[i]);
	}
	trig_comp->SetHalfScale(m_scale);
}
void LevelObject::RunTrigger()
{
	TriggeredObstacle * trig_comp = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	if (trig_comp)
		trig_comp->SetTriggered();
}
void LevelObject::ResetTrigger()
{
	TriggeredObstacle * trig_comp = dynamic_cast<TriggeredObstacle*>(m_instance->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	if (trig_comp)
		trig_comp->ResetTrigger();
}


void LevelObject::SetCollectable(bool collectable)
{
	m_iscollectable = collectable;
}
void LevelObject::SetCollectBonusTime(float time)
{
	m_collectbonustime = time;
}
void LevelObject::SetCollectType(int type)
{
	m_collect_type = type;
}
void LevelObject::SetCollectScore(float score)
{
	m_collect_score = score;
}
void LevelObject::CopyCollectableInformation(const LevelObject& source)
{
	m_iscollectable = source.m_iscollectable;
	m_collectbonustime = source.m_collectbonustime;
	m_collect_type = source.m_collect_type;
	m_collect_score = source.m_collect_score;
}
void LevelObject::UpdateCollectable()
{
	Collectable * collect_comp = dynamic_cast<Collectable*>(m_instance->GetComponent(Component::CT_COLLECTABLE));
	if (collect_comp)
	{
		collect_comp->SetActive(m_iscollectable);
		collect_comp->SetBonusTime(m_collectbonustime);
		collect_comp->SetType(m_collect_type);
		collect_comp->SetScore(m_collect_score);
	}
}

void SetScale(float scaleX, float scaleY, float scaleZ);

//End LevelObject Definitions

//LevelSegment Definitions

LevelSegment::LevelSegment(const std::string& filename)
{
	LoadGameLevelSegmentFromJSON(filename);
}

//End LevelSegment Definitions

void LevelSegment::CreateInstances()
{
	size_t size = m_objects.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_objects[i].CreateInstance(m_length);
		//m_physics_update_ref.push_back(m_objects[i].GetInstance());
		//Physics * phy = reinterpret_cast<Physics*>(m_objects[i].GetInstance()->GetComponent(Component::CT_PHYSICS));//new Physics(objList[i]);
		//phy->SetBoundType(m_objects[i].GetBoundingType());
		//phy->SetResitution(1.0f);
		//phy->SetMass(1.0f);
	}
}
void LevelSegment::SetInstanceAtIndex(ObjectInstance * inst, int index)
{
	m_objects[index].SetInstance(inst, m_length);
}

void LevelSegment::Update()
{
	bool deleted = true;
	while (deleted)
	{
		deleted = false;
		for (unsigned i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i].GetInstance() && m_objects[i].GetInstance()->CheckDelete())
			{
				std::swap(m_objects[i], m_objects[m_objects.size() - 1]);
				m_objects.pop_back();
				deleted = true;
				break;
			}
		}
	}

	size_t size = m_objects.size();
	//m_physics_update_ref.clear();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_objects[i].GetInstance())
			m_objects[i].GetInstance()->Update();
		//m_physics_update_ref.push_back(m_objects[i].GetInstance());
	}
}

void LevelSegment::RenderShadow()
{
	size_t size = m_objects.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_objects[i].GetInstance())
			m_objects[i].GetInstance()->renderShadow();
	}
}
void LevelSegment::ResetCheckpointMovement()
{
	size_t size = m_objects.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_objects[i].ResetMovement();
	}
}

void LevelSegment::Render()
{
	size_t size = m_objects.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_objects[i].GetInstance())
			m_objects[i].GetInstance()->render();
	}
}
void LevelSegment::RenderDebug()
{
	size_t size = m_objects.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_objects[i].GetBoundingType() == Physics::BOUNDINGTYPE::BOX || m_objects[i].GetBoundingType() == Physics::BOUNDINGTYPE::OBB)
		{
			ModelManager_Inst->RenderDebugAABB(m_objects[i].GetInstance()->GetTransform(), false);
		}
	}
	for (size_t i = 0; i < size; ++i)
	{
		if (m_objects[i].GetBoundingType() == Physics::BOUNDINGTYPE::SPHERE)
		{
			ModelManager_Inst->RenderDebugBS(m_objects[i].GetInstance()->GetTransform(), false);
		}
	}
}


void LevelSegment::SaveFile(const std::string& filename)
{
	SaveGameLevelSegmentToJSON(filename);
}

LevelSegment::~LevelSegment()
{
	m_objects.clear();
}

/*
#define STARTLOOP(vecname) size_t size = vecname.size() \
for (size_t i = 0; i < size; ++i) \
{ \

#define ENDLOOP } \
*/

//unserialize the game segment from file
void LevelSegment::LoadGameLevelSegmentFromJSON(const std::string& filename)
{
	/*this is where it gets tricky. Read here first
	
	I want to do this:
	1. Open the JSON file
	2. Read for the m_length of the grid
	3. Read all objects for its properties as defined in LevelObject.
	4. Using data in LevelObject, make its corresponding ObjectInstance and save it into the vector.

	*/
	/*
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	ASSERT(doc["length"].IsInt(), "Error obtaining segment m_length");
	m_length = doc["length"].GetInt();
	m_difficulty = 0;
	if (doc.HasMember("difficulty") && doc["difficulty"].IsInt())
	{
		m_difficulty = doc["difficulty"].GetInt();
	}
	ASSERT(doc["LevelObjects"].IsArray(), "Error obtaining array of level objects");

	const rapidjson::Value& _array = doc["LevelObjects"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		int x = 0, y = 0, z = 0;
		ASSERT(_array[i].HasMember("x"), "Error reading x of current level object");
		ASSERT(_array[i].HasMember("y"), "Error reading y of current level object");
		ASSERT(_array[i].HasMember("z"), "Error reading z of current level object");
		ASSERT(_array[i].HasMember("modeltype"), "Error reading type of current level object");
		ASSERT(_array[i].HasMember("texturetype"), "Error reading type of current level object");
		ASSERT(_array[i].HasMember("scale"), "Error reading scale");
		ASSERT(_array[i].HasMember("movementtype"), "Error reading type");
		LevelObject obj (_array[i]["x"].GetInt(),
						 _array[i]["y"].GetInt(),
						 _array[i]["z"].GetInt(),
						 OBJMAP_INST->GetIndexToModel(_array[i]["modeltype"].GetString()),
						 OBJMAP_INST->GetIndexToTexture(_array[i]["texturetype"].GetString()),
						 (float)_array[i]["scale"]["x"].GetDouble(),
						 (float)_array[i]["scale"]["y"].GetDouble(),
						 (float)_array[i]["scale"]["z"].GetDouble());
		std::string bstr = _array[i]["boundtype"].GetString();
		if (bstr.compare("AABB") == 0)
		{
			obj.SetBoundingType(Physics::BOUNDINGTYPE::BOX);
		}
		else if (bstr.compare("Sphere") == 0)
		{
			obj.SetBoundingType(Physics::BOUNDINGTYPE::SPHERE);
		}
		else if (bstr.compare("OBB") == 0)
		{
			obj.SetBoundingType(Physics::BOUNDINGTYPE::OBB);
		}

		obj.SetgVector(Vec3((float)_array[i]["gVector"]["x"].GetDouble(), (float)_array[i]["gVector"]["y"].GetDouble(), 0));
		obj.SetgOffset((float)_array[i]["gOffset"].GetDouble());
		obj.SetlSpeed((float)_array[i]["lSpeed"].GetDouble());
		if ((int)_array[i]["lSpeed"].GetDouble())
		{
			ASSERT(_array[i]["angles"].IsArray(), "Error reading laser angles");
			const rapidjson::Value& angles = _array[i]["angles"];

			for (rapidjson::SizeType k = 0; k < angles.Size(); ++k)
			{
				Vec2 angle(static_cast<float>(angles[k]["alpha"].GetDouble()), static_cast<float>(angles[k]["beta"].GetDouble()));
				obj.AddAngle(angle);
			}
		}

		std::string str = _array[i]["movementtype"].GetString();
		if (str.compare("moving") == 0)
		{
			ASSERT(_array[i]["checkpoints"].IsArray(), "Error reading moving obstacle checkpoints");
			const rapidjson::Value& moving_comp_checkpoints = _array[i]["checkpoints"];

			for (rapidjson::SizeType k = 0; k < moving_comp_checkpoints.Size(); ++k)
			{
				//ASSERT(moving_comp_checkpoints[k]["x"].IsDouble(), "Error reading moving obstacle checkpoint %d's x component", static_cast<unsigned>(k));
				//ASSERT(moving_comp_checkpoints[k]["y"].IsDouble(), "Error reading moving obstacle checkpoint %d's y component", static_cast<unsigned>(k));
				//ASSERT(moving_comp_checkpoints[k]["z"].IsDouble(), "Error reading moving obstacle checkpoint %d's z component", static_cast<unsigned>(k));
				//ASSERT(moving_comp_checkpoints[k]["time"].IsDouble(), "Error reading moving obstacle checkpoint %d's time component", static_cast<unsigned>(k));
				Vec3 pos(static_cast<float>(moving_comp_checkpoints[k]["x"].GetDouble()),
					static_cast<float>(moving_comp_checkpoints[k]["y"].GetDouble()),
					static_cast<float>(moving_comp_checkpoints[k]["z"].GetDouble()));
				obj.AddNewCheckpoint(pos, static_cast<float>(moving_comp_checkpoints[k]["time"].GetDouble()));
				//moving_comp->AddCheckpoint(pos, static_cast<float>(moving_comp_checkpoints[k]["time"].GetDouble()));
			}
		}
		if (_array[i].HasMember("rotations") && _array[i]["rotations"].IsArray())
		{
			const rapidjson::Value& rot_comp_checkpoints = _array[i]["rotations"];

			for (rapidjson::SizeType k = 0; k < rot_comp_checkpoints.Size(); ++k)
			{
				Vec3 pos(static_cast<float>(rot_comp_checkpoints[k]["x"].GetDouble()),
					static_cast<float>(rot_comp_checkpoints[k]["y"].GetDouble()),
					static_cast<float>(rot_comp_checkpoints[k]["z"].GetDouble()));
				obj.AddNewRotation(pos, static_cast<float>(rot_comp_checkpoints[k]["time"].GetDouble()));
				//moving_comp->AddCheckpoint(pos, static_cast<float>(moving_comp_checkpoints[k]["time"].GetDouble()));
			}
		}
		if (_array[i].HasMember("triggerinfo"))
		{
			obj.SetTriggerDistance(static_cast<float>(_array[i]["triggerdist"].GetDouble()));
			const rapidjson::Value& trig_comp_checkpoints = _array[i]["triggerinfo"];
			if (trig_comp_checkpoints.Size())
				obj.SetBoundingType(Physics::BOUNDINGTYPE::OBB);
			for (rapidjson::SizeType k = 0; k < trig_comp_checkpoints.Size(); ++k)
			{
				Vec3 info(static_cast<float>(trig_comp_checkpoints[k]["angle"].GetDouble()),
					static_cast<float>(trig_comp_checkpoints[k]["time"].GetDouble()),
					0.f);
				obj.AddTriggerInformation(info);
				//moving_comp->AddCheckpoint(pos, static_cast<float>(moving_comp_checkpoints[k]["time"].GetDouble()));
			}
		}
		if (_array[i].HasMember("collectable"))
		{
			obj.SetCollectable(_array[i]["collectable"].GetBool());
		}
		if (_array[i].HasMember("collectbonustime"))
		{
			obj.SetCollectBonusTime(static_cast<float>(_array[i]["collectbonustime"].GetDouble()));
		}
		if (_array[i].HasMember("collecttype"))
		{
			obj.SetCollectType(_array[i]["collecttype"].GetInt());
		}
		if (_array[i].HasMember("collectscore"))
		{
			obj.SetCollectScore(static_cast<float>(_array[i]["collectscore"].GetDouble()));
		}
		m_objects.push_back(std::move(obj));
	}*/
	rapidjson::Document doc;
	GetJSONObject(&doc, filename);
	Scripter::GetInt(&doc, "difficulty", &m_difficulty);
	ASSERT(doc["LevelObjects"].IsArray(), "Error obtaining array of level objects");

	int index_x, index_z;

	const rapidjson::Value& _array = doc["LevelObjects"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		index_x = _array[i]["x"].GetInt();
		index_z = _array[i]["x"].GetInt();
	}
}
//serialize game segment into file
void LevelSegment::SaveGameLevelSegmentToJSON(const std::string& filename)
{
	/*
	I want to do this:
	1. Create a document object from the Scripter namespace
	2. Add the m_length object for grid, in int.
	3. Add an array for all LevelObjects
	4. Serialize all LevelObject information sequentially (type, ijk and others)
	*/

	rapidjson::Document doc;
	Scripter::InitializeDocument(&doc);

	Scripter::Value _array;
	Scripter::InitializeArray(_array);

	rapidjson::Value jsonlength(m_length);
	doc.AddMember("length", jsonlength, doc.GetAllocator());

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		Scripter::Value obj, scaleobj, vecobj;
		Scripter::InitializeObject(obj);
		Scripter::InitializeObject(scaleobj);
		Scripter::InitializeObject(vecobj);
		int x, y, z;
		m_objects[i].GetObjectCoordinates(x, y, z);
		Vec3 scale = m_objects[i].GetInstance()->GetScale();
		Vec3 vec = m_objects[i].GetgVector();
		Scripter::AppendToJSONObject<int>(obj, x, "x", &doc);
		Scripter::AppendToJSONObject<int>(obj, y, "y", &doc);
		Scripter::AppendToJSONObject<int>(obj, z, "z", &doc);
		Scripter::AppendStringToJSONObject(obj, "modeltype", OBJMAP_INST->GetMappedObjectName(m_objects[i].GetModelType()).c_str(), &doc);
		Scripter::AppendStringToJSONObject(obj, "texturetype", OBJMAP_INST->GetMappedTextureName(m_objects[i].GetTextureType()).c_str(), &doc);
		Scripter::AppendStringToJSONObject(obj, "boundtype", m_objects[i].GetBoundingType() == Physics::BOUNDINGTYPE::BOX ? "AABB" : "Sphere", &doc); 
		Scripter::AppendToJSONObject<float>(scaleobj, scale.x, "x", &doc);
		Scripter::AppendToJSONObject<float>(scaleobj, scale.y, "y", &doc);
		Scripter::AppendToJSONObject<float>(scaleobj, scale.z, "z", &doc);
		Scripter::AppendJSONObjectToJSONObject(scaleobj, obj, "scale", &doc);
		Scripter::AppendToJSONObject<float>(vecobj, vec.x, "x", &doc);
		Scripter::AppendToJSONObject<float>(vecobj, vec.y, "y", &doc);
		Scripter::AppendJSONObjectToJSONObject(vecobj, obj, "gVector", &doc);
		Scripter::AppendToJSONObject<float>(obj, m_objects[i].GetgOffset(), "gOffset", &doc);
		Scripter::AppendToJSONObject<float>(obj, m_objects[i].GetlSpeed(), "lSpeed", &doc);

		if (!m_objects[i].GetAngles().empty())
		{
			Scripter::Value _angles;
			Scripter::InitializeArray(_angles);

			for (unsigned j = 0; j < m_objects[i].GetAngles().size(); ++j)
			{
				Scripter::Value CP;
				Scripter::InitializeObject(CP);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetAngles()[j].x, "alpha", &doc);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetAngles()[j].y, "beta", &doc);
				_angles.PushBack(CP, doc.GetAllocator());
			}
			Scripter::AppendJSONObjectToJSONObject(_angles, obj, "angles", &doc);
		}

		if (!m_objects[i].GetCheckPoints().empty())
		{
			Scripter::AppendStringToJSONObject(obj, "movementtype", "moving", &doc);
			Scripter::Value _checkpoints;
			Scripter::InitializeArray(_checkpoints);

			for (unsigned j = 0; j < m_objects[i].GetCheckPoints().size(); ++j)
			{
				Scripter::Value CP;
				Scripter::InitializeObject(CP);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetCheckPoints()[j].m_pos.x, "x", &doc);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetCheckPoints()[j].m_pos.y, "y", &doc);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetCheckPoints()[j].m_pos.z, "z", &doc);
				Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetCheckPoints()[j].m_timetoarrive, "time", &doc);
				_checkpoints.PushBack(CP, doc.GetAllocator());
			}
			Scripter::AppendJSONObjectToJSONObject(_checkpoints, obj, "checkpoints", &doc);
		}
		else
		{
			Scripter::AppendStringToJSONObject(obj, "movementtype", "static", &doc);
		}

		Scripter::Value _rotations;
		Scripter::InitializeArray(_rotations);

		for (unsigned j = 0; j < m_objects[i].GetRotations().size(); ++j)
		{
			Scripter::Value CP;
			Scripter::InitializeObject(CP);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetRotations()[j].m_pos.x, "x", &doc);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetRotations()[j].m_pos.y, "y", &doc);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetRotations()[j].m_pos.z, "z", &doc);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetRotations()[j].m_timetoarrive, "time", &doc);
			_rotations.PushBack(CP, doc.GetAllocator());
		}
		Scripter::AppendJSONObjectToJSONObject(_rotations, obj, "rotations", &doc);

		Scripter::Value _triggers;
		Scripter::InitializeArray(_triggers);

		for (unsigned j = 0; j < m_objects[i].GetTriggerInformation().size(); ++j)
		{
			Scripter::Value CP;
			Scripter::InitializeObject(CP);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetTriggerInformation()[j].x, "angle", &doc);
			Scripter::AppendToJSONObject<float>(CP, (float)m_objects[i].GetTriggerInformation()[j].y, "time", &doc);
			_triggers.PushBack(CP, doc.GetAllocator());
		}
		Scripter::AppendToJSONObject<int>(obj, static_cast<int>(m_objects[i].GetTriggerDistance()), "triggerdist", &doc);
		Scripter::AppendJSONObjectToJSONObject(_triggers, obj, "triggerinfo", &doc);
		Scripter::AppendToJSONObject<bool>(obj, (m_objects[i].GetCollectable()), "collectable", &doc);
		Scripter::AppendToJSONObject<float>(obj, static_cast<float>(m_objects[i].GetCollectBonusTime()), "collectbonustime", &doc);
		Scripter::AppendToJSONObject<int>(obj, static_cast<int>(m_objects[i].GetCollectType()), "collecttype", &doc);
		Scripter::AppendToJSONObject<float>(obj, static_cast<float>(m_objects[i].GetCollectScore()), "collectscore", &doc);
		_array.PushBack(obj,doc.GetAllocator());
	}
	doc.AddMember("LevelObjects", _array, doc.GetAllocator());
	Scripter::ExportDocument(&doc, filename.c_str());
	/*
	rapidjson::Document doc;
	Scripter::InitializeDocument(&doc);
	rapidjson::Value jsonlength(m_length);
	doc.AddMember("m_length", jsonlength,doc.GetAllocator());

	rapidjson::Value _array;
	_array.SetArray();
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		int _i, _j, _k;
		m_objects[i].GetObjectCoordinates(_i, _j, _k);
		rapidjson::Document current_levelobj;
		current_levelobj.SetObject();
		rapidjson::Value x(_i), y(_j), z(_k),
						modeltype(OBJMAP_INST->GetMappedObjectName(m_objects[i].GetModelType()).c_str()), 
						texturetype(OBJMAP_INST->GetMappedObjectName(m_objects[i].GetModelType()).c_str());
		current_levelobj.AddMember("x", x, current_levelobj.GetAllocator());
		current_levelobj.AddMember("y", x, current_levelobj.GetAllocator());
		current_levelobj.AddMember("z", x, current_levelobj.GetAllocator());
		current_levelobj.AddMember("modeltype", modeltype, current_levelobj.GetAllocator());
		current_levelobj.AddMember("texturetype", texturetype, current_levelobj.GetAllocator());
		_array.PushBack(current_levelobj, doc.GetAllocator());
	}

	doc.AddMember("LevelObjects", _array, doc.GetAllocator());
	*/
}

void LevelSegment::AddLevelObject(int x, int y, int z, 
								  const std::string& modelname, const std::string& texturename, int boundType, 
								  float scaleX, float scaleY, float scaleZ, 
								  Vec3 gVector, float gOffset)
{
	LevelObject obj(x, y, z, OBJMAP_INST->GetIndexToModel(modelname), OBJMAP_INST->GetIndexToTexture(texturename), scaleX, scaleY, scaleZ);
	obj.CreateInstance(m_length);
	obj.GetObjectCoordinates(x, y, z);
	obj.GetInstance()->SetPosition(GetGridToWorldWithScale(x, y, z, m_length,obj.GetInstance()->GetScale()));
	//obj.GetInstance()->SetFlag(RENDER_AABB);
	obj.GetInstance()->UpdateTransform();
	//obj.GetInstance()->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, obj.GetInstance()));
	//Physics * phy_comp = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, obj.GetInstance());
	obj.SetBoundingType(boundType);
	obj.SetgVector(gVector);
	obj.SetgOffset(gOffset);
	obj.SetlSpeed(0);
	//phy_comp->SetBoundType(boundType);
	//phy_comp->SetResitution(1.0f);
	//phy_comp->SetMass(1.0f);
	//obj.GetInstance()->AddComponent(reinterpret_cast<Component*>(phy_comp));

	m_objects.push_back(std::move(obj));
}


LevelObject& LevelSegment::GetLevelObject(int index)
{
	return m_objects[index];
}

int LevelSegment::GetAmountofObjects() const
{
	return m_objects.size();
}
std::vector<ObjectInstance*>& LevelSegment::GetInstances()
{
	return m_physics_update_ref;
}

int LevelSegment::GetLength() const		{ return m_length; }
int LevelSegment::GetDifficulty() const { return m_difficulty; }

void LevelSegment::SetLength(int length_new)
{
	m_length = length_new;
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		int _i, _j, _k;
		m_objects[i].GetObjectCoordinates(_i, _j, _k);
		if (_k >= m_length)
		{
			m_objects[i].GetInstance()->SetDelete();
		}
		else
		{
			m_objects[i].GetInstance()->SetPosition(GetGridToWorld(_i, _j, _k, m_length));
		}
	}
}

//End LevelSegment Definitions

//LevelEditor Definitions

void LevelEditor::Update()
{
	//update objects in the current active segment
	m_segments[m_curr_active_segment]->Update();
}

void LevelEditor::Render()
{
	//render objects in the current active segment
	m_segments[m_curr_active_segment]->Render();
	if (m_debugdraw)
		m_segments[m_curr_active_segment]->RenderDebug();
}

int LevelEditor::GetNumberOfSegmentsLoaded()				{ return m_number_of_segments;  }
LevelSegment* LevelEditor::GetCurrentSegment()				{ return m_segments[m_curr_active_segment]; }
int LevelEditor::GetCurrentSegmentLength() const			{ return m_segments[m_curr_active_segment]->GetLength(); }
void LevelEditor::SetCurrentSegmentLength(int length_new)	{ m_segments[m_curr_active_segment]->SetLength(length_new); }
void LevelEditor::AddGameLevelSegment(const std::string& filename)
{
	//call constructor and push back immediately
	m_segments.push_back(new LevelSegment(filename));
	m_segments.back()->CreateInstances(); //Level Editor calls create on object instances because instances are managed locally in leveleditor.
	++m_number_of_segments;
}
void LevelEditor::SaveGameLevelSegment(const std::string& filename)
{
	//call the save file function on the current segment
	m_segments[m_curr_active_segment]->SaveFile(filename); 
}
void LevelEditor::AddObjectToCurrentSegment(int x, int y, int z, const 
											std::string& modelname, const std::string& texturename, int boundType, 
											float scaleX, float scaleY, float scaleZ, Vec3 gVector, float gOffset)
{
	m_segments[m_curr_active_segment]->AddLevelObject(x, y, z, modelname, texturename, boundType, scaleX, scaleY, scaleZ, gVector, gOffset);
}

LevelEditor::LevelEditor() : m_curr_active_segment(0), m_debugdraw(false)
{
}
LevelEditor::~LevelEditor()
{
}

void LevelEditor::MoveToPreviousSegment()
{
	--m_curr_active_segment;
	if (m_curr_active_segment < 0)
		m_curr_active_segment = m_number_of_segments - 1;
}
void LevelEditor::MoveToNextSegment()
{
	++m_curr_active_segment;
	if (m_curr_active_segment == m_number_of_segments)
		m_curr_active_segment = 0;
}
void LevelEditor::RemoveAllSegments()
{
	for (auto elem : m_segments)
	{
		delete elem;
	}
		
	m_segments.clear();
	m_number_of_segments = 0;
	m_curr_active_segment = 0;
}
void LevelEditor::ToggleDebug()
{
	m_debugdraw = !m_debugdraw;
	for (unsigned i = 0; i < m_segments.size(); ++i)
		m_segments[i]->ToggleDebug(m_debugdraw);
}
void LevelSegment::ToggleDebug(bool debugdraw)
{
	for (unsigned i = 0; i < m_objects.size(); ++i)
		m_objects[i].ToggleDebug(debugdraw);
}
void LevelObject::ToggleDebug(bool debugdraw)
{
	if (m_instance)
	{
		int flag;
		switch (GetBoundingType())
		{
		case Physics::BOUNDINGTYPE::BOX:
		case Physics::BOUNDINGTYPE::OBB:	flag = RENDER_AABB; break;
		case Physics::BOUNDINGTYPE::SPHERE: flag = RENDER_BS; break;
		}
		if (debugdraw) m_instance->SetFlag(flag);
		else m_instance->UnsetFlag(flag);
	}
}

const std::vector<std::string> LevelEditor::GetModelNames() const
{
	std::vector<std::string> result;
	for (int i = 0; i < OBJMAP_INST->GetObjectMapSize(); ++i)
	{
		result.push_back(OBJMAP_INST->GetMappedObjectName(i));
	}
	return result;
}

const std::vector<std::string> LevelEditor::GetTextureNames() const
{
	std::vector<std::string> result;
	for (int i = 0; i < OBJMAP_INST->GetTextureMapSize(); ++i)
	{
		result.push_back(OBJMAP_INST->GetMappedTextureName(i));
	}
	return result;
}

//End LevelEditor Definitions