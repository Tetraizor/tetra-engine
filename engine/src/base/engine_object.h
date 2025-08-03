#pragma once

#include <atomic>
#include <string>
#include <functional>
#include <memory>

#include "base/event.h"
#include "base/event_base.h"

namespace Engine
{
    class EngineObject : std::enable_shared_from_this<EngineObject>
    {
    public:
        EngineObject();
        virtual ~EngineObject();

        size_t get_instance_id() const { return instance_id; }

        template <typename... Args>
        void subscribe(const std::string &event_name,
                       std::function<void(Args...)> callback)
        {
            get_or_create_event<Args...>(event_name)->subscribe(std::move(callback));
        }

        template <typename T, typename... Args>
        void subscribe(const std::string &event_name,
                       std::shared_ptr<T> owner,
                       std::function<void(Args...)> callback)
        {
            get_or_create_event<Args...>(event_name)->subscribe(owner, std::move(callback));
        }

        template <typename... Args>
        void invoke(const std::string &event_name, Args... args)
        {
            auto it = events.find(event_name);
            if (it == events.end())
                return;

            // We know this EventBase really is Event<Args...>
            using EventType = Event<Args...>;
            auto *evt = static_cast<EventType *>(it->second.get());
            evt->notify(args...);
        }

    private:
        // Helper: obtain (or create) an Event<Args...> for this event name
        template <typename... Args>
        Event<Args...> *get_or_create_event(const std::string &name)
        {
            auto it = events.find(name);
            if (it != events.end())
            {
                // Ensure type matches!
                return static_cast<Event<Args...> *>(it->second.get());
            }

            // Create a new Event<Args...>
            auto evt = std::make_unique<Event<Args...>>();
            auto ptr = evt.get();
            events.emplace(name, std::move(evt));
            return ptr;
        }

        size_t instance_id;
        static std::atomic<size_t> global_id_counter;

        std::unordered_map<std::string, std::unique_ptr<EventBase>> events;
    };
}