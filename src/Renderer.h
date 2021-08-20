#pragma once

#include <GL/glew.h>


#define ASSERT(x) if (!(x)){__debugbreak();}//intrinsic to the compiler
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall());

void GLClearErrors();
bool GLLogCall();