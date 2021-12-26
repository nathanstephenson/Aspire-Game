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
	Object();
	virtual ~Object();

	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnImGuiRender();

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

	void SetVertexPosition(unsigned int i);
	unsigned int GetVertexPosition();

	void SetDimensions(float width, float height);
	vec3 GetDimensions();

	void SetOrigin(float x, float y, float z);
	vec3 GetOrigin();

	void SetVertices(std::vector<Vertex> vertices);
	Vertex* GetVertices();

	void SetIndices(std::vector<unsigned int> indices);
	unsigned int* GetIndices();
};