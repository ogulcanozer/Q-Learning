#include "renderer.h"
#include<iostream>


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const char* line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "OPEN_GL_ERROR :: " << error << " :: " << function << " : " << file << " in " << line << std::endl;
		return false;
	}
	return true;
}

void renderer::clear() const
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
}

void renderer::draw(const vertexArray& va, const indexBuffer& ib,const shader& shader) const
{
	shader.useShader();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

}

