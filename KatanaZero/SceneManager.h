#pragma once
#include "Singleton.h"
#include "config.h"

class GameObject;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, GameObject*> mapScenes;
	map<string, GameObject*> mapLoadingScenes;
	string currKey;
	string NextLoading;
	string NextScene;
	bool bChange;
public:
	static GameObject* currentScene;
	static GameObject* loadingScene;
	static GameObject* nextScene;

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	bool GetChange() const { return bChange; }
	string GetNextLoading() const { return NextLoading; }
	string GetNextScene() const { return NextScene; }

	HRESULT ChangeScene(string key);
	HRESULT ChangeScene(string key, string loadingKey);
	void ChangeScene(string key, string loadingKey, bool InbChange) {
		NextScene = key;
		NextLoading = loadingKey;
		bChange = InbChange;
	};

	HRESULT RestartCurrentScene();

	GameObject* AddScene(string key, GameObject* scene);
	GameObject* AddLoadingScene(string key, GameObject* scene);
};

