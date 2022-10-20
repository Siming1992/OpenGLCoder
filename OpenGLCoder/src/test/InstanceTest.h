#pragma once

#include "Test.h"

namespace test {
	class InstanceTest:public Test
	{
	public:
		InstanceTest();
		~InstanceTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;

		//void processInput(GLFWwindow* window)override;
		//void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
		//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;

	private:
		std::unique_ptr<Shader> shader;
		// cube VAO
		unsigned int quadVAO, quadVBO;
		float quadVertices[5 * 6] = {
			// positions     // colors
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		glm::vec2 translations[100];
	};
}
