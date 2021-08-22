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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Built using youtube channel The Cherno's OpenGL tutorial:
//https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2


int main(void)//using default types so that it is nicer to deal with non-opengl apis
{
    float wWidth = 960.0f;
    float wHeight = 540.0f;

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
    glfwSwapInterval(1);//value of 1 binds it to the monitor's refresh rate, so VSync

    if (glewInit() != GLEW_OK) {//this needs to come after setting the window as openGL context
        std::cout << "Error with GLEW" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;//print opengl version

    {//provides a scope so that the application terminates correctly


    float vertices[] = {//each line is a vertex
         100.0f, 100.0f, 0.0f, 0.0f,//bottom left (0)
         200.0f, 100.0f, 1.0f, 0.0f,//bottom right (1)
         200.0f, 200.0f, 1.0f, 1.0f,//top right (2)
         100.0f, 200.0f, 0.0f, 1.0f,//top left (3)
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
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));//view matrix ("camera")
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));//model matrix
    glm::mat4 mvp = proj * view * model;
    Shader shader("res/shaders/shader.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    shader.SetUniformMat4f("u_MVP", mvp);//transforming vertices to match the already defined mvp matrix

    Texture texture("res/textures/weirdKEKW.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);//0 because texture is bound to slot 0 (as it is unspecified in the constructor)

    //clear buffer bindings
    vao.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float b = 1.0f;
    float increment = 0.005f;

    
    while (!glfwWindowShouldClose(window)){//Loop until the user closes the window
        /* Render here */
        GLCall(glClearColor(r, 0.13f, b, 1.0f));//set background colour
        renderer.Clear();//clear frame to background colour

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        GLClearErrors();
        renderer.Draw(vao, ib, shader);//Draw all
        ASSERT(GLLogCall());

        if (r > 1.0f) {//animated colour by changing value of red
            increment = -0.005f;
        } else if (r < 0.0f) {
            increment = 0.005f;
        }
        r += increment;
        b -= increment;


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    //buffers and shader deleted by destructor as the scope ends
    }//end application scope before killing glfw
    glfwTerminate();
    return 0;
}