#pragma once
#include "Singleton.h"
#include "TimeLineBuffer.h"
#include "config.h"
#include <vector>
#include <list>
#include "SnapShot.h"
#include "ObjectManager.h"

enum class EObjectClassType : uint8_t
{
	Player,
	Enemy,
	Effect,
	End
};

class SnapShotManager2 : public Singleton<SnapShotManager2>
{
};

