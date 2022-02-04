#ifndef MPE_WORLD_HPP
#define MPE_WORLD_HPP

#include "Entity.hpp"
#include "ComponentBuffer.hpp"
#include <MPE/Core/Utils.hpp>
#include <vector>
#include <utility>

namespace mpe
{
	//Singleton class to manage the ECS
	class World
	{
	public:
		World() = default;
		~World();
		Entity NewEntity();
		template <typename C, typename... Args>
		C& SetComponent(Entity e, Args&&... args);
		template <typename C>
		C& GetComponent(Entity e);
		//TODO: implement systems
	private:
		std::vector<IComponentBuffer*> buffers;
		template <typename C>
		ComponentBuffer<C>& GetBuffer();
	};

	template<typename C>
	ComponentBuffer<C>& World::GetBuffer()
	{
		static size_t index = ~0;
		ComponentBuffer<C>* output;
		if (index != ~0)
			output = static_cast<ComponentBuffer<C>*>(buffer[index]);
		else
		{
			index = buffers.size();
			output = new ComponentBuffer<C>();	//new is absolutely fine!
			buffers.push_back(output);
		}
		return *output;
	}

	template<typename C, typename ...Args>
	C& World::SetComponent(Entity e, Args && ...args)
	{
		ComponentBuffer<C>& buffer = GetBuffer<C>();
		return buffer.SetComponent(e, std::forward<Args>(args)...);
	}

	template<typename C>
	C& World::GetComponent(Entity e)
	{
		ComponentBuffer<C>& buffer = GetBuffer<C>();
		return buffer.GetComponent(e);
	}
}

#endif