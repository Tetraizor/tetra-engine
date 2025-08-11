#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <cassert>

#include "engine/engine.h"

#include "engine/base/singleton.h"

namespace Engine
{
    /**
     * @brief Central registry for component types, enabling creation by name and lookup of type names.
     */

    class ComponentRegistry : public Singleton<ComponentRegistry>
    {
        friend class ComponentManager;

    public:
        /**
         * @brief Get the registered name for a component instance.
         * @param comp Pointer to a component instance.
         * @return The registered string name, or empty string if not registered.
         */
        std::string get_name(const Component *component) const;

        /**
         * @brief Get the names for all registered components with types.
         * @return Unordered list with types as keys and their names as values.
         */
        std::unordered_map<std::type_index, std::string> get_type_names() const { return type_names; }

        /**
         * @brief Register a component type T with a unique string name.
         *
         * @tparam T Concrete component type, must derive from Component.
         * @param name Unique name for serialization.
         */
        template <typename T>
        void register_type(const std::string &name)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            Logger::log_info("[ComponentRegistry] Registered component with name " + name);

            std::type_index type_index(typeid(T));
            creators[name] = []()
            { return std::make_unique<T>(); };

            type_names[type_index] = name;
        }

    private:
        using create_func = std::function<std::shared_ptr<Component>()>;

        /**
         * @brief Create a new component instance by type.
         * @tparam T Concrete component type, must be registered.
         * @return shared_ptr to the new Component, or nullptr if type not registered.
         */
        template <typename T>
        std::shared_ptr<Component> instantiate_raw() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            std::type_index id(typeid(T));

            std::string type_name{};
            auto type_name_it = type_names.find(id);
            if (type_name_it != type_names.end())
            {
                type_name = type_name_it->second;
            }

            if (type_name.empty())
            {
                throw std::runtime_error("No matching component found in registry with type provided.");
            }

            auto creator_it = creators.find(type_name);
            if (creator_it != creators.end())
            {
                return creator_it->second();
            }

            throw std::runtime_error("No matching component found in registry with name " + type_name + ".");
        }

        /**
         * @brief Create a new component instance by type.
         * @tparam T Concrete component type, must be registered.
         * @return shared_ptr to the new T, or nullptr if type not registered.
         */
        template <typename T>
        std::shared_ptr<T> instantiate_raw_typed() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            std::shared_ptr<Component> base = instantiate_raw<T>();

            return std::static_pointer_cast<T>(base);
        }

        /**
         * @brief Create a new component instance by registered name.
         * @param name The registered name of the component type.
         * @return shared_ptr to the new Component, or nullptr if name not found.
         */
        std::shared_ptr<Component> instantiate_raw(const std::string &name) const;

        /**
         * @brief Get the registered name for a component type T.
         * @tparam T Concrete component type, must be registered.
         * @return The registered string name, or empty string if not registered.
         */
        template <typename T>
        std::string get_name() const
        {
            std::type_index id(typeid(T));

            auto it = type_names.find(id);
            if (it != type_names.end())
            {
                return it->second;
            }

            return {};
        }

        std::unordered_map<std::string, create_func> creators;
        std::unordered_map<std::type_index, std::string> type_names;
    };
}

#define REGISTER_COMPONENT(ComponentName, ComponentClassName) \
    namespace                                                 \
    {                                                         \
        const bool registrar_##ComponentName = []() {                                               \
            Engine::ComponentRegistry::get_instance().register_type<ComponentClassName>(                \
                #ComponentName                \
            );                                                                                      \
            return true; }();    \
    }
