#include "TransformBoxTest.h"

namespace test {
	TransformBoxTest::TransformBoxTest()
	{
		camera = std::make_unique<Camera>();

		// build and compile our shader program
		shader = std::make_unique<Shader>("res/shaders/TransformBox.shader");
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glGenVertexArrays(1,&VAO);
		glGenBuffers(1, &VBO);

		//首先绑定顶点数组对象，然后绑定和设置顶点缓冲区，然后配置顶点属性。
		glBindVertexArray(VAO);
		//从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//位置
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//纹理
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnable(GL_DEPTH_TEST);

		shader->Bind();
		texture1 = std::make_unique<Texture>("res/Textures/container.jpg", GL_RGB, GL_CLAMP_TO_EDGE);
		texture2 = std::make_unique<Texture>("res/Textures/awesomeface.png", GL_RGBA, GL_MIRRORED_REPEAT);

		shader->SetInt("texture1", 0);
		shader->SetInt("texture2", 1);

		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	TransformBoxTest::~TransformBoxTest()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

	}
	void TransformBoxTest::OnUpdate(float delta)
	{
		deltaTime = delta;
	}
	void TransformBoxTest::OnRender()
	{
		texture1->Bind();
		texture2->Bind(1);

		shader->Bind();

		//camPosX = sin(glfwGetTime()) * radius;
		//camPosZ = cos(glfwGetTime()) * radius;
		view = camera->GetViewMatrix();

		shader->SetMat4("view", view);

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0), cubePositions[i]);
			float angle = 20 * i;
			if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
				angle = glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
	}
	void TransformBoxTest::OnImGuiRender()
	{
		//float fov = 45.0f;

		ImGui::Begin("TransformBoxTest");

		ImGui::Text(std::to_string(screenWidth / screenHeight).c_str());

		ImGui::SliderFloat("screenWidth", &screenWidth, 200, 1000);
		ImGui::SliderFloat("screenHeight", &screenHeight, 200, 1000);
		//ImGui::SliderFloat("fov", &fov, 30, 90);
		ImGui::Text("FOV: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(camera->Zoom).c_str());

		ImGui::End();
		proj = glm::perspective(glm::radians(camera->Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		shader->SetMat4("proj", proj);
	}

	void TransformBoxTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window,deltaTime);
	}
	void TransformBoxTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		camera->ProcessMouseMovement(xpos,ypos);
	}
	void TransformBoxTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}
