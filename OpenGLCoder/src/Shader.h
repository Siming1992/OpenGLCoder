#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

enum class ShaderType
{
	NONE = -1, 
	VERTEX = 0, 
	FRAGMENT = 1,
	GEOMETRY = 2,
};
/// <summary>
/// [0]vertex ,[1]fragment ,[2]geometry
/// </summary>
static struct  ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
public:
	std::string m_FilePath;
	unsigned int m_RendererID;

	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//Set uniforms
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec2(const std::string& name, float x, float y) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w) const;
	void SetMat2(const std::string& name, const glm::mat2& value) const;
	void SetMat3(const std::string& name, const glm::mat3& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
	//为了优化每次都通过glGetUniformLocation获取位置信息
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);

	int GetUniformLoaction(const std::string& name) const;

	void CheckCompileErrors(unsigned int ID, unsigned int type);
	void CheckLinkErrors(unsigned int ID);
};