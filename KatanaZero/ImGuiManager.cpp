#include "ImGuiManager.h"

#undef new
#include "Reference/Headers/Imgui/imgui_impl_win32.h"
#include "Reference/Headers/Imgui/imgui_impl_dx11.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include <fstream>
#include "LineManager.h"
#include "ImageManager.h"
#include "ObjectManager.h"

#include "Background.h"
#include "DefaultObject.h"

static TCHAR filter[] = L"��� ����\0*.*\0dat ����\0*.dat";
static int item_current = -1;
static int Object_current = -1;
static int World_current = -1;


//!!=========================================================!!
//! ������ �ΰ� �� ����ȭ�ؾ��մϴ�.
// ������Ʈ ����Ʈ �̸�
static const char* Objectnames[] = { "StartPoint", "SimpleTestObject", };
// ������Ʈ �̹��� �̸�
static const char* ObjectImagenames[] = { "rocket", "rocket", };
//! ������ �ΰ� �� ����ȭ�ؾ��մϴ�.
//!!=========================================================!!
static const int ObjectArrlength = sizeof(Objectnames) / sizeof(Objectnames[0]);
//!!=========================================================!!


ImGuiManager::ImGuiManager()
	:BackgroundObj(nullptr), PlayerStartPoint(nullptr), SelectObject(nullptr)
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
	
	CurrentBackgroundIndex = 0;
}

void ImGuiManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ShowGui();

	/*ImGui::EndFrame();*/
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
		static int LineMode = 0;
		ImGui::SeparatorText(u8"���� ���");
		ImGui::RadioButton(u8"���� �Ϲݸ��", &LineMode, (int)ELineEditState::Create); ImGui::SameLine();
		ImGui::RadioButton(u8"���� �������", &LineMode, (int)ELineEditState::Adjust); ImGui::SameLine();
		ImGui::RadioButton(u8"���찳", &LineMode, (int)ELineEditState::Eraser);

		LineManager::GetInstance()->SetLineEditState((ELineEditState)LineMode);

		static int LineType = 0;
		ImGui::SeparatorText(u8"���� Ÿ��");
		ImGui::RadioButton(u8"��", &LineType, (int)ELineType::Normal); ImGui::SameLine();
		ImGui::RadioButton(u8"�������� ��", &LineType, (int)ELineType::DownLine); ImGui::SameLine();
		ImGui::RadioButton(u8"��", &LineType, (int)ELineType::Wall); ImGui::SameLine();
		ImGui::RadioButton(u8"õ��", &LineType, (int)ELineType::Ceiling);

		LineManager::GetInstance()->SetLineType((ELineType)LineType);


		if (ImGui::Button(u8"�ʱ�ȭ"))
		{
			LineManager::GetInstance()->ResetLinePoint();
		}
		ImGui::SameLine();
		ImGui::TextWrapped(u8"����Ű : NumPad 0");
		ImGui::SameLine();

		if (ImGui::Button(u8"��ü ���� �����"))
		{
			if (MessageBox(g_hWnd, L"��ü ������ ����ðڽ��ϱ�?", TEXT("����"), MB_YESNO) == IDYES)
			{
				LineManager::GetInstance()->DestroyAllLine();
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
		LineManager::GetInstance()->ResetLinePoint();
}

void ImGuiManager::Tile()
{
	if (ImGui::BeginTabItem("Tile"))
	{
		//Background
		if (ImGui::CollapsingHeader("Background"))
		{
			ImGui::ListBox("Background List", &item_current, BackgroundList, BackGroundMap.size(), 4);

			CreateBackground(item_current);

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
			
		}


		ImGui::EndTabItem();
	}
}

void ImGuiManager::Object()
{
	if (ImGui::BeginTabItem("Object"))
	{
		//static int LineMode = 0;
		//ImGui::SeparatorText(u8"���� ���");
		//ImGui::RadioButton(u8"���� �Ϲݸ��", &LineMode, (int)ELineEditState::Create); ImGui::SameLine();
		//ImGui::RadioButton(u8"���� �������", &LineMode, (int)ELineEditState::Adjust); ImGui::SameLine();

		ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
		ImGui::ListBox("Object List", &Object_current, Objectnames, ObjectArrlength, 4);
		ImGui::SameLine();
		ImGui::ListBox("World", &World_current, ObjectImagenames, ObjectArrlength, 4);
		
		ObjectTap();

		ObjectUpdate();

		//PlayerStartPointTap();		

		ImGui::EndTabItem();
	}
	else
	{
		DestorySelectObject();
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

void ImGuiManager::SaveLine()
{
	OPENFILENAME ofn;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWndParent;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = L"dat";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
	{
		if (SUCCEEDED(LineManager::GetInstance()->SaveFile(ofn.lpstrFile)))
			MessageBox(g_hWnd, L"���� ���� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::SaveBackGround()
{
	if(BackgroundObj == nullptr)
		MessageBox(g_hWnd, L"��׶��� ����.", TEXT("���"), MB_OK);

	OPENFILENAME ofn;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWndParent;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = L"dat";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"SaveBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		string str = BackgroundList[CurrentBackgroundIndex];
		int Size = str.length();
		WriteFile(hFile, &CurrentBackgroundIndex, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &Size, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, str.c_str(), Size, &dwByte, NULL);
		

		CloseHandle(hFile);

		MessageBox(g_hWnd, L"��׶��� ���� ����", TEXT("����"), MB_OK);
	}
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
	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWndParent;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetOpenFileName(&ofn))
	{
		if (SUCCEEDED(LineManager::GetInstance()->LoadFile(ofn.lpstrFile)))
			MessageBox(g_hWnd, L"���� �ҷ����� ����", TEXT("����"), MB_OK);
	}
}

void ImGuiManager::LoadBackGround()
{
	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWndParent;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetOpenFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, L"LoadBackGround Failed.", TEXT("���"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		int Index;
		int Size;
		
		ReadFile(hFile, &Index, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &Size, sizeof(int), &dwByte, NULL);

		char* buffer = new char[Size + 1];
		ReadFile(hFile, buffer, Size, &dwByte, NULL);
		buffer[Size] = '\0';
		
		string BackgroundName = buffer;

		delete[] buffer;

		if (CheckBakcground(Index))
		{
			item_current = Index;
			CurrentBackgroundIndex = Index;

			BackgroundObj = new Background();
			static_cast<Background*>(BackgroundObj)->Init(BackgroundName);
			ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);

			MessageBox(g_hWnd, L"��׶��� �ҷ����� ����", TEXT("����"), MB_OK);
		}		

		CloseHandle(hFile);
	}
}

vector<string> ImGuiManager::GetFileNames(const string& InFolderPath)
{
	vector<string> files;

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(InFolderPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return files;

	do
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			files.push_back(findData.cFileName);
		
	} while (FindNextFileA(hFind,&findData));
	
	FindClose(hFind);

	return files;
}

void ImGuiManager::InitBackground()
{
	BackgroundObj = nullptr;

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

		wstring wsPath = L"Image/";
		wsPath += wstring(backgrounds[i].begin(), backgrounds[i].end());

		ImageManager::GetInstance()->AddImage(nameOnly, wsPath.c_str(), false);
	}
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

	if (CheckBakcground(Index))
	{
		CurrentBackgroundIndex = Index;

		BackgroundObj = new Background();
		static_cast<Background*>(BackgroundObj)->Init(BackgroundList[Index]);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, BackgroundObj);
	}	
}

bool ImGuiManager::CheckBakcground(int Index)
{
	if (BackgroundObj != nullptr)
	{
		if (CurrentBackgroundIndex == Index)
			return false;

		BackgroundObj->SetDead(true);
	}
	return true;
}

void ImGuiManager::PlayerStartPointTap()
{
	if (ImGui::CollapsingHeader("PlayerStartPoint"))
	{
		if (PlayerStartPoint != nullptr)
		{
			static float PlayerX = 0.0f;
			static float PlayerY = 0.0f;

			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
			ImGui::DragFloat("X", &PlayerX);
			ImGui::SameLine();

			ImGui::PushItemWidth(TILEMAPTOOL_X * 0.3f);
			ImGui::DragFloat("Y", &PlayerY);



		}
		if (ImGui::Button(u8"����"))
		{
			if (PlayerStartPoint != nullptr)
				MessageBox(g_hWnd, L"�̹� �÷��̾� ��ġ�� �ֽ��ϴ�.", TEXT("����"), MB_OK);
			else
			{
				PlayerStartPoint = new DefaultObject();
				static_cast<DefaultObject*>(PlayerStartPoint)->Init("rocket", { 0.f,0.f }, false, ERenderGroup::NonAlphaBlend);
				ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, PlayerStartPoint);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"����"))
		{
			if (PlayerStartPoint != nullptr)
			{
				PlayerStartPoint->SetDead(true);
				PlayerStartPoint = nullptr;
			}
		}
	}
}

void ImGuiManager::ObjectTap()
{
	if (Object_current == -1)
		return;

	DestorySelectObject();

	// �÷��̾� ������
	if (Object_current == 0)
	{
		if (PlayerStartPoint != nullptr)
		{
			MessageBox(g_hWnd, L"�̹� �÷��̾� ��ġ�� �ֽ��ϴ�.", TEXT("����"), MB_OK);
			Object_current = -1;
		}
		else
		{
			PlayerStartPoint = new DefaultObject();
			static_cast<DefaultObject*>(PlayerStartPoint)->Init(ObjectImagenames[Object_current], {0.f,0.f}, false, ERenderGroup::NonAlphaBlend);
			ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, PlayerStartPoint);

			Object_current = -1;
			SelectObject = PlayerStartPoint;
		}			
	}
	// �� �� ������Ʈ��
	else
	{
		DefaultObject* Object = new DefaultObject();
		Object->Init(ObjectImagenames[Object_current], { 0.f,0.f }, false, ERenderGroup::NonAlphaBlend);
		ObjectManager::GetInstance()->AddGameObject(EObjectType::GameObject, PlayerStartPoint);

		Object_current = -1;
		SelectObject = Object;
	}	
}

void ImGuiManager::ObjectUpdate()
{
	if (SelectObject != nullptr)
	{

	}
	else
	{

	}
}

void ImGuiManager::DestorySelectObject()
{
	if (SelectObject != nullptr)
	{
		SelectObject->SetDead(true);
		SelectObject = nullptr;
	}
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

	ReleaseInstance();
}