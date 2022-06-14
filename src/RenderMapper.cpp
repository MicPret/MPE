#include <MPE/Graphics/RenderMapper.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/ThreadPool.hpp>

namespace mpe
{
	void RenderMapper::AddToBatch(const Shader& shader, const Mesh& mesh, const Material& material, const Transform& transform)
	{
		groups[shader].AddToGroup(mesh, material, transform);
	}

	void RenderMapper::Reset()
	{
		static ThreadPool& tp = ThreadPool::Get();
		std::vector<std::future<void>> futures;
		futures.reserve(groups.size());

		for (auto& g : groups)
		{
			auto& group = g.second;
			futures.emplace_back(tp.Execute([&]() { group.Reset(); }));
		}

		for (const auto& f : futures)
			f.wait();
	}
}