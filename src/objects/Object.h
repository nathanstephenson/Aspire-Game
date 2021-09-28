#pragma once
#include "Renderer.h"
#include <array>
#include <valarray>

class Object {
private:
	unsigned int m_VertexPosition;
	vec3 m_Size;
	vec3 m_Origin;
	float m_Rotation;
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	ObjectType m_Type;
public:
	Object() : m_VertexPosition(0), m_Size({ 0.0, 0.0, 0.0 }), m_Origin({ 0.0, 0.0, 0.0 }), m_Rotation(0), m_Vertices({}), m_Indices({}), m_Type(blank) {}
	virtual ~Object() {}

	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}

	/*void Transform(float x, float y, float z, float rotation) {
		std::vector<Vertex> v = *GetVertices();		
		for (unsigned int i = 0; i < v.size(); i++) {
			std::cout << v[i].Position.x << ", " << v[i].Position.y << std::endl;
			v[i].Position.x += x;
			v[i].Position.y += y;
		}
		SetVertices(v);
		SetOrigin(v[0].Position.x, v[0].Position.y);
	}*/

	void SetVertexPosition(unsigned int i) { m_VertexPosition = i; }
	unsigned int GetVertexPosition() { return m_VertexPosition; }

	void SetDimensions(float width, float height) { m_Size = {width, height}; }
	vec3 GetDimensions() { return m_Size; }

	void SetOrigin(float x, float y, float z) { 
		m_Origin = {x, y};
		OnUpdate();
	}
	vec3 GetOrigin() { return m_Origin; }

	void SetVertices(std::vector<Vertex> vertices) { m_Vertices = vertices;/*memcpy(m_Vertices.data(), vertices.data(), m_Vertices.size());*/ }
	Vertex* GetVertices() { return m_Vertices.data(); }

	void SetIndices(std::vector<unsigned int> indices) { m_Indices = indices; }
	unsigned int* GetIndices() { //allows indices to be manipulated based on the object's starting vertex position in the vertexbuffer
		std::valarray<unsigned int> ind(m_Indices.data(), m_Indices.size());
		ind += m_VertexPosition;
		std::vector<unsigned int>indices(std::begin(ind), std::end(ind));
		return m_Indices.data();
	}
};