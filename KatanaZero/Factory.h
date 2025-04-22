#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "GameObject.h"
#include "SimpleTestObject.h"
using namespace std;


// 맵툴에 필요한 모든 오브젝트
static unordered_map<string, function<GameObject* ()>> ObjectInfo = {
	{"SimpleTestObject", []() {return new SimpleTestObject(); } },
};

GameObject* CreateObject(string ClassName)
{
	auto iter = ObjectInfo.find(ClassName);
	if (iter != ObjectInfo.end())
		return iter->second();

	return nullptr;
}