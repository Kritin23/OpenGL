#pragma once
#include "Renderer.h"

#define _CRT_SECURE_NO_WARNINGS

/*
#include "Renderer.h"

class Texture
{
	unsigned int RendererID;
	std::string textureFile;
	int Width, Height, BPP;	//BPP - Bytes Per Pixel

public:
	Texture(const std::string path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() { return Width; }
	inline int GetHeight() { return Height; }

};*/

GLuint LoadTexture(const char* filename, int width, int height);
void FreeTexture(GLuint texture);

