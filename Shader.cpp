#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "gl_framework.h"


Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader)
    : filepath_fragment_shader(fragment_shader), filepath_vertex_shader(vertex_shader), RendererID(0)
{
    ShaderProgramSource source;
    source.FragmentSource = ParseShader(fragment_shader);
    source.VertexSource = ParseShader(vertex_shader);
    RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(RendererID));
}

std::string Shader::ParseShader(std::string filepath)
{
    std::stringstream ss;
    std::ifstream shaderStream(filepath);
    std::string line;

    while (std::getline(shaderStream, line))
    {
        ss << line << "\n";
    }
    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << " Shader - " << message << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLuint location = GetUniformLocation(name);

    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, glm::vec4& v)
{
    SetUniform4f(name, v.x, v.y, v.z, v.w);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& mat)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]))
}

void Shader::SetUniform1i(const std::string& name, int v)
{
    GLint location = GetUniformLocation(name);
    if (location == -1)
        return;
    std::cout << "location " << name << ": " << location << "\n";
    GLCall(glUniform1i(location, v));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (UniformLocationCache.find(name) != UniformLocationCache.end())
    {
        return UniformLocationCache[name];  
    }
    GLCall(int location = glGetUniformLocation(RendererID, name.c_str()));
    if (location == -1)
        std::cerr << "Warning: uniform '" << name << "' does not exist.\n ";
    return location;
}
