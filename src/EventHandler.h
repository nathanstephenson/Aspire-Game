#pragma once
#include <GL/glew.h>//needs to be included before other openGL stuff
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "Application.h"

enum EventType { KbInput, MInput, MMove };

class EventHandler {
private:
	vec2 m_cursorPos;
	Application* m_app;
public:
	EventHandler();
	~EventHandler();
	void Poll();
	void Wait();
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CursorCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	void SetApplication(Application* app);
};