#ifndef MPE_SHADER_HPP
#define MPE_SHADER_HPP

#include <MPE/Core/Identifier.hpp>

#include <string>

namespace mpe
{
	struct Shader;
	using ShaderId = Identifier<Shader>;
	enum class ShaderPriority : unsigned char
	{
		Low = 0,
		Medium,
		High
	};
	
	struct Shader
	{
		ShaderId id;
		ShaderPriority priority;
	};

	inline constexpr auto SHADERS_PATH_PROPERTY = "SHADERS_PATH";

	//Loads a shader from a file
	Shader LoadShader(const std::string& shader_name, ShaderPriority priority = ShaderPriority::Medium);

	//Frees the identified shader. Behaviour is undefined for invalid ids
	void FreeShader(ShaderId shader);

}

#endif