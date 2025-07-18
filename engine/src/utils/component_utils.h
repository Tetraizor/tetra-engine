#pragma once
#include "component_registry.h"
#include <memory>

#define REGISTER_COMPONENT_TYPE(Type, name_str)                      \
    namespace                                                        \
    {                                                                \
        /* one static bool per expansion, unique via __LINE__ */     \
        static const bool AutoRegisterAtLine##__LINE__ = []() {                   \
        Engine::ComponentRegistry::instance().register_type(                  \
            name_str,                                                         \
            []() -> std::unique_ptr<Engine::Component> {                      \
                return std::make_unique<Type>();                              \
            },                                                                \
            typeid(Type)                                                      \
        );                                                                    \
        return true; }(); \
    }
