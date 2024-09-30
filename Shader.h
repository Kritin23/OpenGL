#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string filepath_vertex_shader;
	std::string filepath_fragment_shader;
	unsigned int RendererID;

	//cache uniform name
	std::unordered_map<std::string, int> UniformLocationCache;

public:
	Shader(const std::string& vertex_shader, const std::string& fragment_shader);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4& v);
	void SetUniformMat4f(const std::string& name, glm::mat4& mat);
	void SetUniform1i(const std::string& name, int v);

private:
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string ParseShader(std::string filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};