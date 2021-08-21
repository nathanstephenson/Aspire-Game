#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0){
    ShaderSource shaders = ParseShader(filepath);
    m_RendererID = CreateShader(shaders.vertexSource, shaders.fragmentSource);//create shader from sources
    std::cout << "VERTEX" << std::endl << shaders.vertexSource;
    std::cout << "FRAGMENT" << std::endl << shaders.fragmentSource;
    
    
}

Shader::~Shader(){
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderSource Shader::ParseShader(const std::string& filepath) {
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
            }
            else if (line.find("fragment") != std::string::npos) {
                t = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)t] << line << '\n';//based on the type of shader, insert the current line into the correct shader stringstream
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

void Shader::Bind() const{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const{
    glUseProgram(0);
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name){
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {//doesn't check location if already cached
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "WARNING: uniform '" << name << "' doesn't exist" << std::endl;
    } 
    m_UniformLocationCache[name] = location;
    return location;
}
