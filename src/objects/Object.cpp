#include "Object.h"

Object::Object() : m_VertexPosition(0), m_Size({ 0.0, 0.0, 0.0 }), m_Origin({ 0.0, 0.0, 0.0 }), m_Rotation(0), m_Vertices({}), m_Indices({}), m_Type(blank) {

}
Object::~Object() {}

void Object::OnUpdate() {}
void Object::OnRender() {}
void Object::OnImGuiRender() {}

void Object::SetVertexPosition(unsigned int i) { 
	m_VertexPosition = i; 
}
unsigned int Object::GetVertexPosition() { return m_VertexPosition; }

void Object::SetDimensions(float width, float height) { 
	m_Size = { width, height }; 
}
vec3 Object::GetDimensions() { return m_Size; }

void Object::SetOrigin(float x, float y, float z) {
	m_Origin = { x, y };
	OnUpdate();
}
vec3 Object::GetOrigin() { return m_Origin; }

void Object::SetVertices(std::vector<Vertex> vertices) { 
	m_Vertices = vertices;/*memcpy(m_Vertices.data(), vertices.data(), m_Vertices.size());*/ 
}
Vertex* Object::GetVertices() { return m_Vertices.data(); }

void Object::SetIndices(std::vector<unsigned int> indices) { 
	m_Indices = indices; 
}
unsigned int* Object::GetIndices() { //allows indices to be manipulated based on the object's starting vertex position in the vertexbuffer
	std::valarray<unsigned int> ind(m_Indices.data(), m_Indices.size());
	ind += m_VertexPosition;
	std::vector<unsigned int>indices(std::begin(ind), std::end(ind));
	return m_Indices.data();
}