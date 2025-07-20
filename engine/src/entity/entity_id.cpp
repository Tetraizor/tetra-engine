#include "entity/entity_id.h"

namespace Engine
{

    const EntityID EntityID::Invalid = {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1)};

    void EntityID::serialize(SerializationContext &ctx) const
    {
        ctx.write_UInt(index, "index");
        ctx.write_UInt(generation, "generation");
    }

    void EntityID::deserialize(SerializationContext &ctx)
    {
        index = ctx.read_UInt("index");
        generation = ctx.read_UInt("generation");
    }
}