#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <iostream>

#include "stb_image/stb_image.h"

class CubeMap
{
public:
	CubeMap(std::vector<std::string> faces);
	~CubeMap();

	unsigned int m_RendererID;

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Channels;
};