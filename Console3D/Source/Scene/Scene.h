#pragma once

#include "Console3D/External/entt/include/entt.hpp"

// Design taken from The Cherno : https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Scene.h
namespace LCN
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity();

		void DestroyEntity(Entity entity);

		inline entt::registry& Registry() { return m_Registry; }

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}