
#include "GeometryShaderTest.h"

namespace test {
	GeometryShaderTest::GeometryShaderTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(0.0, 10.0f, 10.0f));
		houseShader = std::make_unique<Shader>("res/shaders/GeometryShader/houseShader.shader");
		explodeShader = std::make_unique<Shader>("res/shaders/GeometryShader/explodeShader.shader");
		gameObj = std::make_unique<Model>("res/objects/nanosuit/nanosuit.obj");

		glGenVertexArrays(1,&VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

	}

	GeometryShaderTest::~GeometryShaderTest()
	{
	}

	void GeometryShaderTest::OnUpdate(float deltaTime)
	{
		this->deltaTime = deltaTime;
	}

	void GeometryShaderTest::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw points
		houseShader->Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);

		explodeShader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjMatrix();

		explodeShader->SetMat4("model", model);
		explodeShader->SetMat4("view", view);
		explodeShader->SetMat4("proj", projection);
		// add time component to geometry shader in the form of a uniform
		explodeShader->SetFloat("time", static_cast<float>(glfwGetTime()));

		gameObj->Draw(*explodeShader);
	}

	void GeometryShaderTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}

	void GeometryShaderTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}

	void GeometryShaderTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}

}