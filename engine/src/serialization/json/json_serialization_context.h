#pragma once

#include <nlohmann/json.hpp>
#include <unordered_map>
#include "serialization/serialization_context.h"

namespace Engine
{
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
        JSONSerializationContext(Stage *stage, const nlohmann::json &input);

        /**
         * @brief After serialization, retrieve the JSON document.
         */
        nlohmann::json get_result() const;

        // --- Writing overrides ---

        void write_UInt(uint32_t value, const std::string &field) override;
        void write_int(int32_t value, const std::string &field) override;
        void write_float(float value, const std::string &field) override;
        void write_string(const std::string &value, const std::string &field) override;
        void write_GUID(const GUID &value, const std::string &field) override;

        // --- Reading Overrides ---

        uint32_t read_UInt(const std::string &field) override;
        int32_t read_int(const std::string &field) override;
        float read_float(const std::string &field) override;
        std::string read_string(const std::string &field) override;
        GUID read_GUID(const std::string &field) override;

        // --- Object and Array Scoping

        void begin_object(const std::string &field) override;
        void begin_object() override;
        void end_object() override;

        void begin_array(const std::string &field) override;
        void begin_array() override;
        void end_array() override;
        size_t array_size() const override;

        // --- Helpers ---

        virtual void register_entity(const EntityID &old_id, const EntityID &new_id) override;
        virtual EntityID resolve_entity(const EntityID &old_id) override;

        virtual void register_component(const ComponentID &old_id, const ComponentID &new_id) override;
        virtual ComponentID resolve_component(const ComponentID &old_id) override;

        virtual Stage *get_stage() override { return stage; }

    private:
        Stage *stage;
        nlohmann::json json;
        bool reading = false;

        std::unordered_map<EntityID, EntityID> entity_map;
        std::unordered_map<ComponentID, ComponentID> component_map;

        std::vector<nlohmann::json *> node_stack;

    protected:
    };
}