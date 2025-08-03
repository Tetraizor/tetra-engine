#include <exception>
#include <string>

namespace Engine::Exceptions
{
    class JsonParseException : public std::exception
    {
    public:
        explicit JsonParseException(const std::string &text) : message("Could not parse text to a valid json object: \"" + text + "\"") {}

        const char *what() const noexcept override
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

}