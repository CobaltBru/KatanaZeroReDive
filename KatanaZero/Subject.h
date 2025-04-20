#pragma once


class Observer;
class Subject
{
	virtual ~Subject() = default;
	virtual void addObserver(Observer* o) = 0;
	virtual void removeObserver(Observer* o) = 0;
	virtual void notify() = 0;
};

