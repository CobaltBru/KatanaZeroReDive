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
    // ���� �� �� �� ������ child ��带 �Ѱ��ݴϴ�.
    OnceNode(Node* child)
        : child(child), done(false)
    {
    }

    ~OnceNode() override {
        delete child;
    }

    NodeStatus tick() override {
        if (done) {
            // �̹� �� �� �����ߴٸ�, ��ٷ� Success
            return NodeStatus::Success;
        }
        // �� ���� �������� �ʾ����� child�� tick
        NodeStatus s = child->tick();
        if (s == NodeStatus::Success) {
            // ������ ���ƿ��� done �÷��� ����
            done = true;
        }
        // Running�̳� Failure �� �״�� ����
        return s;
    }

private:
    Node* child;
    bool  done;
};

class CooldownNode : public Node {
    Node* child;
    float cooldown;     // ��ٿ� �ð�
    float timer = 0.f;  // ���� Ÿ�̸�
public:
    CooldownNode(Node* c, float cd) : child(c), cooldown(cd) {}
    ~CooldownNode() override { delete child; }

    NodeStatus tick() override {
        float dt = TimerManager::GetInstance()->GetDeltaTime();
        if (timer > 0.f) {
            timer -= dt;
            return NodeStatus::Success; // ��ٿ� �߿� �ϴ� Success
        }
        NodeStatus s = child->tick();
        if (s == NodeStatus::Success) {
            timer = cooldown;
        }
        return s;
    }
};