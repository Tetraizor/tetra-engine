#include "engine/serialization/json/json_document.h"

namespace Engine::Serialization::Json
{

    JsonDocument::JsonDocument() : root_ptr(std::make_unique<nlohmann::json>(nlohmann::json::object())) {}

    JsonDocument::JsonDocument(const std::string &text)
    {
        try
        {
            root_ptr = std::make_unique<nlohmann::json>(nlohmann::json::parse(text));
        }
        catch (const nlohmann::json::parse_error &e)
        {
            throw std::runtime_error(std::string("JSON parse error: ") + e.what());
        }
    }

    const std::string JsonDocument::to_text(bool pretty) const
    {
        return get_root().to_text(pretty);
    }

    JsonValue::JsonValue(nlohmann::json *json_ptr) : json_ptr(json_ptr) {}
}
