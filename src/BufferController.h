#pragma once

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

class BufferController {
public:
	BufferController();
	~BufferController();
	void AddObject();
	void RemoveObject();
	void UpdateObject();
	void BindBuffers();
	void UnbindBuffers();
	void BindTextureSlots(unsigned int start, unsigned int end);//max end is 32
	void Draw(glm::mat4 mvp);
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	Vertex* m_Vertices;
	unsigned int* m_Indices;

	void SetVertices(Vertex* vertices, unsigned int count);//size is number of vertices
	void SetIndices(unsigned int* indices, unsigned int count);//size is number of indices
};