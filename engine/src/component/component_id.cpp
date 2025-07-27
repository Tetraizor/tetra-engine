#include "component/component_id.h"

namespace Engine
{
    const ComponentID ComponentID::Invalid{UINT32_MAX, UINT32_MAX};

    void ComponentID::deserialize(Engine::Serialization::SerializationContext &ctx)
    {
        index = ctx.read<uint32_t>("index");
        generation = ctx.read<uint32_t>("generation");
    }

    void ComponentID::serialize(Engine::Serialization::SerializationContext &ctx) const
    {
        ctx.write("index", index);
        ctx.write("generation", generation);
    }
}
