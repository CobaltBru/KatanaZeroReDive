#pragma once
#include <vector>
#include <functional>

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
    ActionNode(std::function<NodeStatus()> action) : action(action) {}

    NodeStatus tick() override {
        return action();
    }

private:
    std::function<NodeStatus()> action;
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
            if (status != NodeStatus::Success) {
                return status;
            }
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

