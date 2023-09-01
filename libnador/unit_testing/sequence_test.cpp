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

    nador::Sequence<Elem> seq1(elem, list, event);

    seq1.Play();

    event(1ms);
    EXPECT_EQ(elem.i, 0);
    EXPECT_EQ(count, 0);

    event(9ms);
    EXPECT_EQ(elem.i, 1);
    EXPECT_EQ(count, 1);

    // move test
    nador::Sequence<Elem> seq2 = std::move(seq1);

    event(10ms);
    EXPECT_EQ(elem.i, 1);
    EXPECT_EQ(count, 1);

    event(10ms);
    EXPECT_EQ(elem.i, 2);
    EXPECT_EQ(count, 2);

    event(10ms);
    EXPECT_EQ(elem.i, 2);
    EXPECT_EQ(count, 2);

    event(10ms);
    EXPECT_EQ(elem.i, list.size());
    EXPECT_EQ(count, list.size());
}