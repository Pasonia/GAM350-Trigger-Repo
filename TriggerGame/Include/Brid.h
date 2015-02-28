#ifndef BIRD_H
#define BIRD_H
#include "LaserObstacle.h"
#include "GameTracker.h"
#include "ObjectInstance.h"
#include "PhysicsManager.h"
#include "Physics.h"
#include "Picking.h"
#include "FactoryController.h"
class bird
{
public:
	bird(){};
	bird(Vec3& pos);
	~bird()
	{
		delete object;
	};
	Vec3 GetPos(){return object->GetPosition();};
    ObjectInstance* object;
    bool chase;
	bool dash;
	bool dash_done;
	float chasing_timer;
};

class BIRDS
{
public:
	BIRDS(){};
	BIRDS(Vec3 & pos,ObjectInstance* player);
	 ~BIRDS();
	void Init();
	void Update();
	void Render();
	void renderShadow();


private:

	//std::vector<bird*> birds;
	ObjectInstance* objCont[8];
	ObjectInstance* m_player;
	//float active_distance;
	//float chasing_speed;
	//float dashing_speed;
   
	//float   timer[3];
};

#endif