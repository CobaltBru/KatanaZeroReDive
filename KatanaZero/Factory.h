#pragma once
#include <unordered_map>
#include <string>
#include <functional>

#include "SimpleTestObject.h"
#include "SimpleObject.h"
#include "Player.h"
#include "HeadHunter.h"

#include "Enemies.h"
#include "PickUp.h"
using namespace std;

// 맵툴에 필요한 모든 오브젝트
static unordered_map<string, function<GameObject* ()>> ObjectInfo = {
	{"StartPoint", []() {return new SimpleObject(); }},
	{"SimpleTestObject", []() {return new SimpleTestObject(); } },
	{"HeadHunter", []() {return new HeadHunter(); }},
	{"Grunt", []() {return new Grunt(); }},
	{"Pomp", []() {return new Pomp(); }},
	{"Gangster", []() {return new Gangster(); }},
	{"Bottle1", []() {return new PickUp(); } },
	{"Bottle2", []() {return new PickUp(); } },
	{"Player", []() {return new Player(); } },
};

inline GameObject* CreateObject(string ClassName)
{
	auto iter = ObjectInfo.find(ClassName);
	if (iter != ObjectInfo.end())
		return iter->second();

	return nullptr;
}