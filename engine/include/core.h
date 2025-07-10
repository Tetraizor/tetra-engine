#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <optional>
#include "platform/window_manager.h"

namespace Engine
{
    class Core
    {
    public:
        Core();
        ~Core();

        bool initialize();
        void run();
        void shutdown();

    private:
        void update();
        void render();

        std::unique_ptr<WindowManager> windowManager;
        std::optional<size_t> mainWindowId;

        bool running = false;
    };
}