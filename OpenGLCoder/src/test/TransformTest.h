#pragma once

#include "Test.h"

namespace test {
	class TransformTest : public Test
	{
	public:
		TransformTest();
		~TransformTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture1;
		std::unique_ptr<Texture> texture2;

		unsigned int VBO, VAO, EBO;

		glm::mat4 trans;
	};
}

