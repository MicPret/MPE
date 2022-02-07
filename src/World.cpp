#include <MPE/ECS/World.hpp>

namespace mpe
{
	World::~World()
	{
		for (IComponentBuffer* buf : buffers)
			delete buf;
	}

	Entity World::NewEntity()
	{
		static Entity e = 0;
		return ++e;
	}

	void World::Update(float dt)
	{
		for (const auto& [bucket_name, bucket] : buckets)
		{
			//TODO parallelize
			for (auto& system : bucket)
				system(*this, dt);
		}
	}
}