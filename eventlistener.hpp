/**
 * @file eventlistener.hpp
 * @author Ty Qualters (contact@tyqualters.com)
 * @brief A simple multi-threaded event listener in C++.
 * @version 0.1
 * @date 2021-10-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <vector>
#include <mutex>
#include <functional>
#include <algorithm>
#include <memory>

namespace EventListener
{
    /**
     * @brief The object passed (event) => event's associated function.
     * 
     */
    struct SEvent
    {
        int id;
        uintptr_t address;
        std::string name;
    };

    /**
     * @brief Typedef for the EventFunction to make life easier.
     * 
     */
    typedef std::function<void(SEvent)> EventFunction;

    /**
     * @brief The object stored for the event listener.
     * 
     */
    struct SListener
    {
        int id;
        void *address;
        const char *name;
        EventFunction *fn;
    };

    /**
     * @brief The list of events to listen for.
     * 
     */
    std::vector<SListener> g_events{};
    /**
     * @brief The mutex for events.
     * 
     */
    std::mutex g_events_mutex;

    /**
     * @brief The function to call an event.
     * 
     * @param objAddress The address of the assoc. object.
     * @param eventName The name of the event to call.
     */
    void CallEvent(void *objAddress, const char *eventName)
    {
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        for (auto &event : g_events)
        {
            if (event.address == objAddress && eventName)
                (*event.fn)(SEvent{event.id, (uintptr_t)objAddress, std::string(eventName)});
        }
    }

    /**
     * @brief Create an Event Listener.
     * 
     * @param objAddress The address to the associated object. If global, set the value to nullptr.
     * @param eventName The name of the event.
     * @param pfn A pointer to a function that will accept the SEvent argument.
     */
    void CreateEventListener(void *objAddress, const char *eventName, EventFunction *pfn)
    {
        static int id = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        g_events.push_back(SListener{id++, objAddress, eventName, pfn});
    }

    /**
     * @brief Delete an Event Listener.
     * 
     * @param id The ID of the Event Listener.
     * @return int Count of how many Event Listeners were deleted.
     */
    int DeleteEventListener(int id)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&count, &id](SListener &listener) -> bool
            {
                if (listener.id == id)
                {
                    ++count;
                    return true;
                }
                else
                    return false;
            });
        return count;
    }

    /**
     * @brief Deletes any Event Listener associated with an address.
     * 
     * @param objAddress The address associated with the events to be deleted.
     * @return int Count of how many Event Listeners were deleted.
     */
    int DeleteEventListeners(void *objAddress)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&objAddress, &count](SListener &listener) -> bool
            {
                if (listener.address == objAddress)
                {
                    return true;
                    ++count;
                }
                else
                    return false;
            });
        return count;
    }

    /**
     * @brief Deletes any Event Listener associated with an event name.
     * 
     * @param eventName The name for an event.
     * @return int Count of how many Event Listeners were deleted.
     */
    int DeleteEventListeners(const char *eventName)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&eventName, &count](SListener &listener) -> bool
            {
                if (listener.name == eventName)
                {
                    return true;
                    ++count;
                }
                else
                    return false;
            });
        return count;
    }

    /**
     * @brief Push an event to all Event Listeners associated with an event name.
     * 
     * @param eventName The name for an event.
     * @return int Count for how many events have been pushed.
     */
    int PushEvent(const char *eventName)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        std::for_each(g_events.begin(), g_events.end(), [&count, &eventName](SListener &listener) -> void
            {
                if (listener.name == eventName)
                {
                    CallEvent(listener.address, eventName);
                    ++count;
                }
            });
        return count;
    }

    /**
     * @brief Push an event to all Event Listeners associated with an address and event name.
     * 
     * @param objAddress The address associated with the events.
     * @param eventName The name for an event.
     * @return int Count for how many events have been pushed.
     */
    int PushEvent(void *objAddress, const char *eventName)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        std::for_each(g_events.begin(), g_events.end(), [&count, &objAddress, &eventName](SListener &listener) -> void
            {
                if (listener.address == objAddress && listener.name == eventName)
                {
                    CallEvent(listener.address, eventName);
                    ++count;
                }
            });
        return count;
    }
}

/* Push to the global namespace */
using EventListener::CreateEventListener;
using EventListener::DeleteEventListener;
using EventListener::DeleteEventListeners;
using EventListener::EventFunction;
using EventListener::PushEvent;
using EventListener::SEvent;