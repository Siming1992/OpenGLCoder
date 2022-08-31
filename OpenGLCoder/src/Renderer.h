#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear() const;
	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const;

private:

};
