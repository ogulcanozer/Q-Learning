#pragma once
#include "vertexBuffer.h"

class vertexBufferLayout;

class vertexArray {
private:
	unsigned int m_RendererID;
public:
	vertexArray();
	~vertexArray();

	void addLayout(const vertexBuffer& vbo, const vertexBufferLayout& vbl);

	void bind()const;
	void unbind()const;
};