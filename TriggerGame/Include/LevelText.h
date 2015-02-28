/******************************************************************************/
/*!
\file LevelText.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for LevelText class object.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef LEVELTEXT_H_
#define LEVELTEXT_H_

#include <string>
#include "mainGame.h"
#include "ObjectManager.h"
#include "TEMath.h"

namespace
{
  bool A[15] = { 1, 1, 1,
                 1, 0, 1,
                 1, 1, 1,
                 1, 0, 1,
                 1, 0, 1 };
  bool B[15];
  bool C[15] = { 1, 1, 1,
				 1, 0, 0,
				 1, 0, 0,
				 1, 0, 0,
				 1, 1, 1 };
  bool D[15];
  bool E[15] = { 1, 1, 1,
                 1, 0, 0,
                 1, 1, 0,
                 1, 0, 0,
                 1, 1, 1 };

  bool F[15];
  bool G[15] = { 1, 1, 1,
                 1, 0, 0,
                 1, 0, 0,
                 1, 0, 1,
                 1, 1, 1 };
  bool H[15];
  bool I[15];
  bool J[15];
  bool K[15];
  bool L[15] = { 1, 0, 0,
				 1, 0, 0,
				 1, 0, 0,
				 1, 0, 0,
				 1, 1, 1 };
  bool M[15] = { 1, 1, 1,
				 1, 1, 1,
				 1, 1, 1,
				 1, 0, 1,
				 1, 0, 1 };
  bool N[15];
  bool O[15] = { 1, 1, 1,
				 1, 0, 1,
				 1, 0, 1,
				 1, 0, 1,
				 1, 1, 1 };
  bool P[15] = { 1, 1, 1,
				 1, 0, 1,
				 1, 1, 1,
				 1, 0, 0,
				 1, 0, 0 };
  bool Q[15];
  bool R[15] = { 1, 1, 1,
				 1, 0, 1,
				 1, 1, 1,
				 1, 1, 0,
				 1, 0, 1 };
  bool S[15] = { 1, 1, 1,
                 1, 0, 0,
                 1, 1, 1,
                 0, 0, 1,
                 1, 1, 1 };
  bool T[15] = { 1, 1, 1,
                 0, 1, 0,
                 0, 1, 0,
                 0, 1, 0,
                 0, 1, 0 };
  bool U[15];
  bool V[15] = { 1, 0, 1,
				 1, 0, 1,
				 1, 0, 1,
				 1, 1, 1,
				 0, 1, 0 };
  bool W[15];
  bool X[15];
  bool Y[15];
  bool Z[15];

  bool num0[15] = { 1, 1, 1,
					1, 0, 1,
					1, 0, 1,
					1, 0, 1,
					1, 1, 1 };
  bool num1[15] = { 1, 1, 0,
                    0, 1, 0,
                    0, 1, 0,
                    0, 1, 0,
                    1, 1, 1 };
  bool num2[15] = { 1, 1, 1,
                    0, 0, 1,
                    0, 1, 0,
                    1, 0, 0,
                    1, 1, 1 };
  bool num3[15] = { 1, 1, 1,
                    0, 0, 1,
                    0, 1, 1,
                    0, 0, 1,
                    1, 1, 1 };
  bool num4[15] = { 1, 0, 1,
                    1, 0, 1,
                    1, 1, 1,
                    0, 0, 1,
                    0, 0, 1 };
  bool num5[15] = { 1, 1, 1,
				    1, 0, 0,
				    1, 1, 1,
				    0, 0, 1,
				    1, 1, 1 };
  bool num6[15] = { 1, 1, 1,
					1, 0, 0,
					1, 1, 1,
					1, 0, 1,
					1, 1, 1 };
  bool num7[15] = { 1, 1, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1 };
  bool num8[15] = { 1, 1, 1,
					1, 0, 1,
					1, 1, 1,
					1, 0, 1,
					1, 1, 1 };
  bool num9[15] = { 1, 1, 1,
					1, 0, 1,
					1, 1, 1,
					0, 0, 1,
					1, 1, 1 };

  bool * Letters[36] {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                      num0, num1, num2, num3, num4, num5, num6, num7, num8, num9 };
}

class LevelText
{
public:
  LevelText(std::string text, Vec3 pos, float scale, char * texture);
  void Render();
  void Update();
  void SetText(std::string text) { m_text = text; }
  Vec3& GetPos() { return m_pos; }
  float& GetScale() { return m_scale; }
  std::string GetText() { return m_text; }
  void SetRender(bool render) { m_render = render; }
private:
  ObjectInstance * cube;
  std::string m_text;
  Vec3 m_pos;
  float m_scale;
  bool m_render;
};

#endif