#include "json_utils.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace Engine::Serialization::Json
{
    struct JsonDocument::Impl
    {
        nlohmann::json root_json;
    };

    struct JsonValue::Impl
    {
        std::shared_ptr<nlohmann::json> node;

        Impl() = default;
        explicit Impl(std::shared_ptr<nlohmann::json> n) : node(std::move(n)) {}
    };

    JsonDocument::JsonDocument() : impl_ptr(std::make_unique<Impl>()) {}
    JsonDocument::~JsonDocument() = default;

    void JsonDocument::from_text(const std::string &json_text)
    {
        try
        {
            impl_ptr->root_json = nlohmann::json::parse(json_text);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("JSON parse error: ") + e.what());
        }
    }

    std::string JsonDocument::to_text(bool indent) const
    {
        if (indent)
        {
            return impl_ptr->root_json.dump(4);
        }
        else
        {
            return impl_ptr->root_json.dump();
        }
    }

    JsonValue JsonDocument::root()
    {
        std::shared_ptr<nlohmann::json> ptr(&impl_ptr->root_json, [](nlohmann::json *) {});
        return JsonValue(new JsonValue::Impl(ptr));
    }

    JsonValue::JsonValue() : impl_ptr(std::make_unique<Impl>(nullptr)) {}
    JsonValue::JsonValue(Impl *impl) : impl_ptr(std::unique_ptr<Impl>(impl)) {}
    JsonValue::~JsonValue() = default;

    bool JsonValue::is_null() const
    {
        return impl_ptr->node == nullptr || impl_ptr->node->is_null();
    }

    bool JsonValue::is_object() const
    {
        return impl_ptr->node && impl_ptr->node->is_object();
    }

    bool JsonValue::is_array() const
    {
        return impl_ptr->node && impl_ptr->node->is_array();
    }

    bool JsonValue::is_string() const
    {
        return impl_ptr->node && impl_ptr->node->is_string();
    }

    bool JsonValue::is_int() const
    {
        if (!impl_ptr->node)
            return false;

        return impl_ptr->node->is_number_integer() || impl_ptr->node->is_number_unsigned();
    }

    bool JsonValue::is_float() const
    {
        return impl_ptr->node && impl_ptr->node->is_number_float();
    }

    bool JsonValue::is_bool() const
    {
        return impl_ptr->node && impl_ptr->node->is_boolean();
    }

    bool JsonValue::has_key(const std::string &key) const
    {
        if (!is_object())
            return false;
        return impl_ptr->node->contains(key);
    }

    JsonValue JsonValue::get(const std::string &key) const
    {
        if (!is_object() || !has_key(key))
            return JsonValue::null();

        nlohmann::json *child_raw = &(*impl_ptr->node)[key];

        std::shared_ptr<nlohmann::json> child_ptr(impl_ptr->node, child_raw);

        return JsonValue(new Impl(std::move(child_ptr)));
    }

    void JsonValue::set(const std::string &key, const JsonValue &value)
    {
        if (!is_object())
        {
            make_object();
        }

        (*impl_ptr->node)[key] = *(value.impl_ptr->node);
    }

    size_t JsonValue::size() const
    {
        if (!is_array())
            return 0;
        return impl_ptr->node->size();
    }

    JsonValue JsonValue::get(size_t index) const
    {
        if (!is_array() || index >= size())
            return JsonValue::null();

        nlohmann::json *child_raw = &(*impl_ptr->node)[index];

        std::shared_ptr<nlohmann::json> child_ptr(
            impl_ptr->node,
            child_raw);

        return JsonValue(new Impl(std::move(child_ptr)));
    }

    void JsonValue::append(const JsonValue &value)
    {
        if (!is_array())
        {
            make_array();
        }

        impl_ptr->node->push_back(*(value.impl_ptr->node));
    }

    std::optional<std::string> JsonValue::get_string() const
    {
        if (!is_string())
            return std::nullopt;
        return impl_ptr->node->get<std::string>();
    }

    void JsonValue::set_string(const std::string &value)
    {
        *impl_ptr->node = value;
    }

    std::optional<int> JsonValue::get_int() const
    {
        if (!is_int())
            return std::nullopt;
        return impl_ptr->node->get<int>();
    }

    void JsonValue::set_int(int value)
    {
        *impl_ptr->node = value;
    }

    std::optional<float> JsonValue::get_float() const
    {
        if (!is_float())
            return std::nullopt;
        return impl_ptr->node->get<float>();
    }

    void JsonValue::set_float(float value)
    {
        *impl_ptr->node = value;
    }

    std::optional<bool> JsonValue::get_bool() const
    {
        if (!is_bool())
            return std::nullopt;
        return impl_ptr->node->get<bool>();
    }

    void JsonValue::set_bool(bool value)
    {
        *impl_ptr->node = value;
    }

    JsonValue JsonValue::null()
    {
        return JsonValue(new Impl(nullptr));
    }

    JsonValue JsonValue::object()
    {
        auto obj_ptr = std::make_shared<nlohmann::json>(nlohmann::json::object());
        return JsonValue(new Impl(std::move(obj_ptr)));
    }

    JsonValue JsonValue::array()
    {
        auto arr_ptr = std::make_shared<nlohmann::json>(nlohmann::json::array());
        return JsonValue(new Impl(std::move(arr_ptr)));
    }

    void JsonValue::make_object()
    {
        if (!impl_ptr->node)
        {
            impl_ptr->node = std::make_shared<nlohmann::json>(nlohmann::json::object());
        }
        else
        {
            *(impl_ptr->node) = nlohmann::json::object();
        }
    }

    void JsonValue::make_array()
    {
        if (!impl_ptr->node)
        {
            impl_ptr->node = std::make_shared<nlohmann::json>(nlohmann::json::array());
        }
        else
        {
            *(impl_ptr->node) = nlohmann::json::array();
        }
    }

    JsonValue JsonValue::operator[](const std::string &key) const
    {
        return get(key);
    }

    JsonValue JsonValue::operator[](size_t index) const
    {
        return get(index);
    }
}