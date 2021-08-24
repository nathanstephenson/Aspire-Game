#pragma once
#include <memory>
#include "Test.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test {
	class TestBatchingColor : public Test {
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_Proj, m_View;//projection and view matrices
		glm::vec3 m_TranslationA, m_TranslationB;
	public:
		TestBatchingColor();
		~TestBatchingColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}