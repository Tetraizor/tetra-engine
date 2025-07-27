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

        void serialize(Engine::Serialization::SerializationContext &ctx) const
        {
            ctx.write("index", index);
            ctx.write("generation", generation);
        }

        void deserialize(Engine::Serialization::SerializationContext &ctx)
        {
            index = ctx.read<uint32_t>("index");
            generation = ctx.read<uint32_t>("generation");
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
