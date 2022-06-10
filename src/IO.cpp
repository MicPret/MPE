#include <MPE/Utils/IO.hpp>

#include <fstream>
#include <sstream>

namespace mpe
{
    std::optional<std::string> ReadFileAsString(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
            return std::nullopt;
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::optional<std::string> ReadFileAsString(std::string_view filepath)
    {
        return ReadFileAsString(std::string(filepath));
    }
}