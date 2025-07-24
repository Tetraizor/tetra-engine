#include "serialization/json/json_value.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <cassert>

namespace Engine::Serialization::Json
{
    size_t JsonValue::size() const
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        return json_ptr->size();
    }

    bool JsonValue::has(const std::string &field) const
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        return json_ptr->is_object() && json_ptr->contains(field);
    }

    bool JsonValue::remove(const std::string &field)
    {
        if (!json_ptr || !json_ptr->is_object())
            return false;

        return json_ptr->erase(field) > 0;
    }

    JsonValue JsonValue::set(const std::string &field, const JsonValue &value)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            *json_ptr = nlohmann::json::object();

        if (!value.json_ptr)
            (*json_ptr)[field] = nullptr;
        else
            (*json_ptr)[field] = *value.json_ptr;

        return JsonValue(&(*json_ptr)[field]);
    }

    JsonValue JsonValue::set_empty_object(const std::string &field)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            *json_ptr = nlohmann::json::object();

        (*json_ptr)[field] = nlohmann::json::object();
        return JsonValue(&(*json_ptr)[field]);
    }

    JsonValue JsonValue::set_empty_array(const std::string &field)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            *json_ptr = nlohmann::json::object();

        (*json_ptr)[field] = nlohmann::json::array();
        return JsonValue(&(*json_ptr)[field]);
    }

    JsonValue JsonValue::get(const std::string &field) const
    {
        if (!json_ptr || !json_ptr->is_object() || json_ptr->find(field) == json_ptr->end())
            throw std::runtime_error("Trying to get a field that does not exist!");

        return JsonValue(&(*json_ptr)[field]);
    }

    JsonValue JsonValue::get(int index) const
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to get an element on null JsonValue");

        if (!json_ptr->is_array())
            throw std::runtime_error("Trying to get an element on a non-array JsonValue");

        if (index < 0 || static_cast<size_t>(index) >= json_ptr->size())
            throw std::out_of_range("Index out of bounds");

        return JsonValue(&(*json_ptr)[index]);
    }

    JsonValue JsonValue::operator[](size_t index) const
    {
        if (!json_ptr || !json_ptr->is_array())
            throw std::runtime_error("Trying to access null JsonValue");

        if (index >= json_ptr->array().size())
            throw std::out_of_range("Index out of bounds");

        return JsonValue(&(*json_ptr)[index]);
    }

    bool JsonValue::is_empty() const
    {
        if (!json_ptr)
            return true;

        return json_ptr->empty();
    }

    void JsonValue::clear()
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to clear a null JsonValue");

        json_ptr->clear();
    }
}