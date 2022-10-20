#include "InstanceTest.h"

namespace test {
	InstanceTest::InstanceTest()
	{
		shader = std::make_unique<Shader>("res/shaders/Instance/instance.shader");

		// generate a list of 100 quad locations/translation-vectors
		// ---------------------------------------------------------
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}
		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
	}

	InstanceTest::~InstanceTest()
	{
	}

	void InstanceTest::OnUpdate(float deltaTime)
	{
	}

	void InstanceTest::OnRender()
	{
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw 100 instanced quads
		shader->Bind();
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
		glBindVertexArray(0);

	}

}