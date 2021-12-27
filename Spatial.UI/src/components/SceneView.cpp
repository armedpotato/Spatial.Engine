#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/render/TextureView.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/SceneView.h>

namespace spatial::ui
{

void SceneView::image(const ecs::Registry& registry, ecs::Entity sceneViewEntity, math::float2 size)
{
	{
		const auto context = ImGui::GetCurrentContext();
		const auto currentPosition = context->CurrentWindow->DC.CursorPos;
		const auto rectMax = ImVec2(currentPosition.x + size.x, currentPosition.y + size.y);

		ImGui::GetWindowDrawList()->AddRectFilled(currentPosition, rectMax, IM_COL32(34, 34, 34, 255));
	}

	if (!registry.isValid(sceneViewEntity))
		return;

	const auto& sceneView = registry.getComponent<const ecs::SceneView>(sceneViewEntity);
	const auto& textureView = registry.getComponent<const render::TextureView>(sceneViewEntity);

	double aspectRatio = 1.0;
	const auto* perspectiveCamera = registry.tryGetComponent<const ecs::PerspectiveCamera>(sceneView.camera);
	if (perspectiveCamera)
		aspectRatio = perspectiveCamera->aspectRatio;

	const auto imageSize = aspectRatio >= 1 ? math::int2{static_cast<double>(size.y) * aspectRatio, size.y}
										   : math::int2{size.x, static_cast<double>(size.x) / aspectRatio};

	ImGui::SetCursorPosX((size.x - static_cast<float>(imageSize.x)) * 0.5f);
	ui::image(textureView.getColorTexture().ref(), imageSize, math::float4{0, 1, 1, 0});
}

} // namespace spatial::ui