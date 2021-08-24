#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestBatchingColor.h"


namespace test {
	TestBatchingColor::TestBatchingColor() : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
            m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){

        float vertices[] = {//each line is a vertex with an rgba value associated to it
             -50.0f, -50.0f, 0.18f, 0.6f, 0.96f, 1.0f,//bottom left (0)
              50.0f, -50.0f, 0.18f, 0.6f, 0.96f, 1.0f,//bottom right (1)
              50.0f,  50.0f, 1.0f, 1.0f, 1.0f, 1.0f,//top right (2)
             -50.0f,  50.0f, 0.18f, 0.6f, 0.96f, 1.0f,//top left (3)

              50.0f,   50.0f, 1.0f, 1.0f, 1.0f, 1.0f,//bottom left (4)
             150.0f,   50.0f, 1.0f, 0.96f, 0.24f, 1.0f,//bottom right (5)
             150.0f,  150.0f, 1.0f, 0.96f, 0.24f, 1.0f,//top right (6)
              50.0f,  150.0f, 1.0f, 0.96f, 0.24f, 1.0f,//top left (7)
        };

        unsigned int indices[] = {//each line is a triangle
            0, 1, 2,//up left
            2, 3, 0,//down right

            4, 5, 6,//up left
            6, 7, 4,//down right
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//tells OpenGL how to blend alpha pixels (which also allows transparency)


        //Buffers and binding vb to vao
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8*6*sizeof(float));//vertices' rows*columns
        VertexBufferLayout layout;
        layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
        layout.Push<float>(4);//repeated to include rgba values (would be 2 if including texture coordinates)
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/color.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	}

	TestBatchingColor::~TestBatchingColor(){
	}

	void TestBatchingColor::OnUpdate(float deltaTime){
	}

	void TestBatchingColor::OnRender(){
        Renderer renderer;

        m_Shader->Bind();//only need to bind once and all objects can be drawn
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);//model matrix
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);//Draw
        }
	}

	void TestBatchingColor::OnImGuiRender(){
        ImGui::SliderFloat3("Position A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}