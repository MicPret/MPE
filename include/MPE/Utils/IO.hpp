#ifndef MPE_IO_HPP
#define MPE_IO_HPP

#include <optional>
#include <string>

namespace mpe
{
    //Returns the content of the file if it's found
    std::optional<std::string> ReadFileAsString(const std::string& filepath);

    //Returns the content of the file if it's found
    std::optional<std::string> ReadFileAsString(std::string_view filepath);
}

#endif