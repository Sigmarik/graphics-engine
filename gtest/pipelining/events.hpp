/**
 * @file events.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Event test suite
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "pipelining/event.hpp"

TEST(Events, Trigger) {
    Event event;

    bool received = false;

    Event::Subscriber receiver([&received]() { received = true; });

    event.subscribe(receiver);

    event.trigger();

    ASSERT_EQ(received, true);
}

TEST(Events, SendReceive) {
    int value = 0;

    typename HeavyEvent<int>::Subscriber receiver(
        [&value](int payload) { value = payload; });

    HeavyEvent<int> event;

    event.subscribe(receiver);

    event.trigger(42);

    ASSERT_EQ(value, 42);
}

TEST(Events, MultipleReceivers) {
    int value_a = 0;
    int value_b = 0;

    typename HeavyEvent<int>::Subscriber receiver_a(
        [&value_a](int payload) { value_a = payload; });
    typename HeavyEvent<int>::Subscriber receiver_b(
        [&value_b](int payload) { value_b = payload; });

    HeavyEvent<int> event;

    event.subscribe(receiver_a);
    event.subscribe(receiver_b);

    event.trigger(42);

    ASSERT_EQ(value_a, 42);
    ASSERT_EQ(value_b, 42);
}

TEST(Events, MoveEvent) {
    Event event;

    bool received = false;

    Event::Subscriber receiver([&received]() { received = true; });

    event.subscribe(receiver);

    Event new_event(std::move(event));

    new_event.trigger();

    ASSERT_EQ(received, true);
}

TEST(Events, MoveSubscriber) {
    Event event;

    int signals = 0;

    Event::Subscriber receiver([&signals]() { ++signals; });

    event.subscribe(receiver);

    Event::Subscriber new_receiver(std::move(receiver));

    Event new_event(std::move(event));

    new_event.trigger();

    ASSERT_EQ(signals, 1);
}

TEST(Events, Unsubscribe) {
    Event::Subscriber receiver([]() {});

    EXPECT_EQ(receiver.subscribed(), false);

    Event event;

    event.subscribe(receiver);

    EXPECT_EQ(receiver.subscribed(), true);

    event.unsubscribe(receiver);

    EXPECT_EQ(receiver.subscribed(), false);
}

TEST(Events, EventDeath) {
    Event::Subscriber receiver([]() {});

    EXPECT_EQ(receiver.subscribed(), false);

    {
        EXPECT_EQ(receiver.subscribed(), false);

        Event event;

        event.subscribe(receiver);

        EXPECT_EQ(receiver.subscribed(), true);
    }

    EXPECT_EQ(receiver.subscribed(), false);
}

TEST(Events, SubscriberDeath) {
    bool triggered = false;

    Event event;

    {
        Event::Subscriber receiver([&triggered]() { triggered = true; });
        event.subscribe(receiver);
    }

    event.trigger();

    ASSERT_EQ(triggered, false);
}
