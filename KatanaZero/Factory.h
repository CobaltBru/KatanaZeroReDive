#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "GameObject.h"
#include "SimpleTestObject.h"
using namespace std;


// ������ �ʿ��� ��� ������Ʈ
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