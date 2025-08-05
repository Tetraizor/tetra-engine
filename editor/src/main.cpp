#include "runtime/editor_instance.h"

#include "engine.h"

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

    Logger::log_info("[Editor] Starting...");
    Logger::log_info("[Editor] Running with " + std::to_string(arguments.size()) + " arg(s).");

    Editor::EditorInstance instance;

    std::vector<std::string> args;
    for (int i = 0; i < arguments.size(); i++)
    {
        args.push_back(arguments[i]);
        Logger::log_info("[Editor] " + std::to_string(i) + ": " + arguments[i]);
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
        Logger::log_info("[Editor] Launching project at path \"" + args.at(1) + "\"");

        try
        {
            instance.init(arguments[1]);
            result = instance.run(arguments);
        }
        catch (const std::exception &e)
        {
            Logger::log_error("[Engine] Failed to launch project: " + std::string(e.what()));
            return 1;
        }

        return -1;
    }
    else
    {
        Logger::log_info("[Engine] Program launched with invalid arguments, exiting...");
        return -1;
    }

    return result;
}
