#pragma once
#include "Loopie/Core/Math.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Resources/Types/Texture.h"
#include "Loopie/Render/VertexArray.h"

#include <filesystem>

namespace Loopie {
	class Transform;

	class Renderer {
	public:

		struct RenderItem {
			std::shared_ptr<VertexArray> VAO;
			unsigned int IndexCount;

			std::shared_ptr<Material> Material;
			const Transform* Transform;
		};

		static void Init(void* context);
		static void Shutdown();

		static void Clear();
		static void SetClearColor(const vec4& color);
		static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		static void BeginScene(const matrix4& viewProjectionMatrix);
		static void EndScene();

		static void AddRenderItem(std::shared_ptr<VertexArray> vao, std::shared_ptr<Material> material, const Transform* transform);

		static void SetDefaultTexture(std::shared_ptr<Texture> texture) { s_DefaultTexture = texture; }

		static void EnableDepth();
		static void DisableDepth();

	private:
		static void SetRenderUniforms(std::shared_ptr<Material> material, const Transform* transform);
		static void FlushRenderQueue();

	public:
		static std::shared_ptr<Texture> s_DefaultTexture;
		static matrix4 s_ViewProjection;
	private:
		static std::vector<RenderItem> s_RenderQueue;

	};
}