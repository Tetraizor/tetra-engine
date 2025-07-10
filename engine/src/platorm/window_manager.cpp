#include "platform/window_manager.h"

namespace Engine
{
    WindowManager::WindowManager() = default;
    WindowManager::~WindowManager()
    {
        shutdown();
    }

    bool WindowManager::initialize()
    {
        if (initialized)
            return true;

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            return false;
        }

        initialized = true;
        return true;
    }

    void WindowManager::shutdown()
    {
        windows.clear();

        if (initialized)
        {
            SDL_Quit();
            initialized = false;
        }
    }

    std::optional<size_t> WindowManager::createWindow(const std::string &title, int width, int height, bool headless)
    {
        auto window = std::make_unique<WindowInstance>();

        if (!window->create(title, width, height, headless))
        {
            return std::nullopt;
        }

        windows.push_back(std::move(window));
        return windows.size() - 1;
    }

    bool WindowManager::pollEvent(SDL_Event &outEvent)
    {
        return SDL_PollEvent(&outEvent);
    }

    WindowInstance *WindowManager::getWindow(size_t id)
    {
        if (id >= windows.size())
            return nullptr;

        return windows[id].get();
    }

    size_t WindowManager::getWindowCount() const
    {
        return windows.size();
    }
}