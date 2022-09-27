#include "DepthTestingTest.h"

namespace test {
	DepthTestingTest::DepthTestingTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0f, 3.0f));
		camera->MovementSpeed = 0.0001f;
		shader = std::make_unique<Shader>("res/shaders/DepthTesting/depthTesting.shader");

		// cube VAO
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		// plane VAO
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		cubeTexture = std::make_unique<Texture>("res/Textures/marble.jpg");
		floorTexture = std::make_unique<Texture>("res/Textures/metal.png", GL_REPEAT);

		shader->Bind();
		shader->SetInt("texture1", 0);

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
	}

	DepthTestingTest::~DepthTestingTest()
	{
	}

	void DepthTestingTest::OnUpdate(float deltaTime)
	{
		this->deltaTime = deltaTime;
	}

	void DepthTestingTest::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjMatrix();

		shader->SetMat4("view", view);
		shader->SetMat4("proj", projection);

		glBindVertexArray(cubeVAO);
		cubeTexture->Bind();
		glActiveTexture(GL_TEXTURE0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(planeVAO);
		floorTexture->Bind();
		shader->SetMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	void DepthTestingTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}
	void DepthTestingTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}
	void DepthTestingTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}
