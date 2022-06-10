#ifndef MPE_RENDER_MAPPER_HPP
#define MPE_RENDER_MAPPER_HPP

#include "Shader.hpp"
#include "RenderGroup.hpp"

#include <map>
#include <type_traits>

namespace mpe
{
	struct RenderMapper
	{
		struct ShaderPriorityComparator
		{
			constexpr bool operator()(const Shader& a, const Shader& b) const
			{
				using impl = std::underlying_type<ShaderPriority>::type;
				return static_cast<impl>(a.priority) > static_cast<impl>(b.priority)
					|| (static_cast<impl>(a.priority) == static_cast<impl>(b.priority) && a.id.value < b.id.value);
			}
		};
		std::map<Shader, RenderGroup, ShaderPriorityComparator> groups;

		void AddToBatch(const Shader& shader, const Mesh& mesh, const Material& material, const Transform& transform);
		void Reset();
	};
}

#endif