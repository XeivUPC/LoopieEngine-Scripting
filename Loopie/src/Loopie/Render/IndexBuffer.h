#pragma once

namespace Loopie
{
    class IndexBuffer
    {
    private:
        unsigned int m_rendererID = 0;
        unsigned int m_count = 0;

    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;

        void Unbind() const;

        unsigned int GetCount() const;
        unsigned int GetRendererID()const { return m_rendererID; }
    };
}