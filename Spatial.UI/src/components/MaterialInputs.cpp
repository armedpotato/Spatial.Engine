#include <imgui.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/TextureUtils.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/MaterialInputs.h>
#include <spatial/ui/components/PreviewTexture.h>

namespace spatial::ui
{

bool albedoInput(std::string_view label, math::float3& color, ecs::Registry& registry, ecs::Entity& resource,
				 const filament::Texture* icons)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	constexpr auto size = math::float2{20.0f};

	ui::previewTexture(registry, resource, icons, Icons::picture.uv());

	ImGui::SameLine();
	ui::spanToAvailWidth();

	changed |= ui::Search::searchResource<ecs::tags::IsImageTexture>("##Path", icons, registry, resource);

	ImGui::NextColumn();
	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool mapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons,
			  const math::float4& uv)
{
	bool changed = false;

	ui::previewTexture(registry, resource, icons, uv);

	ImGui::SameLine();

	changed |= ui::Search::searchResource<ecs::tags::IsImageTexture>(label.data(), icons, registry, resource);

	return changed;
}

bool mapInput(std::string_view label, float& value, ecs::Registry& registry, ecs::Entity& resource,
			  const filament::Texture* icons, const math::float4& uv)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	ui::previewTexture(registry, resource, icons, uv);

	ImGui::SameLine();
	ui::spanToAvailWidth();

	changed |= ui::Search::searchResource<ecs::tags::IsImageTexture>("##Path", icons, registry, resource);

	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.4f);
	changed |= ImGui::SliderFloat(label.data(), &value, .0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool colorPicker(std::string_view label, math::float4& color, const filament::Texture* icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit4(label.data(), &color.r,
								 ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs
									 | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

	return changed;
}

bool colorPicker(std::string_view label, math::float3& color, const filament::Texture* icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);

	return changed;
}

bool cubemapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource,
				  const filament::Texture* icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::cubemap.uv());

	ImGui::SameLine();
	changed |= ui::Search::searchResource<ecs::tags::IsCubeMapTexture>(label.data(), icons, registry, resource);

	return changed;
}

} // namespace spatial::ui