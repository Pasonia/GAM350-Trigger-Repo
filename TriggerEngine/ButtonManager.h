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
					  Vec3 position_, Vec3 scale_, void(*function_)());
	void CreateButton(const std::string& name_, const std::string& trigger_,
					  Vec2 position_, Vec2 scale_, void(*function_)());

	static ButtonManager* GetInstance();

	void SetCursorPosition(const Vec3& position_);

	void Update();
	void Render();
	void Free();
	void Destroy();

private:
	ButtonManager();
	~ButtonManager();

	void ConstructBoundingGeometry(Button* button_);
	void ConstructBoundingGeometry(const Model* model_);

	bool PointCollideAABB(const BoundShape& aBox, const Vec3& aPoint);

	static ButtonManager* m_instance;

	ButtonContainer m_buttonCont;
	TriggerContainer m_triggerCont;

	Vec3 m_Cursor;
};

#define ButtonManager_Inst ButtonManager::GetInstance()

#endif
