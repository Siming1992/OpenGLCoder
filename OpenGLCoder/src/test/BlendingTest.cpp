#include "BlendingTest.h"

namespace test {
	BlendingTest::BlendingTest()
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

		glGenVertexArrays(1, &transparentVAO);
		glGenBuffers(1, &transparentVBO);
		glBindVertexArray(transparentVAO);
		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		cubeTexture = std::make_unique<Texture>("res/Textures/marble.jpg");
		floorTexture = std::make_unique<Texture>("res/Textures/metal.png");
		transparentTexture = std::make_unique<Texture>("res/Textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);

		shader->Bind();
		shader->SetInt("texture1", 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	BlendingTest::~BlendingTest()
	{
	}
	void BlendingTest::OnUpdate(float deltaTime)
	{
		this->deltaTime = deltaTime;
	}
	void BlendingTest::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera->Position - windows[i]);
			sorted[distance] = windows[i];
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjMatrix();

		shader->Bind();
		shader->SetMat4("view", view);
		shader->SetMat4("proj", projection);

		// cubes
		glBindVertexArray(cubeVAO);
		cubeTexture->Bind();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// floor
		glBindVertexArray(planeVAO);
		floorTexture->Bind();
		model = glm::mat4(1.0f);
		shader->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(transparentVAO);
		transparentTexture->Bind();
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			shader->SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	void BlendingTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}
	void BlendingTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}
	void BlendingTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}
