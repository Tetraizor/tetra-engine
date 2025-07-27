#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <cassert>

namespace Engine::Serialization
{
    class SerializationContext;

    template <typename T>
    constexpr void ensure_type_valid_for_deserialization()
    {
        static_assert(!std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char>,
                      "Cannot deserialize into `const char*` or `char*`. Use `std::string` instead.");
    }

    template <typename T, typename Enable = void>
    struct Serializer;

    template <typename T>
    struct Serializer<T, void>
    {
        static void write_to_ctx(SerializationContext &, const std::string &, const T &)
        {
            static_assert(sizeof(T) == 0, "No Serializer defined for this type");
        }
        static void append_to_ctx(SerializationContext &, const T &)
        {
            static_assert(sizeof(T) == 0, "No Serializer defined for this type");
        }
        static void read_from_ctx(SerializationContext &, const std::string &, T &)
        {
            static_assert(sizeof(T) == 0, "No Serializer defined for this type");
        }
        static void read_at_from_ctx(SerializationContext &, const size_t, T &)
        {
            static_assert(sizeof(T) == 0, "No Serializer defined for this type");
        }
    };

#pragma region Generic write/read interface

    template <typename T>
    void write_to_ctx(SerializationContext &ctx, const std::string &key, const T &value)
    {
        Serializer<T>::write_to_ctx(ctx, key, value);
    }

    template <typename T>
    void append_to_ctx(SerializationContext &ctx, const T &value)
    {
        Serializer<T>::append_to_ctx(ctx, value);
    }

    template <typename T>
    void read_from_ctx(SerializationContext &ctx, const std::string &key, T &value)
    {
        ensure_type_valid_for_deserialization<T>();

        Serializer<T>::read_from_ctx(ctx, key, value);
    }

    template <typename T>
    void read_at_from_ctx(SerializationContext &ctx, const size_t index, T &value)
    {
        ensure_type_valid_for_deserialization<T>();

        Serializer<T>::read_at_from_ctx(ctx, index, value);
    }

#pragma endregion

#pragma region Primitive specializations

    template <>
    struct Serializer<bool>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, bool value)
        {
            ctx.write_bool(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, bool value)
        {
            ctx.append_bool(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, bool &value)
        {
            value = ctx.read_bool(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, bool &value)
        {
            value = ctx.read_bool_at(index);
        }
    };

    template <>
    struct Serializer<int>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, int value)
        {
            ctx.write_int(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, int value)
        {
            ctx.append_int(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, int &value)
        {
            value = ctx.read_int(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, int &value)
        {
            value = ctx.read_int_at(index);
        }
    };

    template <>
    struct Serializer<uint32_t>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, uint32_t value)
        {
            ctx.write_uint(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, uint32_t value)
        {
            ctx.append_uint(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, uint32_t &value)
        {
            value = ctx.read_uint(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, uint32_t &value)
        {
            value = ctx.read_uint_at(index);
        }
    };

    template <>
    struct Serializer<float>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, float value)
        {
            ctx.write_float(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, float value)
        {
            ctx.append_float(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, float &value)
        {
            value = ctx.read_float(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, float &value)
        {
            value = ctx.read_float_at(index);
        }
    };

    template <>
    struct Serializer<double>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, double value)
        {
            ctx.write_double(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, double value)
        {
            ctx.append_double(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, double &value)
        {
            value = ctx.read_double(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, double &value)
        {
            value = ctx.read_double_at(index);
        }
    };

    template <>
    struct Serializer<std::string>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, const std::string &value)
        {
            ctx.write_string(key, value);
        }
        static void append_to_ctx(SerializationContext &ctx, const std::string &value)
        {
            ctx.append_string(value);
        }
        static void read_from_ctx(SerializationContext &ctx, const std::string &key, std::string &value)
        {
            value = ctx.read_string(key);
        }
        static void read_at_from_ctx(SerializationContext &ctx, size_t index, std::string &value)
        {
            value = ctx.read_string_at(index);
        }
    };

    template <typename T>
    struct dependent_false : std::false_type
    {
    };

    template <size_t N>
    struct Serializer<char[N]>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, const char (&value)[N])
        {
            ctx.write_string(key, std::string(value));
        }

        static void append_to_ctx(SerializationContext &ctx, const char (&value)[N])
        {
            ctx.append_string(std::string(value));
        }
    };

    template <>
    struct Serializer<const char *>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, const char *value)
        {
            ctx.write(key, std::string(value));
        }

        static void append_to_ctx(SerializationContext &ctx, const char *value)
        {
            ctx.append(std::string(value));
        }

        static void read_from_ctx(SerializationContext &, const std::string &, const char *&) = delete;
        static void read_at_from_ctx(SerializationContext &, size_t, const char *&) = delete;
    };

#pragma endregion

#pragma region Container specializations

    template <typename T>
    struct Serializer<std::vector<T>>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, const std::vector<T> &vec)
        {
            ctx.begin_array_key(key);
            for (const auto &elem : vec)
                Serializer<T>::append_to_ctx(ctx, elem);
            ctx.end_array();
        }

        static void append_to_ctx(SerializationContext &ctx, const std::vector<T> &vec)
        {
            ctx.begin_array_push();
            for (const auto &elem : vec)
                Serializer<T>::append_to_ctx(ctx, elem);
            ctx.end_array();
        }

        static void read_from_ctx(SerializationContext &ctx, const std::string &key, std::vector<T> &vec)
        {
            size_t count = ctx.begin_array_key(key);

            vec.clear();
            vec.reserve(count);

            for (size_t i = 0; i < count; ++i)
            {
                T value{};
                Serializer<T>::read_at_from_ctx(ctx, i, value);
                vec.push_back(std::move(value));
            }

            ctx.end_array();
        }

        static void read_at_from_ctx(SerializationContext &ctx, const size_t index, std::vector<T> &vec)
        {
            size_t count = ctx.begin_array_index(index);

            vec.clear();
            vec.reserve(count);

            for (size_t i = 0; i < count; ++i)
            {
                T value{};
                Serializer<T>::read_at_from_ctx(ctx, i, value);
                vec.push_back(std::move(value));
            }

            ctx.end_array();
        }
    };

    template <typename K, typename V>
    struct Serializer<std::unordered_map<K, V>>
    {
        static void write_to_ctx(SerializationContext &ctx, const std::string &key, const std::unordered_map<K, V> &map)
        {
            ctx.begin_object_key(key);
            for (const auto &pair : map)
            {
                std::string key_str = serialize_key(pair.first);
                Serializer<V>::write_to_ctx(ctx, key_str, pair.second);
            }
            ctx.end_object();
        }

        static void append_to_ctx(SerializationContext &ctx, const std::unordered_map<K, V> &map)
        {
            ctx.begin_object_push();

            for (const auto &pair : map)
            {
                std::string key_str = serialize_key(pair.first);
                Serializer<V>::write_to_ctx(ctx, key_str, pair.second);
            }

            ctx.end_object();
        }

        static void read_from_ctx(SerializationContext &ctx, const std::string &key, std::unordered_map<K, V> &map)
        {
            ctx.begin_object_key(key);

            map.clear();
            std::vector<std::string> keys = ctx.get_keys();
            for (const std::string &key : keys)
            {
                K typed_key = deserialize_key<K>(key);
                V value;
                Serializer<V>::read_from_ctx(ctx, key, value);
                map[typed_key] = std::move(value);
            }

            ctx.end_object();
        }

        static void read_at_from_ctx(SerializationContext &ctx, const size_t index, std::unordered_map<K, V> &map)
        {
            ctx.begin_object_index(index);

            map.clear();
            std::vector<std::string> keys = ctx.get_keys();
            for (const std::string &key : keys)
            {
                K typed_key = deserialize_key<K>(key);
                V value;
                Serializer<V>::read_from_ctx(ctx, key, value);
                map[typed_key] = std::move(value);
            }

            ctx.end_object();
        }
    };

#pragma endregion

#pragma region Helper key conversion

    template <typename K>
    std::string serialize_key(const K &key) { return std::to_string(key); }

    template <>
    inline std::string serialize_key<std::string>(const std::string &key) { return key; }

    template <typename K>
    K deserialize_key(const std::string &str);

    template <>
    inline int deserialize_key<int>(const std::string &str) { return std::stoi(str); }

    template <>
    inline std::string deserialize_key<std::string>(const std::string &str) { return str; }

#pragma endregion
}
