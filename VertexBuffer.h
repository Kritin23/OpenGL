#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void DeleteBuffer();
	void Bind() const;
	void Unbind() const;

};

#endif