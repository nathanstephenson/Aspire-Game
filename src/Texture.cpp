#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path) : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0){
	stbi_set_flip_vertically_on_load(1);//flips the texture image upside down as opengl expects pixels to start at the bottom left not top left
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);//4 for rgba (4 channels)

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));//s and t is like x and y but for textures
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture(){
	GLCall(glDeleteTextures(GL_TEXTURE_2D, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//technically goes up to GL_TEXTURE32 but that's only for huge games
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}