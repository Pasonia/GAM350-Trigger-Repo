/******************************************************************************/
/*!
\file LevelText.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for LevelText class object.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "LevelText.h"

LevelText::LevelText(std::string text, Vec3 pos, float scale, char* texture) : m_text(text), m_pos(pos), m_scale(scale)
{
  cube = OBJECTMANAGER_INST.CreateObjectInstance("Box", texture);
  cube->SetRender(false);
  cube->SetScale(scale + 0.3f, scale + 0.3f, 0.3f);
  cube->SetPosition(pos);
  m_render = false;
}

void LevelText::Render()
{
  if (!m_render)
    return;

  Vec3 lPos(0,0,0);
  for (unsigned letter = 0; letter < m_text.size(); ++letter)
  {
    char index;

    if (m_text[letter] != ' ')
    {
      if (m_text[letter] >= 65 && m_text[letter] <= 90)
        index = m_text[letter] - 65;

      if (m_text[letter] >= 48 && m_text[letter] <= 57)
        index = m_text[letter] - 48 + 26;

      cube->SetRender(true);
      Vec3 cPos = m_pos + lPos;
      cPos.x -= m_scale;
      cPos.y += m_scale * 2;
      bool * curr = Letters[index];
      for (int i = 0; i < 5; ++i)
      {
        for (int j = 0; j < 3; ++j)
        {
          if (curr[i * 3 + j] == 1)
          {
            cube->SetPosition(cPos + Vec3(m_scale, 0.f, 0.f) * (float)j - Vec3(0.f, m_scale, 0.f) * (float)i);
            cube->Update();
            cube->render();
          }
        }
      }
    }
    
    lPos += Vec3(m_scale * 3 + m_scale, 0, 0);
  }
  
  
}