#pragma once

namespace Engine
{
    class Entity;
    class Component
    {
    public:
        virtual ~Component() = default;

        virtual void update(float deltaTime) = 0;
        virtual void setup() = 0;

        Entity *entity;
    };
}