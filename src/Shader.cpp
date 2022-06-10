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
	static GLuint CompileShader(const std::string& source, GLenum type, GLint* success);
	static bool ValidateForMPE(GLuint shader_program);

	Shader LoadShader(const std::string& shader_name, ShaderPriority priority)
	{
		Shader invalid{ 0, priority };

		MPE_INFO("Compiling shader: " + shader_name);

		std::string path = GetGlobalProperty(SHADERS_PATH_PROPERTY) + shader_name;
		std::string vertex_path = path + ".vert";
		std::string fragment_path = path + ".frag";

		std::string could_not_find_or_open = "Failure: could not find/open shader file: ";
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
		GLuint vertex = CompileShader(source.value(), GL_VERTEX_SHADER, &success);
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
		GLuint fragment = CompileShader(source.value(), GL_FRAGMENT_SHADER, &success);
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
		source = ReadFileAsString(path + ".geom");
		if (!source.has_value())
			MPE_INFO(could_not_find_or_open + path + ".geom");
		else
		{
			GLuint geometry = CompileShader(source.value(), GL_GEOMETRY_SHADER, &success);
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

		//MPE VALIDATION
		if (!ValidateForMPE(program))
			MPE_WARNING("Shader is not MPE-approved! :(");
		else
			MPE_INFO("Shader is MPE-approved! :)");
		return Shader{ program, priority };
	}

	void FreeShader(ShaderId id)
	{
		MPE_INFO("Freeing shader with ID: " + std::to_string(id.value));
		glDeleteProgram(id.value);
		MPE_INFO(id.value != 0 ? "Succesfully freed!" : "Implementation ignored the call on ID 0");
	}

	GLuint CompileShader(const std::string& source, GLenum type, GLint* success)
	{
		constexpr GLsizei LOG_BUFFER_SIZE = 512;

		GLuint shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, success);
		if (*success == GL_FALSE)
		{
			std::array<GLchar, LOG_BUFFER_SIZE> log_buffer;
			GLsizei len;
			glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &len, log_buffer.data());

			[[maybe_unused]]
			std::string msg = "Shader compilation failed!\n" + std::string(log_buffer.data(), len);
			MPE_ERROR(msg);
		}

		return shader;
	}

	bool ValidateForMPE(GLuint shader_program)
	{
		std::string uniform_blocks[] = { MPE_CAMERA_BLOCK_NAME, MPE_MESH_BLOCK_NAME, /*MPE_LIGHT_BUFFER_NAME*/ };
		unsigned uniform_bindings[] = { MPE_CAMERA_BINDING, MPE_MESH_BINDING, /*MPE_LIGHT_BUFFER_NAME*/ };

		std::array<GLenum, 3> properties{ GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE };
		std::array<GLint, properties.size()> values{};

		GLint number_of_uniforms;
		glGetProgramInterfaceiv(shader_program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &number_of_uniforms);
		for (GLint i = 0; i < number_of_uniforms; i++)
		{
			glGetProgramResourceiv(
				shader_program, GL_UNIFORM_BLOCK, (GLuint)i,
				properties.size(), properties.data(),
				values.size(), nullptr, values.data());

			std::array<char, 128> name_data{};
			glGetProgramResourceName(
				shader_program,
				GL_UNIFORM_BLOCK,
				(GLuint)i,
				name_data.size(),
				nullptr,
				name_data.data());
			std::string name(name_data.data(), values[0] - 1);
			MPE_INFO("Uniform block found: " + name);

			for (size_t j = 0; j < sizeof(uniform_blocks) / sizeof(std::string); j++)
			{
				if (name == uniform_blocks[j])
				{
					glUniformBlockBinding(shader_program, (GLuint)i, uniform_bindings[j]);
					MPE_INFO("Uniform block bound at binding point " + std::to_string(uniform_bindings[j]));

					uniform_bindings[j] = ~0;
					break;
				}
			}
		}
		for (int x : uniform_bindings)
			if (x != ~0)
				return false;
		/*
		glUseProgram(shader_program);
		constexpr GLint MAX_TEXTURES = 8;
		for (GLint i = 0; i < MAX_TEXTURES; i++)
		{
			std::string uniform_name = "textures[" + std::to_string(i) + ']';
			GLint lc = glGetUniformLocation(shader_program, uniform_name.c_str());
			if (lc == -1)
				return false;
			glUniform1i(lc, i);
		}
		*/

		return true;
	}
}