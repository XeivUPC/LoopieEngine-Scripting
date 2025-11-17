#pragma once
#include "VertexBuffer.h"
#include "Loopie/Render/IndexBuffer.h"
#include "Loopie/Render/BufferLayout.h"

namespace Loopie
{
    class VertexArray
    {
    private:
        unsigned int m_rendererID;
        VertexBuffer* m_vbo = nullptr;
        IndexBuffer* m_ebo = nullptr;

    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddBuffer(VertexBuffer* vbo, IndexBuffer* ebo);

        unsigned int GetRendererID()const { return m_rendererID; }

        const IndexBuffer& GetIndexBuffer() const;
    };
}