#ifndef LIGHTORBS_H
#define LIGHTORBS_H

#include "Component.h"

class LightOrbs : public Component
{
public:
	LightOrbs(ObjectInstance* parent_);

	void Update();
	void Free();



private:

};

#endif
