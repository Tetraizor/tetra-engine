#pragma once

namespace Engine
{
    template <typename T>
    class Singleton
    {
    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;

        static T &get_instance()
        {
            static T instance;
            return instance;
        }

    protected:
        Singleton() = default;
        ~Singleton() = default;
    };
}