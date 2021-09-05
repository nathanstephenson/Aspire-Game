#pragma once
#include "Renderer.h"

class Object {
private:
	vec2 m_Size;
	vec2 m_Origin;
public:
	Object() : m_Size({ 0.0, 0.0 }), m_Origin({ 0.0, 0.0 }) {}
	virtual ~Object() {}

	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}

	virtual void SetDimensions(float width, float height) { m_Size = {width, height}; }
	vec2 GetDimensions() { return m_Size; }
	virtual void SetOrigin(float x, float y) { m_Origin = {x, y}; }
	vec2 GetOrigin() { return m_Origin; }
};