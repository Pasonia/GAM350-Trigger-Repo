/******************************************************************************/
/*!
\file Debugger.h
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

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <AntTweakBar/AntTweakBar.h>
#include <vector>
#include <string>
#include "TEMath.h"
#include "Timer.h"

std::string Convert(float number);

class TweakBar
{
public:
	TweakBar(const char * barName);
	~TweakBar();
	void TweakBarAdd(const char * varName, TwType TWTYPE, void *var, const char *def);
	void TweakBarAddButton(const char * varName, TwButtonCallback callback, void *var, const char *def);
	void TweakBarAddEnum(const char * varName, std::string title, std::string enumList, void *var, const char *def);
	void TweakBarAddSeparator();
	void TweakBarHide();
	void TweakBarShow();
	void TweakBarDarkText();
	void TweakBarLightText();
	void TweakBarBGDark();
	void TweakBarRefreshRate(float f);
	void TweakBarFitCol();
	void TweakBarSetPos(int x, int y);
	void TweakBarSetSize(int w, int h);
	void TweakBarSetAlpha(int alpha);
	void TweakBarRemoveVar(const char * varName);
	void TweakBarDelete();
	bool GetTwVisible();
	bool IsMouseinBar(const Vec3& mousePos);
	std::vector<std::string *> enumStorage;
private:
	TwBar * bar;
	std::string m_barName;
	bool m_visible;
};

class Profiler
{
public:
	Profiler();
	void StartProfile();
	float EndProfile();
	float& GetProfiledTime() { return profiledTime; }
private:
	LARGE_INTEGER startInfo;
	float profiledTime;
};

class Debugger
{
public:
	Debugger();
	TweakBar * CreateBar(const char * barName);
	void DeleteBar(TweakBar * twInst);
	void DebuggerInit();
	void DebuggerDraw();
	void DebuggerFree();
	void DebuggerUpdate();
	void HideAntTweak();
	bool GetIsUsingAT();
	bool& GetIsATVisible();
	Profiler& GetRenderProfile() { return rProfile; }
	~Debugger();

private:
	std::vector<TweakBar *> bars;
	Profiler rProfile;
	bool isUsingAntTweak;
	bool isAntTweakVisible;
};

#endif 