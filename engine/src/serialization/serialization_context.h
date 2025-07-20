#pragma once

#include <string>
#include <cstdint>

namespace Engine
{
    class Stage;
    class GUID;
    struct EntityID;
    struct ComponentID;
    struct Serializable;

    /**
     * @brief Abstract interface for writing and reading fields by name.
     */
    class SerializationContext
    {
    public:
        virtual ~SerializationContext() = default;

        // --- Writing (Serialization) ---

        virtual void write_UInt(uint32_t value, const std::string &field) = 0;
        virtual void write_int(int32_t value, const std::string &field) = 0;
        virtual void write_float(float value, const std::string &field) = 0;
        virtual void write_string(const std::string &value, const std::string &field) = 0;
        virtual void write_guid(const GUID &value, const std::string &field) = 0;

        // --- Reading (Deserialization) ---

        virtual uint32_t read_UInt(const std::string &field) = 0;
        virtual int32_t read_int(const std::string &field) = 0;
        virtual float read_float(const std::string &field) = 0;
        virtual std::string read_string(const std::string &field) = 0;
        virtual GUID read_guid(const std::string &field) = 0;

        // --- Object and Array Scoping

        virtual void begin_object(const std::string &field) = 0;
        virtual void begin_object() = 0;
        virtual void end_object() = 0;

        virtual void begin_array(const std::string &field) = 0;
        virtual void begin_array() = 0;
        virtual void end_array() = 0;
        virtual size_t array_size() const = 0;

        virtual Stage *get_stage() = 0;

    private:
    protected:
        SerializationContext() = default;
    };
}