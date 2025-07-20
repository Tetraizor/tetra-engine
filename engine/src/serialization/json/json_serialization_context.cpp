#include "serialization/json/json_serialization_context.h"

#include "data/guid.h"
#include "serialization/json/json_utils.h"

namespace Engine
{
    using Engine::Serialization::Json::JsonValue;

    JSONSerializationContext::JSONSerializationContext(Stage *stage)
        : stage(stage), reading(false), document()
    {
        node_stack.emplace_back(document.root());
    }

    JSONSerializationContext::JSONSerializationContext(Stage *stage, JsonValue &&input)
        : stage(stage), reading(true), document()
    {
        node_stack.push_back(std::move(input));
    }

    JSONSerializationContext::~JSONSerializationContext() = default;

    Serialization::Json::JsonValue JSONSerializationContext::get_result()
    {
        return document.root();
    }

    // Writing
    void JSONSerializationContext::write_UInt(uint32_t value, const std::string &field)
    {
        auto &parent = node_stack.back();

        if (parent.is_null())
            parent.make_object();

        if (!parent.is_object())
            throw std::runtime_error("write_int() on non-object parent");

        parent.set(field, JsonValue());

        parent.get(field).set_int(static_cast<int>(value));
    }

    void JSONSerializationContext::write_int(int32_t value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_null())
            parent.make_object();

        if (!parent.is_object())
            throw std::runtime_error("write_int() on non-object parent");

        parent.set(field, JsonValue());
        parent.get(field).set_int(value);
    }

    void JSONSerializationContext::write_float(float value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_null())
            parent.make_object();

        if (!parent.is_object())
            throw std::runtime_error("write_float() on non-object parent");

        parent.set(field, JsonValue());
        parent.get(field).set_float(value);
    }

    void JSONSerializationContext::write_string(const std::string &value, const std::string &field)
    {
        auto &parent = node_stack.back();
        if (parent.is_null())
            parent.make_object();

        if (!parent.is_object())
            throw std::runtime_error("write_string() on non-object parent");

        parent.set(field, JsonValue());
        parent.get(field).set_string(value);
    }

    void JSONSerializationContext::write_guid(const GUID &value, const std::string &field)
    {
        write_string(value.to_string(), field);
    }

    // Reading
    uint32_t JSONSerializationContext::read_UInt(const std::string &field)
    {
        return static_cast<uint32_t>(node_stack.back().get(field).get_int().value());
    }

    int32_t JSONSerializationContext::read_int(const std::string &field)
    {
        return node_stack.back().get(field).get_int().value();
    }

    float JSONSerializationContext::read_float(const std::string &field)
    {
        return node_stack.back().get(field).get_float().value();
    }

    std::string JSONSerializationContext::read_string(const std::string &field)
    {
        return node_stack.back().get(field).get_string().value();
    }

    GUID JSONSerializationContext::read_guid(const std::string &field)
    {
        std::string guid_string = node_stack.back().get(field).get_string().value();
        return GUID::from_string(guid_string);
    }

    // Object and Array Scoping
    void JSONSerializationContext::begin_object(const std::string &field)
    {
        auto &parent = node_stack.back();

        if (parent.is_null())
            parent.make_object();

        parent.set(field, JsonValue::object());
        node_stack.push_back(parent.get(field));
    }

    void JSONSerializationContext::begin_object()
    {
        auto &parent = node_stack.back();

        if (parent.is_null())
            parent.make_array();

        parent.append(JsonValue::object());
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

        if (parent.is_null())
            parent.make_object();

        parent.set(field, JsonValue::array());
        node_stack.push_back(parent.get(field));
    }

    void JSONSerializationContext::begin_array()
    {
        auto &parent = node_stack.back();

        if (parent.is_null())
            parent.make_array();

        parent.append(JsonValue::array());
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