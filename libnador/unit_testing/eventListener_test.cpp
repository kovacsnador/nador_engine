#include <gtest/gtest.h>

#include "nador/utils/event/Event.h"

class EventTest : public ::testing::TestWithParam<size_t>
{
};

TEST(EventTest, SimpleEventTest)
{
    bool fired = false;

    nador::Event simpleEvent;
    nador::EventListener<> eventListener([&fired](){ fired = true;});

    simpleEvent += eventListener;

    EXPECT_FALSE(fired);

    // fire event
    simpleEvent();

    EXPECT_TRUE(fired);
}

INSTANTIATE_TEST_CASE_P(
    EventTest,
    EventTest,
    ::testing::Values(
         1, 10, 100
));

TEST_P(EventTest, MultiListenersEventTest)
{
    size_t count = 0;

    nador::Event simpleEvent;

    std::vector<std::unique_ptr<nador::EventListener<>>> vec;

    for(size_t i = 0; i < GetParam(); ++i)
    {
        auto listener = std::make_unique<nador::EventListener<>>([&count](){ ++count; });
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

