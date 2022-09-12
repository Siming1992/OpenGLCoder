#pragma once

#include "test/Test.h"
#include "Camera.h"

namespace test {
	class TransformBoxTest:public Test
	{
	public:
		TransformBoxTest();
		~TransformBoxTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		void processInput(GLFWwindow* window)override;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture1, texture2;

		unsigned int VAO, VBO;

		std::unique_ptr<Camera> camera;

		float cameraSpeed;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		float deltaTime;
		//float radius = 10.0f;
		//float camPosX;
		//float camPosZ;

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
	};
}

