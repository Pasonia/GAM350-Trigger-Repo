#include "Terrain.h"
#define SEG_WIDTH 100 // X
#define HALF_SEG_WIDTH 50
#define MIN_SEG_LENGTH 8 // Z
#define RAND_SEG_LENGTH 2
#define ANGLE_VARIOUS 30
#define LOWER_LIMIT 1.0f
#define UPPER_LIMIT 10.0f

void terrains::makeSegment(const Vec3& Position,bool& use_flat)
{
	ObjectInstance* obj;
	if (piece_cont.size() < (unsigned)cont_size)
	{
		obj = ObjectManager::GetInstance().CreateObjectInstance("Box", "DirtFloor");
		obj->SetUV(Vec2(5.5f, 5.5f));
		piece_cont.push_back(obj);
		segmentTracker = 0;
	}
	else
	{
		obj = piece_cont[segmentTracker];
	}

	float angle = 0;
	float sign = rand() % 2 ? 1.0f : -1.0f;
	if (!use_flat)
	{

		if ((Pos + offset).y < LOWER_LIMIT)
			angle = PI / 8.0f;
		else if ((Pos + offset).y > UPPER_LIMIT)
			angle = -PI / 8.0f;
		else
			angle = (sign*(rand() % ANGLE_VARIOUS)) / 57.3f;
	}
	use_flat = !use_flat;

	float length = float(MIN_SEG_LENGTH + rand() % RAND_SEG_LENGTH);
	float half_length = length / 2.0f;
	obj->SetScale(SEG_WIDTH, 1, length);
	obj->SetPitch(angle);
	offset += Vec3(0, sinf(last_angle) * last_half_length, -cosf(last_angle)* last_half_length) += Vec3(0, sinf(angle) * half_length, -cosf(angle)* half_length);
	obj->SetPosition(Position + offset);
	//compute the offset
	last_angle = angle;
	last_half_length = half_length;

	if (++segmentTracker == cont_size)
		segmentTracker = 0;
}

terrains::terrains(Vec3 Position, int cont_size_)
{
	//Initialize 
	cont_size = cont_size_;
	Pos = Position;
	offset = Vec3(0, 0, 0);
	last_angle = 0;
	last_half_length = 0;
	segmentTracker = 0;
	use_flat = true;
	for (int i = 0; i < cont_size_; ++i)
	{
		makeSegment(Position,use_flat);
    }
}

void terrains::Update(const Vec3& PlayerPos)
{
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
		(*it)->Update();

	if (piece_cont[segmentTracker]->GetPosition().z - PlayerPos.z >= 0.f)
	{
		makeSegment(Pos,use_flat);
	}
}

void terrains::Render()
{
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
		(*it)->render();
}

void terrains::Free()
{
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
	{
        (*it)->free();
	}
}

Vec3 terrains::getpoint()
{
	float sign;
	int index = rand() % cont_size;
	int count = 0;
	float pitch = 0;
	float half_seg_width = 0;
	Vec3 Position;
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
	{
		count++;
		if (count == index)
		{
			pitch = (*it)->GetPitch();
			half_seg_width = (*it)->GetScaleZ() / 2.0f;
			Position = (*it)->GetPosition();
			break;
		}
	}
	
	Vec3 verVector(0,sinf(pitch),-cosf(pitch));
	glm::normalize(verVector);

	float horioffset = ((rand() % 100) / 100.0f)*HALF_SEG_WIDTH;
	sign = rand() % 2 ? 1.0f : -1.0f;
	horioffset *= sign;

	float vertioffset = ((rand() % 100) / 100.0f)*half_seg_width;
	sign = rand() % 2 ? 1.0f : -1.0f;
	vertioffset *= sign;

	return Position + horioffset*Vec3(1, 0, 0) + vertioffset * verVector;
}

Vec3 terrains::GetDirection(float dragon_z)
{
	Vec3 dir;
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
	{
		float pitch = (*it)->GetPitch();
		float width = (*it)->GetScaleZ() / 2.0f;
		float left = (*it)->GetPosition().z - cosf(pitch) * width;
		float right = (*it)->GetPosition().z + cosf(pitch) * width;
		if (dragon_z < right && dragon_z > left)
		{
			dir = glm::normalize(Vec3(0, sinf(pitch), -cosf(pitch)));
			break;
		}
	}
	return dir;
}

float terrains::GetHeight(float z)
{
	for (auto it = piece_cont.begin(); it != piece_cont.end(); ++it)
	{
		float pitch = (*it)->GetPitch();
		float width = (*it)->GetScaleZ() / 2.0f;
		float left = (*it)->GetPosition().z - cosf(pitch) * width;
		float right = (*it)->GetPosition().z + cosf(pitch) * width;
		float positon_z = (*it)->GetPosition().z;
		float positon_y = (*it)->GetPosition().y;
		float diff_z = z - positon_z;
		if (z < right && z > left)
		{
			return positon_y + tanf(pitch)*diff_z;
			break;
		}
	}
	return 0;
}