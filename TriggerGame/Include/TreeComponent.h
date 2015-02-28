//1. Set up component header as shown.

#ifndef TREECOMPONENT_H
#define TREECOMPONENT_H

#include "Component.h"
#include "TEMath.h"
#include <vector>

class TrunkInst
{
public:
	TrunkInst(Vec3 pos, Vec3 rot, Vec3 sca, Mat4 t) : position(pos), rotation(rot), scale(sca), transf(t){ }
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
	Mat4 transf;
};

class Tree : public Component
{
public:
	Tree(ObjectInstance * parent);
	Mat4 worldMat;
	~Tree();
	void Init(); //must be defined
	void Update(); //must be defined
	void Free(); //must be defined
	void Render();
	void ForwardRender();
private:
	Vec3 tScale;
	std::vector<TrunkInst> trunks;
	std::vector<TrunkInst> leaves;
};

#endif

//Next -> Go to SampleComponent.cpp