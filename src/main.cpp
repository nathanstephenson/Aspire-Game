#include <GL/glew.h>//needs to be included before other openGL stuff
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//using default types so that it is nicer to deal with non-opengl apis
#define ASSERT(x) if (!(x)){__debugbreak();}//intrinsic to the compiler
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall());

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall() {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error << std::endl;
        return false;
    }
    return true;
}

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Aspire", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {//this needs to come after setting the window as openGL context
        std::cout << "Error with GLEW" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;//print opengl version

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

    //Create and bind vertex buffer, then feed it data
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (const void*)0);//define the buffer
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), vertices, GL_STATIC_DRAW);//add vertex data
    glEnableVertexAttribArray(0);//enable buffer (at index 0) so it can be used for rendering
    glBindBuffer(GL_ARRAY_BUFFER, 0);//clear buffer binding

    //Create and bind index buffer, then feed it data
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);//add index data
    //dont clear IBO as it causes an error

    ShaderSource shaders = ParseShader("res/shaders/shader.shader");
    std::cout << "VERTEX" << std::endl << shaders.vertexSource;
    std::cout << "FRAGMENT" << std::endl << shaders.fragmentSource;
    //create shader from sources
    unsigned int shader = CreateShader(shaders.vertexSource, shaders.fragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//background colour
        glClear(GL_COLOR_BUFFER_BIT);
        GLClearErrors();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        ASSERT(GLLogCall());
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}