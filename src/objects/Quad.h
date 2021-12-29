#pragma once
#include "Object.h"
static std::vector<Vertex>CreateQuad(float size, float x, float y, float z, float textureID) {
	Vertex v0{};//bottom left
	v0.Position = { x - (size / 2), y - (size / 2), 0.0 };
	v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v0.TexPos = { 0.0f, 0.0f };
	v0.TexIndex = textureID;
	Vertex v1{};//bottom right
	v1.Position = { x + (size / 2), y - (size / 2), 0.0 };
	v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v1.TexPos = { 1.0f, 0.0f };
	v1.TexIndex = textureID;
	Vertex v2{};//top right
	v2.Position = { x + (size / 2),  y + (size / 2), 0.0 };
	v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	v2.TexPos = { 1.0f, 1.0f };
	v2.TexIndex = textureID;
	Vertex v3{};//top left
	v3.Position = { x - (size / 2),  y + (size / 2), 0.0 };
	v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v3.TexPos = { 0.0f, 1.0f };
	v3.TexIndex = textureID;
	return { v0, v1, v2, v3 };
}

class Quad : public Object {
private:
public:
	Quad() { SetDimensions(20.0, 30.0); SetVertices(CreateQuad(250, GetOrigin().x - 50, GetOrigin().y - 50, 0, 1)); SetIndices({ 0, 1, 2, 2, 3, 0 }); std::cout << "created quad" << std::endl; }
	~Quad() {}

	void OnUpdate() override { SetVertices(CreateQuad(100, GetOrigin().x - 50, GetOrigin().y - 50, 0, 0)); }
	void OnRender() override {}
	void OnImGuiRender() override {}
};