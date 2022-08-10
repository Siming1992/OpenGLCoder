#include "Shader.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_RendererID(0)
{
	ShaderProgramSource soure = ParseShader(filepath);
	m_RendererID = CreateShader(soure.VertexSource, soure.FragmentSource, soure.GeometrySource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(GetUniformLoaction(name), value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLoaction(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLoaction(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(GetUniformLoaction(name), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(GetUniformLoaction(name), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(GetUniformLoaction(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(GetUniformLoaction(name), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(GetUniformLoaction(name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(GetUniformLoaction(name), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) const
{
	glUniformMatrix2fv(GetUniformLoaction(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const
{
	glUniformMatrix3fv(GetUniformLoaction(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(GetUniformLoaction(name), 1, GL_FALSE, &value[0][0]);
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[3];

	ShaderType type = ShaderType::NONE;

	while (getline(stream,line))	//	逐行检测写入
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			else if (line.find("geometry") != std::string::npos)
				type == ShaderType::GEOMETRY;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return ShaderProgramSource{ss[0].str(),ss[1].str(),ss[2].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	CheckCompileErrors(id,type);

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	unsigned int gs;
	if (!geometryShader.empty())
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}

	glLinkProgram(program);
	//glValidateProgram检查程序中包含的可执行文件是否可以在当前 OpenGL 状态下执行。
	//验证过程生成的信息将存储在程序的信息日志中。
	//验证信息可能由空字符串组成，也可能是包含有关当前程序对象如何与当前 OpenGL 状态的其余部分交互的信息的字符串。
	//这为 OpenGL 实现者提供了一种方式来传达更多信息，说明为什么当前程序效率低下、次优、执行失败等原因。
	glValidateProgram(program);
	CheckLinkErrors(program);

	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
	glDeleteShader(vs);
	glDeleteShader(fs);
	if (!geometryShader.empty())
		glDeleteShader(gs);

	return program;
}

int Shader::GetUniformLoaction(const std::string& name) const
{
	//注意，查询uniform地址不要求你之前使用过着色器程序，
	//但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	//如果glGetUniformLocation返回-1就代表没有找到这个位置值。
	int location = glGetUniformLocation(m_RendererID,name.c_str());
	if (location == -1)
		std::cout << "Warning Uniform: '" << name << "' doesn't exist" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::CheckCompileErrors(unsigned int ID, unsigned int type)
{
	int result;
	glGetShaderiv(ID, GL_COMPILE_STATUS,&result);
	if (result != GL_TRUE)
	{
		int length;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(ID, length, &length, message);
		
		std::string shaderType = "unKnow";
		switch (type)
		{
		case GL_VERTEX_SHADER:
			shaderType = "vertex";
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment";
		case GL_GEOMETRY_SHADER:
			shaderType = "geometry";
		}
		std::cout << "Failed to compile " << shaderType << "shader" << std::endl;
		std::cout << message << std::endl;
	}
}

void Shader::CheckLinkErrors(unsigned int ID) 
{
	int result;
	glGetProgramiv(ID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		int length;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetProgramInfoLog(ID, length, &length, message);

		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << std::endl;
		std::cout << message << std::endl;
	}
}
