#pragma once
#include <unordered_map>
#include <string>
#include <functional>

#include "SimpleTestObject.h"
#include "SimpleObject.h"
#include "Player.h"
#include "HeadHunter.h"
#include "PickUp.h"
using namespace std;

// ������ �ʿ��� ��� ������Ʈ
static unordered_map<string, function<GameObject* ()>> ObjectInfo = {
	{"StartPoint", []() {return new SimpleObject(); }},
	{"SimpleTestObject", []() {return new SimpleTestObject(); } },
	{"HeadHunter", []() {return new HeadHunter(); }}
	{"Bottle1", []() {return new PickUp(); } },
	{"Bottle2", []() {return new PickUp(); } },
};

inline GameObject* CreateObject(string ClassName)
{
	auto iter = ObjectInfo.find(ClassName);
	if (iter != ObjectInfo.end())
		return iter->second();

	return nullptr;
}