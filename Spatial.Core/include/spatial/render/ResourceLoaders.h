#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/ImageBasedLight.h>

#include <filesystem>

namespace spatial
{

Material createMaterial(filament::Engine* engine, const std::filesystem::path& filePath);

Mesh createMesh(filament::Engine* engine, filament::MaterialInstance* material, const std::filesystem::path& filePath);

Texture createTexture(filament::Engine* engine, const std::filesystem::path& filePath);

Texture createKtxTexture(filament::Engine* engine, const std::filesystem::path& filePath);

ImageBasedLight createIblFromKtx(filament::Engine* engine, const std::filesystem::path& folder);

} // namespace spatial::render