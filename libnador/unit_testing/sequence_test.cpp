#include <gtest/gtest.h>

#include "nador/utils/Sequence.h"

struct Elem
{
    int i { 0 };
};

TEST(SequneceTest, Sequence_test)
{
    nador::Event<std::chrono::milliseconds> event;
    Elem elem;

    size_t count { 0 };

    auto action = [&count] (Elem& elem) { elem.i++; ++count; };

    nador::ElementSequenceVec_t<Elem> list { { 10ms, action }, { 15ms, action }, { 20ms, action }, { 5ms, action } };

    nador::Sequence<Elem> sequence(elem, list, event);

    sequence.Play();

    for (size_t i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(10ms);

        event(10ms);
    }

    EXPECT_EQ(elem.i, list.size());
    EXPECT_EQ(count, list.size());
}