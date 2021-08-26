#pragma once
#include <GL/glew.h>




class VertexBuffer {
private:
	unsigned int m_RendererID;//unique id for the specific vb object being created
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateData(const void* data) const;
};