#include "CubeMap.h"

CubeMap::CubeMap(std::vector<std::string> faces)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* m_LocalBuffer = stbi_load(faces[i].c_str(), &m_Width, &m_Height, &m_Channels, 0);
		if (m_LocalBuffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
			stbi_image_free(m_LocalBuffer);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(m_LocalBuffer);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_RendererID);
}

void CubeMap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void CubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
