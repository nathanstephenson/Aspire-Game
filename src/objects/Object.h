#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

class Object {
private:

public:
	Object() {}
	virtual ~Object() {}

	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};
class ObjectContainer : public Object {
private:
	std::vector <Object*> m_Objects;
public:
	ObjectContainer(Object*& currentTestPointer) {}

	void OnImGuiRender() override;

	std::vector <Object*> GetObjects() { return m_Objects; }

	void AddObject(Object* obj) {
		m_Objects.push_back(obj);
	}

};