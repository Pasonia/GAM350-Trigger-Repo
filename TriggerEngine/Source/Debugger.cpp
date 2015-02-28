/******************************************************************************/
/*!
\file Debugger.cpp
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
#include "Debugger.h"
#include "System.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

//convert number to string
std::string Convert(float number){
	std::ostringstream buff;
	buff << number;
	return buff.str();
}
/**************************************

	AntTweakBar Wrapper functions

***************************************/

TweakBar::TweakBar(const char * barName)
{
	bar = TwNewBar(barName);
	m_barName = barName;
	TweakBarRefreshRate(0.2f);
	m_visible = true;
}

void TweakBar::TweakBarAdd(const char * varName, 
						   TwType TWTYPE,
						   void *var,
						   const char *def)
{
	if (TWTYPE == TW_TYPE_STDSTRING)
	{
		TwCopyStdStringToClientFunc(CopyStdStringToClient);
	}
	TwAddVarRW(this->bar, varName, TWTYPE, var, def);
}

void TweakBar::TweakBarAddButton(const char * varName, TwButtonCallback callback, void *var, const char *def)
{
	TwAddButton(this->bar, varName, callback, var, def);
}

void TweakBar::TweakBarAddEnum(const char * varName, std::string title, std::string enumList, void *var, const char *def)
{
	TwType enumType;
	std::string * titleP = new std::string(title);
	enumStorage.push_back(titleP);
	std::string * enumListP = new std::string(enumList);
	enumStorage.push_back(enumListP);
	// Defining new enum type
	enumType = TwDefineEnumFromString(titleP->c_str(), enumListP->c_str());

	TwAddVarRW(this->bar, varName, enumType, var, def);
}

void TweakBar::TweakBarAddSeparator()
{
	TwAddSeparator(this->bar, "", "");
}

TweakBar::~TweakBar()
{
	for (size_t i = 0; i < enumStorage.size(); ++i)
	{
		delete enumStorage[i];
	}
}

void TweakBar::TweakBarRefreshRate(float f)
{
	std::string tempName(m_barName);
	tempName.append(" refresh=");
	tempName.append(Convert(f));
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarFitCol()
{
	std::string tempName(m_barName);
	tempName.append("  valueswidth=fit ");
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarRemoveVar(const char * varName)
{
	TwRemoveVar(this->bar, varName);
}

void TweakBar::TweakBarDelete()
{
	TwDeleteBar(bar);
}

void TweakBar::TweakBarHide()
{
	std::string tempName(m_barName);
	tempName.append(" visible=false");
	TwDefine(tempName.c_str());
	m_visible = false;
}

void TweakBar::TweakBarShow()
{
	std::string tempName(m_barName);
	tempName.append(" visible=true");
	TwDefine(tempName.c_str());
	m_visible = true;
}

void TweakBar::TweakBarBGDark()
{
	std::string tempName(m_barName);
	tempName.append(" color='0 0 0'");
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarDarkText()
{
	std::string tempName(m_barName);
	tempName.append(" text=dark");
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarLightText()
{
	std::string tempName(m_barName);
	tempName.append(" text=light");
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarSetPos(int x, int y)
{
	std::string tempName(m_barName);
	char tmp[100];
	sprintf_s(tmp, " position=\'%d %d\' ", x, y);
	tempName.append(tmp);
	TwDefine(tempName.c_str());
	m_visible = true;
}

void TweakBar::TweakBarSetSize(int w, int h)
{
	std::string tempName(m_barName);
	char tmp[100];
	sprintf_s(tmp, " size=\'%d %d\' ", w, h);
	tempName.append(tmp);
	TwDefine(tempName.c_str());
}

void TweakBar::TweakBarSetAlpha(int alpha)
{
	std::string tempName(m_barName);
	char tmp[100];
	sprintf_s(tmp, " alpha=\'%d\' ", alpha);
	tempName.append(tmp);
	TwDefine(tempName.c_str());
}

bool TweakBar::GetTwVisible()
{
	return m_visible;
}

bool TweakBar::IsMouseinBar(const Vec3& mousePos)
{
	int pos[2], size[2];
	TwGetParam(this->bar, NULL, "position", TW_PARAM_INT32, 2, pos);
	TwGetParam(this->bar, NULL, "size", TW_PARAM_INT32, 2, size);
	return (mousePos.x >= pos[0] && mousePos.x < pos[0] + size[0] && mousePos.y >= pos[1] && mousePos.y < pos[1] + size[1]);
}

/**************************************

Profiler Functions

***************************************/

Profiler::Profiler()
{
	profiledTime = 0.0f;
}

void Profiler::StartProfile()
{
	startInfo = TIMER_INST->GetStartTime();
}

float Profiler::EndProfile()
{
	profiledTime = TIMER_INST->GetTimeElapsedSinceStart(startInfo);
	return profiledTime;
}


/**************************************

Debugger functions

***************************************/

Debugger::Debugger():isUsingAntTweak(true), isAntTweakVisible(true){}

Debugger::~Debugger(){};

TweakBar * Debugger::CreateBar(const char * barName)
{
	TweakBar * tmp = new TweakBar(barName);
	bars.push_back(tmp);

	return tmp;
}

void Debugger::DeleteBar(TweakBar * twInst)
{
	std::vector<TweakBar*>::iterator ir = std::remove(bars.begin(), bars.end(), twInst);
	bars.erase(ir, bars.end());
	twInst->TweakBarDelete();
	if (twInst)
	{
		delete twInst;
	}
}

void Debugger::DebuggerInit()
{
	if (isUsingAntTweak)
	{
		TwInit(TW_OPENGL_CORE, NULL);
		//RECT DesktopSize;
		//GetWindowRect(GetDesktopWindow(), &DesktopSize);
		Vec3 size = System::GetInstance()->GetWindowSize();
		TwWindowSize((int)size.x, (int)size.y);
		//TwWindowSize(DesktopSize.right, DesktopSize.bottom);
	}
}

void DebuggerUpdate()
{
	
}

void Debugger::DebuggerDraw()
{
	if (isUsingAntTweak && isAntTweakVisible)
	{
		TwDraw();
	}
}

void Debugger::DebuggerFree()
{
	if (isUsingAntTweak)
	{
		for (unsigned i = 0; i < bars.size(); ++i)
		{
			bars[i]->TweakBarDelete();
			//bars[i]->enumStorage.clear();
			delete bars[i];
		}
		bars.clear();
		TwTerminate();
	}
}

//Setters and getters
bool Debugger::GetIsUsingAT()
{
	return isUsingAntTweak;
}

bool& Debugger::GetIsATVisible()
{
	return isAntTweakVisible;
}

void Debugger::HideAntTweak()
{
	isAntTweakVisible = !isAntTweakVisible;
}