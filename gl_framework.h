#ifndef GL_FRAMEWORK_H
#define GL_FRAMEWORK_H

#include <GL/glew.h>
#include <glm/glm.hpp>


//error handling
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#endif