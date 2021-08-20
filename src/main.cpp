#include <GL/glew.h>//needs to be included before other openGL stuff
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
//using default types so that it is nicer to deal with non-opengl apis


struct ShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string& filepath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType t = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                t = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                t = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)t] << line << '\n';//based on the type of shader, insert the current line into the correct shader stringstream
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();//decode the shader source into a char array
    glShaderSource(id, 1, &src, nullptr);//sets shader code
    glCompileShader(id);
    //check for errors and output error details to console
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type==GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);//creates executables to run data on each attached shader
    glValidateProgram(program);
    //Clean up shaders once they have been compiled on the shader program
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    //vao setup
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    //Buffers and binding vb to vao
    VertexBuffer vb(vertices, 4*2*sizeof(float));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);//define the buffer
    glEnableVertexAttribArray(0);//enable buffer (at index 0) so it can be used for rendering
    IndexBuffer ib(indices, 6);


    ShaderSource shaders = ParseShader("res/shaders/shader.shader");
    std::cout << "VERTEX" << std::endl << shaders.vertexSource;
    std::cout << "FRAGMENT" << std::endl << shaders.fragmentSource;
    //create shader from sources
    unsigned int shader = CreateShader(shaders.vertexSource, shaders.fragmentSource);
    GLCall(glUseProgram(shader));


    //clear buffer bindings
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 


    //uniform const in shader
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1)
    //GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//background colour
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glBindVertexArray(vao));
        ib.Bind();

        GLClearErrors();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
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

    glDeleteProgram(shader);
    }
    //delete vbo and ibo
    glfwTerminate();
    return 0;
}