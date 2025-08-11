#include <fstream>
#include <sstream>

#include "engine/utils/io.h"

namespace Engine::Utils::IO
{
    std::optional<std::string> read_file_contents(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            return std::nullopt;

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}
