#include "LightingTest.h"

namespace test {
	LightingTest::LightingTest()
	{
		camera = std::make_unique<Camera>(glm::vec3(-3.0f, 3.0f, 10.0f));

		lightingShader = std::make_unique<Shader>("res/shaders/Lighting/Lighting.shader");
		lightCubeShader = std::make_unique<Shader>("res/shaders/Lighting/LightCube.shader");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		// first, configure the cube's VAO (and VBO)
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coords
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//-------------Ȼ���lightVAO-----------------
		glGenVertexArrays(1, &lightPointVAO);
		glBindVertexArray(lightPointVAO);
		// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);

		diffuse = std::make_unique<Texture>("res/Textures/container2.png",GL_RGBA);
		specular = std::make_unique<Texture>("res/Textures/container2_specular.png", GL_RGBA);
		emission = std::make_unique<Texture>("res/Textures/matrix.jpg");
		// shader configuration
		// --------------------
		lightingShader->Bind();
		lightingShader->SetInt("material.diffuse", 0);
		diffuse->Bind();
		lightingShader->SetInt("material.specular", 1);
		specular->Bind(1);
		lightingShader->SetInt("material.emission", 2);
		emission->Bind(2);

		lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
	}
	LightingTest::~LightingTest()
	{
	}
	void LightingTest::OnUpdate(float delta)
	{
		deltaTime = delta;
	}
	void LightingTest::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader->Bind();
		//lightingShader->SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//lightingShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), camera->screenWidth / camera->screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();
		lightingShader->SetMat4("proj", projection);
		lightingShader->SetMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		float angle = glfwGetTime() * 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//���ģ�;���ִ���˲��ȱ����ţ�����ĸı�ᵼ�·��������ٴ�ֱ�ڱ����ˡ���ˣ����ǲ�����������ģ�;������任��������
		//���߾��󱻶���Ϊ��ģ�;������Ͻ�3x3���ֵ�������ת�þ���
		glm::mat3 normalMat = glm::transpose(glm::inverse(view * model));
		lightingShader->SetMat4("model", model);
		lightingShader->SetMat3("normalMat", normalMat);
		//lightingShader->SetVec3("viewPos", camera->Position);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// also draw the lamp object
		lightCubeShader->Bind();
		lightCubeShader->SetMat4("proj", projection);
		lightCubeShader->SetMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightCubeShader->SetMat4("model", model);

		glBindVertexArray(lightPointVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void LightingTest::OnImGuiRender()
	{
		float lightPosition[] = { lightPos[0],lightPos[1],lightPos[2]};

		ImGui::Begin("Basic Lighting");
		//ImGui::SliderFloat("ambient Strength", &ambientStrength, 0.0f, 1.0f);
		//ImGui::SliderFloat("specular Strength", &specularStrength, 0.0f, 1.0f);
		
		ImGui::InputFloat3("light Position", lightPosition);
		ImGui::End();

		lightPos = glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]);
		
		lightingShader->Bind(); 
		lightingShader->SetFloat("material.shininess", 64.0f);

		lightingShader->SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader->SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // �����յ�����һЩ�Դ��䳡��
		lightingShader->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		lightingShader->SetVec3("lightPos", lightPos);
	}
	void LightingTest::processInput(GLFWwindow* window)
	{
		camera->ProcessKeyboard(window, deltaTime);
	}
	void LightingTest::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, 0))
		{
			camera->ProcessMouseMovement(xpos, ypos);
		}
	}
	void LightingTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->ProcessMouseScroll(yoffset);
	}
}
