
#include "Test.h"

#include <imgui/imgui.h>

#include "test/TransformTest.h"

namespace test {

	TestMenu::TestMenu(Test*& testCurrentPointer)
		:m_CurrentTest(testCurrentPointer)
	{
		RegisterTest<test::TransformTest>("Transform Texture");
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