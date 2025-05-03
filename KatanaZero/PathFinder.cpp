#include "PathFinder.h"
#include "CommonFunction.h"
#include <queue>

int PathFinder::findClosestNode(const FPOINT& pos) const
{
	int bestIdx = 0;
	float bestDist = numeric_limits<float>::infinity();
	for (int i = 0; i < (int)nodes.size(); ++i)
	{
		float d = Distance(pos, nodes[i]);
		if (d < bestDist)
		{
			bestDist = d;
			bestIdx = i;
		}
	}
	return bestIdx;
}

vector<int> PathFinder::FindPath(int start, int goal) const
{
	const int N = nodes.size();
	vector<float> gScore(N, numeric_limits<float>::infinity());
	vector<float> fScore(N, numeric_limits<float>::infinity());

	vector<int> from(N, -1);
	vector<bool> closedSet(N, false);

	auto heuristic = [&](int u) { return Distance(nodes[u], nodes[goal]); };
	
	struct NodeRecord
	{
		int idx;
		float f;
		bool operator>(NodeRecord const& other) const { return f > other.f; }
	};
	priority_queue<NodeRecord, vector<NodeRecord>, greater<NodeRecord>> openPQ;

	gScore[start] = 0.f;
	fScore[start] = heuristic(start);
	openPQ.push({ start, fScore[start] });

	while (!openPQ.empty())
	{
		int current = openPQ.top().idx;
		openPQ.pop();

		if (current == goal)
		{
			vector<int> path;
			for (int at = goal; at != -1; at = from[at])
			{
				path.push_back(at);
			}
			reverse(path.begin(), path.end());
			return path;
		}

		if (closedSet[current]) continue;
		closedSet[current] = true;

		for (auto const& e : graph[current])
		{
			int neighbor = e.to;
			if (closedSet[neighbor]) continue;

			float newG = gScore[current] + e.cost;
			if (newG < gScore[neighbor])
			{
				from[neighbor] = current;
				gScore[neighbor] = newG;
				fScore[neighbor] = newG + heuristic(neighbor);
				openPQ.push({ neighbor, fScore[neighbor] });
			}
		}
	}

	return vector<int>();
}
