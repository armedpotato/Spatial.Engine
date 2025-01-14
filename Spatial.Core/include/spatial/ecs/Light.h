#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

namespace tags
{

struct IsLight
{
	constexpr static auto typeName = "tag_is_light";
};

} // namespace tags

struct PointLight
{
	constexpr static auto typeName = "point_light";

	float intensity{10000.0f};
	float falloff{1.0f};
	math::vec3 color{1.0f, 1.0f, 1.0f};
};

struct SpotLight
{
	constexpr static auto typeName = "spot_light";

	float intensity{10000.0f};
	math::vec3 direction{.0f, -1.0f, .0f};
	math::vec3 color{1.0f, 1.0f, 1.0f};
	bool castShadows{true};
	float falloff{1.0f};
	float innerAngle{math::pi / 4.0f};
	float outerAngle{math::pi / 2.0f};
};

struct DirectionalLight
{
	constexpr static auto typeName = "directional_light";

	float intensity{10000.0f};
	math::vec3 direction{.0f, -1.0f, .0f};
	math::vec3 color{1.0f, 1.0f, 1.0f};
	bool castShadows{true};
};

struct SunLight
{
	constexpr static auto typeName = "sun_light";

	float intensity{10000.0f};
	math::vec3 color{1.0f, 1.0f, 1.0f};
	float angularRadius{0.545f};
	float haloFalloff{80.0f};
	float haloSize{10.0f};
	bool castShadows{true};
};

struct IndirectLight
{
	constexpr static auto typeName = "indirect_light";

	ecs::Entity reflectionsTexture{ecs::NullEntity};
	ecs::Entity irradianceValues{ecs::NullEntity};
	float intensity{30000.0f};
};

} // namespace spatial::ecs