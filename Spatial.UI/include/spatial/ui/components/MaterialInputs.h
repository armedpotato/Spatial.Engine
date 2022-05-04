#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include "Icons.h"

namespace spatial::ui
{

void previewTexture(const filament::Texture* texture, const filament::Texture* icons, const math::float4& uv = {0, 0, 1, 1});

bool colorPicker(std::string_view label, math::float4& color, const filament::Texture* icons);

bool colorPicker(std::string_view label, math::float3& color, const filament::Texture* icons);

bool cubemapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons);

bool albedoInput(std::string_view label, math::float3& color, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons);

bool mapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons, const math::float4& uv = Icons::bwPicture.uv());

bool mapInput(std::string_view label, float& value, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons, const math::float4& uv = Icons::bwPicture.uv());

}