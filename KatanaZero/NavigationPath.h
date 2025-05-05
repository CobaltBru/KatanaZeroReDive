#pragma once
#include "config.h"

class NavigationPath
{
public:
    // 새 경로를 받아 들여 초기화
    void setPath(const std::vector<int>& newPath) {
        pathNodes = newPath;
        currentIdx = 0;
    }

    // 남은 웨이포인트가 있는지
    bool isEmpty() const {
        return currentIdx >= pathNodes.size();
    }

    // 현재 목표로 해야 할 노드 인덱스 반환
    // 경로가 비어 있으면 -1 반환
    int  getCurrentNode() const {
        if (isEmpty()) return -1;
        return pathNodes[currentIdx];
    }

    // 목표 도달 시 호출 → 다음 웨이포인트로 이동
    void advance() {
        if (!isEmpty()) ++currentIdx;
    }

    // 경로 완전 초기화(멈출 때)
    void clear() {
        pathNodes.clear();
        currentIdx = 0;
    }

    // (선택) 남은 경로 전체를 복사 반환
    std::vector<int> getRemainingPath() const {
        if (isEmpty()) return {};
        return std::vector<int>(pathNodes.begin() + currentIdx, pathNodes.end());
    }

    void setCurrentIdx(int newIdx) { currentIdx = newIdx; }

    int peekNextNode() const
    {
        if (currentIdx + 1 < pathNodes.size())
        {
            return pathNodes[currentIdx + 1];
        }
        return -1;
    }

private:
    std::vector<int> pathNodes;  // A*가 반환한 노드 인덱스 리스트
    std::size_t      currentIdx = 0;  // 지금 타겟으로 삼고 있는 pathNodes 인덱스
};
