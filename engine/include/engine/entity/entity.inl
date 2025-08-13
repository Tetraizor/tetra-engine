#include "entity.h"
#include "engine/component/component_manager.h"
#include "engine/stage/stage_manager.h"

namespace Engine
{
    template <typename T>
    T *Entity::add_component()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        try
        {
            auto &stage_manager = StageManager::get_instance();
            Stage *stage = stage_manager.get_current_stage();
            if (stage == nullptr)
                throw std::runtime_error("Current scene is null");

            ComponentManager &component_manager = stage->get_component_manager();

            std::shared_ptr<Component> component_ptr = component_manager.create_component<T>(id);
            Component *ptr = component_ptr.get();

            components.push_back(component_ptr);
            return static_cast<T *>(ptr);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("Failed to add component: ") + e.what());
        }
    }

    template <typename T>
    bool Entity::has_component() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        return get_component<T>() != nullptr;
    }

    template <typename T>
    std::vector<T *> Entity::get_all_components_of_type() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        std::vector<T *> found_components;

        for (const auto &comp : components)
        {
            if (auto ptr = dynamic_cast<T *>(comp.get()))
                found_components.push_back(ptr);
        }

        return found_components;
    }

    template <typename T>
    T *Entity::get_component() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        for (const auto &comp : components)
        {
            if (auto ptr = dynamic_cast<T *>(comp.get()))
                return ptr;
        }

        return nullptr;
    }
}