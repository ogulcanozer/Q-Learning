#include "shader.h"



shader::shader(const GLchar* vShaderPath, const GLchar* fShaderPath)
    :m_RendererID(0)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        // Open files
        vShaderFile.open(vShaderPath);
        fShaderFile.open(fShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexShaderCode = vShaderStream.str();
        fragmentShaderCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* vShaderCode = vertexShaderCode.c_str();
    const GLchar* fShaderCode = fragmentShaderCode.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader Program
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, fragment);
    glLinkProgram(m_RendererID);

    // Print linking errors if any
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

shader::~shader()
{
    glDeleteProgram(m_RendererID);
}

void shader::useShader(void)const
{
    glUseProgram(m_RendererID);
}

void shader::removeShader() const
{
    glUseProgram(0);
}

void shader::setUniform1i(const std::string& name, int v1)
{
    glUniform1i(getUniformLocation(name), v1);
}

void shader::setUniform1f(const std::string& name, float v1)
{
    glUniform1f(getUniformLocation(name), v1);
}


void shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1 , GL_FALSE, &matrix[0][0]);
}

int shader::getUniformLocation(const std::string& name)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    return location;
}
