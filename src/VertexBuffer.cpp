//#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    //Create and bind vertex buffer, then feed it data
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//add vertex data
}

VertexBuffer::~VertexBuffer(){
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
