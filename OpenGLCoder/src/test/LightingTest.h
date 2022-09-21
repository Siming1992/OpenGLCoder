#pragma once
#include "test/Test.h"
#include "Camera.h"
#include "light caster/DirectionLight.h"

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
		
		unsigned int cubeVAO, VBO;

		glm::vec3 dirLightPos;

		float ambientStrength = 0.1f;
		float specularStrength = 0.5f;


		float deltaTime;
	private:
	};

}
