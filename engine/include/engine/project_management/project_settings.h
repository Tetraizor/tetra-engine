#pragma once

#include "serialization/json/json_document.h"
#include "serialization/json/json_value.h"
#include "exceptions/json_parse_exception.h"

#include <string>

using namespace Engine::Serialization::Json;

namespace Engine
{
    struct ProjectSettings
    {
        std::string project_name;
        std::string engine_version;
        std::string project_definition_version;
        std::string last_open_stage;

        std::string serialize() const
        {
            JsonDocument doc;
            JsonValue root = doc.get_root();

            root.set("project_name", project_name);
            root.set("engine_version", engine_version);
            root.set("project_definition_version", project_definition_version);
            root.set("last_open_stage", last_open_stage);

            return doc.to_text();
        }

        void deserialize(const std::string &project_settings_json)
        {
            JsonDocument doc(project_settings_json);
            JsonValue root = doc.get_root();

            std::optional<std::string> project_name_opt = root.get("project_name").as<std::string>();
            std::optional<std::string> engine_version_opt = root.get("engine_version").as<std::string>();
            std::optional<std::string> project_definition_version_opt = root.get("project_definition_version").as<std::string>();
            std::optional<std::string> last_open_stage_opt = root.get("last_open_stage").as<std::string>();

            if (
                !project_name_opt.has_value() ||
                !engine_version_opt.has_value() ||
                !project_definition_version_opt.has_value() ||
                !last_open_stage_opt.has_value())
            {
                throw std::runtime_error("Cannot deserialize ProjectSettings json text\n" + doc.to_text());
            }

            project_name = project_name_opt.value();
            engine_version = engine_version_opt.value();
            project_definition_version = project_definition_version_opt.value();
            last_open_stage = last_open_stage_opt.value();
        }
    };
}