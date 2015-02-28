/******************************************************************************/
/*!
\file System.cpp
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

#include "System.h"
#include "TriggerAssert.h"
#include "JSONWrapper.h"
#include "ObjectMap.h"
#include "Factory.h"
#include "PhysicsManager.h"
#include "ObjectManager.h"

System::System(unsigned int height, unsigned int width) :mWindow(0),
														 mWindowH(height),
														 mWindowW(width),
														 mGameName("Wayward Beta Demo by Trigger Games"),
														 mNear(1.f),
														 mFar(1000.f) // 3000
{
	Debugger_ = new Debugger();
	gProfile = new Profiler();
	ASSERT(Debugger_, "Debugger failed.");

	mSettings.fullScreen = false;
}

System::~System()
{
	//if (Debugger_)
	//	delete Debugger_;

	if (gProfile)
		delete gProfile; 

	if (mGraphic)
		delete mGraphic;

	if (mFont)
		delete mFont;
	
	if (mOpenGL)
		delete mOpenGL;

	//delete model loader
	ModelLoader_Inst->Destroy();
	TextureLoader_Inst->Destroy();
	TextureManager_Inst->Destroy();
	ModelManager_Inst->Destroy();
	//delete timer
	TIMER_INST->Destroy();
	OBJMAP_INST->Destroy();
	FACTORY_INST->Destroy();
	SOUNDMGR_INST->Destroy();
	OBJECTMANAGER_INST.Free();
	PhysicsManager_Inst->Destroy();
	Shutdown();
}

bool System::Init()
{ 
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	RECT DesktopSize;
	GetWindowRect(GetDesktopWindow(), &DesktopSize);

	if (mSettings.fullScreen)
	{
		mWindowW = DesktopSize.right;
		mWindowH = DesktopSize.bottom;
		//Open a window and create its OpenGL context
		mWindow = glfwCreateWindow(mWindowW, mWindowH, mGameName.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		mWindow = glfwCreateWindow(mWindowW, mWindowH, mGameName.c_str(), NULL, NULL);
	}
	if (mWindow == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.2 compatible.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowPos(mWindow, DesktopSize.right/8, DesktopSize.bottom/12);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(mWindow, mWindowH / 2, mWindowW / 2);
	
	//create opengl
	if (CreateOpenGL() == false)
	{
		fprintf(stderr, "Failed to create openGL");
		glfwTerminate();
		return false;
	}
	//create graphic
	if (CreateGraphic() == false)
	{
		fprintf(stderr, "Failed to create Graphic");
		glfwTerminate();
		return false;
	}
	//create bounding box
	ModelManager_Inst->CreateBoundingBox();
	//create time
	Timer::GetInstance()->Init();
	//start FONT
	mFont = new TextureMapFont;

	//Ant Tweak Callbacks
	glfwSetMouseButtonCallback(mWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(mWindow, (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(mWindow, (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(mWindow, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(mWindow, (GLFWcharfun)TwEventCharGLFW3);

	LoadSystemSettings();
	std::cout << mSettings.bgmVolume << " " << mSettings.sfxVolume << std::endl;
	SOUNDMGR_INST->SetVolumeChannelGroup(static_cast<float>(1.0f / 100 * mSettings.bgmVolume), CHANNEL_TYPES::BGM);
	SOUNDMGR_INST->SetVolumeChannelGroup(static_cast<float>(1.0f / 100 * mSettings.sfxVolume), CHANNEL_TYPES::SFX);
	SOUNDMGR_INST->LoadSound("MainBGM", "../Resource/Audio/BGM/WaywardGameTheme02.ogg", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, true);
	SOUNDMGR_INST->LoadSound("ping1", "../Resource/Audio/SFX/collectping1.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping2", "../Resource/Audio/SFX/collectping2.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping3", "../Resource/Audio/SFX/collectping3.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping4", "../Resource/Audio/SFX/collectping4.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping5", "../Resource/Audio/SFX/collectping5.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping6", "../Resource/Audio/SFX/collectping6.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping7", "../Resource/Audio/SFX/collectping7.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("ping8", "../Resource/Audio/SFX/collectping8.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("HitWall",  "../Resource/Audio/SFX/HitObstacle.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("PickUp", "../Resource/Audio/SFX/Pickup.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("EnemyAlerted", "../Resource/Audio/SFX/EnemyAlerted.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("Menugoto", "../Resource/Audio/SFX/Menu_goto.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("Menuback", "../Resource/Audio/SFX/Menu_back.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("Menuhover","../Resource/Audio/SFX/Menu_hover.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	SOUNDMGR_INST->LoadSound("Menuenter","../Resource/Audio/SFX/Menu_enter.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, false);
	//SOUNDMGR_INST->StartPlayingSound("MainBGM");
	
	//LoadPathNamesFromJSON("../Resource/GameGlobalFiles/Paths/paths.json");
	//FOLDERMANAGER_INST->Initialize("../Resource/GameGlobalFiles/Paths/paths.json");
	
	return true;
}

void System::DisplayCursor(bool tbool)
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, (tbool) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

Debugger * System::GetDebugger()
{
	return Debugger_;
}


OpenGL* System::GetOpenGL()const
{
	return mOpenGL;
}

bool System::CreateOpenGL()
{
	mOpenGL = new OpenGL;

	if (mOpenGL->InitOpenGL(mWindow, mWindowH, mWindowW,mNear,mFar) == false)
		return false;

	return true;
}

bool System::CreateGraphic()
{
	mGraphic = new Graphic;

	if (mGraphic->DeferredStartUp(mOpenGL,mWindowH, mWindowW) == false)
		return false;

	return true;
}

void System::Shutdown()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

System* System::GetInstance()
{
	if (!sEngine)
		sEngine = new System;

	return sEngine;
}

bool System::Run()
{
	//TIMER_INST->StartTimer();
	//if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
	if (glfwWindowShouldClose(mWindow) != 0)
		return false;
	if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS ||
		glfwWindowShouldClose(mWindow) != 0)
	{
		if (Debugger_ && Debugger_->GetIsUsingAT())
			Debugger_->GetIsATVisible() = !Debugger_->GetIsATVisible();
	}

	return true;
}
void System::LoadSystemSettings()
{
	Scripter::Document doc;
	GetJSONObject(&doc, "../Resource/System/settings.json");
	if (doc.IsObject())
	{
		Scripter::GetInt(&doc, "ResolutionX", &mSettings.resolution_x);
		Scripter::GetInt(&doc, "ResolutionY", &mSettings.resolution_y);
		Scripter::GetBool(&doc, "FullScreen", &mSettings.fullScreen);
		Scripter::GetBool(&doc, "Mute", &mSettings.mute);
		Scripter::GetInt(&doc, "BGM_Volume", &mSettings.bgmVolume);
		Scripter::GetInt(&doc, "SFX_Volume", &mSettings.sfxVolume);
		Scripter::GetInt(&doc, "WindowedResolutionX", &mSettings.windowedres_x);
		Scripter::GetInt(&doc, "WindowedResolutionY", &mSettings.windowedres_y);
		std::cout << "System Settings\n==============\n";
		std::cout << mSettings.resolution_x << " " << mSettings.resolution_y << "\n"
				  << mSettings.fullScreen << " " << mSettings.mute << "\n"
				  << mSettings.bgmVolume << " " << mSettings.sfxVolume << "\n";
	}
}
Vec3 System::GetWindowPos()
{
	int x;
	int y;
	glfwGetWindowPos(mWindow, &x, &y);
	return Vec3(x, y, 0);
}
Vec3 System::GetWindowSize()
{
	return Vec3(mWindowW, mWindowH, 0);
}

Vec2 System::GetWindowVPSize()
{
	return Vec2(mWindowW, mWindowH);
}

void System::SetWindowSizeToShader(GLuint ProgID)
{
	int loc = glGetUniformLocation(ProgID, "WINDOWSIZE");
	glUniform2fv(loc, 1, glm::value_ptr(Vec2(mWindowW,mWindowH)));
}

Camera* System::GetCamera()
{
	return mGraphic->GetCamera();
}

Graphic* System::GetGraphic()
{
	return mGraphic;
}

bool System::IsFullScreen()
{
	return mSettings.fullScreen;
}

TextureMapFont* System::GetFont()
{
	return mFont;
}





