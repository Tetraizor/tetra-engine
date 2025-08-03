#include "runtime/editor_instance.h"

#include <iostream>
#include <memory>

int main(int argc, char **argv)
{

    std::vector<std::string> arguments{};

    for (int i = 0; i < argc; i++)
    {
        arguments.push_back(argv[i]);
    }

    int result = -1;

    std::cout << "[Editor] Starting initialization... " << std::endl;
    std::cout << "[Editor] Running with " << arguments.size() << " arg(s)." << std::endl;

    Editor::EditorInstance instance;

    try
    {
        std::vector<std::string> args;
        for (int i = 0; i < arguments.size(); i++)
        {
            args.push_back(arguments[i]);
            std::cout << "[Editor] " << i << ": " << arguments[i] << std::endl;
        }

        if (arguments.size() == 1)
        {
            // TODO: Initialize Project Manager with empty project
            throw std::runtime_error("[Editor] Project Manager is not implemented yet!");

            return -1;
        }
        else if (arguments.size() == 2)
        {
            // TODO: Initialize project directly from console
            std::cout << "[Editor] Launching project at path \"" << args.at(1) << "\""
                      << std::endl;

            try
            {
                instance.init(arguments[1]);
                result = instance.run(arguments);
            }
            catch (const std::exception &e)
            {
                std::cerr << "[Engine] Failed to launch project: " << e.what() << std::endl;
                return 1;
            }

            return -1;
        }
        else
        {
            std::cout << "[Engine] Program launched with invalid arguments, exiting..." << std::endl;
            return -1;
        }

        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }

    return -1;
}