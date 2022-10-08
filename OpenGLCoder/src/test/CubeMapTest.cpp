#include "CubeMapTest.h"

namespace test {
	CubeMapTest::CubeMapTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0f, 3.0f));
		camera->MovementSpeed = 0.0001f;
		shader = std::make_unique<Shader>("res/shaders/CubeMap/cubemap.shader");
		skyboxShader = std::make_unique<Shader>("res/shaders/CubeMap/skybox.shader");
		gameObj = std::make_unique<Model>("res/objects/nanosuit/nanosuit.obj");

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		cubeTexture = std::make_unique<Texture>("res/Textures/container.jpg");
		cubemapTexture = std::make_unique<CubeMap>(faces);

		shader->Bind();
		shader->SetInt("skybox", 3);
		skyboxShader->Bind();
		skyboxShader->SetInt("skybox", 0);

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
	}

	CubeMapTest::~CubeMapTest()
	{
	}

	void CubeMapTest::OnUpdate(float deltaTime)
	{
		this->deltaTime = deltaTime;
	}

	void CubeMapTest::OnRender()
	{
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene as normal
		shader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjMatrix();
		model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0));
		shader->SetMat4("model", model);
		glm::mat3 normalMat = glm::transpose(glm::inverse(model));
		shader->SetMat3("normalMat", normalMat);
		shader->SetMat4("view", view);
		shader->SetMat4("proj", projection);
		shader->SetVec3("cameraPos", camera->Position);
		// cubes
		glBindVertexArray(cubeVAO);
		cubemapTexture->Bind(3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader->Bind();
		view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
		skyboxShader->SetMat4("view", view);
		skyboxShader->SetMat4("proj", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		cubemapTexture->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		model = glm::translate(model, glm::vec3(2.0, -1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.1));
		shader->Bind();
		shader->SetMat4("model", model);
		gameObj->Draw(*shader);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void CubeMapTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}

	void CubeMapTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}

	void CubeMapTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}