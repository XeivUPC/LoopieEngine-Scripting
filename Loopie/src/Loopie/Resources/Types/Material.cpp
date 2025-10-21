#include "Material.h"
namespace Loopie
{
	Material::Material()
	{

	}

	Material::~Material()
	{
	}

	void Material::SetShader(Shader& shader)
	{
	}

	void Material::SetTexture(std::shared_ptr<Texture> texture)
	{
		m_texture = texture;

		m_shader.Bind();
		m_texture->m_tb->Bind();
		m_shader.SetUniformInt("u_Albedo", 0);
		m_shader.Unbind();
	}

	void Material::Bind()
	{
		m_shader.Bind();
		if (m_texture) {
			m_texture->m_tb->Bind();
			m_shader.SetUniformInt("u_Albedo", 0);
		}
	}

	void Material::Unbind() const
	{
	}
}