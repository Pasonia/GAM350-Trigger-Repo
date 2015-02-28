/******************************************************************************/
/*!
\file ButtonManager.h
\author Leung Kwan Yee, TEAM TRIGGER
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

#ifndef _BUTTONMANAGER_H_
#define _BUTTONMANAGER_H_

#include <map>
#include <vector>

#include "GUI2D.h"
#include "Model.h"
#include "Button.h"
#include "TEMath.h"
#include "System.h"
#include "TriggerInput.h"

typedef std::map<std::string, Button> ButtonContainer;
typedef std::map<std::string, std::string> TriggerContainer;

class ButtonManager
{
public:
	void CreateButton(const std::string& name_, const std::string& trigger_, 
					  Vec3 position_, Vec3 scale_, void(*function_)(void*));
	void CreateButton(const std::string& name_, const std::string& trigger_,
					  Vec2 position_, Vec2 scale_, void(*function_)(void*));

	static ButtonManager* GetInstance();

	void SetCursorPosition(const Vec3& position_);
	void SetWindowSize(const Vec3& position_);
	void SetHiddenState(const std::string& name_);
	void SetAppearState(const std::string& name_);
	void SetAllHidden();

	bool GetHiddenState(const std::string& name_) ;

	void Update();
	void Render();
	void Free();
	void Destroy();

private:
	ButtonManager();
	~ButtonManager();

	void ConstructBoundingGeometry(Button* button_);
	void ConstructBoundingGeometry(const Model* model_);

	bool PointCollideAABB(const BoundShape& aBox, const Vec3& bPoint);

	static ButtonManager* m_instance;

	ButtonContainer m_buttonCont;
	TriggerContainer m_triggerCont;

	Vec3 m_cursor;
	Vec3 m_windowSize;
};

#define ButtonManager_Inst ButtonManager::GetInstance()

#endif
