/******************************************************************************/
/*!
\file   WholeLine.cpp
\author Long Jiquan Joel
\par    email: guangtian.guo\@digipen.edu
\par	email: julian.teh\@digipen.edu
\par	email: shikang.n\@digipen.edu
\par	email: longjiquan.joel\@digipen.edu
\par    DigiPen guangtian.guo
\par	DigiPen julian.teh
\par	Digipen shikang.n
\par	Digipen longjiquan.joel
\par    Course: GAM200
\par    Game Name: TeleK
\date   01/8/2013
\brief  
    This file contains definitions to the WholeLine object.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "WholeLine.h"

//operator overload of extraction operator for WholeLine
std::istream& operator>>(std::istream& in, WholeLine& data)
{
  std::getline(in,data.mLongLineData);
  return in;
}