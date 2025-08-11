#pragma once

#include <exception>
#include <string>

namespace Engine::Exceptions
{
    class JsonTypeMismatchException : public std::exception
    {
    public:
        JsonTypeMismatchException(const std::string &key, const std::string &expected_type)
        {
            message = "Type mismatch for key \"" + key + "\". Expected type: " + expected_type;
        }

        const char *what() const noexcept override
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

}