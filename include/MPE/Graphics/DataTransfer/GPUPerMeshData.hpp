#ifndef MPE_RENDER_PROPERTIES_HPP
#define MPE_RENDER_PROPERTIES_HPP

#include <MPE/Maths/Mat4f.hpp>

namespace mpe
{
	struct GPUPerMeshData
	{
		std::array<int32_t, 8> textureIndices;
		std::array<float, 8> floats;
		std::array<Vec4f, 8> vecs;
		Mat4f modelMatrix;

		inline GPUPerMeshData() : modelMatrix(Mat4f::Identity())
		{
			textureIndices.fill(-1);
			floats.fill(0.0f);
			vecs.fill(Vec4f());
		}
	};
}

#endif