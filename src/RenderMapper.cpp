#include <MPE/Graphics/RenderMapper.hpp>
#include <MPE/Core/Log.hpp>

namespace mpe
{
	void RenderMapper::AddToBatch(const Shader& shader, const Mesh& mesh, const Material& material, const Transform& transform)
	{
		groups[shader].AddToGroup(mesh, material, transform);
	}

	void RenderMapper::Reset()
	{
		//TODO parallelize
		for (auto& [_, rg] : groups)
			rg.Reset();
	}
}