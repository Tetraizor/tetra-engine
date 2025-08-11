#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "engine/base/event_base.h"

namespace Engine
{

    /**
     * @brief A generic, typed event system with optional lifetime-tracked listeners.
     *
     * @tparam Args The argument types that this event will carry.
     */
    template <typename... Args>
    class Event : public EventBase
    {
    public:
        /// Convenience alias for subscriber callbacks.
        using Callback = std::function<void(Args...)>;

        /**
         * @brief Subscribe a callback to this event.
         *
         * The callback will remain registered until explicitly unsubscribed
         * or until the Event itself is destroyed.
         *
         * @param cb The function or lambda to invoke when the event fires.
         * @return A unique subscription ID that can be passed to unsubscribe().
         */
        size_t subscribe(Callback cb)
        {
            size_t id = next_id++;
            entries.push_back({id, std::move(cb), std::weak_ptr<void>(), false});
            return id;
        }

        /**
         * @brief Subscribe a free/function member on a raw instance (no auto-unsubscribe).
         *
         * @tparam T       Class type of the instance.
         * @param instance Pointer to the object whose method will be called.
         * @param method   Pointer to member function on T matching this event's Args.
         * @return A unique subscription ID.
         */
        template <typename T>
        size_t subscribe(T *instance, void (T::*method)(Args...))
        {
            // wrap the member pointer into a Callback
            return subscribe([instance, method](Args... args)
                             { (instance->*method)(args...); });
        }

        /**
         * @brief Subscribe a member function with automatic lifetime tracking.
         *
         * When @p owner expires, the listener is removed on next invoke().
         *
         * @tparam T       Class type of the owner object.
         * @param owner    Shared pointer owning the subscriber.
         * @param method   Pointer to member function on T matching this event's Args.
         * @return A unique subscription ID.
         */
        template <typename T>
        size_t subscribe(std::shared_ptr<T> owner, void (T::*method)(Args...))
        {
            // wrap the member pointer into the tracked subscribe overload
            return subscribe(owner, [owner, method](Args... args)
                             { (owner.get()->*method)(args...); });
        }

        /**
         * @brief Subscribe a callback with lifetime tracking.
         *
         * As long as @p owner is alive (its shared_ptr use_count > 0),
         * the callback will be invoked.  Once @p owner is destroyed,
         * the listener is automatically removed on the next invoke().
         *
         * @tparam T    Type of the owner object.
         * @param owner Shared pointer to the owner whose lifetime you track.
         * @param cb    The function or lambda to invoke when the event fires.
         * @return A unique subscription ID that can be passed to unsubscribe().
         */
        template <typename T>
        size_t subscribe(std::shared_ptr<T> owner, Callback cb)
        {
            size_t id = next_id++;
            entries.push_back({id, std::move(cb), std::weak_ptr<void>(owner), false});
            return id;
        }

        /**
         * @brief Unsubscribe a specific listener by its subscription ID.
         *
         * If no listener has the given @p id, this is a no-op.
         *
         * @param id The subscription ID returned by subscribe().
         */
        void unsubscribe(size_t id)
        {
            entries.erase(
                std::remove_if(
                    entries.begin(), entries.end(),
                    [&](auto &e)
                    { return e.id == id; }),
                entries.end());
        }

        /**
         * @brief Unsubscribe *all* listeners tied to a given owner.
         *
         * This removes every callback that was registered via
         * subscribe(owner, ...).
         *
         * @tparam T    Type of the owner object.
         * @param owner The same shared_ptr instance used when subscribing.
         */
        template <typename T>
        void unsubscribe_owner(const std::shared_ptr<T> &owner)
        {
            std::weak_ptr<void> target{owner};
            entries.erase(
                std::remove_if(
                    entries.begin(), entries.end(),
                    [&](auto &e)
                    {
                        // Compare two weak_ptr<void> for equality
                        return !e.owner.owner_before(target) && !target.owner_before(e.owner);
                    }),
                entries.end());
        }

        /**
         * @brief Fire the event, invoking all live listeners with the given args.
         *
         * Listeners whose associated owner has expired (if any) are removed
         * before invocation.  Callbacks registered without an owner are always called.
         *
         * @param args The arguments to forward to each callback.
         */
        void invoke(Args... args)
        {
            std::vector<size_t> expired;
            for (auto &entry : entries)
            {
                if (
                    !entry.owner.owner_before(std::weak_ptr<void>{}) &&
                    !std::weak_ptr<void>{}.owner_before(entry.owner))
                {
                    entry.cb(args...);
                }
                else
                {
                    expired.push_back(entry.id);
                }
            }

            if (!expired.empty())
            {
                entries.erase(
                    std::remove_if(
                        entries.begin(), entries.end(),
                        [&](auto &e)
                        {
                            return std::find(expired.begin(), expired.end(), e.id) != expired.end();
                        }),
                    entries.end());
            }
        }

    private:
        /**
         * @brief Internal record for each subscriber.
         *
         * - id:       Unique subscription identifier.
         * - cb:       The callback function to invoke.
         * - owner:    If non-empty, used to auto-unsubscribe when expired.
         * - once:     (reserved for future one-shot listeners).
         */
        struct Entry
        {
            size_t id;
            Callback cb;
            std::weak_ptr<void> owner;
            bool once;
        };

        std::vector<Entry> entries; // All current subscriber entries.
        size_t next_id{1};          // Monotonically increasing ID generator.
    };

} // namespace Engine
