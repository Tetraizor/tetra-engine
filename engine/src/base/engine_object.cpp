#include "engine/base/engine_object.h"

namespace Engine
{
    std::atomic<size_t> EngineObject::global_id_counter{1};

    EngineObject::EngineObject() : instance_id(global_id_counter.fetch_add(1, std::memory_order_relaxed)) {}
    EngineObject::~EngineObject() = default;
}
