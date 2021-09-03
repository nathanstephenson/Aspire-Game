#include "BufferController.h"

BufferController::BufferController() : m_Vertices(), m_Indices() {
    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 1000 * sizeof(Vertex));//can hold up to 1000 vertices
    m_Shader = std::make_unique<Shader>("res/shaders/shader.shader");
    m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, 1500 * sizeof(unsigned int));//can hold up to 1500 indices to be mapped to vertices
    VertexBufferLayout layout;
    layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
    layout.Push<float>(4);//repeated to include rgba values
    layout.Push<float>(2);//repeated to include texture coordinates
    layout.Push<float>(1);//repeated to include texture index
    m_VAO->AddBuffer(*m_VertexBuffer, layout);//needs to come after updateData
    m_Shader = std::make_unique<Shader>("res/shaders/shader.shader");
}

BufferController::~BufferController() {
}

void BufferController::AddObject() {
}

void BufferController::RemoveObject() {
}

void BufferController::UpdateObject() {
}

void BufferController::BindBuffers() {
    m_Shader->Bind();
    m_VAO->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
}

void BufferController::UnbindBuffers() {
    m_IndexBuffer->Unbind();
    m_VertexBuffer->Unbind();
    m_VAO->Unbind();
    m_Shader->Unbind();
}

void BufferController::BindTextureSlots(unsigned int start, unsigned int end) {
    for (unsigned int i = start; i <= end; i++) {
        m_Shader->SetUniform1i("u_Textures[" + std::to_string(i) + "]", i);//may require "#include <string>"
    }
}

void BufferController::Draw(glm::mat4 mvp) {
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void BufferController::SetVertices(Vertex* vertices, unsigned int count) {
    memcpy(m_Vertices, vertices, count * sizeof(Vertex));
    m_VertexBuffer->UpdateData(m_Vertices, count * sizeof(Vertex));
}

void BufferController::SetIndices(unsigned int* indices, unsigned int count) {
    memcpy(m_Indices, indices, count * sizeof(unsigned int));
    m_IndexBuffer->UpdateData(m_Indices, count * sizeof(unsigned int));
}
