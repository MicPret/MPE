#ifndef MPE_WORLD_HPP
#define MPE_WORLD_HPP

#include "Entity.hpp"
#include "ComponentBuffer.hpp"
#include "SystemBucket.hpp"
#include <MPE/Core/Utils.hpp>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <map>
#include <unordered_map>

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
		//Returns all the Entities with the specified Component set
		template <typename... CompSet>
		std::vector<Entity> GetEntities();
		template <typename S>
		void RegisterSystem(SystemBucket bucket, S&& system);
		void Update(float dt);
	private:
		using Bucket = std::vector<std::function<void(World&, float dt)>>;
		std::map<SystemBucket, Bucket> buckets;
		std::vector<IComponentBuffer*> buffers;
		template <typename C>
		ComponentBuffer<C>& GetBuffer();
		template <typename Comp, typename... CompSet>
		void Count(std::unordered_map<Entity, unsigned>& counter);
	};

	template<typename C>
	ComponentBuffer<C>& World::GetBuffer()
	{
		static size_t index = ~0;
		ComponentBuffer<C>* output;
		if (index != ~0)
			output = static_cast<ComponentBuffer<C>*>(buffers[index]);
		else
		{
			index = buffers.size();
			output = new ComponentBuffer<C>();	//new is absolutely fine!
			buffers.push_back(output);
		}
		return *output;
	}

	template<typename Comp, typename... CompSet>
	void World::Count(std::unordered_map<Entity, unsigned>& counter)
	{
		const std::vector<Entity>& entities = GetBuffer<Comp>().Entities();
		for (Entity e : entities)
			counter[e]++;
		if constexpr (sizeof...(CompSet) > 0)
			Count<CompSet...>(counter);
	}

	template<typename C, typename ...Args>
	C& World::SetComponent(Entity e, Args&& ...args)
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

	template<typename... CompSet>
	std::vector<Entity> World::GetEntities()
	{
		constexpr size_t NUM_COMPS = sizeof...(CompSet);
		static_assert(NUM_COMPS > 0, "You should specify a set of Components!");
		std::vector<Entity> entities;
		std::unordered_map<Entity, unsigned> count;
		Count<CompSet...>(count);
		for (auto [e, c] : count)
			if (c == NUM_COMPS)
				entities.push_back(e);
		return entities;
	}
	template<typename S>
	inline void World::RegisterSystem(SystemBucket bucket, S&& system)
	{
		buckets[bucket].push_back({ system });
	}
}

#endif