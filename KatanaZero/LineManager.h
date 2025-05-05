#pragma once
#include "Singleton.h"
#include "config.h"
#include <list>

enum class ELineEditState : uint8_t
{
	Create,
	Eraser,
	Adjust,
	End
};

struct Edge
{
	int to;
	float cost;
	ELineType type;
};

class Line;
class LineManager : public Singleton<LineManager>
{
private:
	enum Direction { LEFT, RIGHT, END };
public:
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	ELineEditState GetCurrentEditState() const { return CurrentEditState; }
	Line* GetLineFront(ELineType InLineType) const { 
		if (LineList[(int)InLineType].empty())
			return nullptr;

		return LineList[(int)InLineType].front(); 	
	}

	void SetLineType(ELineType InLineType) { CurrentLineType = InLineType; }
	void SetLineEditState(ELineEditState InLineEditState) { CurrentEditState = InLineEditState; }
	void AddLine(float InX, float InY);
	void ResetLinePoint();
	void DestroyLine();
	void DestroyAllLine();

	bool CollisionLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);
	bool CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance = 3.f, bool IsDown = false);
	bool CollisionLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, bool IsGround, float tolerance = 3.f, bool IsDown = false);
	bool CollisionWallLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, FPOINT InSize);
	bool CollisionCeilingLine(FPOINT InPos, FPOINT InLastPos, FLineResult& OutResult, float tolerance = 3.f);
	bool CollisionDownLine(FPOINT InPos, FLineResult& OutResult, float tolerance = 3.f);

	HRESULT SaveFile(LPCWSTR InSavePath);
	HRESULT LoadFile(LPCWSTR InLoadPath);

	void CreateLine(int InX, int InY);
	void AdjustLine(float InX, float InY);

	// 적 AI용 비탈길 찾기
	pair<FPOINT, FPOINT> FindNearestSlope(const FPOINT& targetPos, int fromFloor, int toFloor) const;

	int GetNodeIdx(const FPOINT& p);
	void MakeGraphFromLines();
	vector<FPOINT>& GetNodes() { return nodes; }
	vector<vector<Edge>>& GetGraph() { return graph; }
private:
	list<Line*> LineList[(int)ELineType::End];
	vector<FPOINT> nodes;
	vector<vector<Edge>> graph;
	map<pair<int, int>, int> nodeIndex;

	FPOINT LinePoint[END];
	ELineType CurrentLineType;
	bool bStraight;
	ELineEditState CurrentEditState;
	float LineSize;
};

