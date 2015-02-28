#include "Effects.h"

// if lifetime over, just dont update and render
// if need that particle system again then shit it and update render
// 2d vector of emitters, if not enuf then add, if enough then reuse
// set density to 0 when pause

// lightorb
// lightorb destruct at night
// collect lightorb
// collide ground
// collide trees

EffectsManager::EffectsManager()
{
	Emitters.resize(EMITTER_TYPE::ENUMSIZE);
}

ParticleSystem* EffectsManager::makeEmitter(EMITTER_TYPE type)
{
	ParticleSystem* result = new ParticleSystem;
	if (type == EMITTER_TYPE::LIGHTORB)
	{
		result->InitParticle("../Resource/Image/particle.png");
		result->SetProperties(glm::vec3(0.0f, 0.0f, 0.0f),
			Vec3(-1, 1, 0),
			Vec3(1, 2, 0),
			Vec3(0, 0, 0),
			glm::vec3(0.784314, 0.196078, 0), 0.1f, 1.f, 0.5f, 0.1f, 8);
	}
	else if (type == EMITTER_TYPE::COLLECT)
	{
		result->InitParticle("../Resource/Image/particle.png");
		result->SetProperties(glm::vec3(0.0f, 0.0f, 0.0f), // Where the particles are generated
			Vec3(-2, -2, 0),        // Minimal velocity
			Vec3(2, 2, 0), // Maximal velocity
			Vec3(0, 0, 0), // Gravity force applied to particles
			glm::vec3(0.9f, 0.1f, 0.02f), // Color 
			0.5f, // Minimum lifetime in seconds
			0.5f, // Maximum lifetime in seconds
			0.1f, // Rendered size
			0.1f, // Spawn rate
			4); // Density
	}
	else if (type == EMITTER_TYPE::GROUND_COLLISION)
	{
		result->InitParticle("../Resource/Image/smoke.png");
		result->SetProperties(glm::vec3(0.0f, 0.0f, 0.0f), Vec3(-0.285978, 0.0797935, 0.124562), Vec3(0.154783, 0.0754623, 1.11166), Vec3(0, 0, 1.5), glm::vec3(0.784314, 0.196078, 0), 31.00f, 62.00f, 0.50f, 0.20f, 8);
	}
	else if (type == EMITTER_TYPE::DISAPPEAR)
	{
		result->InitParticle("../Resource/Image/particle.png");
		result->SetProperties(glm::vec3(0.0f, 0.0f, 0.0f), Vec3(-5, 0, 0), Vec3(5, 70, 10), Vec3(0, 4, 0), glm::vec3(0.784314, 0.196078, 0), 1.00f, 4.00f, 1.00f, 0.20f, 256);
	}
	else if (type == EMITTER_TYPE::COLLIDE_TREE)
	{
		result->InitParticle("../Resource/Image/particle.png");
		result->SetProperties(glm::vec3(0.0f, 0.0f, 0.0f), Vec3(-5, 0, 0), Vec3(5, 70, 10), Vec3(0, 4, 0), glm::vec3(0.784314, 0.196078, 0), 1.00f, 4.00f, 1.00f, 0.20f, 256);
	}
	else
	{
	}
	return result;
}

EffectsManager::~EffectsManager() 
{ 
	for (auto i = Emitters.begin(); i != Emitters.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			delete j->emitter;
		}
	}
}

ParticleSystem* EffectsManager::mount(const ObjectInstance& obj, EMITTER_TYPE type, const Vec3& offset, float lifetime)
{
	// check for existing emitters
	ParticleSystem* result = 0;
	for (auto it = Emitters[type].begin(); it != Emitters[type].begin(); ++it)
	{
		if (!it->active)
		{
			result = it->emitter;
			it->active = true;
			it->obj = &obj;
			it->pos = Vec3();
			if (lifetime)
			{
				it->repeat = false;
				it->life = lifetime;
			}
			else
			{
				it->repeat = true;
			}
			break;
		}
	}
	
	// if no emitters available
	if (!result)
	{
		result = makeEmitter(type);
		Emitters[type].push_back(EmitterStruct(result, true, &obj, Vec3(), offset, lifetime ? false : true, lifetime, result->GetNumberOfParticles()));
	}

	return result;
}

ParticleSystem* EffectsManager::mount(const Vec3& pos, EMITTER_TYPE type, const Vec3& offset, float lifetime)
{
	// check for existing emitters
	ParticleSystem* result = 0;
	for (auto it = Emitters[type].begin(); it != Emitters[type].begin(); ++it)
	{
		if (!it->active)
		{
			result = it->emitter;
			it->active = true;
			it->obj = 0;
			it->pos = pos;
			if (lifetime)
			{
				it->repeat = false;
				it->life = lifetime;
			}
			else
			{
				it->repeat = true;
			}
			break;
		}
	}

	// if no emitters available
	if (!result)
	{
		result = makeEmitter(type);
		Emitters[type].push_back(EmitterStruct(result, true, 0, pos, offset, lifetime ? false : true, lifetime, result->GetNumberOfParticles()));
	}

	return result;
}

void EffectsManager::unmount(ParticleSystem* emitter)
{
	for (auto i = Emitters.begin(); i != Emitters.end(); ++i)
		for (auto j = i->begin(); j != i->end(); ++j)
			if (emitter == j->emitter) j->active = false;
}

void EffectsManager::unmount(const ObjectInstance& obj, EMITTER_TYPE type)
{
	for (auto i = Emitters[type].begin(); i != Emitters[type].end(); ++i)
			if (i->obj == &obj) i->active = false;
}

void EffectsManager::play(const ObjectInstance& obj, EMITTER_TYPE type)
{
	for (auto i = Emitters[type].begin(); i != Emitters[type].end(); ++i)
		if (i->obj == &obj)
			i->emitter->SetNumberOfParticles(i->density);
}

void EffectsManager::pause(const ObjectInstance& obj, EMITTER_TYPE type)
{
	for (auto i = Emitters[type].begin(); i != Emitters[type].end(); ++i)
		if (i->obj == &obj) 
			i->emitter->SetNumberOfParticles(0);
}

void EffectsManager::Update(float dt)
{
	for (auto i = Emitters.begin(); i != Emitters.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			if (j->active)
			{
				if (!j->repeat) // reduce lifetime of particle effect
				{
					if (j->life >= 0)
					{
						j->life -= dt;
					}
					else
					{
						j->active = false;
					}
				}
				j->emitter->SetPosition(j->offset + (j->obj == 0 ? (j->pos) : (j->obj->GetPosition())));
				j->emitter->Update(dt);
			}
		}
	}
}

void EffectsManager::Render(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2)
{
	for (auto i = Emitters.begin(); i != Emitters.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			if (j->active)
			{
				j->emitter->Render(proj, view, q1, q2);
			}
		}
	}
}
