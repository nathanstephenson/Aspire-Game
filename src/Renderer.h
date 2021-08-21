#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if (!(x)){__debugbreak();}//intrinsic to the compiler
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall());

void GLClearErrors();
bool GLLogCall();

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const;
};