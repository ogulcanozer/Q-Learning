#pragma once
#include <GL/glew.h>
#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

class renderer {

public:

	void clear() const;
	void draw(const vertexArray& va, const indexBuffer& ib, const shader& shader) const;
	

};

