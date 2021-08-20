#include "Renderer.h"

#include <iostream>

void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall() {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error << std::endl;
        return false;
    }
    return true;
}