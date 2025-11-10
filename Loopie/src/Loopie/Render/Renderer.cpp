#include "Renderer.h"

#include "Loopie/Core/Assert.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Render/Gizmo.h"
#include <iostream>

#include <glad/glad.h>
#include <IL/il.h>
#include <IL/ilu.h>

namespace Loopie {

	std::shared_ptr<Texture> Renderer::s_DefaultTexture = nullptr;
	matrix4 Renderer::s_ViewProjection = matrix4(1);
	std::vector<Renderer::RenderItem> Renderer::s_RenderQueue = std::vector<Renderer::RenderItem>();

	void Renderer::Init(void* context) {
		ASSERT(!gladLoadGLLoader((GLADloadproc)context), "Failed to Initialize GLAD!");

		glEnable(GL_BLEND);
		EnableDepth();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// DevIL Init
		ilInit();
		iluInit();

		ilSetInteger(IL_KEEP_DXTC_DATA, IL_FALSE);
		ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

		// Gizmo Data Structure Init
		Gizmo::Init();
	}

	void Renderer::Shutdown() {
		ilShutDown();
		Gizmo::Shutdown();
	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetClearColor(const vec4& color) {
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void Renderer::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::BeginScene(const matrix4& viewProjectionMatrix)
	{
		s_ViewProjection = viewProjectionMatrix;
		Gizmo::BeginGizmo(viewProjectionMatrix);
	}

	void Renderer::EndScene()
	{
		FlushRenderQueue();
		Gizmo::EndGizmo();
	}

	void Renderer::AddRenderItem(std::shared_ptr<VertexArray> vao, std::shared_ptr<Material> material, const Transform* transform)
	{
		s_RenderQueue.emplace_back(RenderItem{ vao, vao->GetIndexBuffer().GetCount(), material, transform});
	}

	void Renderer::FlushRenderQueue()
	{
		/// SORT By Material


		///

		for (const RenderItem& item : s_RenderQueue) {
			
			item.VAO->Bind();
			item.Material->Bind();
			SetRenderUniforms(item.Material, item.Transform);
			glDrawElements(GL_TRIANGLES, item.IndexCount, GL_UNSIGNED_INT, nullptr);
			item.VAO->Unbind();
		}

		s_RenderQueue.clear();
	}

	void Renderer::SetRenderUniforms(std::shared_ptr<Material> material, const Transform* transform)
	{
		material->GetShader().SetUniformMat4("lp_ViewProjection", s_ViewProjection);
		material->GetShader().SetUniformMat4("lp_Transform", transform->GetWorldToLocalMatrix());
	}

	void Renderer::EnableDepth()
	{
			glEnable(GL_DEPTH_TEST);
	}

	void Renderer::DisableDepth()
	{
			glDisable(GL_DEPTH_TEST);
	}
}
