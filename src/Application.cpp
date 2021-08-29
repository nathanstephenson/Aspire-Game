#include "Application.h"

Application::Application() : m_CursorPos({0.0, 0.0}) {

}

Application::~Application() {
}

void Application::OnUpdate() {

}
void Application::OnRender() {

}
void Application::OnImGuiRender() {

}

void Application::SetCursorPos(float x, float y) {
	m_CursorPos = { x, y };
}

vec2 Application::GetCursorPos() {
	return m_CursorPos;
}
