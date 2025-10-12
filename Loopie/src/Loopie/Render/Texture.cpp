#include "Texture.h"

#include "Loopie/Core/Log.h"

#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <glad/glad.h>

namespace Loopie
{
	Texture::Texture(const std::string& path, bool flipVertically) : m_texture_ID(0), m_width(0), m_height(0), m_channels(0)
	{
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		// If we need to flip, just use the following code
		//if (flipVertically)
		//	ilEnable(IL_ORIGIN_SET);
		//else
		//	ilDisable(IL_ORIGIN_SET);

		if (ilLoadImage(path.c_str()))
		{
			// Convert to RGBA format
			if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				Log::Error("Failed to convert image {0}", path);
				ilDeleteImages(1, &imageID);
				return;
			}

			m_width = ilGetInteger(IL_IMAGE_WIDTH);
			m_height = ilGetInteger(IL_IMAGE_HEIGHT);
			m_channels = ilGetInteger(IL_IMAGE_CHANNELS);

			ILubyte* data = ilGetData();

			GLenum format = GL_RGB;
			if (m_channels == 1)
				format = GL_RED;
			else if (m_channels == 4)
				format = GL_RGBA;

			glGenTextures(1, &m_texture_ID);
			glBindTexture(GL_TEXTURE_2D, m_texture_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			Unbind();

			ilDeleteImages(1, &imageID);
		}
		else
		{
			Log::Error("Failed to load image {0}", path);
			ilDeleteImages(1, &imageID);
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_texture_ID);
	}

	void Texture::Bind(unsigned int unit) const
	{
		if (unit > 31)
		{
			Log::Error("Texture unit {0} exceeds maximum (31).", unit);
			return;
		}
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);

	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}