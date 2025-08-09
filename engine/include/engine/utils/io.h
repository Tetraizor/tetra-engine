#pragma once

#include <optional>
#include <string>

namespace Engine::Utils::IO
{
    std::optional<std::string> read_file_contents(const std::string &path);
}