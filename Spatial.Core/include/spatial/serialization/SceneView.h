#pragma once

#include <cereal/cereal.hpp>
#include <fmt/format.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/FileSystem.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::SceneView& sceneView)
{
	ar(make_nvp("size", sceneView.size));
	ar(make_nvp("camera", sceneView.camera));
	ar(make_nvp("indirectLight", sceneView.indirectLight));
}

} // namespace cereal