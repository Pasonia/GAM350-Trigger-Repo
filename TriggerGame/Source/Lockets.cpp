/******************************************************************************/
/*!
\file Lockets.cpp
\author Yang Shu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Lockets class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "Lockets.h"

lockets::lockets(Vec3 head_pos, Vec3 head_dir)
{
	num_of_lockest = 5;
	y_init_value = head_pos.y;
	head_pos_ = head_pos;
	for (int i = 0; i < num_of_lockest; ++i)
	{
		std::string tex;
		switch (i)
		{
		case 0:
			tex = "Exit";
			break;
		case 1:
			tex = "Credits";
			break;
		case 2:
			tex = "Options";
			break;
		case 3:
			tex = "Howto";
			break;
		case 4:
			tex = "Rust";
			break;
		}
		plane = ObjectManager::GetInstance().CreateObjectInstance("Box","Panel");
		plane->SetScale(35, 1, 1);
		plane->SetPosition(Vec3(0,0,0));
		plane->SetRender(FALSE);
		lockets_array_up[i] = ObjectManager::GetInstance().CreateObjectInstance("Locket", tex.c_str());
		lockets_array_down[i] = ObjectManager::GetInstance().CreateObjectInstance("Locket", tex.c_str());

		lockets_array_up[i]->SetPosition(head_pos - (i + 1)*35.0f*head_dir + 6.0f*Vec3(0, 1, 0));
		lockets_array_down[i]->SetPosition(head_pos - (i + 1)*35.0f*head_dir - 6.0f*Vec3(0, 1, 0));
		lockets_array_down[i]->SetPitch(PI);

		float  scaleS = 40.0f;
		lockets_array_up[i]->SetScale(scaleS, scaleS, scaleS);
		lockets_array_down[i]->SetScale(scaleS, scaleS, scaleS);
		Physics * phy_comp_UP = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, lockets_array_up[i]);
		lockets_array_up[i]->AddComponent(phy_comp_UP);
		phy_comp_UP->SetBoundType(1);
		phy_comp_UP->SetResitution(0.6f);
		phy_comp_UP->SetMass(10.0f);
		phy_comp_UP->SetPhysicsType(false);
		phy_comp_UP->SetFalling(false);
		phy_up[i] = phy_comp_UP;
		PhysicsManager_Inst->RegisterObject(lockets_array_up[i]);

		Physics * phy_comp_DOWN = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, lockets_array_down[i]);
		lockets_array_down[i]->AddComponent(phy_comp_DOWN);
		phy_comp_DOWN->SetBoundType(1);
		phy_comp_DOWN->SetResitution(0.6f);
		phy_comp_DOWN->SetMass(10.0f);
		phy_comp_DOWN->SetPhysicsType(false);
		phy_comp_DOWN->SetFalling(false);
		phy_down[i] = phy_comp_DOWN;
		PhysicsManager_Inst->RegisterObject(lockets_array_down[i]);

		//lockets_array_up[i]->SetFlag(RENDER_AABB);
		//lockets_array_down[i]->SetFlag(RENDER_AABB);
	}

};
int lockets::IntersectCheck(Vec3 cursor, int window_width, int window_height)
{
	bool dont_check = false;
	for (int i = 0; i < num_of_lockest; ++i)
	{
		if (int_cont[i].can_be_clicked == false)
		{
			dont_check = true;
		}
	}
	if (dont_check == false)
	{

		for (int i = 0; i < num_of_lockest; ++i)
		{
			if (Picking::RayIntersectAABB(cursor, phy_up[i]->GetBoundGeometry().aabb_, window_width, window_height) || Picking::RayIntersectAABB(cursor, phy_down[i]->GetBoundGeometry().aabb_, window_width, window_height))
			{
				if (int_cont[i].can_be_clicked)
				{
					if (int_cont[i].open)
					{
						//if (i == 1)
					//	{
					//		plane->SetTexture("CreditPanel");
						//}
						//if (i == 3)
					//	{
					//		plane->SetTexture("HowToPanel");
					//	}
						lockets_array_up[i]->SetVelocity(Vec3(0, 30, 0));
						lockets_array_down[i]->SetVelocity(Vec3(0, -30, 0));
						plane->SetPosition(head_pos_ - (i + 1)*35.0f*Vec3(1,0,0) +5.0f*Vec3(0,0,1));
						plane->SetScale(35,1,1);
	                    plane->SetRender(TRUE);
						plane_speed = 60.0f;
						int_cont[i].can_be_clicked = false;

						for (int j = 0; j < num_of_lockest; ++j) // J4YP33: changed from i to j, may cause errors
						{
							if (int_cont[j].open == false)
							{
								lockets_array_up[j]->SetVelocity(Vec3(0, -30, 0));
								lockets_array_down[j]->SetVelocity(Vec3(0, 30, 0));
								int_cont[j].can_be_clicked = false;
							}
						}
						return i;

					}
					else
					{
						lockets_array_up[i]->SetVelocity(Vec3(0, -30, 0));
						lockets_array_down[i]->SetVelocity(Vec3(0, 30, 0));
						int_cont[i].can_be_clicked = false;
					}
				}
				break;
			}

		}
	}
	return -1;
};
void lockets::Update()
{
	if (plane->GetScale().y != 35.0f || plane->GetScale().y != 0.0f)
	{
       
		plane->SetScaleY(plane->GetScale().y + plane_speed * g_dt);
	}
	if (plane->GetScale().y > 35.0f)
	{
		
		plane->SetScaleY(35.0f);
    }
	if (plane->GetScale().y < 0.0f)
	{
		plane->SetScaleY(0.0f);
	}
	for (int i = 0; i < num_of_lockest; ++i)
	{
		if (int_cont[i].open == false)
		{

			if (std::abs(lockets_array_up[i]->GetPosition().y - y_init_value) < 6.0f || std::abs(lockets_array_down[i]->GetPosition().y - y_init_value) < 6.0f)
			{
				lockets_array_up[i]->SetPosition(Vec3(lockets_array_up[i]->GetPosition().x, y_init_value + 6.0f, lockets_array_up[i]->GetPosition().z));
				lockets_array_down[i]->SetPosition(Vec3(lockets_array_down[i]->GetPosition().x, y_init_value - 6.0f, lockets_array_down[i]->GetPosition().z));
				lockets_array_up[i]->SetVelocity(Vec3(0, 0, 0));
				lockets_array_down[i]->SetVelocity(Vec3(0, 0, 0));
				int_cont[i].timer = 0;
				int_cont[i].open = !int_cont[i].open;
				int_cont[i].can_be_clicked = true;
				//plane->SetTexture("Panel");
			}
		}
		if (int_cont[i].open == true)
		{
            if (std::abs(lockets_array_up[i]->GetPosition().y - y_init_value) > 20.0f || std::abs(lockets_array_down[i]->GetPosition().y - y_init_value) > 20.0f)
			{
				lockets_array_up[i]->SetPosition(Vec3(lockets_array_up[i]->GetPosition().x, y_init_value + 20.0f, lockets_array_up[i]->GetPosition().z));
				lockets_array_down[i]->SetPosition(Vec3(lockets_array_down[i]->GetPosition().x, y_init_value - 20.0f, lockets_array_down[i]->GetPosition().z));
				lockets_array_up[i]->SetVelocity(Vec3(0, 0, 0));
				lockets_array_down[i]->SetVelocity(Vec3(0, 0, 0));
				int_cont[i].timer = 0;
				int_cont[i].open = !int_cont[i].open;
				int_cont[i].can_be_clicked = true;
				//plane->SetTexture("Panel");
			}
		}
	}
}
void lockets::ResetLockets()
{
	
	for (int i = 0; i < num_of_lockest; ++i)
	{
		if (int_cont[i].open == FALSE)
		{
			lockets_array_up[i]->SetVelocity(Vec3(0, -30, 0));
			lockets_array_down[i]->SetVelocity(Vec3(0, 30, 0));
			plane_speed = -60.0f;
        }
	}
}
void lockets::Free()
{


}

