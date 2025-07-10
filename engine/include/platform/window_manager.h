#pragma once

#include "platform/window_instance.h"
#include <vector>
#include <memory>
#include <optional>
#include <string>

namespace Engine
{
    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();

        bool initialize();
        void shutdown();

        std::optional<size_t> createWindow(const std::string &title, int width, int height, bool headless = false);

        bool pollEvent(SDL_Event &outEvent);

        WindowInstance *getWindow(size_t id);
        size_t getWindowCount() const;

    private:
        std::vector<std::unique_ptr<WindowInstance>> windows;
        bool initialized = false;
    };
}