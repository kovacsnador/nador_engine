#include <gtest/gtest.h>

#include "nador/utils/event/Event.h"

class EventTest : public ::testing::TestWithParam<size_t>
{
};

TEST(EventTest, SimpleEventTest)
{
    bool fired = false;

    nador::Event           simpleEvent;
    nador::EventListener<> eventListener([&fired]() { fired = true; });

    simpleEvent += eventListener;

    EXPECT_FALSE(fired);

    // fire event
    simpleEvent();

    EXPECT_TRUE(fired);
}

INSTANTIATE_TEST_SUITE_P(EventTest, EventTest, ::testing::Values(1, 10, 100));

TEST_P(EventTest, MultiListenersEventTest)
{
    size_t count = 0;

    nador::Event simpleEvent;

    std::vector<std::unique_ptr<nador::EventListener<>>> vec;

    for (size_t i = 0; i < GetParam(); ++i)
    {
        auto listener = std::make_unique<nador::EventListener<>>([&count]() { ++count; });
        simpleEvent += *listener.get();

        vec.push_back(std::move(listener));
    }

    EXPECT_EQ(count, 0);

    // fire event
    simpleEvent();

    EXPECT_EQ(count, GetParam());

    vec.clear();

    // fire event
    simpleEvent();

    EXPECT_EQ(count, GetParam());
}

TEST(EventTest, EventListenerMoveTest)
{
    int val { 0 };
    int plus { 1 };

    auto cb = [&val](int i) { val += i; };

    std::function<void(int)> func{cb};

    nador::Event<int>         event;
    nador::EventListener listener1 { func };

    event += listener1;

    nador::EventListener<int> listener2 { std::move(listener1) };

    listener1 = std::move(listener2);
    listener2 = std::move(listener1);

    listener1.SetCallback(cb);

    // fire event
    event(plus);

    // need to be called only once
    EXPECT_EQ(plus, val);

    listener2.Suspend(true);

    listener1 = std::move(listener2);

    // fire event
    event(plus);

    EXPECT_EQ(plus, val);
}

TEST(EventTest, EventMoveTest)
{
    bool val { false };
    auto cb = [&val](auto v) { val = v; };

    nador::Event<bool>         event1;
    nador::EventListener<bool> listener {cb};

    event1 += listener;

    nador::Event<bool> event2 { std::move(event1) };

    event1(true);

    EXPECT_FALSE(val);

    event2(true);

    EXPECT_TRUE(val);
}