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

	while (getline(stream,line))	//	���м��д��
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
	//glValidateProgram�������а����Ŀ�ִ���ļ��Ƿ�����ڵ�ǰ OpenGL ״̬��ִ�С�
	//��֤�������ɵ���Ϣ���洢�ڳ������Ϣ��־�С�
	//��֤��Ϣ�����ɿ��ַ�����ɣ�Ҳ�����ǰ����йص�ǰ�����������뵱ǰ OpenGL ״̬�����ಿ�ֽ�������Ϣ���ַ�����
	//��Ϊ OpenGL ʵ�����ṩ��һ�ַ�ʽ�����������Ϣ��˵��Ϊʲô��ǰ����Ч�ʵ��¡����š�ִ��ʧ�ܵ�ԭ��
	glValidateProgram(program);
	CheckLinkErrors(program);

	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(vs);
	glDeleteShader(fs);
	if (!geometryShader.empty())
		glDeleteShader(gs);

	return program;
}

int Shader::GetUniformLoaction(const std::string& name) const
{
	//ע�⣬��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ������
	//���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	//���glGetUniformLocation����-1�ʹ���û���ҵ����λ��ֵ��
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
