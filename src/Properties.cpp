#include <MPE/Core/Properties.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Utils/IO.hpp>

#include <unordered_map>
#include <sstream>

namespace mpe
{
	static std::unordered_map<std::string, std::string> props;

	void SetGlobalProperty(const std::string& key, const std::string& value)
	{
		props.insert(std::make_pair(key, value));
	}

	const std::string& GetGlobalProperty(const std::string& key)
	{
		return props.at(key);
	}

	void ParsePropertiesFromFile(const std::string& filename)
	{
		std::optional<std::string> result = ReadFileAsString(filename);
		if (!result.has_value())
		{
			MPE_ERROR("Could not find property file named " + filename + '!');
			return;
		}
		std::istringstream props(result.value());
		std::string line;
		while (std::getline(props, line))
		{
			auto equal_sign = line.find('=');
			if (equal_sign == std::string::npos)
				continue;
			std::string key = line.substr(0, equal_sign);
			std::string value = line.substr(equal_sign + 1);
			SetGlobalProperty(key, value);
		}
	}
}