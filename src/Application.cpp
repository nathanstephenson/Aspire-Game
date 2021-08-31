#include "Application.h"

static std::array<Vertex, 4>CreateQuad(float x, float y, float textureID) {
    float size = 100.0f;
    Vertex v0;
    v0.Position = { x, y };
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.TexPos = { 0.0f, 0.0f };
    v0.TexIndex = textureID;
    Vertex v1;
    v1.Position = { x + size, y };
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.TexPos = { 1.0f, 0.0f };
    v1.TexIndex = textureID;
    Vertex v2;
    v2.Position = { x + size,  y + size };
    v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    v2.TexPos = { 1.0f, 1.0f };
    v2.TexIndex = textureID;
    Vertex v3;
    v3.Position = { x,  y + size };
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.TexPos = { 0.0f, 1.0f };
    v3.TexIndex = textureID;
    return { v0, v1, v2, v3 };
}

Application::Application(float windowWidth, float windowHeight) : m_CursorPos({0.0, 0.0}), m_CharPos({ 0.0, 0.0 }), m_CharVel({ 0.0, 0.0 }), m_lastFrameTime(0.0), m_deltaTime(0.0) {
    SetWindowSize(windowWidth, windowHeight);
    m_CharPos = {m_WindowSize.x / 2, m_WindowSize.y / 2 };
    unsigned int indices[] = {//each line is a triangle
        0, 1, 2,//up left
        2, 3, 0,//down right

        4, 5, 6,//up left
        6, 7, 4,//down right
    };
    
    //Buffers and binding vb to vao
    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 8 * sizeof(Vertex));//rows*columns
    VertexBufferLayout layout;
    layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
    layout.Push<float>(4);//repeated to include rgba values
    layout.Push<float>(2);//repeated to include texture coordinates
    layout.Push<float>(1);//repeated to include texture index

    m_VAO->AddBuffer(*m_VertexBuffer, layout);//needs to come after updateData
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

    m_Shader = std::make_unique<Shader>("res/shaders/shader.shader");
    m_Shader->Bind();

    m_Texture0 = std::make_unique<Texture>("res/textures/weirdKEKW.png");
    m_Texture1 = std::make_unique<Texture>("res/textures/hands.png");
}

Application::~Application() {
}

void Application::OnUpdate() {
	m_deltaTime = glfwGetTime() - m_lastFrameTime;
    frameCount += 1;

    m_CharPos = {m_CharPos.x + (m_CharVel.x * m_deltaTime), m_CharPos.y + (m_CharVel.y * m_deltaTime)};//position changes based on velocity and deltatime
    auto q0 = CreateQuad(m_CharPos.x, m_CharPos.y, 0.0f);
    auto q1 = CreateQuad(50.0f, 50.0f, -1.0f);
    Vertex vertices[8];
    memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
    memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
    m_VertexBuffer->UpdateData(vertices, 8 * sizeof(Vertex));
}
void Application::OnRender() {
	m_lastFrameTime = glfwGetTime();

    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    Renderer renderer;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Textures[0]", 0);
    m_Shader->SetUniform1i("u_Textures[1]", 1);
    m_Texture0->Bind(0);//each texture must be bound to its slot for rendering
    m_Texture1->Bind(1);

    {
        float cursorMovementMultiplier = 0.05;

        glm::mat4 m_Proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, -1.0f, 1.0f);
        glm::vec3 mouseTranslation = glm::vec3({ -m_CursorPos.x * cursorMovementMultiplier, -m_CursorPos.y * cursorMovementMultiplier, 0.0f });
        glm::vec3 moveTranslation = glm::vec3({ m_WindowSize.x/2-m_CharPos.x, m_WindowSize.y/2-m_CharPos.y, 0.0f });
        glm::mat4 viewMove = glm::translate(glm::mat4(1.0f), moveTranslation);//view matrix
        glm::mat4 viewMouse = glm::translate(glm::mat4(1.0f), mouseTranslation);//view matrix
        glm::mat4 view = viewMouse*viewMove;//view matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3({ 0.0f, 0.0f, 0.0f }));//model matrix
        glm::mat4 mvp = m_Proj * view * model;

        m_Shader->SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);//Draw all bound data. All 3 are bound in this call.
    }
    m_Texture0->Unbind();
    m_Texture1->Unbind();
    m_Shader->Unbind();

}
void Application::OnImGuiRender() {

}

void Application::SetCursorPos(float x, float y) {
	m_CursorPos = { x, y };
}

vec2 Application::GetCursorPos() {
	return m_CursorPos;
}

void Application::SetCharVel(float x, float y) {
    m_CharVel = { x, y };
}

vec2 Application::GetCharVel() {
    return m_CharVel;
}

float Application::GetDelta() {
	return m_deltaTime;
}

void Application::SetWindowSize(float x, float y) {
    m_WindowSize = {x, y};
}

vec2 Application::GetWindowSize() {
    return m_WindowSize;
}
