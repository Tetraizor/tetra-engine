#pragma once

#include "serialization/serialization_context.h"

#include "serialization/json/json_value.h"
#include "serialization/json/json_document.h"

#include "entity/entity_id.h"
#include "component/component_id.h"

#include <unordered_map>
#include <vector>

namespace Engine
{
    using Serialization::Json::JsonDocument;
    using Serialization::Json::JsonValue;

    class JSONSerializationContext : public SerializationContext
    {
    public:
        /**
         * @brief Construct for writing (serialization).
         * @param stage Pointer to the Stage being serialized.
         */
        JSONSerializationContext(Stage *stage);

        /**
         * @brief Construct for reading (deserialization).
         * @param stage Pointer to the Stage being deserialized into.
         * @param input The JSON object to read from.
         */
        JSONSerializationContext(Stage *stage, JsonDocument &document);

        ~JSONSerializationContext() override;

        /**
         * @brief After serialization, retrieve the JSON result.
         */
        Serialization::Json::JsonValue get_result();

        // --- Writing overrides ---
        void write_UInt(uint32_t value, const std::string &field) override;
        void write_int(int32_t value, const std::string &field) override;
        void write_float(float value, const std::string &field) override;
        void write_string(const std::string &value, const std::string &field) override;
        void write_guid(const GUID &value, const std::string &field) override;

        // --- Reading overrides ---
        uint32_t read_UInt(const std::string &field) override;
        int32_t read_int(const std::string &field) override;
        float read_float(const std::string &field) override;
        std::string read_string(const std::string &field) override;
        GUID read_guid(const std::string &field) override;

        // --- Object and Array Scoping ---
        void begin_object(const std::string &field) override;
        void begin_object() override;
        void end_object() override;

        void begin_array(const std::string &field) override;
        void begin_array() override;
        void end_array() override;
        size_t array_size() const override;

        template <typename T>
        void append(T item) { get_current().append<T>(item); }

        // --- Array Readers ---
        uint32_t read_UInt(const int index) override;
        int32_t read_int(const int index) override;
        float read_float(const int index) override;
        std::string read_string(const int index) override;
        GUID read_guid(const int index) override;

        // --- Helpers ---
        Stage *get_stage() override { return stage; }
        JsonValue get_current() const { return node_stack.back(); }
        JsonValue get_root() const { document.get_root(); }

    private:
        Stage *stage = nullptr;
        bool reading = false;

        JsonDocument &document_ref = document;
        JsonDocument document;

        std::vector<JsonValue> node_stack;

        std::unordered_map<EntityID, EntityID> entity_map;
        std::unordered_map<ComponentID, ComponentID> component_map;
    };
}
