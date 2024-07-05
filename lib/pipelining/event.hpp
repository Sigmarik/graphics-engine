/**
 * @file event.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Event class
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <set>
#include <stdexcept>

/**
 * @brief Event class. Can be triggered to notify its subscribers and deliver a
 * payload to them.
 *
 * @tparam Ts payload contents
 */
template <class... Ts>
struct Event final {
    /**
     * @brief Event listener
     *
     */
    struct Listener {
        explicit Listener(const std::function<void(Ts...)>& action)
            : action_(action){};

        Listener() : Listener([](Ts...) {}) {}

        Listener(const Listener& other)
            : action_(other.action_), event_(other.event_) {
            if (event_) event_->subscribe(*this);
        }

        Listener& operator=(const Listener& other) {
            if (&other == this) return *this;

            if (event_) event_->unsubscribe(*this);

            action_ = other.action_;
            event_ = other.event_;

            if (event_) event_->subscribe(*this);

            return *this;
        }

        Listener(Listener&& other)
            : action_(other.action_), event_(other.event_) {
            if (event_) event_->subscribe(*this);
            if (other.event_) other.event_->unsubscribe(other);
        }

        Listener& operator=(Listener&& other) {
            if (event_) event_->unsubscribe(*this);

            action_ = other.action_;
            event_ = other.event_;

            if (event_) event_->subscribe(*this);

            if (other.event_) other.event_->unsubscribe(other);

            return *this;
        }

        ~Listener() {
            if (event_) event_->unsubscribe(*this);
        }

        typename Event<Ts...>::Listener& operator=(Listener& other);

        void operator()(Ts... args) { action_(args...); }

        /**
         * @brief Check if subscription is still valid
         *
         * @return true if the subscription is bound to a valid event,
         * @return false otherwise
         */
        bool is_subscribed() const { return event_ != nullptr; }

        friend Event;

       private:
        std::function<void(Ts...)> action_;
        Event<Ts...>* event_ = nullptr;
    };

    Event() = default;

    Event(const Event&) = delete;
    Event<Ts...>& operator=(const Event&) = delete;

    Event(Event&&);
    Event<Ts...>& operator=(Event&&);

    ~Event();

    /**
     * @brief Subscribe a listener to the event
     *
     * @param[in] subscriber
     */
    void subscribe(typename Event<Ts...>::Listener& subscriber);

    /**
     * @brief Unsubscribe the subscriber from the event
     * @throws `std::runtime_error` if the subscriber is not subscribed to the
     * event
     *
     * @param[in] subscriber
     */
    void unsubscribe(typename Event<Ts...>::Listener& subscriber);

    /**
     * @brief Trigger the event and send the payload to its subscribers
     *
     */
    void trigger(Ts... payload);

   private:
    std::set<typename Event<Ts...>::Listener*> subscribers_{};
};

using SimpleEvent = Event<>;

template <class... Ts>
inline typename Event<Ts...>::Listener& Event<Ts...>::Listener::operator=(
    Listener& other) {
    if (event_) event_->unsubscribe(*this);
    action_ = other.action_;
    event_ = other.event_;
    if (event_) event_->subscribe(*this);

    return *this;
}

template <class... Ts>
inline Event<Ts...>::Event(Event&& event) : subscribers_() {
    for (auto subscriber : event.subscribers_) {
        subscribers_.insert(subscriber);
        subscriber->event_ = this;
    }

    event.subscribers_.clear();
}

template <class... Ts>
inline Event<Ts...>& Event<Ts...>::operator=(Event&& event) {
    for (auto subscriber : subscribers_) {
        subscriber->event_ = nullptr;
    }

    subscribers_.clear();

    for (auto subscriber : event.subscribers_) {
        subscribers_.insert(subscriber);
        subscriber->event_ = this;
    }

    event.subscribers_.clear();

    return *this;
}

template <class... Ts>
inline Event<Ts...>::~Event() {
    for (auto subscriber : subscribers_) {
        subscriber->event_ = nullptr;
    }
}

template <class... Ts>
inline void Event<Ts...>::subscribe(
    typename Event<Ts...>::Listener& subscriber) {
    if (subscribers_.count(&subscriber) > 0) return;

    if (subscriber.event_) subscriber.event_->unsubscribe(subscriber);

    subscribers_.insert(&subscriber);
    subscriber.event_ = this;
}

template <class... Ts>
inline void Event<Ts...>::unsubscribe(
    typename Event<Ts...>::Listener& subscriber) {
    auto find = subscribers_.find(&subscriber);

    if (find == subscribers_.end()) {
        throw std::runtime_error(
            "Trying to unsubscribe a non-existant subscriber");
    }

    subscribers_.erase(find);
    subscriber.event_ = nullptr;
}

template <class... Ts>
inline void Event<Ts...>::trigger(Ts... payload) {
    for (auto& subscriber : subscribers_) {
        subscriber->operator()(payload...);
    }
}
