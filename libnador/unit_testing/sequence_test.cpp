#include <gtest/gtest.h>

#include "nador/utils/Sequence.h"

TEST(SequneceTest, Sequence_test)
{
    nador::Event<std::chrono::milliseconds> event;

    size_t count{0};

    auto action = [&count] {
        ++count;
    };

    auto now = std::chrono::system_clock::now();

    if(now + 1ms > now)
    {
        EXPECT_TRUE(true);
    }

    std::vector<nador::ElementSequence<>> list{{10ms, action}, {15ms, action}, {20ms, action}, {5ms, action}};

    nador::Sequence sequence(list, event);

    sequence.Play();

    for(size_t i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(10ms);

        event(10ms);
    }

    EXPECT_EQ(count, list.size());
}