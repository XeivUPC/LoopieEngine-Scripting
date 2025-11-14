#include "UniformBuffer.h"
#include <glad/glad.h>
namespace Loopie
{

	UniformBuffer::UniformBuffer(const BufferLayout& layout) {
		m_layout = layout;

        glGenBuffers(1, &m_rendererID);
        Bind();

        glBufferData(GL_UNIFORM_BUFFER, m_layout.GetStride(), NULL, GL_STATIC_DRAW);

        Unbind();
	}
    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }
    void UniformBuffer::Bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_rendererID);
    }
    void UniformBuffer::BindToLayout(unsigned int layoutIndex) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, layoutIndex, m_rendererID);
    }
    void UniformBuffer::Unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::SetData(const void* data, int index)
    {
        Bind();
        const BufferElement& element = m_layout.GetElementByIndex(index);
        glBufferSubData(GL_UNIFORM_BUFFER, element.Offset, GetGLVariableSize(element.Type) * element.Count, data);
        Unbind();
    }
}