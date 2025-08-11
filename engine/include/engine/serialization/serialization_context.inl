#pragma once

#include "engine/serialization/serialization_context.h"
#include "engine/serialization/serializer.h"
#include <string>

namespace Engine::Serialization
{
    template <typename T>
    void SerializationContext::write(const std::string &key, const T &value)
    {
        write_to_ctx(*this, key, value);
    }

    template <typename T>
    void SerializationContext::append(const T &value)
    {
        append_to_ctx(*this, value);
    }

    template <typename T>
    T SerializationContext::read(const std::string &key)
    {
        T value;
        read_from_ctx(*this, key, value);
        return value;
    }

    template <typename T>
    T SerializationContext::read_at(const size_t index)
    {
        T value;
        read_at_from_ctx(*this, index, value);
        return value;
    }
}