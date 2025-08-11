#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <optional>
#include <memory>

namespace Engine::Serialization::Json
{
    class JsonValue
    {
        friend class JsonDocument;

    public:
        JsonValue(const JsonValue &) = default;
        JsonValue &operator=(const JsonValue &) = default;

        ~JsonValue() = default;

        bool is_object() const
        {
            if (!json_ptr)
                throw std::runtime_error("Trying to access null JsonValue");

            return json_ptr->is_object();
        }

        bool is_array() const
        {
            if (!json_ptr)
                throw std::runtime_error("Trying to access null JsonValue");

            return json_ptr->is_array();
        }

        bool has(const std::string &field) const;

        JsonValue set(const std::string &field, const JsonValue &value);

        template <typename T>
        JsonValue set(const std::string &field, T &&value)
        {
            if (!json_ptr)
                throw std::runtime_error("Trying to access null JsonValue");

            if (!json_ptr->is_object())
                *json_ptr = nlohmann::json::object();

            if constexpr (std::is_same_v<std::decay_t<T>, JsonValue>)
            {
                (*json_ptr)[field] = *(value.json_ptr);
            }
            else
            {
                (*json_ptr)[field] = std::forward<T>(value);
            }

            return JsonValue(&(*json_ptr)[field]);
        }

        bool is_empty() const;

        JsonValue create_empty_object(const std::string &field);
        JsonValue create_empty_object();

        JsonValue create_empty_array(const std::string &field);
        JsonValue create_empty_array();

        JsonValue get(const std::string &field) const;
        JsonValue get(const int index) const;

        template <typename T>
        std::optional<T> as() const
        {
            if (!json_ptr)
                throw std::runtime_error("Trying to access null JsonValue");

            try
            {
                return json_ptr->get<T>();
            }
            catch (const nlohmann::json::exception &)
            {
                return std::nullopt;
            }
        }

        /**
         * @brief Returns the size of the JSON array/object.
         *
         * @return The number of elements in the JSON array/object.
         * @throws std::runtime_error If the underlying JSON pointer is null.
         */
        size_t size() const;

        /**
         * @brief Accesses the element at the specified index in a JSON array.
         *
         * This operator returns a new JsonValue referencing the element at the given index
         * within the underlying JSON array. If the current JsonValue does not reference a valid
         * JSON array, or if the index is out of bounds, an exception is thrown.
         *
         * @param index The zero-based index of the element to access.
         * @return JsonValue A new JsonValue referencing the element at the specified index.
         *
         * @throws std::runtime_error If the current JsonValue is null or not an array.
         * @throws std::out_of_range If the index is out of bounds.
         */
        JsonValue operator[](size_t index) const;

        template <typename T>
        size_t append(T item)
        {
            if (!json_ptr)
                throw std::runtime_error("Trying to access null JsonValue");

            if (!json_ptr->is_array())
                throw std::runtime_error("JsonValue is not an array");

            json_ptr->push_back(item);

            return json_ptr->array().size();
        }

        operator bool() const
        {
            return json_ptr != nullptr;
        }

        std::string to_text(bool pretty = true) const;

        std::vector<std::string> get_keys() const;

        void clear();
        bool remove(const std::string &field);

    private:
        JsonValue(nlohmann::json *json_ptr);
        nlohmann::json *json_ptr = nullptr;
    };
}
