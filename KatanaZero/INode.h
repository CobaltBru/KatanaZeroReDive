#pragma once
#include <vector>
#include <functional>
#include <string>
#include "config.h"

enum NodeStatus
{
    Success,
    Failure,
    Running
};

class Node {
public:
    virtual ~Node() {}
    virtual NodeStatus tick() = 0;
};

class ActionNode : public Node {
public:
    ActionNode(std::string name, std::function<NodeStatus()> action) : name(name), action(action) {}

    NodeStatus tick() override {
        char buf[256];
        sprintf_s(buf, "[BT] %s::tick()\n", name.c_str());
        OutputDebugStringA(buf);
        return action();
    }

private:
    std::function<NodeStatus()> action;
    std::string name;
};

class Selector : public Node {
public:
    void addChild(Node* child) {
        children.push_back(child);
    }

    NodeStatus tick() override {
        for (auto& child : children) {
            NodeStatus status = child->tick();
            if (status != NodeStatus::Failure) {
                return status;
            }
        }
        return NodeStatus::Failure;
    }
    virtual ~Selector() {
        for (Node* c : children)
        {
            delete c;
        }
    }
private:
    std::vector<Node*> children;
};

class Sequence : public Node {
public:
    void addChild(Node* child) {
        children.push_back(child);
    }

    NodeStatus tick() override {
        for (auto& child : children) {
            NodeStatus status = child->tick();
            if (status == NodeStatus::Failure) {
                return status;
            }
            if (status == NodeStatus::Running) return status;
        }
        return NodeStatus::Success;
    }
    virtual ~Sequence() {
        
    }
private:
    std::vector<Node*> children;
};

class Parallel : public Node {
public:
    void addChild(Node* child) {
        children.push_back(child);
    }

    NodeStatus tick() override {
        for (auto& child : children) {
            NodeStatus status = child->tick();
        }
        return NodeStatus::Success;
    }
    virtual ~Parallel() {
        for (Node* c : children)
        {
            delete c;
        }
    }
private:
    std::vector<Node*> children;
};

class ConditionNode : public Node
{
public:
    ConditionNode(std::function<bool()> pred) : predicate(pred) {}
    NodeStatus tick() override
    {
        return predicate() ? NodeStatus::Success : NodeStatus::Failure;
    }
private:
    std::function<bool()> predicate;
};

class OnceNode : public Node {
public:
    // 생성 시 한 번 실행할 child 노드를 넘겨줍니다.
    OnceNode(Node* child)
        : child(child), done(false)
    {
    }

    ~OnceNode() override {
        delete child;
    }

    NodeStatus tick() override {
        if (done) {
            // 이미 한 번 성공했다면, 곧바로 Success
            return NodeStatus::Success;
        }
        // 한 번도 실행하지 않았으면 child를 tick
        NodeStatus s = child->tick();
        if (s == NodeStatus::Success) {
            // 성공이 돌아오면 done 플래그 세팅
            done = true;
        }
        // Running이나 Failure 도 그대로 리턴
        return s;
    }

private:
    Node* child;
    bool  done;
};

class CooldownNode : public Node {
    Node* child;
    float cooldown;     // 쿨다운 시간
    float timer = 0.f;  // 내부 타이머
public:
    CooldownNode(Node* c, float cd) : child(c), cooldown(cd) {}
    ~CooldownNode() override { delete child; }

    NodeStatus tick() override {
        float dt = TimerManager::GetInstance()->GetDeltaTime();
        if (timer > 0.f) {
            timer -= dt;
            return NodeStatus::Success; // 쿨다운 중엔 일단 Success
        }
        NodeStatus s = child->tick();
        if (s == NodeStatus::Success) {
            timer = cooldown;
        }
        return s;
    }
};