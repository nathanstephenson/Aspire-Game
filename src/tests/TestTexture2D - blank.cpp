#include "TestTexture2D.h"
#include "Renderer.h"

namespace test {
	TestTexture2D::TestTexture2D(){
	}

	TestTexture2D::~TestTexture2D(){
	}

	void TestTexture2D::OnUpdate(float deltaTime){
	}

	void TestTexture2D::OnRender(){
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));//set background colour
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestTexture2D::OnImGuiRender(){
		
	}
}