#include <MPE/Importers/OBJLoader.hpp>
#include <MPE/Graphics/Vertex.hpp>
#include <MPE/Graphics/VertexIndex.hpp>
#include <MPE/Utils/IO.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/Properties.hpp>

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <string_view>
#include <utility>
#include <thread>

namespace mpe
{
	Mesh LoadOBJMesh(const std::string& filename)
	{
		std::string obj_format = ".obj";
		if (filename.find(obj_format) != (filename.length() - obj_format.length()))
		{
			MPE_ERROR("OBJ is the only supported mesh format as of now!");
			return Mesh();
		}
		std::string path = GetGlobalProperty(MESHES_PATH_PROPERTY);
		const std::optional<std::string> obj = ReadFileAsString(path + filename);
		if (!obj.has_value())
		{
			MPE_ERROR("Unable to find " + path + filename);
			return Mesh();
		}
		std::string source = obj.value();

		std::vector<Vec3f> positions;
		std::vector<Vec2f> tex_coords;
		std::vector<Vec3f> normals;
		std::vector<std::array<unsigned, 3>> elements;
		bool uses_quads = false;

		std::istringstream stream(source);
		std::string line;
		while (std::getline(stream, line))
		{
			std::string_view start(line.data(), 2);
			size_t a = start.length();
			size_t l;
			if (start == "v ")
			{
				Vec3f v;
				v.x = std::stof(line.substr(a), &l);
				a += l;
				v.y = std::stof(line.substr(a), &l);
				a += l;
				v.z = std::stof(line.substr(a));
				positions.push_back(std::move(v));
			}
			else if (start == "vt")
			{
				Vec2f vt;
				vt.x = std::stof(line.substr(a), &l);
				a += l;
				vt.y = std::stof(line.substr(a));
				tex_coords.push_back(std::move(vt));
			}
			else if (start == "vn")
			{
				Vec3f vn;
				vn.x = std::stof(line.substr(a), &l);
				a += l;
				vn.y = std::stof(line.substr(a), &l);
				a += l;
				vn.z = std::stof(line.substr(a));
				normals.push_back(std::move(vn));
			}
			else if (start == "f ")
			{
				size_t count = elements.size();
				while (line[a] == ' ')
					a++;
				line = line.substr(a);
				std::istringstream line_stream(line);
				std::string element;
				while (std::getline(line_stream, element, ' '))
				{
					std::array<unsigned, 3> elem;
					std::istringstream elem_stream(element);
					std::string token;
					std::getline(elem_stream, token, '/');
					elem[0] = (unsigned)std::stoul(token);
					std::getline(elem_stream, token, '/');
					elem[1] = (unsigned)std::stoul(token);
					std::getline(elem_stream, token);
					elem[2] = (unsigned)std::stoul(token);
					
					elements.push_back(elem);
				}
				uses_quads = (elements.size() - count) == 4;
			}
		}

		std::vector<Vertex> vertices;
		std::vector<VertexIndex> indices;
		
		vertices.resize(positions.size());
		for (const auto& element : elements)
		{
			Vertex& v = vertices[element[0] - 1];
			v.position = positions[element[0] - 1];
			v.texCoords = tex_coords[element[1] - 1];
			v.normal = normals[element[2] - 1];
		}

		if (!uses_quads)
		{
			indices.resize(elements.size());
			for (size_t i = 0; i < elements.size(); i++)
				indices[i] = elements[i][0] - 1;
		}
		else //triangulate
		{
			indices.resize(elements.size() * 3 / 2);
			for (size_t i = 0, j = 0; i < elements.size(); i += 4, j += 6)
			{
				std::array<unsigned, 4> quad_indices =
				{
					elements[i][0] - 1,
					elements[i + 1][0] - 1,
					elements[i + 2][0] - 1,
					elements[i + 3][0] - 1,
				};

				indices[j] = quad_indices[0];
				indices[j + 1] = quad_indices[1];
				indices[j + 2] = quad_indices[2];
				indices[j + 3] = quad_indices[2];
				indices[j + 4] = quad_indices[3];
				indices[j + 5] = quad_indices[0];
			}
		}

		Mesh result;
		result.vertices = std::move(vertices);
		result.indices = std::move(indices);
		return result;
	}
}