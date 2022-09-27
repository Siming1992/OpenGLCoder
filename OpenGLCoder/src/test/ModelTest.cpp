#include "ModelTest.h"

namespace test {
	ModelTest::ModelTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(0, 10, 20));

		shader = std::make_unique<Shader>("res/shaders/Model/Default.shader");
		gameObj = std::make_unique<Model>("res/objects/nanosuit/nanosuit.obj");

		glEnable(GL_DEPTH_TEST);
	}
	ModelTest::~ModelTest()
	{
	}
	void ModelTest::OnUpdate(float deltaTime)
	{
		this->deltaTime = deltaTime;
	}
	void ModelTest::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		shader->SetMat4("model", model);
		view = camera->GetViewMatrix();
		shader->SetMat4("view", view);
		proj = camera->GetProjMatrix();
		shader->SetMat4("proj", proj);

		gameObj->Draw(*shader);
	}
	void ModelTest::OnImGuiRender()
	{
	}
	void ModelTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}
	void ModelTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}
	void ModelTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}
