#pragma once
#pragma once
class vertexBuffer {
private:
	unsigned int m_RendererID;
public:
	vertexBuffer(const void* data, unsigned int size, int mode);
	~vertexBuffer();

	void bind() const;
	void unbind() const;
	void update(const void* data, unsigned int size, int offset);
};