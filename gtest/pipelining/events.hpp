/**
 * @file events.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief SimpleEvent test suite
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "pipelining/event.hpp"

TEST(Events, Trigger) {
    SimpleEvent event;

    bool received = false;

    SimpleEvent::Listener receiver([&received]() { received = true; });

    event.subscribe(receiver);

    event.trigger();

    ASSERT_EQ(received, true);
}

TEST(Events, SendReceive) {
    int value = 0;

    typename Event<int>::Listener receiver(
        [&value](int payload) { value = payload; });

    Event<int> event;

    event.subscribe(receiver);

    event.trigger(42);

    ASSERT_EQ(value, 42);
}

TEST(Events, MultipleReceivers) {
    int value_a = 0;
    int value_b = 0;

    typename Event<int>::Listener receiver_a(
        [&value_a](int payload) { value_a = payload; });
    typename Event<int>::Listener receiver_b(
        [&value_b](int payload) { value_b = payload; });

    Event<int> event;

    event.subscribe(receiver_a);
    event.subscribe(receiver_b);

    event.trigger(42);

    ASSERT_EQ(value_a, 42);
    ASSERT_EQ(value_b, 42);
}

TEST(Events, MoveEvent) {
    SimpleEvent event;

    bool received = false;

    SimpleEvent::Listener receiver([&received]() { received = true; });

    event.subscribe(receiver);

    SimpleEvent new_event(std::move(event));

    new_event.trigger();

    ASSERT_EQ(received, true);
}

TEST(Events, MoveSubscriber) {
    SimpleEvent event;

    int signals = 0;

    SimpleEvent::Listener receiver([&signals]() { ++signals; });

    event.subscribe(receiver);

    SimpleEvent::Listener new_receiver(std::move(receiver));

    SimpleEvent new_event(std::move(event));

    new_event.trigger();

    ASSERT_EQ(signals, 1);
}

TEST(Events, Unsubscribe) {
    SimpleEvent::Listener receiver([]() {});

    EXPECT_EQ(receiver.is_subscribed(), false);

    SimpleEvent event;

    event.subscribe(receiver);

    EXPECT_EQ(receiver.is_subscribed(), true);

    event.unsubscribe(receiver);

    EXPECT_EQ(receiver.is_subscribed(), false);
}

TEST(Events, EventDeath) {
    SimpleEvent::Listener receiver([]() {});

    EXPECT_EQ(receiver.is_subscribed(), false);

    {
        EXPECT_EQ(receiver.is_subscribed(), false);

        SimpleEvent event;

        event.subscribe(receiver);

        EXPECT_EQ(receiver.is_subscribed(), true);
    }

    EXPECT_EQ(receiver.is_subscribed(), false);
}

TEST(Events, SubscriberDeath) {
    bool triggered = false;

    SimpleEvent event;

    {
        SimpleEvent::Listener receiver([&triggered]() { triggered = true; });
        event.subscribe(receiver);
    }

    event.trigger();

    ASSERT_EQ(triggered, false);
}
