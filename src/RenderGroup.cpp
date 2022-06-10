#include <MPE/Graphics/RenderGroup.hpp>

namespace mpe
{
	void RenderGroup::AddToGroup(const Mesh& mesh, const Material& material, const Transform& transform)
	{
		if (batches.empty() || !batches.back().Push(mesh, material, transform))
		{
			batches.resize(batches.size() + 1);
			batches.back().Push(mesh, material, transform);
		}
	}

	void RenderGroup::Reset()
	{
		switch (batches.size())
		{
		default:
			batches.erase(batches.begin() + 1, batches.end());
			[[fallthrough]];
		case 1:
			batches[0].Reset();
			[[fallthrough]];
		case 0:
			return;
		}
	}
}