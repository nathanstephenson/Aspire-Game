#pragma once
#include <GL/glew.h>


class IndexBuffer {
private:
	unsigned int m_RendererID;//unique id for the specific vb object being created
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateData(const unsigned int* data, unsigned int size) const;

	inline unsigned int GetCount() const { return m_Count; }
};