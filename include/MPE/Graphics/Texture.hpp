#ifndef MPE_TEXTURE_HPP
#define MPE_TEXTURE_HPP

#include <MPE/Utils/DataOriented.hpp>

#include <cstdint>
#include <optional>
#include <string>

namespace mpe
{
	MPE_DOD_DECLARE_ID_TYPE(Texture);

	struct Texture
	{
		TextureId id;
		int width;
		int height;
		bool loaded = false;
	};

	//MPE_DOD_DECLARE_READ(Texture);

	inline constexpr auto TEXTURES_PATH_PROPERTY = "TEXTURES_PATH";

	//Loads a texture from a file and puts it in the buffer
	TextureId LoadTexture(const std::string& name, bool filtered);
	//Frees the identified texture, behaviour is undefined for undefined ids
	bool FreeTexture(TextureId id);
}

#endif