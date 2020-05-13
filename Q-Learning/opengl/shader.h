#pragma once
#include <gl/glew.h>
#include <gl/glut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>


class shader
{

public:
    shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ~shader();
    void useShader()const;
    void removeShader()const;
    
    void setUniform1i(const std::string& name, int v1);
    void setUniform1f(const std::string& name, float v1);
    void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniformMat4f(const std::string& name,const glm::mat4& matrix);

 
private:
    unsigned int m_RendererID;
    int getUniformLocation(const std::string& name);
};
