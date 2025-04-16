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
    callback(99);  // 콜백 실행
}

int main() {
    Worker w;
    // std::bind 사용
    DoSomething(std::bind(&Worker::Process, &w, std::placeholders::_1));
} 