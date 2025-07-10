#pragma once

#include <memory>

namespace Engine
{
    class Viewport;

    class Stage
    {
    public:
        explicit Stage(bool headless = false);
        ~Stage();

        void update(float deltaTime);
        void render();

        void attachViewport(std::shared_ptr<Viewport> viewport);
        void detachViewport();

        bool isHeadless() const;

    private:
        bool headless;
        std::shared_ptr<Viewport> viewport;
    };
}