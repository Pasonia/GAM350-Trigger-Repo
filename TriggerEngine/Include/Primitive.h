/******************************************************************************/
/*!
\file Primitive.h
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
#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <iostream>
#include <vector>
#include "TEMath.h"

class Grid
{
public:
	Grid();
	~Grid();
	const std::vector<Vec4>& Get_grid();
	float Get_Cell_Length() const { return mCell_length; }
	
private:
	std::vector<Vec4> mCoords;
	unsigned mCol;
	float mCell_length;
};


#endif
