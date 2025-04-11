#pragma once
#include "Singleton.h"

class ObjectManager : public Singleton<ObjectManager>
{
public:
	void Init();	
	void Update();
	void Release();
};

