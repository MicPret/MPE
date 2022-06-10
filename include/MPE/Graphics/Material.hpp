#ifndef MPE_MATERIAL_HPP
#define MPE_MATERIAL_HPP

#include "Texture.hpp"
#include <MPE/Utils/DataOriented.hpp>
#include <MPE/Maths/Vec4f.hpp>

#include <array>

namespace mpe
{
	MPE_DOD_DECLARE_ID_TYPE(Material)

	struct Material
	{
		std::array<TextureId, 8> textures;
		std::array<float, 8> floats;
		std::array<Vec4f, 8> vecs;

		Material();
		static inline constexpr size_t DIFFUSE_MAP_INDEX = 0;
		static inline constexpr size_t SPECULAR_MAP_INDEX = 1;
		static inline constexpr size_t SHININESS_INDEX = 0;
	};

	MPE_DOD_DECLARE_READ(Material);
	MPE_DOD_DECLARE_WRITE(Material);
}

#endif