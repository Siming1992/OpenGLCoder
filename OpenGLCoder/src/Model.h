#pragma once

#include "Renderer.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const std::string& path);
	~Model();

	void Draw(Shader& shader);
private:
	mutable std::unordered_map<std::string, MeshTexture> m_TexturesLoaded;//�洢�����Ѽ��ص�����ȷ�����ᱻ��μ���

	std::vector<Mesh> m_meshs;
	std::string m_directory;

	void loadModel(const std::string& path);

	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
};

