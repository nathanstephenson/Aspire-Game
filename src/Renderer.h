#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if (!(x)){__debugbreak();}//intrinsic to the compiler
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall());

void GLClearErrors();
bool GLLogCall();

struct vec2 {
	float x, y;
};

struct vec3 {
	float x, y, z;
};

struct vec4 {
	float x, y, z, w;
};

struct Vertex {
	vec3 Position;
	vec4 Color;
	vec2 TexPos;
	float TexIndex;
};

enum ObjectType {
	blank, quad
};

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const;
};