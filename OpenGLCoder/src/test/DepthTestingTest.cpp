#include "DepthTestingTest.h"

namespace test {
	DepthTestingTest::DepthTestingTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0f, 3.0f));
		camera->MovementSpeed = 0.0001f;
		shader = std::make_unique<Shader>("res/shaders/DepthTesting/depthTesting.shader");
		shaderSingleColor = std::make_unique<Shader>("res/shaders/DepthTesting/singleColor.shader");

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

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//如果其中的一个测试失败了，我们什么都不做，我们仅仅保留当前储存在模板缓冲中的值。
		//如果模板测试和深度测试都通过了，那么我们希望将储存的模板值设置为参考值，参考值能够通过glStencilFunc来设置，我们之后会设置为1。
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjMatrix();

		shader->Bind();
		shader->SetMat4("view", view);
		shader->SetMat4("proj", projection);
		shaderSingleColor->Bind();
		shader->SetMat4("view", view);
		shader->SetMat4("proj", projection);
		
		// draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
		glStencilMask(0x00);
		glBindVertexArray(planeVAO);
		shader->Bind();
		floorTexture->Bind();
		shader->SetMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		// --------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
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

		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		shaderSingleColor->Bind();
		float scale = 1.1f;
		// cubes
		glBindVertexArray(cubeVAO);
		cubeTexture->Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSingleColor->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSingleColor->SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
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
