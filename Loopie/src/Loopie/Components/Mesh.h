#pragma once

#include "Loopie/Core/Math.h"
#include "Loopie/Render/Shader.h"
#include "Loopie/Render/IndexBuffer.h"
#include "Loopie/Render/VertexBuffer.h"
#include "Loopie/Render/VertexArray.h"

#include <vector>

namespace Loopie {

	struct Vertex {
		vec3 Position;
		vec3 Color;
	};

	class Mesh {
	public :
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void Render();

		Shader& GetShader() { return m_shader; }
	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		VertexArray m_vao;
		VertexBuffer m_vbo;
		IndexBuffer m_ebo;

		Shader m_shader = Shader("../../../Loopie/src/Loopie/Render/CorrectShader.shader");
	};
}