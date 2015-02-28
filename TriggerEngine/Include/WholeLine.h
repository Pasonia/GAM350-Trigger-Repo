/******************************************************************************/
/*!
\file WholeLine.h
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
This file contains function definitions for WholeLine class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef WHOLELINE_H_
#define WHOLELINE_H_

#include <iostream>
#include <fstream>
#include <string>

//declare a type that allows usage of std::getline
struct WholeLine
{
  std::string mLongLineData;

  operator std::string() const
  {
      return mLongLineData;
  }
};

//operator overload of extraction operator for WholeLine
std::istream& operator>>(std::istream& in, WholeLine& data);

#endif