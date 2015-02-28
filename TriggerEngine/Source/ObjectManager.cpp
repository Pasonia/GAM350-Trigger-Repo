/******************************************************************************/
/*!
\file ObjectManager.cpp
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

#include "ObjectManager.h"
#include "ObjectMap.h"

ObjectManager::ObjectManager()
{
	flag = 0;
}

ObjectManager& ObjectManager::GetInstance()
{
	static ObjectManager manager;
	return manager;
}

ObjectInstance * ObjectManager::CreateObjectInstance(const std::string& model_name, const std::string& texture_name)
{
	ObjectInstance * ObjInst = new ObjectInstance(model_name,texture_name);
	m_objects.push_back(ObjInst);
	flag |= 0x1;
	return ObjInst;
}

ObjectInstance * ObjectManager::operator[](int i)
{
	if (static_cast<size_t>(i) < m_objects.size())
		return m_objects[i];
	return 0;
}
union PtrInst
{
	int ptr_num;
	ObjectInstance * ptr_inst;
};
bool CompareObjectInstances(ObjectInstance * a, ObjectInstance * b)
{
	/*

	if (a->GetPtrModel() != b->GetPtrModel())
		return a->GetPtrModel() < b->GetPtrModel();
	if (a->GetPtrTexture() != b->GetPtrTexture())
		return a->GetPtrTexture() < b->GetPtrTexture();
	PtrInst a_ptr, b_ptr;
	a_ptr.ptr_inst = a;
	b_ptr.ptr_inst = b;
	return a_ptr.ptr_num < b_ptr.ptr_num;
	*/
	char a_c = OBJMAP_INST->GetModelIndex(a->GetModel());
	char b_c = OBJMAP_INST->GetModelIndex(b->GetModel());
	return a_c < b_c;
}
bool CheckDeleted(ObjectInstance * obj)
{
	bool cond = obj->CheckDelete();
	if (cond)
		obj->free();
	return cond;
}

void ObjectManager::Update()
{
	if (flag & 0x1)
		std::sort(m_objects.begin(), m_objects.end(), CompareObjectInstances);
	if (flag & 0x2)
	{
		std::cout << "Old: " << m_objects.size() << "\n";
		std::vector<ObjectInstance *>::iterator itr = std::remove_if(m_objects.begin(), m_objects.end(), CheckDeleted);
		m_objects.erase(itr, m_objects.end());
		std::cout << "New: " << m_objects.size() << "\n";
	}
	flag = 0;

	size_t size = m_objects.size();
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->Update();
		if (m_objects[d]->CheckDelete())
			flag |= 0x2;
	}
}

void ObjectManager::RenderShadow()
{
	size_t size = m_objects.size();
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->renderShadow();
	}
}

void ObjectManager::Render()
{
	size_t size = m_objects.size();
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->render();
	}
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->renderAABB();
	}
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->renderBS();
	}
	for (size_t d = 0; d < size; ++d)
	{
		m_objects[d]->renderOBB();
	}
}
void ObjectManager::Free()
{
	size_t size = m_objects.size();
	for (size_t d = 0; d < size; ++d)
	{
		//std::cout << "delete:" << m_objects[d] << std::endl;
		delete m_objects[d];
	}
	m_objects.clear();
}

void ObjectManager::RemoveObject(ObjectInstance * obj)
{
	flag |= 0x2;
	obj->SetDelete();
}

void ObjectManager::SetDebugRender(int type)
{
	size_t size = m_objects.size();
	if (type == -1)
	{
		for (size_t d = 0; d < size; ++d)
		{
			m_objects[d]->UnsetFlag(RENDER_AABB | RENDER_BS);
		}
	}
	if (type == 0)
	{
		for (size_t d = 0; d < size; ++d)
		{
			m_objects[d]->SetFlag(RENDER_AABB);
			m_objects[d]->UnsetFlag(RENDER_BS);
		}
	}
	if (type == 1)
	{
		for (size_t d = 0; d < size; ++d)
		{
			m_objects[d]->SetFlag(RENDER_BS);
			m_objects[d]->UnsetFlag(RENDER_AABB);
		}
	}
}

void ObjectManager::UnsetDebugRender()
{
	SetDebugRender(-1);
}