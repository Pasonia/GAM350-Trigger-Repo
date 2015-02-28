//2. Provide implementation details. This is a sample of what you can do.
//The bulk of your component development goes right here.
//CheckParent functionality

#include "SampleComponent.h"
#include "ObjectInstance.h" //Object Instance functions
//If you are accessing other components' functions, you must include them here as well.
//I'm aware this creates a tight dependency, but unless we get a global message board, things
//will stay this way. So be careful of your derived components' dependencies and make sure
//you stay alert when you develop new components that talk to others.
#include <iostream>

SampleComponent::SampleComponent(ObjectInstance * parent) :
Component(parent), m_value(0)
{

}
SampleComponent::~SampleComponent()
{

}
void SampleComponent::Init() //must be defined
{
	std::cout << "Sample Component Init\n";
}
void SampleComponent::Update() //must be defined
{
	std::cout << "Sample Component Update\n";
}
void SampleComponent::Free() //must be defined
{

	std::cout << "Sample Component Free\n";
}
const int& SampleComponent::GetValue()
{
	return m_value;
}
void SampleComponent::SetValue(int value)
{
	m_value = value;
}

//Note: This implementation is not involved because it simply is checking for existence.
//If you want to access anything in Physics component you MUST include "Physics.h"
void SampleComponent::CheckParentIfComponentExists()
{
	Component * phy = GetParent()->GetComponent(Component::CT_PHYSICS);
	if (phy)
	{
		std::cout << "Parent has Physics Component\n";
	}
	else
	{
		std::cout << "Parent does not have Physics Component\n";
	}
}

//Next -> FactoryController.cpp bottom, titled "Sample Component initialization"