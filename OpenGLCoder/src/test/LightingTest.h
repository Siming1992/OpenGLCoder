#pragma once
#include "test/Test.h"
#include "Camera.h"
#include "light caster/DirectionLight.h"
#include "light caster/PointLight.h"
#include "light caster/SpotLight.h"

namespace test{
	class LightingTest : public Test
	{
	public:
		LightingTest();
		~LightingTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		void processInput(GLFWwindow* window)override;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;

		std::shared_ptr<Camera> camera;
		std::unique_ptr<Shader> lightingShader;
		//std::unique_ptr<Shader> lightCubeShader;

		std::unique_ptr<Texture> diffuse;
		std::unique_ptr<Texture> specular;
		std::unique_ptr<Texture> emission;

		std::unique_ptr<light::DirectionLight> dir;
		std::unique_ptr<light::PointLight> point[4];
		std::unique_ptr<light::SpotLight> spot;
		
		unsigned int cubeVAO, VBO;

		glm::vec3 dirLightPos;

		glm::vec3 cubePositions[10] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		glm::vec3 pointLightPositions[4] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
		};

		float ambientStrength = 0.1f;
		float specularStrength = 0.5f;


		float deltaTime;
	private:
	};

}
