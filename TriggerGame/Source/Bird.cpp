#include"Brid.h"

void BIRDS::Init()
{
}

BIRDS::BIRDS(Vec3& pos,ObjectInstance* player)
{
	m_player = player;
	for (int i = 0; i <= 7; ++i)
	{
	    objCont[i] = ObjectManager::GetInstance().CreateObjectInstance("Bird", "Panel");
		objCont[i]->SetRender(true);
		objCont[i]->SetScale(0.03f, 0.03f, 0.03f);
		float velocity = rand() % 50 + 10.0f;
		objCont[i]->SetVelocity(Vec3(-velocity, 0.0f, 0.0f));
		objCont[i]->SetPosition(pos + Vec3(0, 0, -30.0f)*i);
		objCont[i]->SetAnim(0);
	}
}

void BIRDS::Update()
{
	for (int i = 0; i <= 7; ++i)
	{
		//std::cout << objCont[i]->GetPosition().x << std::endl;
		if (objCont[i]->GetPosition().x < -500.0f || objCont[i]->GetPosition().x > 0.0f)
		{
			float offset = rand() % 200;
			float y_offset = rand() % 10;
			objCont[i]->SetPosition(Vec3(0.0f, 30.0f+y_offset, m_player->GetPosition().z - offset));
        }
		/*if (glm::distance(objCont[i]->GetPosition(), m_player->GetPosition()) < 300.0f  && glm::length(objCont[i]->GetVelocity()) < 350.0f)
		{
			Vec3 temp = m_player->GetPosition() - objCont[i]->GetPosition();
			temp = glm::normalize(temp);				
			objCont[i]->SetVelocity(temp * 400.0f);
        }*/
		objCont[i]->Update();
	}
}

void BIRDS::Render()
{
    for (int i = 0; i <= 7; ++i)
	{
		objCont[i]->render();
	}
}

void BIRDS::renderShadow()
{
	for (int i = 0; i <= 7; ++i)
	{
		objCont[i]->renderShadow();
	}
}
//bird::bird(Vec3& pos)
//{
//	object = ObjectManager::GetInstance().CreateObjectInstance("Bird", "Panel");
//	object->SetScale(0.03f, 0.03f, 0.03f);
//	object->SetYaw((-PI) / 2);
//	object->SetRender(true);
//	object->SetAnim(0);
//	object->SetPosition(pos);
//	chase = false;
//	dash = false;
//	dash_done = false;
//	chasing_timer = 1.0f;
//}
//
//
//
//
//void BIRDS::Init()
//{
//}
//
//BIRDS::BIRDS(ObjectInstance* player)
//{
//	m_player = player;
//	active_distance = 200.0f;
//	chasing_speed = 10.0f;
//	dashing_speed = 150.0f;
//	for (int i = 0; i < 7; ++i)
//	{
//		//Vec3 temp(-(rand() % 256), (rand() % 10), -(rand() % 512));
//		Vec3 temp(-(rand() % 256), rand() % 20, -200 * i);
//		birds.push_back(new bird(temp));
//	}
//	/*for (int i = 0; i <= 7; ++i)
//	{
//		objCont[i] = ObjectManager::GetInstance().CreateObjectInstance("Bird", "Panel");
//		objCont[i]->SetRender(true);
//		objCont[i]->SetScale(0.03f, 0.03f, 0.03f);
//		float velocity = rand() % 50 + 10.0f;
//		objCont[i]->SetVelocity(Vec3(-velocity, 0.0f, 0.0f));
//		objCont[i]->SetPosition(pos + Vec3(0, 0, -30.0f)*i);
//		objCont[i]->SetAnim(0);
//	}*/
//}
//
//void BIRDS::Update()
//{
//
//	for (std::vector<bird*>::iterator it = birds.begin(); it != birds.end(); ++it)
//	{
//		if (!(*it)->chase && !(*it)->dash && !(*it)->dash_done)
//		{
//			if (glm::distance((*it)->GetPos(), m_player->GetPosition()) < active_distance)
//				(*it)->chase = true;
//		}
//		if ((*it)->chase && !(*it)->dash)
//		{
//			//if ((*it)->chasing_timer > 0)
//			//{
//				//std::cout << (*it)->object->GetVelocity().x << " " << (*it)->object->GetVelocity().y << " " << (*it)->object->GetVelocity().z << std::endl;
//				(*it)->chasing_timer -= g_dt;
//                Vec3 temp = m_player->GetPosition() - (*it)->GetPos();
//				temp = glm::normalize(temp);
//				(*it)->object->SetVelocity(temp * chasing_speed);
//			//}
//		/*	else
//			{
//				(*it)->chasing_timer = 0;
//				(*it)->chase = false;
//				(*it)->dash = true;
//
//			}*/
//		}
//		/*if (!(*it)->chase && (*it)->dash)
//		{
//			Vec3 temp = m_player->GetPosition() - (*it)->GetPos();
//			temp = glm::normalize(temp);
//			(*it)->object->SetVelocity(temp * dashing_speed);
//			(*it)->dash = false;
//			(*it)->dash_done = true;
//        }*/
//
//		(*it)->object->Update();
//
//    }
//	//for (int i = 0; i <= 7; ++i)
//	//{
//	//	//std::cout << objCont[i]->GetPosition().x << std::endl;
//	//	if (objCont[i]->GetPosition().x < -500.0f)
//	//	{
//	//		float offset = rand() % 200;
//	//		float y_offset = rand() % 10;
//	//		objCont[i]->SetPosition(Vec3(0.0f, 30.0f + y_offset, m_player->GetPosition().z - offset));
//
//
//	//	}
//	//	objCont[i]->Update();
//	//}
//}
//
//void BIRDS::Render()
//{
//	for (std::vector<bird*>::iterator it = birds.begin(); it != birds.end(); ++it)
//	{
//		(*it)->object->render();
//	}
//}
//
//void BIRDS::renderShadow()
//{
//	for (std::vector<bird*>::iterator it = birds.begin(); it != birds.end(); ++it)
//	{
//		(*it)->object->renderShadow();
//	}
//}

