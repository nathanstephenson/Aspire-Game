#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestBatchingTexture.h"


namespace test {
	TestBatchingTexture::TestBatchingTexture() : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
            m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){
        float vertices[] = {//each line is a vertex: with an rgba value associated to it, and then texture coordinates and texture index on that (index -1 would mean to use colours instead)
            //  x       y      r     g      b     a     tx    ty    ti
             -50.0f, -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,//bottom left (0)
              50.0f, -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,//bottom right (1)
              50.0f,  50.0f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,//top right (2)
             -50.0f,  50.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,//top left (3)

              50.0f,  50.0f, 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//bottom left (4)
             150.0f,  50.0f, 1.0f, 0.96f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,//bottom right (5)
             150.0f, 150.0f, 1.0f, 0.96f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,//top right (6)
              50.0f, 150.0f, 1.0f, 0.96f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,//top left (7)
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
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8*9*sizeof(float));//rows*columns
        VertexBufferLayout layout;
        layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
        layout.Push<float>(4);//repeated to include rgba values
        layout.Push<float>(2);//repeated to include texture coordinates
        layout.Push<float>(1);//repeated to include texture index
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/texture.shader");
        m_Shader->Bind();

        m_Texture0 = std::make_unique<Texture>("res/textures/weirdKEKW.png");
        m_Texture1 = std::make_unique<Texture>("res/textures/hands.png");

        m_Shader->SetUniform1i("u_Textures[0]", 0);
        m_Shader->SetUniform1i("u_Textures[1]", 1);
	}

	TestBatchingTexture::~TestBatchingTexture(){
	}

	void TestBatchingTexture::OnUpdate(float deltaTime){
	}

	void TestBatchingTexture::OnRender(){
		//GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));//set background colour
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;

        m_Texture0->Bind(0);//each texture must be bound to its slot for rendering
        m_Texture1->Bind(1);

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);//model matrix
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);//Draw. All 3 are bound in this call
        }
	}

	void TestBatchingTexture::OnImGuiRender(){
        ImGui::SliderFloat3("Position", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}