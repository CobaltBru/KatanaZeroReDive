#pragma once
#include "GameObject.h"


class PlayerController: public GameObject
{
public:
	

};

#include <iostream>
#include <functional>

class Worker {
public:
    void Process(int x) {
        std::cout << "Worker processing " << x << std::endl;
    }
};

void DoSomething(std::function<void(int)> callback) {
    std::cout << "Doing something...\n";
    callback(99);  // �ݹ� ����
}

int main() {
    Worker w;
    // std::bind ���
    DoSomething(std::bind(&Worker::Process, &w, std::placeholders::_1));
} 