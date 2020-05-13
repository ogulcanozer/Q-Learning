#pragma once
#include <vector>
#include <gl/glew.h>
#include "renderer.h"

struct vertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized; 

	static unsigned int getTypeSize(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	};
};
class vertexBufferLayout {
private:
	std::vector<vertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	vertexBufferLayout();

	template <typename T>
	void push(unsigned int count);



	const std::vector<vertexBufferElement> getElements() const;
	const unsigned int getStride() const;
};


