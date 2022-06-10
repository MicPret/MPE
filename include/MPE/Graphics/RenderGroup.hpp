#ifndef MPE_RENDER_GROUP_HPP
#define MPE_RENDER_GROUP_HPP

#include "RenderBatch.hpp"
#include <vector>

namespace mpe
{
	struct RenderGroup
	{
		std::vector<RenderBatch> batches;

		void AddToGroup(
			const Mesh& mesh,
			const Material& material,
			const Transform& transform);
		void Reset();
	};
}

#endif