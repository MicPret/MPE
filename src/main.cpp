#include <MPE/Core/Logger.hpp>
#include <MPE/Core/Utils.hpp>
#include <MPE/ECS/World.hpp>

#include <array>
#include <string>
#include <string_view>

struct ComponentA {};
class ComponentB {};

int main()
{
	mpe::World w;
	std::array<mpe::Entity, 4> entities = { w.NewEntity(), w.NewEntity(), w.NewEntity(), w.NewEntity() };

	for (mpe::Entity e : entities)
		w.SetComponent<ComponentA>(e);
	w.SetComponent<ComponentB>(entities[2]);
	w.SetComponent<ComponentB>(entities[3]);

	auto system = [](mpe::World& world, float dt)
	{
		auto interesting_entities = world.GetEntities<ComponentA, ComponentB>();
		constexpr std::array<std::string_view, 2> names = { mpe::TypeName<ComponentA>(), mpe::TypeName<ComponentB>() };

		for (mpe::Entity e : interesting_entities)
		{
			mpe::Global<mpe::Logger>().Log(
				"Entity " + std::to_string(e) + " has " + std::string(names[0]) + " and " + std::string(mpe::TypeName<ComponentB>()),
				mpe::LogSeverity::Info,
				mpe::LogOrigin::User
			);
		}
	};

	w.RegisterSystem(mpe::SystemBucket::Default, system);

	for (int i = 0; i < 10; i++)
		w.Update(0.0f);

	return 0;
}