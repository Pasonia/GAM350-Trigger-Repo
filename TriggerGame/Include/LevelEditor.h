/******************************************************************************/
/*!
\file LevelEditor.h
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for LevelEditor tool class.
*/
/******************************************************************************/
#ifndef LEVELEDITOR_H_
#define LEVELEDITOR_H_

#include "JSONWrapper.h"
#include "FolderManager.h"
#include "ObjectInstance.h"
#include "ObjectMap.h"

#define GRIDSIZE 10.f

template<typename T> Vec3 GetGridToWorld(T i, T j, T k, int rows)
{
	return Vec3((i - 37) * GRIDSIZE, j * GRIDSIZE, (static_cast<float>((rows / 2) - k) - (rows % 2 ? 0.f : 0.5f)) * GRIDSIZE);
}
template<typename T> Vec3 GetGridToWorldWithScale(T i, T j, T k, int rows, const Vec3& scale)
{
	Vec3 initial_vec = GetGridToWorld(i, j, k, rows);
	initial_vec.y += scale.y * 0.5f;
	return initial_vec;
}

struct ObjectInstInfo
{
	int m_cellIndex[3];
	std::string m_textureType;
	std::string m_modelType;
	Vec3 scale;
};

struct TempCheckpoint
{
	Vec3 m_pos;
	float m_timetoarrive;
	TempCheckpoint(Vec3 pos, float time):
	m_pos(pos),
	m_timetoarrive(time)
	{
	}
};

class LevelObject
{
public:
	LevelObject(int i, int j, int k, char modeltype, char texturetype, float scaleX, float scaleY, float scaleZ);
	LevelObject(LevelObject&& obj);
	void GetObjectCoordinates(int& i, int&j, int&k);
	char GetModelType();
	char GetTextureType();
	char GetBoundingType();
	void SetObjectCoordinates(int i, int j, int k);
	void CreateInstance(int rowlength);
	void SetInstance(ObjectInstance * inst, int rowlength);
	void SetModelType(char model);
	void SetTextureType(char texture);
	void SetBoundingType(char bound);
	void SetScale(const Vec3& scale);

	// hack start
	void SetgVector(Vec3 gVec);
	Vec3 GetgVector();
	void SetgOffset(float gOffset);
	void UpdateGravityGate();
	float GetgOffset();
	float GetlSpeed();
	Vec3 GetScale();
	void SetlSpeed(float speed);
	std::vector<Vec2>& GetAngles();
	void AddAngle(const Vec2& angle);
	void UpdateLaser();
	// hack end


	void AddNewCheckpoint(Vec3 pos, float time);
	void RemoveCheckpoint(unsigned index);
	void CopyCheckpoints(const LevelObject& source);
	void SyncCheckpoints(const LevelObject& source);
	void UpdateCheckpoints(Vec3 pos);
	void OffsetCheckpoints(int i, int j, int k);

	void AddNewRotation(Vec3 rot, float time);
	void RemoveRotation(unsigned index);
	void CopyRotations(const LevelObject& source);
	void SyncRotations(const LevelObject& source);
	void UpdateRotations();

	void SetTriggerDistance(float time);
	void AddTriggerInformation(Vec3 information);
	void RemoveTriggerInformation(unsigned index);
	void CopyTriggerInformation(const LevelObject& source);
	void SyncTriggerInformation(const LevelObject& source);
	void UpdateTriggerInformation();
	void RunTrigger();
	void ResetTrigger();

	void SetCollectable(bool collectable);
	void SetCollectBonusTime(float time);
	void SetCollectType(int type);
	void SetCollectScore(float score);
	void CopyCollectableInformation(const LevelObject& source);
	void UpdateCollectable();

	void ToggleDebug(bool drawdebug);

	void ResetMovement();
	std::vector<TempCheckpoint>& GetCheckPoints();
	std::vector<TempCheckpoint>& GetRotations();
	std::vector<Vec3>& GetTriggerInformation();
	float GetTriggerDistance();
	bool GetCollectable();
	float GetCollectBonusTime();
	int GetCollectType();
	float GetCollectScore();
	ObjectInstance* GetInstance();
	~LevelObject();
private:
	char m_type, m_texture_type, m_bounding_type;
	int m_i, m_j, m_k;
	Vec3 m_scale;
	bool m_instancecreated;

	// hack start
	Vec3 m_gVector;
	float m_gOffset;
	float m_lSpeed;
	std::vector<Vec2> m_angles;
	// hack end

	//CP
	std::vector<TempCheckpoint> m_tempcheckpoint;
	//Rotations
	std::vector<TempCheckpoint> m_temprotations;
	//Trigger
	float m_triggerdist;
	std::vector<Vec3> m_temptriggerinfo;
	//Collectable
	bool m_iscollectable;
	float m_collectbonustime;
	int m_collect_type;
	float m_collect_score;

	ObjectInstance* m_instance;
};

class LevelSegment
{
public:
	LevelSegment(const std::string& filename);
	void CreateInstances();
	void SetInstanceAtIndex(ObjectInstance * inst, int index);
	void Update();
	void Render();
	void RenderDebug();
	void RenderShadow();
	void ResetCheckpointMovement();
	void SaveFile(const std::string& filename);
	int GetLength() const;
	int GetDifficulty() const;
	void SetLength(int length_new);
	void AddLevelObject(int x, int y, int z, 
						const std::string& modelname, const std::string& texturename, int boundType, 
						float scaleX, float scaleY, float scaleZ, 
						Vec3 gVector, float gOffset);
	LevelObject& GetLevelObject(int index);
	int GetAmountofObjects() const;
	std::vector<ObjectInstance*>& GetInstances();
	void ToggleDebug(bool drawdebug);
	~LevelSegment();
private:
	int m_difficulty;
	int m_length;
	std::vector<LevelObject> m_objects;
	std::vector<ObjectInstance*> m_physics_update_ref;
private:
	void LoadGameLevelSegmentFromJSON(const std::string& filename);
	void SaveGameLevelSegmentToJSON(const std::string& filename);
};

class LevelEditor
{
public:
	LevelEditor();
	void Update();
	void Render();
	void RenderShadow();
	int GetNumberOfSegmentsLoaded();
	LevelSegment* GetCurrentSegment();
	const std::vector<std::string> GetModelNames() const;
	const std::vector<std::string> GetTextureNames() const;
	int GetCurrentSegmentLength() const;
	void SetCurrentSegmentLength(int length_new);
	void AddGameLevelSegment(const std::string& filename);
	void SaveGameLevelSegment(const std::string& filename);
	void AddObjectToCurrentSegment(int x, int y, int z, 
								   const std::string& modelname, const std::string& texturename, int boundType, 
								   float scaleX, float scaleY, float scaleZ, 
								   Vec3 gVector, float gOffset);
	void MoveToPreviousSegment();
	void MoveToNextSegment();
	void RemoveAllSegments();
	void ToggleDebug();
	~LevelEditor();

private:
	LevelEditor(const LevelEditor&);
	void operator=(const LevelEditor&);
private:
	std::vector<LevelSegment*> m_segments;
	int m_curr_active_segment;
	int m_number_of_segments;
	bool m_debugdraw;
};

#endif