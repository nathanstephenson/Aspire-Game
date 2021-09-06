#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "BufferController.h"
#include "ObjectController.h"

static std::array<Vertex, 4>CreateQuad(float x, float y, float textureID) {
	float size = 100.0f;
	Vertex v0{};
	v0.Position = { x, y };
	v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v0.TexPos = { 0.0f, 0.0f };
	v0.TexIndex = textureID;
	Vertex v1{};
	v1.Position = { x + size, y };
	v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v1.TexPos = { 1.0f, 0.0f };
	v1.TexIndex = textureID;
	Vertex v2{};
	v2.Position = { x + size,  y + size };
	v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v2.TexPos = { 1.0f, 1.0f };
	v2.TexIndex = textureID;
	Vertex v3{};
	v3.Position = { x,  y + size };
	v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v3.TexPos = { 0.0f, 1.0f };
	v3.TexIndex = textureID;
	return { v0, v1, v2, v3 };
}

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

	std::unique_ptr<Texture> m_Texture0;
	std::unique_ptr<Texture> m_Texture1;
	std::unique_ptr<BufferController> m_BufferController;
	std::unique_ptr<ObjectController> m_ObjectContainer;
};