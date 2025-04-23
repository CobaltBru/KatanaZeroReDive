#define _CRT_SECURE_NO_WARNINGS

#include "ImGuiManager.h"

#undef new
#include "Reference/Headers/Imgui/imgui_impl_win32.h"
#include "Reference/Headers/Imgui/imgui_impl_dx11.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)


#define STB_IMAGE_IMPLEMENTATION
#include "Reference/Headers/Imgui/stb_image.h"

#include <fstream>
#include "CommonFunction.h"

#include "ImageManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"
#include "LineManager.h"

#include "Background.h"
#include "DefaultObject.h"
#include "Collider.h"
#include "SimpleObject.h"

static TCHAR filter[] = L"��� ����\0*.*\0dat ����\0*.dat";

static int item_current = -1;
static int Bg_current = -1;
static int Object_current = -1;
static int World_current = -1;
static int Floor_current = -1;

static vector<GameObject*> WorldObject;
static vector<Background*> BackgroundObject;
static vector<FloorZone> FloorZoneObject;

static float Pos[2] = { 0.f,0.f };
static float ColliderOffset[2] = { 0.f,0.f };
static float ColliderSize[2] = { 0.f,0.f };
//!!=========================================================!!
//! ������ �ΰ� �� ����ȭ�ؾ��մϴ�.
// ������Ʈ ����Ʈ �̸�
static const char* Objectnames[] = { "StartPoint", "SimpleTestObject" };
// ������Ʈ �̹��� �̸�
static const char* ObjectImagenames[] = { "TestPlayer", "rocket" };
//! ������ �ΰ� �� ����ȭ�ؾ��մϴ�.
//!!=========================================================!!
static const int ObjectArrlength = sizeof(Objectnames) / sizeof(Objectnames[0]);
//!!=========================================================!!


ImGuiManager::ImGuiManager()
	:PlayerStartPoint(nullptr), selectObject(nullptr), PlayerObject(nullptr), lineManager(nullptr), objectManager(nullptr), scrollManager(nullptr), selectedTile({0.f,0.f})
{
}

void ImGuiManager::Init()
{
	if (!CreateDeviceD3D(g_hWndDX))
	{
		CleanupDeviceD3D();
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWndDX);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Init
	LoadFont();
	InitBackground();
	InitTile();
}

void ImGuiManager::Init(LineManager* InLineManager, ObjectManager* InObjectManager, ScrollManager* InScrollManager, function<void()> InSaveTileCallBack, function<void()> InLoadTileCallBack)
{
	lineManager = InLineManager;
	objectManager = InObjectManager;
	scrollManager = InScrollManager;
	SaveTileCallBack = InSaveTileCallBack;
	LoadTileCallBack = InLoadTileCallBack;
}


void ImGuiManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ShowGui();

	ImGui::EndFrame();
}

void ImGuiManager::APIRender(HDC hdc)
{
	if (!FloorZoneObject.empty() && Floor_current != -1)
		DrawFloor(hdc);
}

void ImGuiManager::Render()
{
	// Rendering
	ImGui::Render();
	const float clear_color_with_alpha[4] = { 0.f,0.f,0.f,0.f };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present
	//HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}

void ImGuiManager::ShowGui()
{
	ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(WINSIZE_X - TILEMAPTOOL_X, WINSIZE_Y), ImGuiCond_Once);

	ImGui::Begin(u8"���ٿ�");

	Editor();

	ImGui::End();
}

void ImGuiManager::Editor()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		MapTool();

		/*ImGui::Text("MousePosition : %.2f, %.2f", (float)g_ptMouse.x, (float)g_ptMouse.y);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);*/
		ImGui::EndTabBar();
	}
}

void ImGuiManager::MapTool()
{
	Line();
	Tile();
	Object();
}

void ImGuiManager::Line()
{
	if (ImGui::BeginTabItem("Line"))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
		{
			if (lineManager->GetCurrentEditState() == ELineEditState::Create || lineManager->GetCurrentEditState() == ELineEditState::Adjust)
			{
				const FPOINT Scroll = scrollManager->GetScroll();
				lineManager->AddLine(g_ptMouse.x - Scroll.x, g_ptMouse.y - Scroll.y);
			}
			else if (lineManager->GetCurrentEditState() == ELineEditState::Eraser)
				lineManager->DestroyLine();
		}

		static int LineMode = 0;
		ImGui::SeparatorText(u8"���� ���");
		ImGui::RadioButton(u8"���� �Ϲݸ��", &LineMode, (int)ELineEditState::Create); ImGui::SameLine();
		ImGui::RadioButton(u8"���� �������", &LineMode, (int)ELineEditState::Adjust); ImGui::SameLine();
		ImGui::RadioButton(u8"���찳", &LineMode, (int)ELineEditState::Eraser);

		lineManager->SetLineEditState((ELineEditState)LineMode);

		static int LineType = 0;
		ImGui::SeparatorText(u8"���� Ÿ��");
		ImGui::RadioButton(u8"��", &LineType, (int)ELineType::Normal); ImGui::SameLine();
		ImGui::RadioButton(u8"�������� ��", &LineType, (int)ELineType::DownLine); ImGui::SameLine();
		ImGui::RadioButton(u8"��", &LineType, (int)ELineType::Wall); ImGui::SameLine();
		ImGui::RadioButton(u8"õ��", &LineType, (int)ELineType::Ceiling);

		lineManager->SetLineType((ELineType)LineType);


		if (ImGui::Button(u8"�ʱ�ȭ"))
		{
			lineManager->ResetLinePoint();
		}
		ImGui::SameLine();
		ImGui::TextWrapped(u8"����Ű : NumPad 0");
		ImGui::SameLine();

		if (ImGui::Button(u8"��ü ���� �����"))
		{
			if (MessageBox(g_hWnd, L"��ü ������ ����ðڽ��ϱ�?", TEXT("����"), MB_YESNO) == IDYES)
			{
				lineManager->DestroyAllLine();
			}
		}

		// ���� �����
		if (ImGui::Button(u8"����"))
		{
			SaveLine();
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"�ҷ�����"))
		{
			LoadLine();
		}

		// ����
		if (ImGui::CollapsingHeader(u8"����"))
		{
			ImGui::TextWrapped(u8"���� ���� ������ ���콺 ��ư�Դϴ�.");
			ImGui::TextWrapped(u8"Shift�� �����鼭 ������ ������ �������� �����ϴ�.");

			if (ImGui::TreeNode(u8"���� ���"))
			{
				ImGui::BulletText(u8"�Ϲݸ�� : �Ϲ����� ���� ���");
				ImGui::BulletText(u8"������� : ���콺�� ���� ����� ���ΰ� �̾���");
				ImGui::BulletText(u8"���찳 : ���콺�� ���� ����� ������ ����");

				ImGui::TextWrapped(u8"���찳 ���� ���� Ÿ�� ������� ����");

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode(u8"���� Ÿ��"))
			{
				ImGui::BulletText(u8"�� : �Ϲ����� ��");
				ImGui::BulletText(u8"�������� �� : ������ ������ �� �ִ� ��");
				ImGui::BulletText(u8"�� : ���� ��");
				ImGui::BulletText(u8"õ�� : õ��");

				ImGui::TextWrapped(u8"���� õ���� �� �����ϼž� �մϴ�. \n ������ ���� ������ �� Ÿ���� �̿����ּ���.");

				ImGui::TreePop();
				ImGui::Spacing();
			}
		}

		ImGui::EndTabItem();
	}
	else
		lineManager->ResetLinePoint();
}

void ImGuiManager::Tile()
{
	if (ImGui::BeginTabItem("Tile"))
	{
		//Background
		if (ImGui::CollapsingHeader("Background"))
		{
			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
			ImGui::ListBox("List", &item_current, BackgroundList, BackGroundMap.size(), 4);
			ImGui::SameLine();
			ImGui::ListBox("WorldBG", &Bg_current, BackGroundName.data(), BackGroundName.size(), 4);

			CreateBackground(item_current);

			if (Bg_current != -1 && !BackgroundObject.empty())
			{
				ImGui::SeparatorText(u8"Position");

				static float Pos[2] = { 0.f,0.f };
				Pos[0] = BackgroundObject[Bg_current]->GetPos().x;
				Pos[1] = BackgroundObject[Bg_current]->GetPos().y;
				ImGui::DragFloat2("Pos", Pos);
				BackgroundObject[Bg_current]->SetPos({ Pos[0],Pos[1] });

				static float ScrollPer = 1.f;
				ScrollPer = BackgroundObject[Bg_current]->GetScrollPercent();
				ImGui::DragFloat("ScrollPer", &ScrollPer);
				BackgroundObject[Bg_current]->SetScrollPercent(ScrollPer);


				if (ImGui::Button(u8"����"))
				{
					DestroyBackGround();
					Bg_current = -1;
				}
				if (Bg_current != -1)
				{
					ImGui::SeparatorText(u8"Transparent");
					static bool IsTransparent = false;
					IsTransparent = BackgroundObject[Bg_current]->GetImage()->GetTransparent();
					ImGui::Checkbox("IsTransparent", &IsTransparent);
					BackgroundObject[Bg_current]->GetImage()->SetTransparent(IsTransparent);
				}
			}

			if (ImGui::Button(u8"����"))
			{
				SaveBackGround();

			}
			ImGui::SameLine();
			if (ImGui::Button(u8"�ҷ�����"))
			{
				LoadBackGround();
			}
		}

		//Tile
		if (ImGui::CollapsingHeader("Tile"))
		{
			if (ImGui::Button(u8"Ÿ�� ����"))
			{
				SaveTile();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Ÿ�� �ҷ�����"))
			{
				LoadTile();
			}

			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
			ImGui::ListBox("TileList", &Tile_current, TileList.data(), TileList.size(), 4);
			ImGui::SameLine();
			
			ImGui::Checkbox(u8"���찳", &TileEraser);

			DrawTile();

			
		}

		ImGui::EndTabItem();
	}
	else
	{
		Tile_current = -1;
	}
}

void ImGuiManager::Object()
{
	if (ImGui::BeginTabItem("Object"))
	{
		ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
		ImGui::ListBox("Object List", &Object_current, Objectnames, ObjectArrlength, 4);
		ImGui::SameLine();
		ImGui::ListBox("World", &World_current, WorldObjectName.data(), WorldObjectName.size(), 4);

		ObjectTap();

		ObjectUpdate();

		// FloorZone
		Floor();

		//����
		if (ImGui::CollapsingHeader(u8"����"))
		{
			ImGui::TextWrapped(u8"������Ʈ ������ ��Ŭ��");
			ImGui::TextWrapped(u8"������Ʈ ��ġ �� �̵��� ��Ŭ��");

			ImGui::TextWrapped(u8"Start Point�� �÷��̾��� ���� ��ġ�Դϴ�.");

			ImGui::TextWrapped(u8"Auto Line�� ���� ����� �������� �̵��մϴ�.");
			ImGui::TextWrapped(u8"Auto Line�� EndŰ�ε� �����մϴ�.");

			ImGui::TextWrapped(u8"������ ���õ� ������Ʈ�� �����մϴ�.");
			ImGui::TextWrapped(u8"��� ������ ��ġ�� ������Ʈ ��� �����մϴ�.");
		}

		if (ImGui::Button(u8"�÷��̾� ����"))
		{
			CreatePlayerObject();
		}

		if (ImGui::Button(u8"�÷��̾� ����"))
		{
			if (PlayerObject == nullptr)
				MessageBox(g_hWnd, L"�÷��̾ �����ϴ�.", TEXT("���"), MB_OK);
			else
			{
				PlayerObject->SetDead(true);
				PlayerObject = nullptr;
			}
		}

		ImGui::EndTabItem();
	}
	else
	{
		DestroySelectObject();
	}
}

bool ImGuiManager::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void ImGuiManager::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void ImGuiManager::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void ImGuiManager::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void ImGuiManager::Reset()
{
	DestroyAllObject();
	DestroyAllBackGround();

	for (int i = 0; i < FloorName.size(); ++i)
		delete[] FloorName[i];
	FloorName.clear();


	lineManager = nullptr;
	objectManager = nullptr;
	scrollManager = nullptr;
}

void ImGuiManager::SaveLine()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetSaveInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		if (SUCCEEDED(lineManager->SaveFile(ofn.lpstrFile)))
			MessageBox(g_hWnd, L"���� ���� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::SaveBackGround()
{
	if (BackGroundName.empty())
		MessageBox(g_hWnd, L"��׶��� ����.", TEXT("���"), MB_OK);

	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetSaveInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"SaveBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		for (auto& iter : BackgroundObject)
		{
			string str = iter->GetImageName();
			int Size = str.length();
			float ScrollPer = iter->GetScrollPercent();
			FPOINT Pos = iter->GetPos();
			bool bTransparent = iter->GetImage()->GetTransparent();
			WriteFile(hFile, &ScrollPer, sizeof(float), &dwByte, NULL);
			WriteFile(hFile, &Size, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, str.c_str(), Size, &dwByte, NULL);
			WriteFile(hFile, &Pos, sizeof(FPOINT), &dwByte, NULL);
			WriteFile(hFile, &bTransparent, sizeof(bool), &dwByte, NULL);			
		}

		CloseHandle(hFile);

		MessageBox(g_hWnd, L"��׶��� ���� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::SaveObject()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetSaveInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"SaveObject Failed.", TEXT("���"), MB_OK);
			return;
		}

		//�����ؾ��� �͵� �������� ����������, Pos, ColliderOffset, ColliderSize, ClassName, �̹����̸�
		DWORD dwByte = 0;


		for (auto& iter : WorldObject)
		{
			DefaultObject* obj = static_cast<DefaultObject*>(iter);

			FObjectData ObjData;
			ZeroMemory(&ObjData, sizeof(FObjectData));
			ObjData.ClsasNameSize = obj->GetName().size();
			ObjData.ImageNameSize = obj->GetImageName().size();

			ObjData.Pos = obj->GetPos();
			if (obj->GetCollider() != nullptr)
			{
				ObjData.Offset = obj->GetCollider()->GetPivot();
				ObjData.Size = obj->GetCollider()->GetSize();
			}
			ObjData.bLeft = obj->GetFlip();

			WriteFile(hFile, &ObjData.ClsasNameSize, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, &ObjData.ImageNameSize, sizeof(int), &dwByte, NULL);
			WriteFile(hFile, obj->GetName().c_str(), obj->GetName().size(), &dwByte, NULL);
			WriteFile(hFile, obj->GetImageName().c_str(), obj->GetImageName().size(), &dwByte, NULL);
			WriteFile(hFile, &ObjData.Pos, sizeof(FPOINT), &dwByte, NULL);
			WriteFile(hFile, &ObjData.Offset, sizeof(FPOINT), &dwByte, NULL);
			WriteFile(hFile, &ObjData.Size, sizeof(FPOINT), &dwByte, NULL);
			WriteFile(hFile, &ObjData.bLeft, sizeof(bool), &dwByte, NULL);

		}

		CloseHandle(hFile);

		MessageBox(g_hWnd, L"������Ʈ ���� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::SaveFloor()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetSaveInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"SaveObject Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;

		for (auto& iter : FloorZoneObject)
		{
			FloorZone fz;
			fz.TopY = iter.TopY;
			fz.BottomY = iter.BottomY;

			WriteFile(hFile, &fz, sizeof(FloorZone), &dwByte, NULL);
		}

		CloseHandle(hFile);
		MessageBox(g_hWnd, L"Floor ���� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::SaveTile()
{
	SaveTileCallBack();
}

void ImGuiManager::LoadFont()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = nullptr;
	string fontPath = "font/NanumGothicBold.ttf";

	ifstream ifile;
	ifile.open(fontPath);
	if (ifile)
	{
		font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 14.f, NULL, io.Fonts->GetGlyphRangesKorean());
	}

	IM_ASSERT(font != NULL);
}

void ImGuiManager::LoadLine()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetLoadInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		std::wstring fullPath = ofn.lpstrFile;
		std::wstring key = L"Data\\";
		size_t pos = fullPath.find(key);
		std::wstring relativePath;
		relativePath = fullPath.substr(pos);

		if (SUCCEEDED(lineManager->LoadFile(ofn.lpstrFile)))
			MessageBox(g_hWnd, L"���� �ҷ����� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::LoadBackGround()
{
	DestroyAllBackGround();

	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetLoadInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"LoadBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;

		while (true)
		{
			int Size;
			float ScrollPer;
			FPOINT Pos;
			bool bTransparent;
			ReadFile(hFile, &ScrollPer, sizeof(float), &dwByte, NULL);
			ReadFile(hFile, &Size, sizeof(int), &dwByte, NULL);

			char* buffer = new char[Size + 1];
			ReadFile(hFile, buffer, Size, &dwByte, NULL);
			buffer[Size] = '\0';
			ReadFile(hFile, &Pos, sizeof(FPOINT), &dwByte, NULL);
			ReadFile(hFile, &bTransparent, sizeof(bool), &dwByte, NULL);		

			string BackgroundName = buffer;

			delete[] buffer;

			if (dwByte == 0)
				break;

			Background* BackgroundObj = new Background();
			BackgroundObj->Init(BackgroundName, ScrollPer, scrollManager->GetScale() + 0.5f);
			BackgroundObj->SetPos(Pos);
			BackgroundObj->GetImage()->SetTransparent(bTransparent);
			objectManager->AddGameObject(EObjectType::GameObject, BackgroundObj);

			char* Name = new char[BackgroundName.size() + 1];
			strcpy_s(Name, BackgroundName.size() + 1, BackgroundName.c_str());

			BackGroundName.push_back(Name);
			BackgroundObject.push_back(BackgroundObj);

			item_current = -1;
			Bg_current = -1;
		}

		MessageBox(g_hWnd, L"��׶��� �ҷ����� ����", TEXT("����"), MB_OK);

		CloseHandle(hFile);
	}
}

void ImGuiManager::LoadObject()
{
	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetLoadInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"LoadBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		bool bFirst = true;
		while (true)
		{
			FObjectData ObjData;
			ZeroMemory(&ObjData, sizeof(FObjectData));

			ReadFile(hFile, &ObjData.ClsasNameSize, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &ObjData.ImageNameSize, sizeof(int), &dwByte, NULL);
			ObjData.ClassName = new char[ObjData.ClsasNameSize + 1];
			ObjData.ImageName = new char[ObjData.ImageNameSize + 1];

			ReadFile(hFile, ObjData.ClassName, ObjData.ClsasNameSize, &dwByte, NULL);
			ReadFile(hFile, ObjData.ImageName, ObjData.ImageNameSize, &dwByte, NULL);
			ReadFile(hFile, &ObjData.Pos, sizeof(FPOINT), &dwByte, NULL);
			ReadFile(hFile, &ObjData.Offset, sizeof(FPOINT), &dwByte, NULL);
			ReadFile(hFile, &ObjData.Size, sizeof(FPOINT), &dwByte, NULL);
			ReadFile(hFile, &ObjData.bLeft, sizeof(bool), &dwByte, NULL);

			ObjData.ClassName[ObjData.ClsasNameSize] = '\0';
			ObjData.ImageName[ObjData.ImageNameSize] = '\0';

			string ClassName = ObjData.ClassName;
			string ImageName = ObjData.ImageName;

			delete[] ObjData.ClassName;
			delete[] ObjData.ImageName;

			if (dwByte == 0)
				break;

			if (bFirst)
			{
				bFirst = false;
				DestroyAllObject();
			}

			DefaultObject* Object = new DefaultObject();
			Object->Init(ImageName, ObjData.Pos, ObjData.bLeft, ERenderGroup::NonAlphaBlend, ClassName);

			WorldObject.push_back(Object);

			char* Name = new char[Object->GetName().size() + 1];
			strcpy_s(Name, Object->GetName().size() + 1, Object->GetName().c_str());
			WorldObjectName.push_back(Name);

			if (Object->GetName() == "StartPoint")
				PlayerStartPoint = Object;

			objectManager->AddGameObject(EObjectType::GameObject, Object);
		}

		CloseHandle(hFile);

		MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::LoadFloor()
{
	DestroyAllFloor();

	TCHAR lpstrFile[MAX_PATH] = L"";
	OPENFILENAME ofn = GetLoadInfo(lpstrFile, filter);

	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);

	if (GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(szOldDir);

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"LoadBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;

		while (true)
		{
			FloorZone fz;
			ZeroMemory(&fz, sizeof(FloorZone));

			ReadFile(hFile, &fz, sizeof(FloorZone), &dwByte, NULL);

			if (dwByte == 0)
				break;

			FloorZoneObject.push_back(fz);

			char* name = new char[6];
			strcpy_s(name, 6, "Floor");
			FloorName.push_back(name);
		}

		CloseHandle(hFile);
		MessageBox(g_hWnd, L"Floor �ҷ����� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::LoadTile()
{
	LoadTileCallBack();
}

bool ImGuiManager::LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

bool ImGuiManager::LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	FILE* f = fopen(file_name, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1)
		return false;
	fseek(f, 0, SEEK_SET);
	void* file_data = IM_ALLOC(file_size);
	fread(file_data, 1, file_size, f);
	fclose(f);
	bool ret = LoadTextureFromMemory(file_data, file_size, out_srv, out_width, out_height);
	IM_FREE(file_data);
	return ret;
}

//vector<string> ImGuiManager::GetFileNames(const string& InFolderPath)
//{
//	vector<string> files;
//
//	WIN32_FIND_DATAA findData;
//	HANDLE hFind = FindFirstFileA(InFolderPath.c_str(), &findData);
//
//	if (hFind == INVALID_HANDLE_VALUE)
//		return files;
//
//	do
//	{
//		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//			files.push_back(findData.cFileName);
//
//	} while (FindNextFileA(hFind, &findData));
//
//	FindClose(hFind);
//
//	return files;
//}

void ImGuiManager::InitBackground()
{
	vector<string> backgrounds = GetFileNames("Image/Background/*.bmp");

	if (backgrounds.empty())
		return;

	BackgroundList = new const char* [backgrounds.size()];

	for (int i = 0; i < backgrounds.size(); ++i)
	{
		int dotPos = backgrounds[i].find_last_of('.');
		string nameOnly = dotPos != string::npos ? backgrounds[i].substr(0, dotPos) : backgrounds[i];

		const int size = nameOnly.size() + 1;
		char* temp = new char[size];
		strcpy_s(temp, size, nameOnly.c_str());

		BackgroundList[i] = temp;
		BackGroundMap.insert({ BackgroundList[i], backgrounds[i] });

		wstring wsPath = L"Image/Background/";
		wsPath += wstring(backgrounds[i].begin(), backgrounds[i].end());

		ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(), true, RGB(255, 0, 255));
	}
}

void ImGuiManager::CreatePlayerObject()
{
	if (PlayerObject != nullptr)
	{
		MessageBox(g_hWnd, L"�÷��̾ �̹� �ֽ��ϴ�.", TEXT("���"), MB_OK);
		return;
	}

	if (PlayerStartPoint == nullptr)
	{
		MessageBox(g_hWnd, L"��ŸƮ ����Ʈ�� �����ϴ�.", TEXT("���"), MB_OK);
		return;
	}

	PlayerObject = new SimpleObject();
	static_cast<SimpleObject*>(PlayerObject)->Init(PlayerStartPoint->GetPos(), "TestPlayer");
	objectManager->AddGameObject(EObjectType::GameObject, PlayerObject);
}

void ImGuiManager::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ImGuiManager::CreateBackground(int Index)
{
	if (Index == -1)
		return;

	Background* BackgroundObj = new Background();
	BackgroundObj->Init(BackgroundList[Index], 1.f, scrollManager->GetScale() + 0.5f);
	objectManager->AddGameObject(EObjectType::GameObject, BackgroundObj);

	string name = BackgroundList[Index];

	char* Name = new char[name.size() + 1];
	strcpy_s(Name, name.size() + 1, name.c_str());

	BackGroundName.push_back(Name);
	BackgroundObject.push_back(BackgroundObj);

	item_current = -1;
}

void ImGuiManager::DestroyBackGround()
{
	if (BackgroundObject.empty())
		return;

	BackgroundObject[Bg_current]->SetDead(true);

	BackgroundObject.erase(BackgroundObject.begin() + Bg_current);

	delete[] BackGroundName[Bg_current];
	BackGroundName.erase(BackGroundName.begin() + Bg_current);
}

void ImGuiManager::DestroyAllBackGround()
{
	for (auto& iter : BackgroundObject)
		iter->SetDead(true);

	BackgroundObject.clear();

	for (int i = 0; i < BackGroundName.size(); ++i)
		delete[] BackGroundName[i];
	BackGroundName.clear();
}

void ImGuiManager::InitTile()
{
	vector<string> Tiles = GetFileNames("Image/Tile/*.bmp");

	if (Tiles.empty())
		return;

	for (int i = 0; i < Tiles.size(); ++i)
	{
		int dotPos = Tiles[i].find_last_of('.');
		string nameOnly = dotPos != string::npos ? Tiles[i].substr(0, dotPos) : Tiles[i];

		const int size = nameOnly.size() + 1;
		char* temp = new char[size];
		strcpy_s(temp, size, nameOnly.c_str());

		TileList.push_back(temp);

		wstring wsPath = L"Image/Tile/";
		wsPath += wstring(Tiles[i].begin(), Tiles[i].end());

		string path = "Image/Tile/" + Tiles[i];

		int Width = 0;
		int Height = 0;
		ID3D11ShaderResourceView* Texture = nullptr;
		bool ret = LoadTextureFromFile(path.c_str(), &Texture, &Width, &Height);
		IM_ASSERT(ret);
		pair<int, int> TextureSize = { Width ,Height };
		pair<ID3D11ShaderResourceView*, pair<int, int>> info = { Texture ,TextureSize };

		TileTextures.insert({ temp, info });

		//ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(), true, RGB(255, 0, 255));
	}
}

void ImGuiManager::ObjectTap()
{
	if (Object_current == -1)
		return;

	DestroySelectObject();

	// �÷��̾� ������
	if (Object_current == 0)
	{
		if (PlayerStartPoint != nullptr)
		{
			MessageBox(g_hWnd, L"�̹� �÷��̾� ��ġ�� �ֽ��ϴ�.", TEXT("����"), MB_OK);
		}
		else
		{
			PlayerStartPoint = new DefaultObject();
			static_cast<DefaultObject*>(PlayerStartPoint)->Init(ObjectImagenames[Object_current], { 0.f,0.f }, false, ERenderGroup::NonAlphaBlend, Objectnames[Object_current]);
			objectManager->AddGameObject(EObjectType::GameObject, PlayerStartPoint);

			selectObject = PlayerStartPoint;
		}
	}
	// �� �� ������Ʈ��
	else
	{
		DefaultObject* Object = new DefaultObject();
		Object->Init(ObjectImagenames[Object_current], { 0.f,0.f }, false, ERenderGroup::NonAlphaBlend, Objectnames[Object_current]);
		objectManager->AddGameObject(EObjectType::GameObject, Object);

		selectObject = Object;
	}

	Object_current = -1;
	World_current = -1;
}

void ImGuiManager::ObjectUpdate()
{
	if (selectObject != nullptr)
	{
		const FPOINT Scroll = scrollManager->GetScroll();
		selectObject->SetPos({ (float)g_ptMouse.x - Scroll.x,(float)g_ptMouse.y - Scroll.y });
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
		{
			DefaultObject* worldObj = static_cast<DefaultObject*>(selectObject);
			WorldObject.push_back(worldObj);
			char* Name = new char[worldObj->GetName().size() + 1];
			strcpy_s(Name, worldObj->GetName().size() + 1, worldObj->GetName().c_str());

			WorldObjectName.push_back(Name);

			selectObject = nullptr;

			World_current = WorldObjectName.size() - 1;
		}
	}
	else
	{
		WorldObjectUpdate();
	}
}

void ImGuiManager::WorldObjectUpdate()
{
	//��ȸ.. ��ŷ.. ��
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		const FPOINT Scroll = scrollManager->GetScroll();
		RECT rc;
		int Index = 0;
		for (auto& iter : WorldObject)
		{
			DefaultObject* obj = static_cast<DefaultObject*>(iter);
			rc.left = (obj->GetPos().x - Scroll.x) - obj->GetImageSize().x;
			rc.right = (obj->GetPos().x - Scroll.x) + obj->GetImageSize().x;
			rc.top = (obj->GetPos().y - Scroll.y) - obj->GetImageSize().y;
			rc.bottom = (obj->GetPos().y - Scroll.y) + obj->GetImageSize().y;

			if (PointInRect(g_ptMouse, rc))
			{
				World_current = Index;
				break;
			}

			++Index;
		}
	}
	else if (World_current != -1 && KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		const FPOINT Scroll = scrollManager->GetScroll();
		WorldObject[World_current]->SetPos({ g_ptMouse.x - Scroll.x, g_ptMouse.y - Scroll.y });
	}


	if (World_current != -1)
	{
		//Pos
		ImGui::SeparatorText(u8"Position");
		Pos[0] = WorldObject[World_current]->GetPos().x;
		Pos[1] = WorldObject[World_current]->GetPos().y;
		ImGui::PushItemWidth(TILEMAPTOOL_X * 0.7f);
		ImGui::DragFloat2("Position", Pos);
		WorldObject[World_current]->SetPos({ Pos[0],Pos[1] });

		//Collider
		if (WorldObject[World_current]->GetCollider() != nullptr)
		{
			Collider* WorldObjCollider = WorldObject[World_current]->GetCollider();

			ImGui::SeparatorText(u8"Collider");
			ColliderOffset[0] = WorldObjCollider->GetPivot().x;
			ColliderOffset[1] = WorldObjCollider->GetPivot().y;
			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.7f);
			ImGui::DragFloat2("Offset", ColliderOffset);
			WorldObjCollider->SetPivot({ ColliderOffset[0],ColliderOffset[1] });

			ColliderSize[0] = WorldObjCollider->GetSize().x;
			ColliderSize[1] = WorldObjCollider->GetSize().y;
			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.7f);
			ImGui::DragFloat2("Size", ColliderSize);

			WorldObjCollider->SetSize({ ColliderSize[0] ,ColliderSize[1] });
			WorldObjCollider->SetDebugDraw(true);
		}

		static bool bFlip = false;
		bFlip = static_cast<DefaultObject*>(WorldObject[World_current])->GetFlip();
		ImGui::SeparatorText(u8"Flip");
		ImGui::Checkbox("Flip", &bFlip);
		static_cast<DefaultObject*>(WorldObject[World_current])->SetFlip(bFlip);
	}

	if (!WorldObject.empty())
	{
		ImGui::SeparatorText(u8"Button");
		if (World_current != -1)
		{
			if (ImGui::Button(u8"Auto Line") || KeyManager::GetInstance()->IsOnceKeyDown(VK_END))
			{
				if (WorldObject[World_current]->GetCollider() == nullptr)
					return;

				FLineResult Result;
				if (lineManager->CollisionLine(WorldObject[World_current]->GetPos(), Result, WorldObject[World_current]->GetCollider()->GetSize().y))
					WorldObject[World_current]->SetPos({ WorldObject[World_current]->GetPos().x, Result.OutPos.y });
			}

			ImGui::SameLine();

			if (ImGui::Button(u8"����"))
			{
				//�÷��̾� ������
				if (!strcmp(WorldObjectName[World_current], Objectnames[0]))
					PlayerStartPoint = nullptr;

				WorldObject[World_current]->SetDead(true);

				WorldObject.erase(WorldObject.begin() + World_current);

				delete[] WorldObjectName[World_current];
				WorldObjectName.erase(WorldObjectName.begin() + World_current);

				World_current = -1;
			}
		}

		if (ImGui::Button(u8"��� ����"))
		{
			if (MessageBox(g_hWnd, L"��ü ������Ʈ�� �����Ͻðڽ��ϱ�?", TEXT("����"), MB_YESNO) == IDYES)
			{
				DestroyAllObject();
			}
		}

		if (ImGui::Button(u8"����"))
		{
			DestroySelectObject();
			World_current = -1;
			SaveObject();
		}
		ImGui::SameLine();
	}


	if (ImGui::Button(u8"�ҷ�����"))
	{
		DestroySelectObject();
		World_current = -1;
		LoadObject();
	}
}

void ImGuiManager::DestroySelectObject()
{
	if (selectObject != nullptr)
	{
		if (PlayerStartPoint != nullptr && PlayerStartPoint == selectObject)
			PlayerStartPoint = nullptr;

		selectObject->SetDead(true);
		selectObject = nullptr;
	}
}

void ImGuiManager::DestroyAllObject()
{
	DestroySelectObject();

	for (auto iter = WorldObject.begin(); iter != WorldObject.end();)
	{
		(*iter)->SetDead(true);
		iter = WorldObject.erase(iter);
	}

	for (auto iter = WorldObjectName.begin(); iter != WorldObjectName.end();)
	{
		delete[](*iter);
		iter = WorldObjectName.erase(iter);
	}

	World_current = -1;
}

void ImGuiManager::Floor()
{
	ImGui::SeparatorText(u8"FloorZone");

	ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
	if (ImGui::Button(u8"�߰�"))
	{
		char* name = new char[6];
		strcpy_s(name, 6, "Floor");
		FloorName.push_back(name);

		FloorZone fz;
		ZeroMemory(&fz, sizeof(FloorZone));
		FloorZoneObject.push_back(fz);

		Floor_current = -1;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
	ImGui::ListBox("FloorList", &Floor_current, FloorName.data(), FloorName.size(), 4);

	static float FloorZone[2] = { 0.f,0.f };
	if (Floor_current != -1)
	{
		FloorZone[0] = FloorZoneObject[Floor_current].TopY;
		FloorZone[1] = FloorZoneObject[Floor_current].BottomY;
		ImGui::PushItemWidth(TILEMAPTOOL_X * 0.7f);
		ImGui::DragFloat2("Top / Bottom", FloorZone);
		FloorZoneObject[Floor_current].TopY = FloorZone[0];
		FloorZoneObject[Floor_current].BottomY = FloorZone[1];

		if (ImGui::Button(u8"����"))
		{
			FloorZoneObject.erase(FloorZoneObject.begin() + Floor_current);

			delete[] FloorName[Floor_current];
			FloorName.erase(FloorName.begin() + Floor_current);
			Floor_current = -1;
		}
	}
	if (!FloorZoneObject.empty())
	{
		if (ImGui::Button(u8"�� ��"))
		{
			SaveFloor();
		}
		ImGui::SameLine();
	}
	if (ImGui::Button(u8"�ҷ� ����"))
	{
		LoadFloor();
		Floor_current = -1;
	}
}

void ImGuiManager::DestroyAllFloor()
{
	FloorZoneObject.clear();

	for (int i = 0; i < FloorName.size(); ++i)
		delete[] FloorName[i];
	FloorName.clear();
}

void ImGuiManager::DrawFloor(HDC hdc)
{
	HPEN hPen = nullptr;

	hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // ���� DC�� ���� ����		

	MoveToEx(hdc, 0, FloorZoneObject[Floor_current].TopY, nullptr);
	LineTo(hdc, WINSIZE_X, FloorZoneObject[Floor_current].TopY);

	MoveToEx(hdc, 0, FloorZoneObject[Floor_current].BottomY, nullptr);
	LineTo(hdc, WINSIZE_X, FloorZoneObject[Floor_current].BottomY);

	// ����� ���� ������� ����
	SelectObject(hdc, hOldPen);
	// �� �޸� ����
	DeleteObject(hPen);
}

void ImGuiManager::DrawTile()
{
	if (Tile_current == -1)
		return;

	auto iter = TileTextures.find(TileList[Tile_current]);
	if (iter == TileTextures.end())
		return;

	ID3D11ShaderResourceView* Texture = iter->second.first;
	int Width = iter->second.second.first;
	int Height = iter->second.second.second;

	ImVec2 childSize = ImVec2(TILEMAPTOOL_X, 600.f);
	ImGui::BeginChild("Image", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Image((ImTextureID)(intptr_t)Texture, ImVec2(Width, Height));
	
	ImVec2 imagePos = ImGui::GetCursorScreenPos();
	ImVec2 imagePos2 = ImGui::GetItemRectMin();
	ImVec2 imageSize = ImVec2((float)Width, (float)Height);
	float scrollY = ImGui::GetScrollY();

	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) 
	{
		ImVec2 localPos = ImVec2(abs(mousePos.x - imagePos.x), abs(mousePos.y - imagePos.y));
		if (localPos.x >= 0 && localPos.y >= 0 && localPos.x < Width && localPos.y < Height) {
			int tileX = static_cast<int>(localPos.x) / 32;
			int tileY = static_cast<int>(localPos.y) / 32;
			if ((int)selectedTile.x != tileX || (int)selectedTile.y != tileY)
				TileEraser = false;
			
			selectedTile.x = (float)tileX;
			selectedTile.y = (float)tileY;
		}		
	}

	if (selectedTile.x >= 0 && selectedTile.y >= 0) 
	{																			//�� �ٺ����� 4�� ����ü ���� ���ΰ�			�е� ���ΰ�����..
		float offsetY = imagePos2.y >= 0.f ? 0.f : (scrollY + abs(imagePos2.y)) / 2 + 32;
		ImVec2 rectMin = ImVec2(imagePos.x + selectedTile.x * 32, abs(imagePos.y - Height - 4) + ((Height / 32 - 1) - selectedTile.y) * 32 - offsetY);
		ImVec2 rectMax = ImVec2(rectMin.x + 32, rectMin.y + 32);
		draw_list->AddRect(rectMin, rectMax, IM_COL32(255, 0, 0, 255)); // ���� �׵θ�
	}

	ImGui::EndChild();
}

void ImGuiManager::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();

	for (int i = 0; i < BackGroundMap.size(); ++i)
		delete[] BackgroundList[i];
	delete[] BackgroundList;

	for (int i = 0; i < WorldObjectName.size(); ++i)
		delete[] WorldObjectName[i];
	WorldObjectName.clear();

	for (int i = 0; i < BackGroundName.size(); ++i)
		delete[] BackGroundName[i];
	BackGroundName.clear();

	for (int i = 0; i < FloorName.size(); ++i)
		delete[] FloorName[i];
	FloorName.clear();

	for (int i = 0; i < TileList.size(); ++i)
		delete[] TileList[i];
	TileList.clear();
	

	ReleaseInstance();
}