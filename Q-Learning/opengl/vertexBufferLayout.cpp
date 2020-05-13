#include "vertexBufferLayout.h"

vertexBufferLayout::vertexBufferLayout()
	:m_Stride(0)
{
	
}

const std::vector<vertexBufferElement> vertexBufferLayout::getElements() const
{
	return m_Elements;
}

const unsigned int vertexBufferLayout::getStride() const
{
	return m_Stride;
}



template<typename T>
void vertexBufferLayout::push(unsigned int count)
{
	static_assert(false);
}
template<>
void vertexBufferLayout::push<float>(unsigned int count) {
	m_Elements.push_back({ GL_FLOAT, count, GL_TRUE });
	m_Stride += count * vertexBufferElement::getTypeSize(GL_FLOAT);
}
template<>
void vertexBufferLayout::push<unsigned int>(unsigned int count) {
	m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_Stride += count * vertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
}
template<>
void vertexBufferLayout::push<unsigned char>(unsigned int count) {
	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	m_Stride += count * vertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
}


