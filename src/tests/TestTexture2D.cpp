#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestTexture2D.h"


namespace test {
	TestTexture2D::TestTexture2D() : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
            m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){
	    float vertices[] = {//each line is a vertex
             -50.0f, -50.0f, 0.0f, 0.0f,//bottom left (0)
              50.0f, -50.0f, 1.0f, 0.0f,//bottom right (1)
              50.0f,  50.0f, 1.0f, 1.0f,//top right (2)
             -50.0f,  50.0f, 0.0f, 1.0f,//top left (3)
        };

        unsigned int indices[] = {//each line is a triangle
            0, 1, 2,//up left
            2, 3, 0,//down right
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//tells OpenGL how to blend alpha pixels (which also allows transparency)


        //Buffers and binding vb to vao
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4*4*sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
        layout.Push<float>(2);//repeated to include texture coordinates
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/texture.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/weirdKEKW.png");
        m_Shader->SetUniform1i("u_Texture", 0);//0 because texture is bound to slot 0 (as it is unspecified in the constructor)
	}

	TestTexture2D::~TestTexture2D(){
	}

	void TestTexture2D::OnUpdate(float deltaTime){
	}

	void TestTexture2D::OnRender(){
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));//set background colour
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;

        m_Texture->Bind();

        m_Shader->Bind();//only need to bind once and all objects can be drawn
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);//model matrix
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);//Draw
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);//model matrix
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            GLClearErrors();
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);//Draw
            ASSERT(GLLogCall());
        }
	}

	void TestTexture2D::OnImGuiRender(){
        ImGui::SliderFloat3("Position A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Position B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}