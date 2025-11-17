#include "Loopie/Render/BufferLayout.h"
namespace Loopie
{
	class UniformBuffer
	{
	public:
		UniformBuffer(const BufferLayout& layout);
		~UniformBuffer();

		void Bind() const;
		void BindToLayout(unsigned int layoutIndex) const;
		void Unbind() const;

		void SetData(const void* data, int index);

		BufferLayout& GetLayout() { return m_layout; }

		unsigned int GetRendererID()const { return m_rendererID; }

	private:
		unsigned int m_rendererID = 0;
		BufferLayout m_layout;
	};
}