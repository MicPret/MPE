#include <MPE/Graphics/Shader.hpp>
#include <MPE/Graphics/GraphicsDefines.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/Properties.hpp>
#include <MPE/Core/Utils.hpp>
#include <MPE/Utils/IO.hpp>

#include <glad/gl.h>

#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <thread>

namespace mpe
{
	static GLuint CompileShader(const std::string& source, const std::string& defines, GLenum type, GLint* success);

	Shader LoadShader(const std::string& shader_name, ShaderPriority priority)
	{
		Shader invalid{ 0, priority };

		MPE_INFO("Compiling shader: " + shader_name);

		std::string could_not_find_or_open = "Failure: could not find/open shader file: ";
		std::string path = GetGlobalProperty(SHADERS_PATH_PROPERTY);
		std::string shader_path = path + shader_name;

		std::string defines_path = path + "common.glsl";
		const auto defines = ReadFileAsString(defines_path);
		if (!defines.has_value())
		{
			MPE_ERROR(could_not_find_or_open + defines_path);
			return invalid;
		}

		std::string vertex_path = shader_path + ".vert";
		std::string fragment_path = shader_path + ".frag";

		int success;

		std::array<GLuint, 8> compiled_shaders;
		unsigned compiled_n = 0;
		compiled_shaders.fill(0);

		//VERTEX
		MPE_INFO("Compiling VERTEX shader...");
		std::optional<std::string> source = ReadFileAsString(vertex_path);
		if (!source.has_value())
		{
			MPE_ERROR(could_not_find_or_open + vertex_path);
			return invalid;
		}
		GLuint vertex = CompileShader(source.value(), defines.value(), GL_VERTEX_SHADER, &success);
		if (success == GL_FALSE)
		{
			glDeleteShader(vertex);
			return invalid;
		}
		else
		{
			compiled_shaders[compiled_n++] = vertex;
			MPE_INFO("Compilation succeeded!");
		}

		//FRAGMENT
		MPE_INFO("Compiling FRAGMENT shader...");
		source = ReadFileAsString(fragment_path);
		if (!source.has_value())
		{
			MPE_ERROR(could_not_find_or_open + fragment_path);
			return invalid;
		}
		GLuint fragment = CompileShader(source.value(), defines.value(), GL_FRAGMENT_SHADER, &success);
		if (success == GL_FALSE)
		{
			glDeleteShader(fragment);
			glDeleteShader(vertex);
			return invalid;
		}
		else
		{
			compiled_shaders[compiled_n++] = fragment;
			MPE_INFO("Compilation succeeded!");
		}

		//GEOMETRY
		MPE_INFO("Compiling GEOMETRY shader...");
		source = ReadFileAsString(shader_path + ".geom");
		if (!source.has_value())
			MPE_INFO(could_not_find_or_open + shader_path + ".geom");
		else
		{
			GLuint geometry = CompileShader(source.value(), defines.value(), GL_GEOMETRY_SHADER, &success);
			if (success == GL_FALSE)
				glDeleteShader(geometry);
			else
			{
				compiled_shaders[compiled_n++] = geometry;
				MPE_INFO("Compilation succeeded!");
			}
		}

		//PROGRAM
		GLuint program = glCreateProgram();
		for (unsigned i = 0; i < compiled_n; i++)
			glAttachShader(program, compiled_shaders[i]);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		for (unsigned i = 0; i < compiled_n; i++)
			glDeleteShader(compiled_shaders[i]);
		if (success == GL_FALSE)
		{
			constexpr GLsizei LOG_BUFFER_SIZE = 512;
			GLsizei len;
			GLchar log_buffer[LOG_BUFFER_SIZE];
			glGetProgramInfoLog(program, LOG_BUFFER_SIZE, &len, log_buffer);
			MPE_ERROR("Shader program linking failed!\n" + std::string(log_buffer, len));
			glDeleteProgram(program);
			return invalid;
		}
		MPE_INFO("Shader program linking succeeded!");
		for (unsigned i = 0; i < compiled_n; i++)
			glDetachShader(program, compiled_shaders[i]);

		return Shader{ program, priority };
	}

	void FreeShader(ShaderId id)
	{
		MPE_INFO("Freeing shader with ID: " + std::to_string(id.value));
		glDeleteProgram(id.value);
		MPE_INFO(id.value != 0 ? "Succesfully freed!" : "Implementation ignored the call on ID 0");
	}

	GLuint CompileShader(const std::string& source, const std::string& defines, GLenum type, GLint* success)
	{
		const std::array<const GLchar*, 2> input = { defines.c_str(), source.c_str()};
		const std::array<const GLint, 2> input_len = { defines.length(), source.length()};
		constexpr GLsizei LOG_BUFFER_SIZE = 512;

		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 2, input.data(), input_len.data());
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, success);
		if (*success == GL_FALSE)
		{
			std::array<GLchar, LOG_BUFFER_SIZE> log_buffer;
			GLsizei len;
			glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &len, log_buffer.data());

			MPE_ERROR("Shader compilation failed!\n" + std::string(log_buffer.data(), len));
		}

		return shader;
	}
}