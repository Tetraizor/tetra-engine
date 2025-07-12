#pragma once

#include <memory>
#include "viewport/viewport.h"
#include "entity/entity_manager.h"

namespace Engine
{
    class Viewport;

    class Stage
    {
    public:
        explicit Stage(bool headless = false);
        ~Stage();

        void setup();
        void render();
        void update(float deltaTime);

        void attach_viewport(std::shared_ptr<Viewport> viewport);
        void detach_viewport();

        bool is_headless() const;

        EntityManager &get_entity_manager();

    private:
        bool headless;
        std::shared_ptr<Viewport> viewport;

        EntityManager entity_manager;
    };
}