#include "serialization/json/json_serialization_context.h"

#include "data/guid.h"
#include "serialization/json/json_value.h"
#include "serialization/json/json_document.h"

#include <stdexcept>

namespace Engine
{
    using Engine::Serialization::Json::JsonValue;

    JSONSerializationContext::JSONSerializationContext(Stage *stage)
        : stage(stage), reading(false), document(), read_root(document.get_root())
    {
    }

    JSONSerializationContext::JSONSerializationContext(Stage *stage, JsonValue input)
        : stage(stage), reading(true), document(), read_root(std::move(input))
    {
        node_stack.emplace_back(read_root);
    }

    JSONSerializationContext::~JSONSerializationContext() = default;

    JsonValue JSONSerializationContext::get_result()
    {
        return document.get_root();
    }

    // Writing
    void JSONSerializationContext::write_UInt(uint32_t value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.set_empty_object(field);

        if (!parent.is_object())
            throw std::runtime_error("write_UInt() on non-object parent");

        parent.set(field, static_cast<int>(value));
    }

    void JSONSerializationContext::write_int(int32_t value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.set_empty_object(field);

        if (!parent.is_object())
            throw std::runtime_error("write_int() on non-object parent");

        parent.set(field, value);
    }

    void JSONSerializationContext::write_float(float value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.set_empty_object(field);

        if (!parent.is_object())
            throw std::runtime_error("write_float() on non-object parent");

        parent.set(field, value);
    }

    void JSONSerializationContext::write_string(const std::string &value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.set_empty_object(field);

        if (!parent.is_object())
            throw std::runtime_error("write_string() on non-object parent");

        parent.set(field, value);
    }

    void JSONSerializationContext::write_guid(const GUID &value, const std::string &field)
    {
        write_string(value.to_string(), field);
    }

    // Reading
    uint32_t JSONSerializationContext::read_UInt(const std::string &field)
    {
        return static_cast<uint32_t>(node_stack.back().get(field).as<int>().value());
    }

    int32_t JSONSerializationContext::read_int(const std::string &field)
    {
        return node_stack.back().get(field).as<int>().value();
    }

    float JSONSerializationContext::read_float(const std::string &field)
    {
        return node_stack.back().get(field).as<float>().value();
    }

    std::string JSONSerializationContext::read_string(const std::string &field)
    {
        return node_stack.back().get(field).as<std::string>().value();
    }

    GUID JSONSerializationContext::read_guid(const std::string &field)
    {
        auto guid_string = node_stack.back().get(field).as<std::string>().value();
        return GUID::from_string(guid_string);
    }

    // Object and Array Scoping
    void JSONSerializationContext::begin_object(const std::string &field)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.set_empty_object(field);
        else if (!parent.is_object())
            throw std::runtime_error("begin_object() on non-object parent");

        parent.set_empty_object(field);
        node_stack.push_back(parent.get(field));
    }

    void JSONSerializationContext::begin_object()
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.set_empty_array(""); // or consider making empty array here

        if (!parent.is_array())
            throw std::runtime_error("begin_object() on non-array parent");

        parent.append(nlohmann::json::object());
        node_stack.push_back(parent.get(parent.size() - 1));
    }

    void JSONSerializationContext::end_object()
    {
        if (node_stack.size() <= 1)
            throw std::runtime_error("end_object() called with no matching begin_object()");

        node_stack.pop_back();
    }

    void JSONSerializationContext::begin_array(const std::string &field)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.set_empty_object(field);

        if (!parent.is_object())
            throw std::runtime_error("begin_array() on non-object parent");

        parent.set_empty_array(field);
        node_stack.push_back(parent.get(field));
    }

    void JSONSerializationContext::begin_array()
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.set_empty_array("");

        if (!parent.is_array())
            throw std::runtime_error("begin_array() on non-array parent");

        parent.append(nlohmann::json::array());
        node_stack.push_back(parent.get(parent.size() - 1));
    }

    void JSONSerializationContext::end_array()
    {
        if (node_stack.size() <= 1)
            throw std::runtime_error("end_array() called with no matching begin_array()");

        node_stack.pop_back();
    }

    size_t JSONSerializationContext::array_size() const
    {
        const auto &curr = node_stack.back();
        return curr.is_array() ? curr.size() : 0;
    }
}
