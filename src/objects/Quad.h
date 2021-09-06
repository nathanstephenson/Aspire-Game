#pragma once
#include "Object.h"

static std::vector<Vertex>CreateQuad(float x, float y, float textureID) {
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

class Quad : public Object {
private:
public:
	Quad() { SetDimensions(20.0, 30.0); SetVertices(CreateQuad(5, 5, 1)); SetIndices({ 0, 1, 2, 2, 3, 0 }); }
	~Quad() {}

	void OnUpdate(float deltaTime) override {}
	void OnRender() override {}
	void OnImGuiRender() override {}
};