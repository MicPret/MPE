#ifndef MPE_WORLD_HPP
#define MPE_WORLD_HPP

#include "Entity.hpp"
#include "ComponentBuffer.hpp"

namespace mpe
{
	//Singleton class to manage the ECS
	class World
	{
	public:
		static World& GetInstance();
		Entity NewEntity();
		template <typename C, typename... Args>
		C& SetComponent(Entity e, Args&&... args);
		template <typename C>
		C& GetComponent(Entity e);
		//TODO: implement systems
	private:
		World();
	};

	template<typename C, typename... Args>
	C& World::SetComponent(Entity e, Args&& ...args)
	{
		ComponentBuffer<C>& buffer = Global<ComponentBuffer<C>>();
		return buffer.SetComponent(e, std::forward<Args>(args)...);
	}

	template<typename C>
	C& World::GetComponent(Entity e)
	{
		ComponentBuffer<C>& buffer = Global<ComponentBuffer<C>>();
		return buffer.GetComponent(e);
	}
}

#endif