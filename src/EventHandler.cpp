#include "EventHandler.h"


EventHandler::EventHandler() : m_cursorPos({0.0, 0.0}), m_app(nullptr) {
}

EventHandler::~EventHandler() {

}

void EventHandler::Poll() {
	glfwPollEvents();
}

void EventHandler::Wait() {
	glfwWaitEvents();
}

void EventHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {//doesn't wait for polling
	std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
}

void EventHandler::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	//std::cout << xpos << " " << ypos << std::endl;
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	m_app->SetCursorPos(xpos, wHeight - ypos);
}

void EventHandler::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	std::cout << button << " " << action << " " << mods << std::endl;
	vec2 cursor = m_app->GetCursorPos();
	if (button==0 && action==0) {//left click release
		std::cout << cursor.x << " " << cursor.y << std::endl;
	}
}

void EventHandler::SetApplication(Application* app) {
	m_app = app;
}
