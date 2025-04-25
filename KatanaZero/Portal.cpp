#include "Portal.h"
#include "GPImage.h"
#include "ScrollManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "Syringe.h"

Portal::Portal()
	:OpenImage(nullptr),CloseImage(nullptr), LoopImage(nullptr), Angle(-1.f), Frame(0), State(EPortal::End), LoopCount(0), MaxLoopCount(0), CurrentTime(0.f)
{
}

HRESULT Portal::Init(FPOINT InPos, float InAngle, int InLoopCount, EPortalType InPortalType)
{
	Pos = InPos;
	Angle = InAngle;
	MaxLoopCount = InLoopCount;
	PortalType = InPortalType;

	wstring str = L"Image/Hidden/spr_portal_open.png";
	OpenImage = new GPImage();
	OpenImage->AddImage(str.c_str(), 6, 1);

	str = L"Image/Hidden/spr_portal_close.png";
	CloseImage = new GPImage();
	CloseImage->AddImage(str.c_str(), 7, 1);

	str = L"Image/Hidden/spr_portal_loop_back.png";
	LoopImage = new GPImage();
	LoopImage->AddImage(str.c_str(), 6, 1);

	CurrentImage = OpenImage;
	State = EPortal::Start;
	return S_OK;
}

void Portal::Update()
{
	CurrentTime += TimerManager::GetInstance()->GetDeltaTime();
	if (CurrentTime >= 0.1f)
	{
		CurrentTime = 0.f;
		++Frame;
		if (CurrentImage->getMaxFrame() == Frame)
		{
			switch (State)
			{
			case EPortal::Start:
				State = EPortal::Loop;
				CurrentImage = LoopImage;
				break;
			case EPortal::Loop:
				if (LoopCount == 0)
				{
					if (PortalType == EPortalType::Attack)
						Attack();
					else
						Object();				
				}
				++LoopCount;
				if (MaxLoopCount < LoopCount)
				{
					State = EPortal::Close;
					CurrentImage = CloseImage;
					LoopCount = 0;
				}
				break;
			case EPortal::Close:
				bDead = true;
				State = EPortal::End;
				break;
			case EPortal::End:
				break;
			default:
				break;
			}

			Frame = 0;
		}
	}

	if(State != EPortal::End)
		RenderManager::GetInstance()->AddRenderGroup(ERenderGroup::NonAlphaBlend, this);
}

void Portal::Render(HDC hdc)
{
	const FPOINT Scroll = ScrollManager::GetInstance()->GetScroll();

	Gdiplus::Graphics graphics(hdc);
	CurrentImage->Middle_RenderFrameAngle(&graphics, Pos + Scroll, Frame, Angle, bFlip);
}

void Portal::Attack()
{
	Syringe* syringe = new Syringe();
	syringe->Init(Pos, Angle);
	ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, syringe);
}

void Portal::Object()
{
}

void Portal::Release()
{
	if (OpenImage != nullptr)
	{
		OpenImage->Release();
		delete OpenImage;
		OpenImage = nullptr;
	}
	if (CloseImage != nullptr)
	{
		CloseImage->Release();
		delete CloseImage;
		CloseImage = nullptr;
	}
	if (LoopImage != nullptr)
	{
		LoopImage->Release();
		delete LoopImage;
		LoopImage = nullptr;
	}

	CurrentImage = nullptr;
}
