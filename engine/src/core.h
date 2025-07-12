#pragma once

#include <memory>
#include <vector>
#include "stage/stage.h"

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

        std::shared_ptr<Stage> createStage(bool headless = false);
        void destroyStage(std::shared_ptr<Stage> stage);

    private:
        bool running = false;
        std::vector<std::shared_ptr<Stage>> stages;
    };
}
