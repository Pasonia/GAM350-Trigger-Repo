#include "DayNightCycle.h"

DayNightCycle::DayNightCycle() : m_sun(NULL), m_far(-5.0f), 
							     m_radius(100.0f), m_angle(0.0f)
{
	state_ = STATE::DAWN;
	timeInState_ = 0;
	dayLength_ = DAYLENGTH;
	nightLength_ = NIGHTLENGTH;
	transitionLength_ = TRANSITIONLENGTH;
	nightIncrement_ = NIGHTINCREMENT;

	m_sun = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "white");
	m_sun->SetScale(2.0f, 2.0f, 2.0f);
	m_sun->SetPosition(Vec3());
	m_sun->SetRender(true);

	for (unsigned i = 0; i < 5; ++i)
	{
		LightSource light_;

		light_.LightColor = Vec3(1.0f, 0.1f, 0.0f);
		light_.Intensity = 40.0f;
		m_sunGlow.push_back(light_);		
	}
}

void DayNightCycle::Update(float dt)
{
	m_sun->Update();

	timeInState_ += dt; 

	float currentCycleLength = 0;	

	if (state_ != STATE::NIGHT)
	{
		if (m_angle <= 250.0f)
		{
			m_angle += 0.5f * g_mt * ((dayLength_ + 2.0f * transitionLength_) / 180.0f);

			//float x = m_radius * cosf(m_angle);
			//float y = m_radius * sinf(m_angle);


			float x = m_radius * cosf(m_angle);
			float y = m_radius * sinf(m_angle);

			m_sun->SetPosition(Vec3(x, y, m_player->GetPosition().z + m_far));

			//m_sun->SetPosition(m_player->GetPosition() + Vec3(0.0f, 0.0f, m_far));

			m_sunGlow[0].LightPos = m_sun->GetPosition() + Vec3(0.0f, 3.0f, 5.0f);
			m_sunGlow[1].LightPos = m_sun->GetPosition() + Vec3(0.0f, -2.0f, 5.0f);
			m_sunGlow[2].LightPos = m_sun->GetPosition() + Vec3(2.0f, 0.0f, 3.0f);
			m_sunGlow[3].LightPos = m_sun->GetPosition() + Vec3(-2.0f, 0.0f, 3.0f);
			
		}
		else
			m_angle = 0.0f;
	}		
	else
	{
		if (m_angle <= 250.0f)
		{
			m_angle += 0.5f * g_mt * ((dayLength_ + 2.0f * transitionLength_) / 180.0f);

			float x = -100.0f + m_radius * cosf(m_angle);
			float y = m_radius * sinf(m_angle);

			m_sun->SetPosition(Vec3(x, y, m_player->GetPosition().z + m_far));
		}
		else
		{
			std::vector<LightSource>::iterator sunItr = m_sunGlow.begin();
			
			while (sunItr != m_sunGlow.end())
			{
				sunItr->LightColor = Vec3(1.0f, 0.1f, 0.0f);

				++sunItr;
			}
		}
	}


	std::vector<LightSource>::iterator sunItr = m_sunGlow.begin();
	
	if (state_ == STATE::DAY)
	{
		while (sunItr != m_sunGlow.end())
		{
			if (sunItr->LightColor.z <= 0.3f)
				sunItr->LightColor.z += 0.002f;

			++sunItr;
		}
			
		currentCycleLength =  dayLength_;
	}		
	else if (state_ == STATE::NIGHT)
	{
		while (sunItr != m_sunGlow.end())
		{		
			sunItr->LightColor = Vec3(1.0f, 0.1f, 0.0f);

			++sunItr;
		}

		currentCycleLength = nightLength_;
	}		
	else if (state_ == STATE::DAWN)
	{
		while (sunItr != m_sunGlow.end())
		{
			if (sunItr->LightColor.y <= 0.5f)
				sunItr->LightColor.y += 0.02f;

			sunItr->Intensity = 40.0f;

				++sunItr;
		}
			
		currentCycleLength = transitionLength_;
	}
	else
	{
		while (sunItr != m_sunGlow.end())
		{
			if (sunItr->LightColor.y >= 0.2f)
				sunItr->LightColor.y -= 0.002f;

			if (sunItr->LightColor.z >= 0.0f)
				sunItr->LightColor.z -= 0.0002f;

			sunItr->Intensity -= GetCycleInfo().progress;

				++sunItr;
		}

		currentCycleLength = transitionLength_;
	}
		
	if (timeInState_ >= currentCycleLength)
	{
		if (state_ == STATE::DAY) state_ = STATE::DUSK;
		else if (state_ == STATE::DUSK) state_ = STATE::NIGHT;
		else if (state_ == STATE::NIGHT) state_ = STATE::DAWN;
		else // STATE::DAWN
		{
			state_ = STATE::DAY;
			nightLength_ += nightIncrement_;
		}

		timeInState_ = 0;
	}
}

void DayNightCycle::Render()
{
	//m_sun->render();
}

cycleInfo DayNightCycle::GetCycleInfo()
{
	float progress = 0;
	if (state_ == STATE::DAY) progress = timeInState_ / dayLength_;
	else if (state_ == STATE::NIGHT) progress = timeInState_ / nightLength_;
	else progress = timeInState_ / transitionLength_;
	return cycleInfo(state_, progress);
}

DayNightCycle::~DayNightCycle() { }

void DayNightCycle::SetPlayerInstance(ObjectInstance* player_)	{ m_player = player_; }
void DayNightCycle::SetSunInstance(ObjectInstance* sun_)		{ m_sun = sun_; }

std::vector<LightSource>& DayNightCycle::GetSunGlow()			{ return m_sunGlow; }