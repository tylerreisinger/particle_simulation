#ifndef EVENT_H_
#define EVENT_H_

#include <list>
#include <functional>
#include <vector>
#include <memory>

#include "IntrusiveList.h"

template <typename T>
class Event;
//template <typename T>
//class EventHandleImpl;

template <typename R, typename... Args>
class EventConnectionHandleImpl;

class EventConnection;

using EventIdType = int;

class IEventConnectionHandle {
public:
    IEventConnectionHandle() = default;
    virtual ~IEventConnectionHandle() = default;

    virtual void unregister() = 0;
    virtual bool is_connected() const = 0;
};

template <typename R, typename... Args>
class EventHandler {
    using FunctionType = std::function<R (Args...)>;
    using Iterator = typename std::list<std::shared_ptr<EventHandler<R, Args...>>>::iterator;
public:
    EventHandler(FunctionType fn, Event<R (Args...)>* event):
        fn(std::move(fn)), event(event) {}

    ~EventHandler() = default;

    FunctionType fn;
    Iterator iter;
    Event<R (Args...)>* event;
};

class EventConnection {
    friend class ScopedEventConnection;
public:
    EventConnection(std::unique_ptr<IEventConnectionHandle> handle):
        m_handle(std::move(handle)) {}

    ~EventConnection() = default;

    EventConnection(const EventConnection& other) = delete;
    EventConnection(EventConnection&& other) noexcept = default;
    EventConnection& operator =(const EventConnection& other) = delete;
    EventConnection& operator =(EventConnection&& other) noexcept = default;

    void unregister() {
        if(m_handle != nullptr) {
            m_handle->unregister();
        }
    }
    bool is_connected() const {
        if(m_handle != nullptr) {
            return m_handle->is_connected();
        }
        return false;
    }

private:
    std::unique_ptr<IEventConnectionHandle> m_handle;
};

template <typename R, typename... Args>
class Event<R (Args...)>
{
public:
    using FunctionType = std::function<R (Args...)>;
    using ContainerType = std::list<
        std::shared_ptr<EventHandler<R, Args...>>>;
    using IteratorType = typename ContainerType::iterator;

    Event() = default;
    ~Event() = default;

    Event(const Event& other) = delete;
    Event(Event&& other) noexcept = default;
    Event& operator =(const Event& other) = delete;
    Event& operator =(Event&& other) noexcept = default;

    void operator ()(Args... args) {
        for(auto& handler : m_handlers) {
            handler->fn(std::forward<Args>(args)...);
        }
    }

    EventConnection register_handler(FunctionType handler) {
        auto id = m_next_id;
        m_next_id += 1;
        auto node = std::make_shared<EventHandler<R, Args...>>(std::move(handler), this);
        m_handlers.push_back(node);
        auto it = --m_handlers.end();
        node->iter = it;

        auto handle = std::make_unique<EventConnectionHandleImpl<R, Args...>>(
                std::weak_ptr<EventHandler<R, Args...>>(node));
        return EventConnection(std::move(handle));
    }

    void unregister(IteratorType it) {
        m_handlers.erase(it);
    }

private:
    ContainerType m_handlers;
    EventIdType m_next_id = 1;
};

template <typename R, typename... Args>
class EventConnectionHandleImpl: public IEventConnectionHandle {
public:
    using FunctionType = std::function<R (Args...)>;
    using ConnectionIterator = typename std::list<FunctionType>::iterator;

    EventConnectionHandleImpl(std::weak_ptr<EventHandler<R, Args...>> handler_node):
        m_handler(std::move(handler_node)) {}

    virtual ~EventConnectionHandleImpl() {}

    virtual void unregister() override {
        if(!m_handler.expired()) {
            auto handler = m_handler.lock();
            auto evt = handler->event;
            if(evt != nullptr) {
                evt->unregister(handler->iter);
                handler->event = nullptr;
            }
        }
    }

    virtual bool is_connected() const override {
        if(!m_handler.expired()) {
            auto handler = m_handler.lock();
            auto evt = handler->event;
            return (evt != nullptr);
        }
        return false;
    }

private:
    std::weak_ptr<EventHandler<R, Args...>> m_handler;
};

class ScopedEventConnection {
public:
    ScopedEventConnection(EventConnection&& connection) :
        m_connection(std::move(connection)) {}

    ~ScopedEventConnection() {
        if(m_connection.m_handle != nullptr) {
            m_connection.unregister();
        }
    }

    ScopedEventConnection(const ScopedEventConnection& other) = delete;
    ScopedEventConnection(ScopedEventConnection&& other) noexcept = default;
    ScopedEventConnection& operator =(const ScopedEventConnection& other) = delete;
    ScopedEventConnection& operator =(ScopedEventConnection&& other) noexcept = default;

    void unregister() {
        return m_connection.unregister();
    }
    bool is_connected() const {
        return m_connection.is_connected();
    }

private:
    EventConnection m_connection;
};
#endif
