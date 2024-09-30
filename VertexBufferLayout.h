#pragma once
#include <vector>
#include <GL/glew.h>
#include "gl_framework.h"



struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;

	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> Elements;
	unsigned int stride;

public:
	VertexBufferLayout() : stride(0)
	{}

	template<typename T>
	void Push(unsigned int count)
	{
	}

	template<>
	void Push<float>(unsigned int count)
	{
		Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<char>(unsigned int count)
	{
		Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const
	{
		return Elements;
	}
	inline unsigned int GetStride() const { return stride; }
};