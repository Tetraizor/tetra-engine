#pragma once

#include "engine/serialization/serialization_context.h"

#include "engine/serialization/json/json_value.h"
#include "engine/serialization/json/json_document.h"

#include "engine/entity/entity_id.h"
#include "engine/component/component_id.h"

#include <unordered_map>
#include <vector>

namespace Engine
{
    class Stage;
}

namespace Engine::Serialization
{
    using Json::JsonDocument;
    using Json::JsonValue;

    /**
     * @brief Abstract interface for writing and reading key-based serialized data.
     *
     * This context supports structured data like JSON: nested objects, arrays, and primitive values.
     * It uses a stack-based approach where `begin_*` enters into a nested scope and `end_*` exits it.
     */
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

        // --- Object and Array Scoping ---

        /**
         * @brief Begin writing/reading an object at the specified key.
         *
         * If the key does not exist, a new object is created and entered.
         * If it exists and is an object, enters it.
         * Throws if the key exists but is not an object.
         */
        void begin_object_key(const std::string &key) override;

        /**
         * @brief Begin writing/reading an object at a given index in the current array.
         *
         * Current context must be an array.
         * Throws if the index is out of bounds or does not point to an object.
         */
        void begin_object_index(const size_t index) override;

        /**
         * @brief Appends a new object to the current array and enters it.
         *
         * Current context must be an array.
         * A new object is added to the array and made current.
         * Throws if the parent is not an array.
         */
        void begin_object_push() override;

        /**
         * @brief Exit from the current object scope.
         *
         * Reverts to the previous context in the stack.
         */
        void end_object() override;

        /**
         * @brief Begin writing/reading an array at the specified key.
         *
         * If the key does not exist, a new array is created and entered.
         * If it exists and is an array, enters it.
         * Throws if the key exists but is not an array.
         *
         * @return The number of elements in the array.
         */
        size_t begin_array_key(const std::string &key) override;

        /**
         * @brief Enter an array value at the given index.
         *
         * Current context must be an array.
         * Throws if the index is out of bounds or invalid.
         *
         * @return The number of elements in the selected array (if nested).
         */
        size_t begin_array_index(const size_t index) override;

        /**
         * @brief Appends a new array to the current array and enters it.
         *
         * Current context must be an array.
         * Adds a new empty array element and enters it.
         *
         * @return The size of the newly added array (initially zero).
         */
        size_t begin_array_push() override;

        /**
         * @brief Exit from the current array scope.
         */
        void end_array() override;

        /**
         * @brief Get the number of elements in the current array/object.
         */
        size_t size() const override;

        // --- Helpers ---

        /**
         * @brief Get the stage associated with this serialization context.
         */
        Stage *get_stage() override { return stage; }

        JsonValue get_current() const { return node_stack.back(); }

        JsonValue get_root() const { return document.get_root(); }

        std::vector<std::string> get_keys() const override;

        void clear() { get_current().clear(); }

        /**
         * @brief Remove the value at the given key, if it exists.
         *
         * Works for objects only. Does nothing if key is not present.
         *
         * @return true if the key was found and removed.
         */
        bool remove(const std::string &key) override;

        bool has_key(const std::string &key) const override;
        bool has_array(const std::string &key) const override;
        bool has_object(const std::string &key) const override;

        bool is_primitive() const override;
        bool is_array() const override;
        bool is_object() const override;

        /**
         * @brief After serialization, retrieve the JSON result.
         */
        Serialization::Json::JsonValue get_root();

    protected:
        // --- Writing overrides ---

        void write_bool(const std::string &key, bool value) override;
        void write_int(const std::string &key, int32_t value) override;
        void write_uint(const std::string &key, uint32_t value) override;
        void write_float(const std::string &key, float value) override;
        void write_double(const std::string &key, double value) override;
        void write_string(const std::string &key, const std::string &value) override;

        // Array writing
        void append_bool(bool value) override;
        void append_int(int32_t value) override;
        void append_uint(uint32_t value) override;
        void append_float(float value) override;
        void append_double(double value) override;
        void append_string(const std::string &value) override;

        // --- Reading Overrides ---

        bool read_bool(const std::string &key) override;
        int32_t read_int(const std::string &key) override;
        uint32_t read_uint(const std::string &key) override;
        float read_float(const std::string &key) override;
        double read_double(const std::string &key) override;
        std::string read_string(const std::string &key) override;

        // Array reading
        bool read_bool_at(const size_t index) override;
        int32_t read_int_at(const size_t index) override;
        uint32_t read_uint_at(const size_t index) override;
        float read_float_at(const size_t index) override;
        double read_double_at(const size_t index) override;
        std::string read_string_at(const size_t index) override;

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
