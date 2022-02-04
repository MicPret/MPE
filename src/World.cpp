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
}