#include "core.h"

int main()
{
    Engine::Core core;

    if (!core.initialize())
    {
        return -1;
    }

    core.run();
    core.shutdown();

    return 0;
}