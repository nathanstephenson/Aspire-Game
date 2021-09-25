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
	//std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
	vec2 charVel = m_app->GetCharVel();
	switch (key) {
	case (65):
		if (action == 1) {
			m_app->SetCharVel(-100.0, charVel.y);
		}
		else if (action == 0) {
			m_app->SetCharVel(0, charVel.y);
		}
		break;
	case (68):
		if (action == 1) {
			m_app->SetCharVel(100.0, charVel.y);
		}
		else if (action == 0) {
			m_app->SetCharVel(0, charVel.y);
		}
		break;
	case (83):
		if (action == 1) {
			m_app->SetCharVel(charVel.x, -25.0);
		}
		else if (action == 0) {
			m_app->SetCharVel(charVel.x, 0);
		}
		break;
	case (87):
		if (action == 1) {
			m_app->SetCharVel(charVel.x, 25.0);
		}
		else if (action == 0) {
			m_app->SetCharVel(charVel.x, 0);
		}
		break;
	}
}

void EventHandler::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
	//std::cout << xpos << " " << ypos << std::endl;
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	m_app->SetCursorPos(xpos, wHeight - ypos);
}

void EventHandler::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	//std::cout << button << " " << action << " " << mods << std::endl;
	vec2 cursor = m_app->GetCursorPos();
	if (button==0 && action==0) {//left click release
		std::cout << "Cursor at (" << cursor.x << ", " << cursor.y << "); Rendered in " << m_app->GetDelta()*1000 << "ms" << std::endl;
	}
}

void EventHandler::WindowSizeCallback(GLFWwindow* window, int width, int height) {
	m_app->SetWindowSize(width, height);
}

void EventHandler::SetApplication(Application* app) {
	m_app = app;
}
