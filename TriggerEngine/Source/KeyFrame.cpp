/******************************************************************************/
/*!
\file KeyFrame.cpp
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

#include "KeyFrame.h"

PositionKeyFrame::PositionKeyFrame(float time, Vec4 position) : m_Time(time), 
																m_Position(position)
{
}

PositionKeyFrame::PositionKeyFrame(aiVectorKey& vectorkey) : m_Time((float)vectorkey.mTime), 
															 m_Position(Vec4(vectorkey.mValue.x, vectorkey.mValue.y, vectorkey.mValue.z, 1.0f)) 
{
}

RotationKeyFrame::RotationKeyFrame(float time, glm::quat rotation) : m_Time(time), 
																	 m_Rotation(rotation)
{
}

RotationKeyFrame::RotationKeyFrame(aiQuatKey& vectorkey) : m_Time((float)vectorkey.mTime), 
														   m_Rotation(glm::quat(vectorkey.mValue.x, vectorkey.mValue.y, vectorkey.mValue.z, vectorkey.mValue.w)) 
{
}

ScaleKeyFrame::ScaleKeyFrame(float time, Vec4 scale) : m_Time(time), 
													   m_Scale(scale)
{
}

ScaleKeyFrame::ScaleKeyFrame(aiVectorKey& vectorkey) : m_Time((float)vectorkey.mTime), 
													   m_Scale(Vec4(vectorkey.mValue.x, vectorkey.mValue.y, vectorkey.mValue.z, 1.0f))
{
}

