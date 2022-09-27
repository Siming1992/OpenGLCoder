#include "LightingTest.h"
#include <format>

namespace test {
	LightingTest::LightingTest()
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));

		Renderer::SceneCamera = camera;

		lightingShader = std::make_unique<Shader>("res/shaders/Lighting/Lighting.shader");

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

		glEnable(GL_DEPTH_TEST);

		diffuse = std::make_unique<Texture>("res/Textures/container2.png");
		specular = std::make_unique<Texture>("res/Textures/container2_specular.png");
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

		// directional light
		lightingShader->SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader->SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader->SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		dir = std::make_unique<light::DirectionLight>(glm::vec3(0.5, 0.3, 0.1), glm::vec3(0, 5, -3.0));

		for (int i = 0; i < 4; i++)
		{
			point[i] = std::make_unique<light::PointLight>(glm::vec3(1.0, 1.0, 0.0), pointLightPositions[i]);
			//PointLight��ʼ����ʱ��bing�˱��shader������һ��Ҫ��bindһ��- -
			lightingShader->Bind();
			lightingShader->SetVec3(std::format("pointLights[{}].position",i), pointLightPositions[i]);
			lightingShader->SetVec3(std::format("pointLights[{}].ambient",i), 0.05f, 0.05f, 0.05f);
			lightingShader->SetVec3(std::format("pointLights[{}].diffuse",i), 0.8f, 0.8f, 0.8f);
			lightingShader->SetVec3(std::format("pointLights[{}].specular",i), 1.0f, 1.0f, 1.0f);
			lightingShader->SetFloat(std::format("pointLights[{}].constant",i), 1.0f);
			lightingShader->SetFloat(std::format("pointLights[{}].linear",i), 0.09f);
			lightingShader->SetFloat(std::format("pointLights[{}].quadratic",i), 0.032f);
		}

		spot = std::make_unique<light::SpotLight>(glm::vec3(0.0, 1.0, 1.0), camera->Position, camera->Front);
		lightingShader->Bind();

		lightingShader->SetVec3("spotLight.position", camera->Position);
		lightingShader->SetVec3("spotLight.direction", camera->Front);
		lightingShader->SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader->SetFloat("spotLight.constant", 1.0f);
		lightingShader->SetFloat("spotLight.linear", 0.09f);
		lightingShader->SetFloat("spotLight.quadratic", 0.032f);
		lightingShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
		lightingShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));
	}
	LightingTest::~LightingTest()
	{
		Renderer::SceneCamera = NULL;
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

		// view/projection transformations
		glm::mat4 projection = camera->GetProjMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		lightingShader->SetMat4("proj", projection);
		lightingShader->SetMat4("view", view);

		// render the cube
		glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		for (unsigned int i = 0; i < 10; i++)
		{
			// world transformation
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 25 * i;
			if (i % 3 == 0)  
				angle = glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader->SetMat4("model", model);
			//���ģ�;���ִ���˲��ȱ����ţ�����ĸı�ᵼ�·��������ٴ�ֱ�ڱ����ˡ���ˣ����ǲ�����������ģ�;������任��������
			//���߾��󱻶���Ϊ��ģ�;������Ͻ�3x3���ֵ�������ת�þ���
			glm::mat3 normalMat = glm::transpose(glm::inverse(view * model));
			lightingShader->SetMat4("model", model);
			lightingShader->SetMat3("normalMat", normalMat);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object

		dir->OnRender();
		for (int i = 0; i < 4; i++)
		{
			point[i]->OnRender();
		}
		spot->OnRender();
	}
	void LightingTest::OnImGuiRender()
	{
		//float lightPosition[] = { lightPos[0],lightPos[1],lightPos[2]};

		ImGui::Begin("Basic Lighting");
		//ImGui::SliderFloat("ambient Strength", &ambientStrength, 0.0f, 1.0f);
		//ImGui::SliderFloat("specular Strength", &specularStrength, 0.0f, 1.0f);
		
		//ImGui::InputFloat3("light Position", lightPosition);
		ImGui::End();

		//lightPos = glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]);
		
		lightingShader->Bind(); 
		lightingShader->SetFloat("material.shininess", 64.0f);

		//lightingShader->SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//lightingShader->SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // �����յ�����һЩ�Դ��䳡��
		//lightingShader->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
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
