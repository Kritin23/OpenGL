#include "gl_framework.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[GLError] (" << error << ") " << function << " " << file << ":" << line << "\n";
        return false;
    }
    return true;
}

