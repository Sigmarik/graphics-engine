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
 * @brief Event with payload
 *
 * @tparam Ts payload contents
 */
template <class... Ts>
struct HeavyEvent final {
    /**
     * @brief Event listener
     *
     */
    struct Subscriber {
        explicit Subscriber(const std::function<void(Ts...)>& action)
            : action_(action){};

        Subscriber(const Subscriber& other)
            : action_(other.action_), event_(other.event_) {
            if (event_) event_->subscribe(*this);
        }

        Subscriber& operator=(const Subscriber& other) {
            if (event_) event_->unsubscribe(*this);

            action_ = other.action_;
            event_ = other.event_;

            if (event_) event_->subscribe(*this);

            return *this;
        }

        Subscriber(Subscriber&& other)
            : action_(other.action_), event_(other.event_) {
            if (event_) event_->subscribe(*this);
            if (other.event_) other.event_->unsubscribe(other);
        }

        Subscriber& operator=(Subscriber&& other) {
            if (event_) event_->unsubscribe(*this);

            action_ = other.action_;
            event_ = other.event_;

            if (event_) event_->subscribe(*this);

            if (other.event_) other.event_->unsubscribe(other);

            return *this;
        }

        ~Subscriber() {
            if (event_) event_->unsubscribe(*this);
        }

        typename HeavyEvent<Ts...>::Subscriber& operator=(Subscriber& other);

        void operator()(Ts... args) { action_(args...); }

        /**
         * @brief Get if subscription is still valid
         *
         * @return true if the subscription is bound to a valid event,
         * @return false otherwise
         */
        bool subscribed() const { return event_ != nullptr; }

        friend HeavyEvent;

       private:
        std::function<void(Ts...)> action_;
        HeavyEvent<Ts...>* event_ = nullptr;
    };

    HeavyEvent() = default;

    HeavyEvent(const HeavyEvent&) = delete;
    HeavyEvent<Ts...>& operator=(const HeavyEvent&) = delete;

    HeavyEvent(HeavyEvent&&);
    HeavyEvent<Ts...>& operator=(HeavyEvent&&);

    ~HeavyEvent();

    /**
     * @brief Subscribe a listener to the event
     *
     * @param[in] subscriber
     */
    void subscribe(typename HeavyEvent<Ts...>::Subscriber& subscriber);

    /**
     * @brief Unsubscribe the subscriber from the event
     * @throws `std::runtime_error` if the subscriber is not subscribed to the
     * event
     *
     * @param[in] subscriber
     */
    void unsubscribe(typename HeavyEvent<Ts...>::Subscriber& subscriber);

    /**
     * @brief Trigger the event and send the payload to its subscribers
     *
     */
    void trigger(Ts... payload);

   private:
    std::set<typename HeavyEvent<Ts...>::Subscriber*> subscribers_{};
};

/**
 * @brief Simple listen-notify event without any payload
 *
 */
using Event = HeavyEvent<>;

template <class... Ts>
inline typename HeavyEvent<Ts...>::Subscriber&
HeavyEvent<Ts...>::Subscriber::operator=(Subscriber& other) {
    if (event_) event_->unsubscribe(*this);
    action_ = other.action_;
    event_ = other.event_;
    if (event_) event_->subscribe(*this);

    return *this;
}

template <class... Ts>
inline HeavyEvent<Ts...>::HeavyEvent(HeavyEvent&& event) : subscribers_() {
    for (auto subscriber : event.subscribers_) {
        subscribers_.insert(subscriber);
        subscriber->event_ = this;
    }

    event.subscribers_.clear();
}

template <class... Ts>
inline HeavyEvent<Ts...>& HeavyEvent<Ts...>::operator=(HeavyEvent&& event) {
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
inline HeavyEvent<Ts...>::~HeavyEvent() {
    for (auto subscriber : subscribers_) {
        subscriber->event_ = nullptr;
    }
}

template <class... Ts>
inline void HeavyEvent<Ts...>::subscribe(
    typename HeavyEvent<Ts...>::Subscriber& subscriber) {
    subscribers_.insert(&subscriber);
    subscriber.event_ = this;
}

template <class... Ts>
inline void HeavyEvent<Ts...>::unsubscribe(
    typename HeavyEvent<Ts...>::Subscriber& subscriber) {
    auto find = subscribers_.find(&subscriber);

    if (find == subscribers_.end()) {
        throw std::runtime_error(
            "Trying to unsubscribe non-existant subscriber");
    }

    subscribers_.erase(find);
    subscriber.event_ = nullptr;
}

template <class... Ts>
inline void HeavyEvent<Ts...>::trigger(Ts... payload) {
    for (auto& subscriber : subscribers_) {
        subscriber->operator()(payload...);
    }
}
