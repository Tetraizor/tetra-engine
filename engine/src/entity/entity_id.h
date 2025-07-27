#pragma once

#include <cstdint>
#include <functional>
#include <iostream>

#include "serialization/serialization_context.h"

namespace Engine
{
    struct EntityID
    {
        uint32_t index;
        uint32_t generation;

        static const EntityID Invalid;

        bool is_valid() const
        {
            return *this != Invalid;
        }

        bool operator==(const EntityID &other) const
        {
            return index == other.index && generation == other.generation;
        }

        bool operator!=(const EntityID &other) const
        {
            return !(*this == other);
        }

        void serialize(Serialization::SerializationContext &ctx) const;

        void deserialize(Serialization::SerializationContext &ctx);
    };

    inline std::ostream &operator<<(std::ostream &os, const EntityID &id)
    {
        os << "EntityID{ index: " << id.index << ", generation: " << id.generation << " }";
        return os;
    }
}

namespace std
{
    template <>
    struct hash<Engine::EntityID>
    {
        size_t operator()(const Engine::EntityID &id) const noexcept
        {
            return (static_cast<uint64_t>(id.generation) << 32) | id.index;
        }
    };
}