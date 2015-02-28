//1. Set up component header as shown.

#ifndef SAMPLECOMPONENT_H
#define SAMPLECOMPONENT_H

#include "Component.h"

class SampleComponent : public Component
{
public:
	SampleComponent(ObjectInstance * parent);
	~SampleComponent();
	void Init(); //must be defined
	void Update(); //must be defined
	void Free(); //must be defined

	//All definitions below are your own implementations.
	const int& GetValue();
	void SetValue(int value);
	void CheckParentIfComponentExists();
private:
	int m_value;
};

#endif

//Next -> Go to SampleComponent.cpp