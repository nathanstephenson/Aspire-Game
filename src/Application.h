#pragma once
#include "Renderer.h"

class Application {
public:
	Application();
	~Application();

	void OnUpdate();
	void OnRender();
	void OnImGuiRender();

	void SetCursorPos(float x, float y);
	vec2 GetCursorPos();
private:
	vec2 m_CursorPos;
};