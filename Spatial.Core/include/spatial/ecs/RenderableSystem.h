#pragma once

#include <entt/entity/entity.hpp>
#include <unordered_map>
#include <utils/EntityManager.h>
#include <utils/Entity.h>
#include <filament/Scene.h>
#include <filament/Engine.h>
#include <entt/entity/registry.hpp>
#include <filament/RenderableManager.h>

namespace spatial::ecs
{
	class RenderableSystem
	{
	private:
		filament::RenderableManager& m_manager;
		filament::Scene& m_scene;
		std::unordered_map<entt::entity, utils::Entity> m_renderables;
	public:
		RenderableSystem(filament::Engine& engine, filament::Scene& scene);

		~RenderableSystem();

		void onConstruct(entt::registry& registry, entt::entity entity);

		void onDestroy(entt::registry& registry, entt::entity entity);

		void onUpdate(entt::registry& registry);
	};
}
