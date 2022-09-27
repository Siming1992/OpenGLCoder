#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MeshTexture
{
	std::string type;
	std::string path;
	std::shared_ptr<Texture> texture;
};

class Mesh
{
public:
	/*  网格数据  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
	~Mesh();

	void Draw(Shader &shader);
private:
	/*  渲染数据  */
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

