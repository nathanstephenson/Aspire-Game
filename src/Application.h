#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Application {
public:
	Application(float windowWidth, float windowHeight);
	~Application();

	void OnUpdate();
	void OnRender();
	void OnImGuiRender();

	void SetCursorPos(float x, float y);
	vec2 GetCursorPos();
	void SetCharVel(float x, float y);
	vec2 GetCharVel();
	float GetDelta();
	void SetWindowSize(float x, float y);
	vec2 GetWindowSize();
private:
	vec2 m_CursorPos;
	vec2 m_CharPos;
	vec2 m_CharVel;
	vec2 m_WindowSize;
	float m_lastFrameTime;//time is in seconds
	float m_deltaTime;
	int frameCount = 0;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture0;
	std::unique_ptr<Texture> m_Texture1;
};