#include "engine/serialization/json/json_serialization_context.h"

#include "engine/exceptions/json_key_not_found_exception.h"
#include "engine/exceptions/json_type_mismatch_exception.h"

#include "engine/data/guid.h"
#include "engine/serialization/json/json_value.h"
#include "engine/serialization/json/json_document.h"

#include <stdexcept>

namespace Engine::Serialization
{
    using Engine::Serialization::Json::JsonValue;

#pragma region Constructors

    JSONSerializationContext::JSONSerializationContext(Stage *stage)
        : stage(stage), reading(false), document()
    {
        node_stack.emplace_back(document.get_root());
    }

    JSONSerializationContext::JSONSerializationContext(Stage *stage, JsonDocument &document)
        : stage(stage), reading(true), document_ref(document)
    {
        node_stack.emplace_back(document.get_root());
    }

    JSONSerializationContext::~JSONSerializationContext() = default;

#pragma endregion

#pragma region Writing

    void JSONSerializationContext::write_bool(const std::string &key, bool value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_bool() on non-object parent");

        parent.set(key, static_cast<bool>(value));
    }

    void JSONSerializationContext::write_uint(const std::string &key, uint32_t value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_UInt() on non-object parent");

        parent.set(key, static_cast<int>(value));
    }

    void JSONSerializationContext::write_int(const std::string &key, int32_t value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_int() on non-object parent");

        parent.set(key, value);
    }

    void JSONSerializationContext::write_float(const std::string &key, float value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_float() on non-object parent");

        parent.set(key, value);
    }

    void JSONSerializationContext::write_double(const std::string &key, double value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_double() on non-object parent");

        parent.set(key, value);
    }

    void JSONSerializationContext::write_string(const std::string &key, const std::string &value)
    {
        auto &parent = node_stack.back();
        if (parent.is_empty())
            parent.create_empty_object(key);

        if (!parent.is_object())
            throw std::runtime_error("write_string() on non-object parent");

        parent.set(key, value);
    }

    void JSONSerializationContext::append_bool(bool value)
    {
        auto &parent = node_stack.back();
        if (!parent.is_array())
            throw std::runtime_error("write_bool() on non-array parent");

        parent.append(value);
    }

    void JSONSerializationContext::append_uint(uint32_t value)
    {
        auto &parent = node_stack.back();
        if (!parent.is_array())
            throw std::runtime_error("write_UInt() on non-array parent");

        parent.append(value);
    }

    void JSONSerializationContext::append_int(int value)
    {
        auto &parent = node_stack.back();

        if (!parent.is_array())
            throw std::runtime_error("write_int() on non-array parent");

        parent.append(value);
    }

    void JSONSerializationContext::append_float(float value)
    {
        auto &parent = node_stack.back();
        if (!parent.is_array())
            throw std::runtime_error("write_float() on non-array parent");

        parent.append(value);
    }

    void JSONSerializationContext::append_double(double value)
    {
        auto &parent = node_stack.back();
        if (!parent.is_array())
            throw std::runtime_error("write_double() on non-array parent");

        parent.append(value);
    }

    void JSONSerializationContext::append_string(const std::string &value)
    {
        auto &parent = node_stack.back();
        if (!parent.is_array())
            throw std::runtime_error("write_string() on non-array parent");

        parent.append(value);
    }

#pragma endregion

#pragma region Reading

    bool JSONSerializationContext::read_bool(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<bool> value_opt = parent.get(key).as<bool>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException(key, "bool");
    }

    uint32_t JSONSerializationContext::read_uint(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<int> value_opt = parent.get(key).as<int>();
        if (value_opt.has_value())
            return static_cast<uint32_t>(value_opt.value());
        else
            throw Exceptions::JsonTypeMismatchException(key, "uint");
    }

    int32_t JSONSerializationContext::read_int(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<int> value_opt = parent.get(key).as<int>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException(key, "int");
    }

    float JSONSerializationContext::read_float(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<float> value_opt = parent.get(key).as<float>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException(key, "float");
    }

    double JSONSerializationContext::read_double(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<double> value_opt = parent.get(key).as<double>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException(key, "double");
    }

    std::string JSONSerializationContext::read_string(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.has(key))
            throw Exceptions::JsonKeyNotFoundException(key);

        std::optional<std::string> value_opt = parent.get(key).as<std::string>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException(key, "string");
    }

    // Array reading
    bool JSONSerializationContext::read_bool_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<bool>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "bool");
    }

    uint32_t JSONSerializationContext::read_uint_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<int32_t>();
        if (value_opt.has_value())
            return static_cast<uint32_t>(value_opt.value());
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "uint");
    }

    int32_t JSONSerializationContext::read_int_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<int>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "int");
    }

    float JSONSerializationContext::read_float_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<float>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "float");
    }

    double JSONSerializationContext::read_double_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<double>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "double");
    }

    std::string JSONSerializationContext::read_string_at(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty() || !parent.is_array())
            throw std::runtime_error("Trying to read from an empty array");

        if (index >= parent.size())
            throw std::out_of_range("Array index out of bounds");

        auto value_opt = parent.get(index).as<std::string>();
        if (value_opt.has_value())
            return value_opt.value();
        else
            throw Exceptions::JsonTypeMismatchException("[" + std::to_string(index) + "]", "string");
    }

#pragma endregion

#pragma region Object and Array Scoping

    void JSONSerializationContext::begin_object_key(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.create_empty_object(key);

        else if (!parent.is_object())
            throw std::runtime_error("begin_object() on non-object parent");

        if (!parent.has(key) || !parent.get(key).is_object())
            parent.create_empty_object(key);

        node_stack.push_back(parent.get(key));
    }

    void JSONSerializationContext::begin_object_index(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.create_empty_array("");

        if (!parent.is_array())
            throw std::runtime_error("begin_object_index() on non-array parent");

        if (index >= parent.size())
            throw std::out_of_range("Index out of bounds of array");

        node_stack.push_back(parent.get(index));
    }

    void JSONSerializationContext::begin_object_push()
    {
        auto &parent = node_stack.back();

        if (!parent.is_array())
            throw std::runtime_error("begin_object() on non-array parent");

        parent.append(nlohmann::json::object());
        node_stack.push_back(parent.get(parent.size() - 1));
    }

    void JSONSerializationContext::end_object()
    {
        if (node_stack.size() <= 1 || !node_stack.back().is_object())
            throw std::runtime_error("end_object() called with no matching begin_object()");

        node_stack.pop_back();
    }

    size_t JSONSerializationContext::begin_array_key(const std::string &key)
    {
        auto &parent = node_stack.back();

        if (!parent.is_object())
            throw std::runtime_error("begin_array() on non-object parent");

        size_t size = 0;

        if (!parent.has(key) || !parent.get(key).is_array())
            parent.create_empty_array(key);
        else
            size = parent.get(key).size();

        node_stack.push_back(parent.get(key));
        return size;
    }

    size_t JSONSerializationContext::begin_array_index(const size_t index)
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.create_empty_array("");

        if (!parent.is_array())
            throw std::runtime_error("begin_array_index() on non-array parent");

        if (index >= parent.size())
            throw std::out_of_range("Index out of bounds of array");

        auto child = parent.get(index);

        node_stack.push_back(child);

        return child.size();
    }

    size_t JSONSerializationContext::begin_array_push()
    {
        auto &parent = node_stack.back();

        if (parent.is_empty())
            parent.create_empty_array("");

        if (!parent.is_array())
            throw std::runtime_error("begin_array() on non-array parent");

        parent.append(nlohmann::json::array());
        node_stack.push_back(parent.get(parent.size() - 1));

        return 0;
    }

    void JSONSerializationContext::end_array()
    {
        if (node_stack.size() <= 1 || !node_stack.back().is_array())
            throw std::runtime_error("end_array() called with no matching begin_array()");

        node_stack.pop_back();
    }

#pragma endregion

#pragma region Helpers

    JsonValue JSONSerializationContext::get_root()
    {
        return document_ref.get_root();
    }

    size_t JSONSerializationContext::size() const
    {
        const auto &curr = node_stack.back();
        return curr.size();
    }

    std::vector<std::string> JSONSerializationContext::get_keys() const
    {
        return get_current().get_keys();
    }

    bool JSONSerializationContext::remove(const std::string &key)
    {
        return get_current().remove(key);
    }

    bool JSONSerializationContext::has_key(const std::string &key) const
    {
        return get_current().has(key);
    }

    bool JSONSerializationContext::has_array(const std::string &key) const
    {
        return has_key(key) && get_current().get(key).is_array();
    }

    bool JSONSerializationContext::has_object(const std::string &key) const
    {
        return has_key(key) && get_current().get(key).is_object();
    }

    bool JSONSerializationContext::is_primitive() const
    {
        return !is_array() && !is_object();
    }

    bool JSONSerializationContext::is_array() const
    {
        return get_current().is_array();
    }

    bool JSONSerializationContext::is_object() const
    {
        return get_current().is_object();
    }

#pragma endregion

}
