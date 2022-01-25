#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <spatial/resources/Resource.h>
#include <spatial/render/ResourceFinders.h>

namespace spatial::editor
{

struct ColorMaterial
{
	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void apply(filament::MaterialInstance& instance) const;
};

struct GridMaterial
{
	math::float3 color{.5f, .5f, .5f};
	math::float2 scale{10.0f};
	float thickness{0.015f};

	void apply(filament::MaterialInstance& instance) const;
};

struct SkyBoxMaterial
{
	bool showSun{false};
	math::float4 color{.0f, .0f, .0f, 1.0f};
	Resource<ResourceType::CubeMapTexture> skybox{};

	template <typename Finder, typename = std::enable_if_t<render::is_cubemap_texture_finder_v<Finder>>>
	void apply(filament::MaterialInstance& instance, const Finder& finder) const
	{
		instance.setParameter("showSun", showSun);
		instance.setParameter("color", color);

		const auto* texture = finder(skybox);
		instance.setParameter("constantColor", texture == nullptr);

		const auto* dummy = finder(Resource<ResourceType::CubeMapTexture>{"engine://dummy_cubemap"});
		assert(dummy != nullptr);

		const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													  filament::TextureSampler::WrapMode::REPEAT};
		instance.setParameter("skybox", texture == nullptr ? dummy : texture, sampler);
	}
};

struct StandardOpaqueMaterial
{
	math::float3 baseColor{1.0f};
	Resource<ImageTexture> albedo{};

	math::float2 tiling{1.0f};
	math::float2 offset{.0f};

	float metallic{.0f};
	Resource<ImageTexture> metallicMap{};

	float roughness{0.5f};
	Resource<ImageTexture> roughnessMap{};

	float reflectance{.0f};
	Resource<ImageTexture> reflectanceMap{};

	Resource<ImageTexture> ambientOcclusionMap{};

	Resource<ImageTexture> normalMap{};

	Resource<ImageTexture> bentNormalMap{};

	math::float4 emissive{.0f};
	Resource<ImageTexture> emissiveMap;

	float height{1.0f};
	Resource<ImageTexture> heightMap{};

	void apply(filament::MaterialInstance& instance, const render::ImageTextureFinder& finder) const;
};

} // namespace spatial::editor