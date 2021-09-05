#pragma once
#include "Object.h"

class Quad : public Object {
private:
public:
	Quad() { SetDimensions(20.0, 30.0); }
	~Quad() {}

	void OnUpdate(float deltaTime) override {}
	void OnRender() override {}
	void OnImGuiRender() override {}
};