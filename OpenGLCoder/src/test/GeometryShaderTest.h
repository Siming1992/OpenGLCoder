#pragma once

#include "Test.h"
#include "Model.h"

namespace test {
	class GeometryShaderTest:public Test
	{
	public:
		GeometryShaderTest();
		~GeometryShaderTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;

		void processInput(GLFWwindow* window)override;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;

	private:

		std::unique_ptr<Shader> houseShader;
		std::unique_ptr<Shader> explodeShader;
		std::unique_ptr<Camera> camera;

		std::unique_ptr<Model> gameObj;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		
		unsigned int VAO, VBO;
		float points[5 * 4] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
		};
	};
}
