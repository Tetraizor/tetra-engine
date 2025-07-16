#include "entity/entity_id.h"

const Engine::EntityID Engine::EntityID::Invalid = {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1)};

void Engine::EntityID::serialize(SerializationContext &ctx) const
{
    ctx.write_UInt(index, "index");
    ctx.write_UInt(generation, "generation");
}

void Engine::EntityID::deserialize(SerializationContext &ctx)
{
    index = ctx.read_UInt("index");
    generation = ctx.read_UInt("generation");
}