#pragma once

#include "engine/serialization/json/json_value.h"

namespace Engine::Serialization::Json
{
    class JsonDocument
    {
    public:
        JsonDocument();
        explicit JsonDocument(const std::string &text);

        ~JsonDocument() = default;

        JsonValue get_root() const { return JsonValue(root_ptr.get()); }

        const std::string to_text(bool pretty = true) const;

    private:
        std::unique_ptr<nlohmann::json> root_ptr;
    };
}