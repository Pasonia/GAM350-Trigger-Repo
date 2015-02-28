/******************************************************************************/
/*!
\file PlayerControl.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for PlayerControl class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#define DAMPING 0.03f
#define TIME_STEPSIZE2 0.5f*0.5f
#define CONSTRAINT_ITERATIONS 15

#include "Particle.h"
#include "Component.h"
#include "TriggerInput.h"
#include <vector>

class CameraController;
class Tail;
class Cloth;

class PlayerControl : public Component
{
public:
	PlayerControl(ObjectInstance * m_parent, CameraController * cam);
	~PlayerControl();
	void Init();
	void Update();
	void Render();
	void Unload();
  void ShowPlayer();
  void HidePlayer();
  void TailRender();
  Vec3 GetPlayerDir() { return playerDir; }
  void SetPlayerDir(Vec3 dir){ playerDir = dir; }
	float& GetSpeedDashMultiplier() { return dashSpeedMultiplier; }
	float GetGravRadius() { return gravityRadius; }
	void Free() {};
	ObjectInstance * getChargeBall() { return chargeBall; }
	float getPlayerSpeed() { return playerSpeed * dashSpeedMultiplier; }
	Cloth * tailT;
	Vec4 holdKey; //x = left, y = right, z = up, w = down
private:
	Vec3 m_playerview;
	Vec3 playerDir;
	
	Vec3 accel;
	float vAngle;
	float hAngle;
	bool lockPlayer;
	bool dash;
	bool chargingGrav;
	bool gravRelease;
	bool chargingPulseUp;
	bool jumping;
	float gravCooldown;
	float gravCharge;
	float gravTime;
	float dashSpeedMultiplier;
	float playerSpeed;
	float timeModifier;
	float gravityRadius;
	float gravChangeDur;
	float sideAccel;
	float vertAccel;

	bool leftWind;
	float windDur;
	Vec3 windDir;
	Tail * tail;
	CameraController * camC;
	ObjectInstance * chargeBall;
	ObjectInstance * gravVec;
	std::vector<ObjectInstance *> gravityBallList;
	void GetPlayerViewVec();
	unsigned char m_forward, m_backward, m_turnleft, m_turnright;
};

struct TailNode
{
	TailNode(ObjectInstance * tailNodeObj_, ObjectInstance * prevTailNode_, float scale_) : tailNodeObj(tailNodeObj_), prevTailNode(prevTailNode_), scale(scale_) {}
	ObjectInstance * tailNodeObj;
	ObjectInstance * prevTailNode;
	ParticleSystem * testParticle;
	float scale;
};

class Tail
{
public:
	Tail(ObjectInstance * player, PlayerControl * parent, int segs_ = 10, float scale_ = 1.5f);
	void Init();
	void Update();
	void Render();
  void ShowTail();
  void HideTail();
  std::vector<TailNode>& GetTail() { return tail; }
private:
	std::vector<TailNode> tail;
	int segs;
	float scale;
	PlayerControl * m_parent;
};

struct clothData
{
	std::vector<Vec3> pos;
	std::vector<Vec3> nrm;
};

class ParticleC
{
private:
	bool movable;

	float mass; 
	Vec3 pos;
	Vec3 old_pos; 
	Vec3 acceleration;
	Vec3 accumulated_normal;

public:
	ParticleC(Vec3 pos) : pos(pos), old_pos(pos), acceleration(Vec3(0, 0, 0)), mass(10), movable(true), accumulated_normal(Vec3(0, 0, 0)){}
	ParticleC(){}

	void addForce(Vec3 f)
	{
		acceleration += f / mass;
	}

	void timeStep()
	{
		if (movable)
		{
			Vec3 temp = pos;
			pos = pos + (pos - old_pos)*(1.0f - DAMPING) + acceleration*g_dt;
			old_pos = temp;
			acceleration = Vec3(0, 0, 0); 	
		}
	}

	Vec3& getPos() { return pos; }
	Vec3& getOldpos() { return old_pos; }
	void resetAcceleration() { acceleration = Vec3(0, 0, 0); }

	void offsetPos(const Vec3 v) { if (movable) pos += v; }
	void setPos(const  Vec3 v){ if (movable) pos = v; }
	void makeUnmovable() { movable = false; }

	void addToNormal(Vec3 normal)
	{
		accumulated_normal += glm::normalize(normal);
	}

	Vec3& getNormal() { return accumulated_normal; } // notice, the normal is not unit length

	void resetNormal() { accumulated_normal = Vec3(0, 0, 0); }

};


class Constraint
{
private:
	float rest_distance; 

public:
	ParticleC *p1, *p2;

	Constraint(ParticleC *p1, ParticleC *p2) : p1(p1), p2(p2)
	{
		Vec3 vec = p1->getPos() - p2->getPos();
		rest_distance = glm::length(vec);
	}

	void satisfyConstraint()
	{
		Vec3 p1_to_p2 = p2->getPos() - p1->getPos(); 
		float current_distance = glm::length(p1_to_p2);
		Vec3 correctionVector = p1_to_p2*(1 - rest_distance / current_distance);
		Vec3 correctionVectorHalf = correctionVector*0.8f;
		p1->offsetPos(correctionVectorHalf); 
		p2->offsetPos(correctionVectorHalf * -1); 
	}

};

class Cloth
{
private:

	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
	// total number of particles is num_particles_width*num_particles_height

	std::vector<ParticleC> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth

	
	void makeConstraint(ParticleC *p1, ParticleC *p2) { constraints.push_back(Constraint(p1, p2)); }

	Vec3 calcTriangleNormal(ParticleC *p1, ParticleC *p2, ParticleC *p3)
	{
		Vec3 pos1 = p1->getPos();
		Vec3 pos2 = p2->getPos();
		Vec3 pos3 = p3->getPos();

		Vec3 v1 = pos2 - pos1;
		Vec3 v2 = pos3 - pos1;

		return glm::cross(v1, v2);//    v1.cross(v2);
	}

	
	void addWindForcesForTriangle(ParticleC *p1, ParticleC *p2, ParticleC *p3, const Vec3 direction)
	{
		Vec3 normal = calcTriangleNormal(p1, p2, p3);
		Vec3 d = glm::normalize(normal);
		Vec3 force = normal * (glm::dot(d, direction));//.dot(direction));
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}

public:
	ParticleC* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

		// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				Vec3 pos = Vec3(width * (x / (float)num_particles_width),
					-height * (y / (float)num_particles_height),
					0);
				particles[y*num_particles_width + x] = ParticleC(pos); // insert particle in column x at y'th row
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				if (x<num_particles_width - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
				if (y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
				if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
				if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				if (x<num_particles_width - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
				if (y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
				if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
				if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
			}
		}


		// making the upper left most three and right most three particles unmovable
		for (int i = 0; i<1; i++)
		{
			//getParticle(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0 + i, 0)->makeUnmovable();

			//getParticle(0 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();
		}
	}

	/* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
	Called from the display() method
	The cloth is seen as consisting of triangles for four particles in the grid as follows:

	(x,y)   *--* (x+1,y)
	| /|
	|/ |
	(x,y+1) *--* (x+1,y+1)

	*/
	//void drawShaded()
	//{
	//	// reset normals (which where written to last frame)
	//	std::vector<Particle>::iterator particle;
	//	for (particle = particles.begin(); particle != particles.end(); particle++)
	//	{
	//		(*particle).resetNormal();
	//	}

	//	float u_step = 1.0f / (num_particles_width);
	//	float v_step = 1.0f / (num_particles_height);
	//	float width_off = u_step;
	//	//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
	//	for (int x = 0; x<num_particles_width - 1; x++)
	//	{
	//		float height_off = 0.f;
	//		for (int y = 0; y<num_particles_height - 1; y++)
	//		{
	//			Vec3 normal = calcTriangleNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
	//			getParticle(x + 1, y)->addToNormal(normal);
	//			getParticle(x, y)->addToNormal(normal);
	//			getParticle(x, y + 1)->addToNormal(normal);
	//			clothCont.nrm.push_back(getParticle(x + 1, y)->getNormal());
	//			clothCont.nrm.push_back(getParticle(x, y)->getNormal());
	//			clothCont.nrm.push_back(getParticle(x, y + 1)->getNormal());
	//			//uv
	//			clothCont.uv.push_back(MatLib::Vec2(width_off + u_step, height_off));
	//			clothCont.uv.push_back(MatLib::Vec2(width_off, height_off));
	//			clothCont.uv.push_back(MatLib::Vec2(width_off, height_off + v_step));
	//			normal = calcTriangleNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
	//			getParticle(x + 1, y + 1)->addToNormal(normal);
	//			getParticle(x + 1, y)->addToNormal(normal);
	//			getParticle(x, y + 1)->addToNormal(normal);
	//			clothCont.nrm.push_back(getParticle(x + 1, y + 1)->getNormal());
	//			clothCont.nrm.push_back(getParticle(x + 1, y)->getNormal());
	//			clothCont.nrm.push_back(getParticle(x, y + 1)->getNormal());
	//			//uv
	//			clothCont.uv.push_back(MatLib::Vec2(width_off + u_step, height_off + v_step));
	//			clothCont.uv.push_back(MatLib::Vec2(width_off + u_step, height_off));
	//			clothCont.uv.push_back(MatLib::Vec2(width_off, height_off + v_step));
	//			//draw tri
	//			drawTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
	//			clothCont.pos.push_back(getParticle(x + 1, y)->getPos());
	//			clothCont.pos.push_back(getParticle(x, y)->getPos());
	//			clothCont.pos.push_back(getParticle(x, y + 1)->getPos());

	//			drawTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
	//			clothCont.pos.push_back(getParticle(x + 1, y + 1)->getPos());
	//			clothCont.pos.push_back(getParticle(x + 1, y)->getPos());
	//			clothCont.pos.push_back(getParticle(x, y + 1)->getPos());

	//			height_off += v_step;
	//		}
	//		width_off += u_step;
	//	}

	//	//return clothCont;
	//}
	///* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
	//void drawTriangle(Particle *p1, Particle *p2, Particle *p3)
	//{
	//	p1->getNormal().Normalize();
	//	p1->getPos();

	//	p2->getNormal().Normalize();
	//	p2->getPos();

	//	p3->getNormal().Normalize();
	//	p3->getPos();
	//}

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/

	std::vector<ParticleC>& GetParticles() { return particles; }
	void timeStep()
	{
		std::vector<Constraint>::iterator constraint;
		for (int i = 0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
		{
			for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
			{
				(*constraint).satisfyConstraint(); // satisfy constraint.
			}
		}

		std::vector<ParticleC>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).timeStep(); // calculate the position of each particle at the next time step.
		}
	}

	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const Vec3 direction)
	{
		std::vector<ParticleC>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).addForce(direction); // add the forces to each particle
		}

	}

	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const Vec3 direction)
	{
		for (int x = 0; x<num_particles_width - 1; x++)
		{
			for (int y = 0; y<num_particles_height - 1; y++)
			{
				addWindForcesForTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
				addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
			}
		}
	}

	void doFrame()
	{

	}
};

#endif