#include "engine/serialization/json/json_value.h"

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

    bool JsonValue::has(const std::string &key) const
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        return json_ptr->is_object() && json_ptr->contains(key);
    }

    bool JsonValue::remove(const std::string &key)
    {
        if (!json_ptr || !json_ptr->is_object())
            return false;

        return json_ptr->erase(key) > 0;
    }

    JsonValue JsonValue::set(const std::string &key, const JsonValue &value)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            *json_ptr = nlohmann::json::object();

        if (!value.json_ptr)
            (*json_ptr)[key] = nullptr;
        else
            (*json_ptr)[key] = *value.json_ptr;

        return JsonValue(&(*json_ptr)[key]);
    }

    JsonValue JsonValue::create_empty_object(const std::string &key)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            *json_ptr = nlohmann::json::object();

        (*json_ptr)[key] = nlohmann::json::object();
        return JsonValue(&(*json_ptr)[key]);
    }

    JsonValue JsonValue::create_empty_object()
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_array())
            throw std::runtime_error("Trying to create empty object on non-array JsonValue");

        json_ptr->push_back(nlohmann::json::object());
        return JsonValue(&(*json_ptr).back());
    }

    JsonValue JsonValue::create_empty_array(const std::string &key)
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_object())
            throw std::runtime_error("Trying to create empty array key on non-object JsonValue");

        (*json_ptr)[key] = nlohmann::json::array();
        return JsonValue(&(*json_ptr)[key]);
    }

    JsonValue JsonValue::create_empty_array()
    {
        if (!json_ptr)
            throw std::runtime_error("Trying to access null JsonValue");

        if (!json_ptr->is_array())
            throw std::runtime_error("Trying to create empty array on non-array JsonValue");

        json_ptr->push_back(nlohmann::json::array());
        return JsonValue(&(*json_ptr).back());
    }

    JsonValue JsonValue::get(const std::string &key) const
    {
        if (!json_ptr || !json_ptr->is_object() || json_ptr->find(key) == json_ptr->end())
            throw std::runtime_error("Trying to get a key that does not exist!");

        return JsonValue(&(*json_ptr)[key]);
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

    std::vector<std::string> JsonValue::get_keys() const
    {
        if (!json_ptr->is_object())
            return {};

        std::vector<std::string> keys;

        for (auto it = json_ptr->begin(); it != json_ptr->end(); ++it)
        {
            keys.push_back(it.key());
        }

        return keys;
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

    std::string JsonValue::to_text(bool pretty) const
    {
        return json_ptr->dump(pretty ? 4 : -1);
    }
}
