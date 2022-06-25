#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/graphics/MaterialController.h>
#include <spatial/graphics/SkyboxResources.h>

namespace spatial::graphics
{

void MaterialController::loadMaterials(filament::Engine& engine, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsMaterial>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			auto material = toShared(createMaterial(engine, resource.data.data(), resource.data.size()));

			registry.addOrReplaceComponent<SharedMaterial>(entity, std::move(material));
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

void MaterialController::createMaterialInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::tags::IsMaterialInstance, const ecs::Child>(
			ecs::ExcludeComponents<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const ecs::Child& child) {
			const auto materialEntity = child.parent;

			if (!registry.hasAllComponents<SharedMaterial>(materialEntity))
				return;

			auto& material = registry.getComponent<const SharedMaterial>(materialEntity);
			auto materialInstance = toShared(createMaterialInstance(engine, material));
			registry.addComponent<SharedMaterialInstance>(entity, std::move(materialInstance));
		});
}

} // namespace spatial::graphics