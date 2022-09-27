#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Shader.h"
#include "Mesh.h"

#include "Camera.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear() const;
	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const;

	static std::shared_ptr<Camera> SceneCamera;

private:

};
