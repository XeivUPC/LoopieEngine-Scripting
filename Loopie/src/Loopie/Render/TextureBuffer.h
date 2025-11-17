#pragma once
#include <string>
namespace Loopie
{
	class TextureBuffer
	{
	public:
		TextureBuffer(const unsigned char* data, int width, int height, int channels);
		~TextureBuffer();

		void Bind(unsigned int unit = 0)const;
		void Unbind()const;

		unsigned int GetRendererID()const { return m_rendererId; }
	private:
		unsigned int m_rendererId;
	};
}