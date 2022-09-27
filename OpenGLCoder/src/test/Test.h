#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "Renderer.h"
#include "Texture.h"

#include <imgui/imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};

		virtual void processInput(GLFWwindow* window) {};
		virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) {};
		virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {};

	protected:
		float deltaTime;
	};


	class TestMenu :public Test
	{
	public:
		TestMenu(Test*& testCurrentPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}


