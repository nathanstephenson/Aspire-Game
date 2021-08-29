#include "EventHandler.h"


EventHandler::EventHandler() {
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
	cursorX = xpos;
	cursorY = ypos;
}

void EventHandler::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	std::cout << button << " " << action << " " << mods << std::endl;
	if (button==0 && action==0) {//left click release
		std::cout << cursorX << " " << cursorY << std::endl;
	}
}