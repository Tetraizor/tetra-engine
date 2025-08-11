#include <exception>
#include <string>

namespace Engine::Exceptions
{
    class JsonKeyNotFoundException : public std::exception
    {
    public:
        explicit JsonKeyNotFoundException(const std::string &key) : message("Key not found in JSON object: \"" + key + "\"") {}

        const char *what() const noexcept override
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

}