#ifndef MPE_PROPERTIES_HPP
#define MPE_PROPERTIES_HPP

#include <string>

namespace mpe
{
	void SetGlobalProperty(const std::string& key, const std::string& value);
	const std::string& GetGlobalProperty(const std::string& key);
	void ParsePropertiesFromFile(const std::string& filename);
}

#endif
