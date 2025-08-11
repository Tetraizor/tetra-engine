#include "engine/entity/entity_id.h"

namespace Engine
{
    const EntityID EntityID::Invalid = {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1)};

    void EntityID::serialize(Serialization::SerializationContext &ctx) const
    {
        ctx.write("index", index);
        ctx.write("generation", generation);
    }

    void EntityID::deserialize(Serialization::SerializationContext &ctx)
    {
        index = ctx.read<uint32_t>("index");
        generation = ctx.read<uint32_t>("generation");
    }
}
