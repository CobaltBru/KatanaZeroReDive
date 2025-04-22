#pragma once
#include <unordered_map>
#include <string>
#include <functional>

#include "SimpleTestObject.h"
#include "SimpleObject.h"
#include "Player.h"
using namespace std;

// ������ �ʿ��� ��� ������Ʈ
static unordered_map<string, function<GameObject* ()>> ObjectInfo = {
	{"StartPoint", []() {return new SimpleObject(); }},
	{"SimpleTestObject", []() {return new SimpleTestObject(); } },
};

inline GameObject* CreateObject(string ClassName)
{
	auto iter = ObjectInfo.find(ClassName);
	if (iter != ObjectInfo.end())
		return iter->second();

	return nullptr;
}