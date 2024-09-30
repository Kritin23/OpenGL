#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "gl_framework.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};

