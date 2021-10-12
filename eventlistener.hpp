/**
 * @file eventlistener.hpp
 * @author Ty Qualters (contact@tyqualters.com)
 * @brief A simple multi-threaded event listener in C++.
 * @version 0.3
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

#ifdef __DEBUG
#include <iostream>
void EventListenerLog(const char* text)
{
    std::cout << text << std::endl;
}
void EventListenerError(const char* text)
{
    std::cerr << text << std::endl;
}
#else
void EventListenerLog(const char* text) {}
void EventListenerError(const char* text) {}
#endif

namespace EventListener
{
    struct SEvent
    {
        int id;
        uintptr_t address;
        std::string name;
    };

    template <typename... arguments>
    using EventFunction = std::function<void(SEvent, arguments...)>;

    struct SListener
    {
        int id;
        void *address;
        const char *name;
        void **fn;
    };

    std::vector<SListener> g_events{};
    std::mutex g_events_mutex;

    template <typename... arguments>
    void CallEvent(void *objAddress, const char *eventName, arguments... args)
    {
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Calling listener event.");
        for (auto &event : g_events)
        {
            if (event.address == objAddress && eventName)
                try
                {
                    (*(EventFunction<arguments...> *)event.fn)(SEvent{event.id, (uintptr_t)objAddress, std::string(eventName)}, args...);
                    EventListenerLog("Successfully called listener event.");
                } catch (std::exception &e) {
                    EventListenerError("WARNING: Listener event threw an exception.");
                }
        }
    }

    template <typename... arguments>
    void CreateEventListener(void *objAddress, const char *eventName, EventFunction<arguments...> *pfn)
    {
        static int id = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Creating listener.");
        g_events.push_back(SListener{id++, objAddress, eventName, (void **)pfn});
        EventListenerLog("Listener created.");
    }

    int DeleteEventListener(int id)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Scanning listeners.");
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&count, &id](SListener &listener) -> bool
            {
                EventListenerLog("Checking listener.");
                if (listener.id == id)
                {
                    EventListenerLog("Deleting listener.");
                    ++count;
                    return true;
                }
                else
                    return false;
            });
        return count;
    }

    int DeleteEventListeners(void *objAddress)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Scanning listeners.");
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&objAddress, &count](SListener &listener) -> bool
            {
                EventListenerLog("Checking listener.");
                if (listener.address == objAddress)
                {
                    EventListenerLog("Deleting listener.");
                    return true;
                    ++count;
                }
                else
                    return false;
            });
        return count;
    }

    int DeleteEventListeners(const char *eventName)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Scanning listeners.");
        std::vector<SListener>::iterator it;
        it = std::remove_if(g_events.begin(), g_events.end(), [&eventName, &count](SListener &listener) -> bool
            {
                EventListenerLog("Checking listener.");
                if (listener.name == eventName)
                {
                    EventListenerLog("Deleting listener.");
                    return true;
                    ++count;
                }
                else
                    return false;
            });
        return count;
    }

    template <typename... Args>
    int PushEvent(const char *eventName, Args... args)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Scanning listeners.");
        for (SListener &listener : g_events)
        {
            EventListenerLog("Checking listener.");
            if (listener.name == eventName)
            {
                EventListenerLog("Calling listener function.");
                CallEvent(listener.address, eventName, std::forward<Args>(args)...);
                ++count;
            }
        }
        return count;
    }

    template <typename... Args>
    int PushEvent(void *objAddress, const char *eventName, Args... args)
    {
        int count = 0;
        const std::lock_guard<std::mutex> lock(g_events_mutex);
        EventListenerLog("Scanning listeners.");
        for (SListener &listener : g_events)
        {
            EventListenerLog("Checking listener.");
            if (listener.address == objAddress && listener.name == eventName)
            {
                EventListenerLog("Calling listener function.");
                CallEvent(listener.address, eventName, std::forward<Args>(args)...);
                ++count;
            }
        }
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
