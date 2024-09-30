#ifndef INDEX_ARRAY_H
#define INDEX_ARRAY_H

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void DeleteBuffer();
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return Count; }

};

#endif