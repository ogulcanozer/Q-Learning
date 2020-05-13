#include "vertexArray.h"
#include "vertexBufferLayout.h"

vertexArray::vertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

vertexArray::~vertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void vertexArray::addLayout(const vertexBuffer& vbo, const vertexBufferLayout& vbl)
{
	bind();
	vbo.bind();
	unsigned int offset = 0;
	const std::vector<vertexBufferElement> elements= vbl.getElements();

	for (unsigned int i = 0; i < elements.size(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].count , elements[i].type, elements[i].normalized, vbl.getStride(), (const void*)offset);
		offset += elements[i].count * vertexBufferElement::getTypeSize(elements[i].type);
	}

}

void vertexArray::bind() const
{
	glBindVertexArray(m_RendererID);
}

void vertexArray::unbind() const
{
	glBindVertexArray(0);
}
