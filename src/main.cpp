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
//using default types so that it is nicer to deal with non-opengl apis

//Built using a youtuber The Cherno's OpenGL tutorial:
//https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2



int main(void)
{
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
    window = glfwCreateWindow(800, 800, "Aspire", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {//this needs to come after setting the window as openGL context
        std::cout << "Error with GLEW" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;//print opengl version

    {//provides a scope so that the application terminates correctly

    float vertices[] = {//each line is a vertex
        -0.5f, -0.5f,//bottom left (0)
         0.5f, -0.5f,//bottom right (1)
         0.5f,  0.5f,//top right (2)
        -0.5f,  0.5f,//top left (3)
    };

    unsigned int indices[] = {//each line is a triangle
        0, 1, 2,//up left
        2, 3, 0,//down right
    };



    //Buffers and binding vb to vao
    VertexArray vao;
    VertexBuffer vb(vertices, 4*2*sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);//setting up the layout of the vertices array with its type and stride
    vao.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/shader.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    //clear buffer bindings
    vao.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//background colour
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


        GLClearErrors();
        renderer.Draw(vao, ib, shader);
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        ASSERT(GLLogCall());

        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;

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