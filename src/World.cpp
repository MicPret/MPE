#include <MPE/ECS/World.hpp>

namespace mpe
{
	World& World::GetInstance()
	{
		static World world;
		return world;
	}

	Entity World::NewEntity()
	{
		static Entity e = 0;
		return ++e;
	}
}