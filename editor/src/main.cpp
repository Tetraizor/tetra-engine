#include "editor.h"

#include <iostream>
#include <memory>

int main(int argc, char **argv)
{
    std::cout << "[Engine] Starting initialization... " << std::endl;

    std::cout << "[Engine] Running with " << argc << " arg(s). Arg(s):" << std::endl;

    try
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
        {
            args.push_back(argv[i]);
            std::cout << "[Engine] " << i << ": " << argv[i] << std::endl;
        }

        if (argc == 1)
        {
            // TODO: Initialize Project Manager
            throw std::runtime_error("[Engine] Project Manager is not implemented yet!");

            return -1;
        }
        else if (argc == 2)
        {
            // TODO: Initialize project directly from console
            std::cout << "[Engine] Launching project at path \"" << args.at(1) << "\""
                      << std::endl;

            try
            {
                Editor::Editor editor(args.at(1));
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
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }

    return -1;
}