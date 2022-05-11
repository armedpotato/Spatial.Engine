#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

void previewTexture(const ecs::Registry& registry, ecs::Entity resourceEntity, const filament::Texture* icons,
					const math::float4& uv = {0, 0, 1, 1});

}