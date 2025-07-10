#include "stage.h"
#include "viewport/viewport.h"

namespace Engine
{
    Stage::Stage(bool headless) : headless(headless) {}
    Stage::~Stage() = default;

    void Stage::update(float deltaTime)
    {
        // TODO: Update scene logic
    }

    void Stage::render()
    {
        if (isHeadless())
            return;

        viewport->beginFrame();

        // TODO: Render things

        viewport->endFrame();
    }

    void Stage::attachViewport(std::shared_ptr<Viewport> vp)
    {
        viewport = std::move(vp);
        headless = false;
    }

    void Stage::detachViewport()
    {
        viewport.reset();
        headless = true;
    }

    bool Stage::isHeadless() const
    {
        return headless || !viewport;
    }
}
