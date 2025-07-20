#pragma once

#include <string>
#include <optional>
#include <memory>

namespace Engine::Serialization::Json
{
    class JsonValue;

    class JsonDocument
    {
    public:
        JsonDocument();
        ~JsonDocument();

        void from_text(const std::string &json_text);
        std::string to_text(bool indent = false) const;

        JsonValue root();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_ptr;
    };

    class JsonValue
    {
    public:
        JsonValue();
        ~JsonValue();

        JsonValue(const JsonValue &) = delete;
        JsonValue &operator=(const JsonValue &) = delete;

        JsonValue(JsonValue &&) noexcept;
        JsonValue &operator=(JsonValue &&) noexcept;

        bool is_null() const;
        bool is_object() const;
        bool is_array() const;
        bool is_string() const;
        bool is_int() const;
        bool is_float() const;
        bool is_bool() const;

        // Object Access
        bool has_key(const std::string &key) const;
        JsonValue get(const std::string &key) const;
        void set(const std::string &key, const JsonValue &value);
        JsonValue operator[](const std::string &key) const;

        // Array Access
        size_t size() const;
        JsonValue get(size_t index) const;
        void append(const JsonValue &value);
        JsonValue operator[](size_t index) const;

        // Value Getters/Setters
        std::optional<std::string> get_string() const;
        void set_string(const std::string &value);

        std::optional<int> get_int() const;
        void set_int(int value);

        std::optional<float> get_float() const;
        void set_float(float value);

        std::optional<bool> get_bool() const;
        void set_bool(bool value);

        static JsonValue null();
        static JsonValue object();
        static JsonValue array();

        void make_object();
        void make_array();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_ptr;

        friend class JsonDocument;
        explicit JsonValue(Impl *impl);
    };
}
