#pragma once
#include "config.h"

class NavigationPath
{
public:
    // �� ��θ� �޾� �鿩 �ʱ�ȭ
    void setPath(const std::vector<int>& newPath) {
        pathNodes = newPath;
        currentIdx = 0;
    }

    // ���� ��������Ʈ�� �ִ���
    bool isEmpty() const {
        return currentIdx >= pathNodes.size();
    }

    // ���� ��ǥ�� �ؾ� �� ��� �ε��� ��ȯ
    // ��ΰ� ��� ������ -1 ��ȯ
    int  getCurrentNode() const {
        if (isEmpty()) return -1;
        return pathNodes[currentIdx];
    }

    // ��ǥ ���� �� ȣ�� �� ���� ��������Ʈ�� �̵�
    void advance() {
        if (!isEmpty()) ++currentIdx;
    }

    // ��� ���� �ʱ�ȭ(���� ��)
    void clear() {
        pathNodes.clear();
        currentIdx = 0;
    }

    // (����) ���� ��� ��ü�� ���� ��ȯ
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
    std::vector<int> pathNodes;  // A*�� ��ȯ�� ��� �ε��� ����Ʈ
    std::size_t      currentIdx = 0;  // ���� Ÿ������ ��� �ִ� pathNodes �ε���
};
