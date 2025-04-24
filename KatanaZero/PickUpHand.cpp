#include "PickUpHand.h"
#include "PickUp.h"
PickUpHand::PickUpHand()
	:PickUpItem(nullptr)
{
}

bool PickUpHand::SetPickUpItem(PickUp* InPickUpItem)
{
	if (PickUpItem != nullptr)
		return false;

	PickUpItem = InPickUpItem;
	PickUpItem->SetVisible(false);
}

void PickUpHand::Shoot(float InAngle)
{
}
