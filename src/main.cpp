#include <GL/glew.h>//needs to be included before other openGL stuff
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

//Built using youtube channel The Cherno's OpenGL tutorial:
//https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2


int main(void)//using default types so that it is nicer to deal with non-opengl apis
{
    float wWidth = 960.0f;
    float wHeight = 540.0f;
    unsigned int swapInterval = 1;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        return -1;
    }
    
    //setting opengl versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(wWidth, wHeight, "Aspire", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(swapInterval);//value of 1 binds it to the monitor's refresh rate, so VSync

    if (glewInit() != GLEW_OK) {//this needs to come after setting the window as openGL context
        std::cout << "Error with GLEW" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;//print opengl version

    {//provides a scope so that the application terminates correctly


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
    VertexArray vao;
    VertexBuffer vb(vertices, 4*4*sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
    layout.Push<float>(2);//repeated to include texture coordinates
    vao.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    //create mvp matrix and apply to shader
    glm::mat4 proj = glm::ortho(0.0f, wWidth, 0.0f, wHeight, -1.0f, 1.0f);//setting the vertex boundaries of the window to follow the (default) window size, so every unit is a pixel
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//view matrix ("camera")
    Shader shader("res/shaders/shader.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/weirdKEKW.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);//0 because texture is bound to slot 0 (as it is unspecified in the constructor)

    //clear buffer bindings
    vao.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    float r = 0.0f;
    float b = 1.0f;
    float increment = 0.005f;

    bool show_demo_window = true;//imgui variables
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    while (!glfwWindowShouldClose(window)){//Loop until the user closes the window
        GLCall(glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f));//set background colour
        renderer.Clear();//clear frame to background colour
        ImGui_ImplGlfwGL3_NewFrame();//new imgui frame
        shader.Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);//model matrix
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            renderer.Draw(vao, ib, shader);//Draw
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);//model matrix
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix
            //shader.Bind();
            GLClearErrors();
            renderer.Draw(vao, ib, shader);//Draw
            ASSERT(GLLogCall());
        }
        
        if (r > 1.0f) {//animated colour by changing value of red
            increment = -0.005f;
        } else if (r < 0.0f) {
            increment = 0.005f;
        }
        r += increment;
        b -= increment;

        {//imgui scope
            ImGui::SliderFloat3("Position A", &translationA.x, 0.0f, wWidth);          // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::SliderFloat3("Position B", &translationB.x, 0.0f, wWidth);          // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("Background", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    //buffers and shader deleted by destructor as the scope ends
    }//end application scope before killing glfw
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}