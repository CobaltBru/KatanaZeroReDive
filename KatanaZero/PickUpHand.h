#pragma once
#include "config.h"

class PickUp;
class PickUpUI;
class GameObject;
class PickUpHand
{
public:
	PickUpHand(GameObject* InOwner);
	~PickUpHand() = default;

	PickUp* GetPickUpItem() { return PickUpItem; }

	bool SetPickUpItem(PickUp* InPickUpItem, FPOINT InOffset = {0.f,0.f});
	
	void Shoot(FPOINT InPos,float InAngle, float InSpeed);
	
private:
	GameObject* Owner;
	PickUp* PickUpItem;
	PickUpUI* PickUpItemUI;

};

