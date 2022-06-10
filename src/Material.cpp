#include <MPE/Graphics/Material.hpp>

namespace mpe
{
	MPE_DOD_IMPLEMENT(Material);

	Material::Material()
	{
		textures.fill(TextureId(0));
		floats.fill(0.0f);
		vecs.fill(Vec4f());
	}
}