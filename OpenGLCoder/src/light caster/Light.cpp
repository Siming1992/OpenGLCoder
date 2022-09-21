#include "Light.h"

namespace light {
	Light::Light(glm::vec3 lightColor, glm::vec3 position, glm::vec3 direction)
		:LightColor(lightColor),Position(position),Direction(direction)
	{
		m_lightCubeShader = std::make_unique<Shader>("res/shaders/Lighting/LightCube.shader");

		glGenVertexArrays(1, &m_cubeVAO);
		glGenBuffers(1, &m_cubeVBO);

		glBindVertexArray(m_cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_lightCubeShader->Bind();
		m_lightCubeShader->SetVec3("lightColor",LightColor);
	}

	Light::~Light()
	{

	}

	void Light::OnRender() {
		if (Renderer::SceneCamera)
		{
			m_lightCubeShader->Bind();

			model = glm::mat4(1);
			model = glm::translate(model, Position);
			model = glm::scale(model, glm::vec3(0.1));

			view = Renderer::SceneCamera->GetViewMatrix();
			proj = Renderer::SceneCamera->GetProjMatrix();

			m_lightCubeShader->SetMat4("model", model);
			m_lightCubeShader->SetMat4("view", view);
			m_lightCubeShader->SetMat4("proj", proj);

			glBindVertexArray(m_cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

}