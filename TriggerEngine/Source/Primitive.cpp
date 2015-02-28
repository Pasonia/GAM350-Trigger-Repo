/******************************************************************************/
/*!
\file Primitive.cpp
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

#include "Primitive.h"

Grid::Grid() : mCell_length(1.f),
			   mCol(256)
{
}

Grid::~Grid()
{
}

const std::vector<Vec4>& Grid::Get_grid()
{
	float halfrow = 0.5f;
	float halfcolumn = (float)mCol / 2;

	for (unsigned i = 0; i <= 1; ++i)
	{
		mCoords.push_back(Vec4(-halfcolumn * mCell_length, 0, (-halfrow + i) * mCell_length, 1.f));
		mCoords.push_back(Vec4(halfcolumn * mCell_length, 0, (-halfrow + i) * mCell_length, 1.f));
	}
	for (unsigned i = 0; i <= mCol; ++i)
	{
		mCoords.push_back(Vec4((-halfcolumn + i) * mCell_length, 0, -halfrow * mCell_length, 1.f));
		mCoords.push_back(Vec4((-halfcolumn + i) * mCell_length, 0, halfrow * mCell_length, 1.f));
	}

	return mCoords;
}

