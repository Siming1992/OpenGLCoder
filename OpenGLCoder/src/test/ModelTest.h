#pragma once
#include "Test.h"
#include "Model.h"

namespace test {
	class ModelTest :public Test
	{
	public:
		ModelTest();
		~ModelTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		void processInput(GLFWwindow* window)override;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Camera> camera;
		std::unique_ptr<Model> gameObj;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
	};
}