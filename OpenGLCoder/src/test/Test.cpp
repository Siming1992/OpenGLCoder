
#include "Test.h"

#include <imgui/imgui.h>

#include "test/TransformTest.h"
#include "test/TransformBoxTest.h"
#include "test/LightingTest.h"
#include "test/ModelTest.h"

namespace test {

	TestMenu::TestMenu(Test*& testCurrentPointer)
		:m_CurrentTest(testCurrentPointer)
	{
		RegisterTest<test::TransformTest>("Transform Texture");
		RegisterTest<test::TransformBoxTest>("Transform Box");
		RegisterTest<test::LightingTest>("Lighting Test");
		RegisterTest<test::ModelTest>("Model Test");
		
	}

	TestMenu::~TestMenu()
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
			}
		}
	}

}