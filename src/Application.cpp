#include "Application.h"


Application::Application(float windowWidth, float windowHeight) : m_CursorPos({0.0, 0.0}), m_CharPos({ 0.0, 0.0 }), m_CharVel({ 0.0, 0.0 }), m_lastFrameTime(0.0), m_deltaTime(0.0),
        m_BufferController(std::make_unique<BufferController>()), m_ObjectController(std::make_unique<ObjectController>("filepath")){
    SetWindowSize(windowWidth, windowHeight);
    m_CharPos = {m_WindowSize.x / 2, m_WindowSize.y / 2 };
    
    //objects are batch loaded into the objectcontroller for a scene, then rendered dynamically using the buffercontroller

    m_Texture0 = std::make_unique<Texture>("res/textures/weirdKEKW.png");
    m_Texture1 = std::make_unique<Texture>("res/textures/hands.png");

    //m_ObjectController->AddObject(quad);
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
    unsigned int indices[] = {//each line is a triangle
        0, 1, 2,//up left
        2, 3, 0,//down right

        4, 5, 6,//up left
        6, 7, 4,//down right
    };
    //std::vector<Vertex> v;
    //std::vector<unsigned int> i;
    //std::copy(vertices[0], vertices[7], v);
    //std::copy(indices[0], indices[11], i);

    //m_ObjectController->GetObjects()[0]->Transform(m_CharPos.x + (m_CharVel.x * m_deltaTime), m_CharPos.y + (m_CharVel.y * m_deltaTime), 0, 0);

    m_BufferController->BindBuffers();

    m_BufferController->UpdateObject(vertices, 8*sizeof(Vertex), indices, 12*sizeof(unsigned int));
    //m_BufferController->UpdateObject(m_ObjectController->GetObjects()[0]->GetVertices(), 8*sizeof(Vertex), m_ObjectController->GetObjects()[0]->GetIndices(), 12*sizeof(unsigned int));
}
void Application::OnRender() {
	m_lastFrameTime = glfwGetTime();

    m_BufferController->BindTextureSlots(0, 1);
    m_Texture0->Bind(0);//each texture must be bound to its slot for rendering
    m_Texture1->Bind(1);

    {
        float cursorMovementMultiplier = 0.05;

        glm::mat4 m_Proj = glm::ortho(0.0f, m_WindowSize.x, 0.0f, m_WindowSize.y, -1.0f, 1.0f);//projection matrix
        glm::vec3 mouseTranslation = glm::vec3({ -m_CursorPos.x * cursorMovementMultiplier, -m_CursorPos.y * cursorMovementMultiplier, 0.0f });
        glm::vec3 moveTranslation = glm::vec3({ m_WindowSize.x/2-m_CharPos.x, m_WindowSize.y/2-m_CharPos.y, 0.0f });
        glm::mat4 viewMove = glm::translate(glm::mat4(1.0f), moveTranslation);//view matrix
        glm::mat4 viewMouse = glm::translate(glm::mat4(1.0f), mouseTranslation);//view matrix
        glm::mat4 view = viewMouse*viewMove;//view matrix
        glm::mat4 mvp = m_Proj * view;//ignoring model matrix in mvp as it is never changed from its origin. 

        m_BufferController->Draw(mvp);
    }
    m_Texture0->Unbind();
    m_Texture1->Unbind();
    m_BufferController->UnbindBuffers();
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
