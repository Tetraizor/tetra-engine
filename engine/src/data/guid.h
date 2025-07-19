#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>

namespace Engine
{
    class GUID
    {
    public:
        static constexpr size_t SIZE = 16;

        GUID()
        {
            data.fill(0);
        }

        static GUID generate()
        {
            GUID guid;

            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);

            for (size_t i = 0; i < SIZE; i += 4)
            {
                uint32_t part = dis(gen);
                guid.data[i] = static_cast<uint8_t>((part >> 24) & 0xFF);
                guid.data[i + 1] = static_cast<uint8_t>((part >> 16) & 0xFF);
                guid.data[i + 2] = static_cast<uint8_t>((part >> 8) & 0xFF);
                guid.data[i + 3] = static_cast<uint8_t>((part) & 0xFF);
            }

            return guid;
        }

        bool operator==(const GUID &other) const
        {
            return data == other.data;
        }

        bool operator!=(const GUID &other) const
        {
            return !(*this == other);
        }

        std::string to_string() const
        {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');

            for (size_t i = 0; i < SIZE; i++)
            {
                ss << std::setw(2) << static_cast<int>(data[i]);
                if (i == 3 || i == 5 || i == 7 || i == 9)
                    ss << "-";
            }

            return ss.str();
        }

        static GUID from_string(const std::string &str)
        {
            GUID guid;

            size_t str_index = 0;
            size_t data_index = 0;

            while (data_index < SIZE && str_index < str.size())
            {
                if (str[str_index] == '-')
                {
                    ++str_index;
                    continue;
                }

                uint8_t high = hex_char_to_byte(str[str_index]);
                uint8_t low = hex_char_to_byte(str[str_index + 1]);

                guid.data[data_index++] = (high << 4) | low;
                str_index += 2;
            }

            return guid;
        }

        struct Hasher
        {
            size_t operator()(const GUID &guid) const noexcept
            {
                const uint64_t *p = reinterpret_cast<const uint64_t *>(guid.data.data());
                size_t h1 = std::hash<uint64_t>{}(p[0]);
                size_t h2 = std::hash<uint64_t>{}(p[1]);
                return h1 ^ (h2 << 1);
            }
        };

    private:
        std::array<uint8_t, SIZE>
            data;

        static uint8_t hex_char_to_byte(char c)
        {
            if ('0' <= c && c <= '9')
                return c - '0';
            if ('a' <= c && c <= 'f')
                return 10 + (c - 'a');
            if ('A' <= c && c <= 'F')
                return 10 + (c - 'A');
            return 0;
        }
    };
}