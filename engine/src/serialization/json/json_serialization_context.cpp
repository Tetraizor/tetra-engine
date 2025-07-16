#include "utils/guid.h"
#include "entity/entity_id.h"
#include "component/component_id.h"
#include "serialization/json/json_serialization_context.h"

namespace Engine
{
    JSONSerializationContext::JSONSerializationContext(Stage *stage) : stage(stage), reading(false)
    {
        json = nlohmann::json::object();
        node_stack.push_back(&json);
    }

    JSONSerializationContext::JSONSerializationContext(Stage *stage, const nlohmann::json &input) : stage(stage), json(input), reading(true)
    {
        node_stack.push_back(&json);
    }

    nlohmann::json JSONSerializationContext::get_result() const
    {
        return json;
    }

    // Writing
    void JSONSerializationContext::write_UInt(uint32_t value, const std::string &field)
    {
        (*node_stack.back())[field] = value;
    }

    void JSONSerializationContext::write_int(int32_t value, const std::string &field)
    {
        (*node_stack.back())[field] = value;
    }

    void JSONSerializationContext::write_float(float value, const std::string &field)
    {
        (*node_stack.back())[field] = value;
    }

    void JSONSerializationContext::write_string(const std::string &value, const std::string &field)
    {
        (*node_stack.back())[field] = value;
    }

    void JSONSerializationContext::write_GUID(const GUID &value, const std::string &field)
    {
        (*node_stack.back())[field] = value.to_string();
    }

    // Reading
    uint32_t JSONSerializationContext::read_UInt(const std::string &field)
    {
        return node_stack.back()->at(field).get<uint32_t>();
    }

    int32_t JSONSerializationContext::read_int(const std::string &field)
    {
        return node_stack.back()->at(field).get<int32_t>();
    }

    float JSONSerializationContext::read_float(const std::string &field)
    {
        return node_stack.back()->at(field).get<float>();
    }

    std::string JSONSerializationContext::read_string(const std::string &field)
    {
        return node_stack.back()->at(field).get<std::string>();
    }

    GUID JSONSerializationContext::read_GUID(const std::string &field)
    {
        std::string s = node_stack.back()->at(field).get<std::string>();
        return GUID::from_string(s);
    }

    // Object and Array Scoping
    void JSONSerializationContext::begin_object(const std::string &field)
    {
        (*node_stack.back())[field] = nlohmann::json::object();
        node_stack.push_back(&((*node_stack.back())[field]));
    }

    void JSONSerializationContext::begin_object()
    {
        node_stack.back()->push_back(nlohmann::json::object());
        node_stack.push_back(&node_stack.back()->back());
    }

    void JSONSerializationContext::end_object()
    {
        if (node_stack.size() > 1)
        {
            node_stack.pop_back();
        }
        else
        {
            throw std::runtime_error("end_object() called with no matching begin_object()");
        }
    }

    void JSONSerializationContext::begin_array(const std::string &field)
    {
        (*node_stack.back())[field] = nlohmann::json::array();
        node_stack.push_back(&((*node_stack.back())[field]));
    }

    void JSONSerializationContext::begin_array()
    {
        node_stack.back()->push_back(nlohmann::json::object());
        node_stack.push_back(&node_stack.back()->back());
    }

    void JSONSerializationContext::end_array()
    {
        if (node_stack.size() > 1)
        {
            node_stack.pop_back();
        }
        else
        {
            throw std::runtime_error("end_array() called with no matching begin_array()");
        }
    }

    size_t JSONSerializationContext::array_size() const
    {
        if (node_stack.empty())
            return 0;

        const nlohmann::json *current = node_stack.back();
        if (current->is_array())
            return current->size();

        return 0;
    }

    // Helpers
    void JSONSerializationContext::register_entity(const EntityID &old_id, const EntityID &new_id)
    {
        entity_map[old_id] = new_id;
    }

    EntityID JSONSerializationContext::resolve_entity(const EntityID &old_id)
    {
        auto it = entity_map.find(old_id);
        if (it == entity_map.end())
        {
            throw std::runtime_error("Unregistered EntityID reference during deserialization!");
        }

        return it->second;
    }

    void JSONSerializationContext::register_component(const ComponentID &old_id, const ComponentID &new_id)
    {
        component_map[old_id] = new_id;
    }

    ComponentID JSONSerializationContext::resolve_component(const ComponentID &old_id)
    {
        auto it = component_map.find(old_id);
        if (it == component_map.end())
        {
            throw std::runtime_error("Unregistered ComponentID reference during deserialization!");
        }

        return it->second;
    }
}