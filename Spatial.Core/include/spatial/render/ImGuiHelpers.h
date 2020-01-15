#pragma once

#include <spatial/render/CommonResources.h>
#include <filesystem>
#include <imgui.h>

namespace spatial::render
{

Material createMaterial(filament::Engine *engine, const std::filesystem::path &name);

void imguiRefreshViewport(std::uint32_t width, std::uint32_t height, float scaleX, float scaleY);

void imguiRefreshDeltaTime(float delta);

std::pair<int, int> imguiGetFrameSize();

bool imguiIsMinimized();

filament::VertexBuffer *imguiCreateVertexBuffer(filament::Engine *engine, size_t capacity);

filament::IndexBuffer *imguiCreateIndexBuffer(filament::Engine *engine, size_t capacity);

uint64_t imguiMakeScissorKey(int fbheight, const ImVec4 &clipRect);

Texture imguiCreateTextureAtlas(filament::Engine *engine);

template <typename FilamentType, typename ImGuiType>
filament::backend::BufferDescriptor imguiCreateDescriptor(const ImVector<ImGuiType> &imVector)
{
	auto nVec = imVector.Size;
	auto alloc = std::allocator<ImGuiType>();
	auto data = alloc.allocate(nVec);

	std::copy(imVector.begin(), imVector.end(), data);

	auto callback = [](void *buffer, size_t size, void *user) {
		auto alloc = std::allocator<ImGuiType>();
		auto nVec = size / sizeof(ImGuiType);
		alloc.deallocate(reinterpret_cast<ImGuiType *>(buffer), nVec);
	};

	auto nVecBytes = nVec * sizeof(ImGuiType);
	return {data, nVecBytes, callback, nullptr};
}

} // namespace spatial::render