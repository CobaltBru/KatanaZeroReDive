#pragma once

class PickUp;
class PickUpHand
{
public:
	PickUpHand();
	~PickUpHand() = default;

	bool SetPickUpItem(PickUp* InPickUpItem);
	
	void Shoot(float InAngle);
	
private:
	PickUp* PickUpItem;
};

