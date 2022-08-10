#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Channels;
public:
	unsigned int m_RendererID;
	std::string m_FilePath;

	Texture(const std::string& path,GLint format = GL_RGB,GLint wrapMode = GL_REPEAT);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
