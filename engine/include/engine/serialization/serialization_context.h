#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace Engine
{
    class Stage;
}

namespace Engine::Serialization
{
    class SerializationContext
    {
    public:
        virtual ~SerializationContext() = default;

        template <typename T>
        void write(const std::string &key, const T &value);

        template <typename T>
        void append(const T &value);

        template <typename T>
        T read(const std::string &key);

        template <typename T>
        T read_at(const size_t index);

        // --- Object and Array Scoping

        virtual void begin_object_key(const std::string &key) = 0;
        virtual void begin_object_index(const size_t index) = 0;
        virtual void begin_object_push() = 0;
        virtual void end_object() = 0;

        virtual size_t begin_array_key(const std::string &key) = 0;
        virtual size_t begin_array_index(const size_t index) = 0;
        virtual size_t begin_array_push() = 0;
        virtual void end_array() = 0;
        virtual size_t size() const = 0;

        virtual Engine::Stage *get_stage() = 0;

        virtual bool remove(const std::string &key) = 0;

        virtual bool has_key(const std::string &key) const = 0;
        virtual bool has_array(const std::string &key) const = 0;
        virtual bool has_object(const std::string &key) const = 0;

        virtual bool is_primitive() const = 0;
        virtual bool is_array() const = 0;
        virtual bool is_object() const = 0;

        virtual std::vector<std::string> get_keys() const = 0;

    private:
    protected:
        SerializationContext() = default;

        // --- Writing (Serialization) ---

        virtual void write_bool(const std::string &key, bool value) = 0;
        virtual void write_int(const std::string &key, int32_t value) = 0;
        virtual void write_uint(const std::string &key, uint32_t value) = 0;
        virtual void write_float(const std::string &key, float value) = 0;
        virtual void write_double(const std::string &key, double value) = 0;
        virtual void write_string(const std::string &key, const std::string &value) = 0;

        virtual void append_bool(bool value) = 0;
        virtual void append_int(int32_t value) = 0;
        virtual void append_uint(uint32_t value) = 0;
        virtual void append_float(float value) = 0;
        virtual void append_double(double value) = 0;
        virtual void append_string(const std::string &value) = 0;

        // --- Reading (Deserialization) ---

        virtual bool read_bool(const std::string &key) = 0;
        virtual int32_t read_int(const std::string &key) = 0;
        virtual uint32_t read_uint(const std::string &key) = 0;
        virtual float read_float(const std::string &key) = 0;
        virtual double read_double(const std::string &key) = 0;
        virtual std::string read_string(const std::string &key) = 0;

        virtual bool read_bool_at(const size_t index) = 0;
        virtual uint32_t read_uint_at(const size_t index) = 0;
        virtual int32_t read_int_at(const size_t index) = 0;
        virtual float read_float_at(const size_t index) = 0;
        virtual double read_double_at(const size_t index) = 0;
        virtual std::string read_string_at(const size_t index) = 0;

        template <typename T, typename Enable>
        friend struct Serializer;
    };
}

#include "serialization/serialization_context.inl"