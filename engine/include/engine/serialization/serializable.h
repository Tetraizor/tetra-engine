#pragma once

namespace Engine::Serialization
{
    class SerializationContext;

    class Serializable
    {
    public:
        virtual ~Serializable() = default;

        virtual void serialize(SerializationContext &ctx) const = 0;
        virtual void deserialize(SerializationContext &ctx) = 0;

    protected:
        Serializable() = default;
    };
}