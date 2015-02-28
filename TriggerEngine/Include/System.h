/******************************************************************************/
/*!
\file System.h
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
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include "Opengl.h"
#include "Graphic.h"
#include "StateManager.h"
#include "AntTweakBar/AntTweakBar.h"
#include "Debugger.h"
#include "Timer.h"
#include "TriggerInput.h"
#include "Audio.h"
#include "Font.h"
#include "GUI2D.h"
#include "Particle.h"
#include "Grass.h"
#include "TerrainHeight.h"
//#include "FolderManager.h"

struct SystemSettings
{
	int resolution_x;
	int resolution_y;
	int windowedres_x;
	int windowedres_y;
	int bgmVolume;
	int sfxVolume;
	bool fullScreen;
	bool mute;
};

class System
{
public:
	System(unsigned int height = 720, unsigned int width = 1280);
	~System();
	static System* GetInstance();
	bool Init();
	void Shutdown();
	bool Run();
	void DisplayCursor(bool tbool);
	Debugger * GetDebugger();
	OpenGL* GetOpenGL()const;
	Camera* GetCamera();
	TextureMapFont* GetFont();
	void SetWindowSizeToShader(GLuint ProgID);
	//game state
	StateManager gameStateMgr;
	Vec3 GetWindowPos();
	Vec3 GetWindowSize();
	Vec2 GetWindowVPSize();
	Graphic * GetGraphic();
	Profiler * GetProfiler() { return gProfile; }
	bool IsFullScreen();
private:
	bool CreateOpenGL();
	bool CreateGraphic();
	Profiler * gProfile;
	Debugger * Debugger_;
	GLFWwindow* mWindow;
	unsigned int mWindowW, mWindowH;
	float mNear, mFar;
	std::string mGameName;
	//graphic
	SystemSettings mSettings;
	OpenGL* mOpenGL;
	void LoadSystemSettings();
	Graphic *mGraphic;
	TextureMapFont *mFont;
};

//global
static System* sEngine = 0;
static SYSTEMINPUT input;
inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods){ TwEventMouseButtonGLFW(button, action); }
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos){ TwMouseMotion(int(xpos), int(ypos)); }
inline void TwEventMouseWheelGLFW3(GLFWwindow* window, int xoffset, int yoffset){ TwEventMouseWheelGLFW(yoffset); }
inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods){ TwEventKeyGLFW(key, action); }
inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint){ TwEventCharGLFW(codepoint, GLFW_PRESS); }

#endif
