#pragma once

#include <list>
#include <memory>

namespace utils {
template <typename... Args>
class IEventHandler;

template <typename... Args>
class IEvent {
   public:
    using THandler = IEventHandler<Args...>;
    virtual void operator+=(std::weak_ptr<THandler>) = 0;
    virtual void operator-=(std::weak_ptr<THandler>) = 0;

    virtual ~IEvent() {};
};

template <typename... Args>
class IEventHandler {
   public:
    virtual void Handle(Args&&... args) = 0;
    virtual ~IEventHandler() {};
};

template <typename... Args>
class Event : public IEvent<Args...> {
   public:
    using THandler = IEvent<Args...>::THandler;

    void operator+=(std::weak_ptr<THandler> handler) final override {
        handlers_.push_back(handler);
    }
    void operator-=(std::weak_ptr<THandler> handler_ptr) final override {
        if (auto handler = handler_ptr.lock()) {
            std::erase_if(handlers_, [&handler](const auto& other_ptr) {
                std::shared_ptr<THandler> other = other_ptr.lock();
                return !other || other == handler;
            });
        }
    }

    void operator()(Args&&... args) {
        for (std::weak_ptr<THandler> handler_ptr : handlers_) {
            if (auto handler = handler_ptr.lock()) {
                handler->Handle(std::forward<Args>(args)...);
            }
        }
    }

   private:
    std::list<std::weak_ptr<IEventHandler<Args...>>> handlers_;
};

}  // namespace utils
