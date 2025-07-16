#pragma once

#include "serialization/serialization_context.h"

#include <cstdint>
#include <functional>
#include <iostream>

namespace Engine
{
    struct ComponentID
    {
        uint32_t index;
        uint32_t generation;

        static const ComponentID Invalid;

        bool is_valid() const
        {
            return *this != Invalid;
        }

        bool operator==(const ComponentID &other) const
        {
            return index == other.index && generation == other.generation;
        }

        bool operator!=(const ComponentID &other) const
        {
            return !(*this == other);
        }

        void serialize(SerializationContext &ctx) const
        {
            ctx.write_UInt(index, "index");
            ctx.write_UInt(generation, "generation");
        }

        void deserialize(SerializationContext &ctx)
        {
            index = ctx.read_UInt("index");
            generation = ctx.read_UInt("generation");
        }
    };

    inline std::ostream &operator<<(std::ostream &os, const ComponentID &id)
    {
        os << "ComponentID{ index: " << id.index
           << ", generation: " << id.generation << " }";
        return os;
    }
}

namespace std
{
    template <>
    struct hash<Engine::ComponentID>
    {
        std::size_t operator()(const Engine::ComponentID &id) const noexcept
        {
            uint64_t packed = (static_cast<uint64_t>(id.generation) << 32) | id.index;
            return std::hash<uint64_t>()(packed);
        }
    };
}
