#include "PickUpHand.h"
#include "PickUp.h"
#include "PickUpUI.h"
#include "ObjectManager.h"

PickUpHand::PickUpHand(GameObject* InOwner)
	:PickUpItem(nullptr), Owner(InOwner) , PickUpItemUI(nullptr)
{
}

bool PickUpHand::SetPickUpItem(PickUp* InPickUpItem, FPOINT InOffset)
{
	if (PickUpItem == nullptr)
	{
		PickUpItem = InPickUpItem;
		PickUpItem->SetVisible(false);

		if (PickUpItemUI == nullptr)
		{
			PickUpItemUI = new PickUpUI();
			PickUpItemUI->Init(PickUpItem->GetImageKey(), Owner->GetPPos(), InOffset, PickUpItem->GetFlip());
			ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, PickUpItemUI);
		}
		
		PickUpItemUI->SetImage(PickUpItem->GetImageKey());
		PickUpItemUI->SetScale(PickUpItem->GetScale());
		PickUpItemUI->Start();

		return true;
	}	

	return false;
}

void PickUpHand::Shoot(FPOINT InPos,float InAngle, float InSpeed)
{
	if (PickUpItem == nullptr)
		return;

	PickUpItem->Shoot(InPos,InAngle, InSpeed);
	PickUpItem = nullptr;
}
