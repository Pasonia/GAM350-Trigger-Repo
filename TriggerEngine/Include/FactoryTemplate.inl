template <typename T>
T * CreateComponentFromFactory(unsigned type, const rapidjson::Value * obj, ObjectInstance * parent)
{
	Component * comp = FACTORY_INST->CreateComponent(type, &obj, parent);
	return reinterpret_cast<T*>(comp);
}

template <typename T>
T * CreateEmptyComponentFromFactory(unsigned type, ObjectInstance * parent)
{
	Component * empty_comp = FACTORY_INST->CreateEmptyComponent(type, parent);
	return reinterpret_cast<T*>(empty_comp);
}