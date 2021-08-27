#pragma once
#include <memory>
#include <array>
#include "Test.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"



namespace test {

	struct vec2 {
		float x, y;
	};

	struct vec3 {
		float x, y, z;
	};

	struct vec4 {
		float x, y, z, w;
	};

	class TestDynamicGeometry : public Test {
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture0;
		std::unique_ptr<Texture> m_Texture1;

		glm::mat4 m_Proj, m_View;//projection and view matrices
		glm::vec3 m_TranslationA, m_TranslationB;
		float m_QuadPos[2] = { -50.0f, -50.0f };
	public:
		TestDynamicGeometry();
		~TestDynamicGeometry();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}