#include <MPE/Graphics/Texture.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/Properties.hpp>

#include <glad/gl.h>
#include <stb/stb_image.h>

#include <vector>

namespace mpe
{
	MPE_DOD_IMPLEMENT(Texture);

	static struct TextureChecker
	{
		~TextureChecker()
		{
			const auto& buffer = ReadTextureBuffer();
			for (const Texture& texture : buffer)
				if (texture.loaded)
					MPE_ERROR("Texture resource not released! ID: " + std::to_string(texture.id.value));
		}
	} check;

	static TextureId LoadInGPU(const stbi_uc* data, int w, int h, bool filtered)
	{
		GLuint id;

		GLint filter = filtered ? GL_LINEAR : GL_NEAREST;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureStorage2D(id, 1, GL_RGBA8, w, h);	//TODO: no mipmaps?
		glTextureSubImage2D(id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

		return TextureId(id);
	}

	static void FreeInGPU(TextureId id)
	{
		glDeleteTextures(1, &id.value);
	}

	TextureId LoadTexture(const std::string& name, bool filtered)
	{
		MPE_INFO("Loading texture: \"" + name + "\"...");

		Texture texture;
		std::string fullpath = GetGlobalProperty(std::string(TEXTURES_PATH_PROPERTY)) + name;

		int channels;
		stbi_uc* data = stbi_load(fullpath.c_str(), &texture.width, &texture.height, &channels, 4);
		if (data == nullptr)
		{
			MPE_ERROR("Texture loading failed!");
			return TextureId(0);
		}
		texture.id = LoadInGPU(data, texture.width, texture.height, filtered);
		texture.loaded = true;

		MPE_INFO("Succesfully loaded! ID: " + std::to_string(texture.id.value));
		SetTexture(texture.id, texture);
		return texture.id;
	}

	bool FreeTexture(TextureId id)
	{
		MPE_INFO("Freeing texture with ID: " + std::to_string(id.value));

		Texture& texture = GetTexture(id);
		if (texture.loaded)
		{
			FreeInGPU(id);
			texture.loaded = false;
			MPE_INFO("Succesfully freed!");
			return true;
		}
		
		MPE_ERROR("Texture is not loaded!");
		return false;
	}
}