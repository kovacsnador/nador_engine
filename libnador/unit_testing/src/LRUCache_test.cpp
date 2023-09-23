#include <gtest/gtest.h>

#include <vector>

#include "nador/utils/LRUCache.hpp"

template<typename CallbackT = std::nullptr_t>
static auto CreateCache(size_t maxSize, CallbackT cb = nullptr)
{
    nador::LRUCache<int, int> cache{maxSize, cb};

    cache.Insert(0, 1);
    cache.Insert(1, 2);
    cache.Insert(2, 3);
    cache.Insert(3, 4);
    cache.Insert(4, 5);
    cache.Insert(5, 6);
    cache.Insert(6, 7);
    cache.Insert(7, 8);
    cache.Insert(8, 9);
    cache.Insert(9, 10);
    cache.Insert(10, 11);
    cache.Insert(11, 12);

    return cache;
}

TEST(LRUCacheTest, LRUCacheSimple_test)
{
    nador::LRUCache<int, int> cache{10};

    cache.Insert(0, 1);

    EXPECT_TRUE(cache.Exist(0));
    EXPECT_EQ(cache.Size(), 1);
}

TEST(LRUCacheTest, LRUCacheMaxSize_test)
{
    auto cache = CreateCache(10);

    EXPECT_FALSE(cache.Exist(0));
    EXPECT_FALSE(cache.Exist(1));

    EXPECT_TRUE(cache.Exist(2));
    EXPECT_TRUE(cache.Exist(3));
    EXPECT_TRUE(cache.Exist(4));
    EXPECT_TRUE(cache.Exist(5));
    EXPECT_TRUE(cache.Exist(6));
    EXPECT_TRUE(cache.Exist(7));
    EXPECT_TRUE(cache.Exist(8));
    EXPECT_TRUE(cache.Exist(9));
    EXPECT_TRUE(cache.Exist(10));
    EXPECT_TRUE(cache.Exist(11));

    EXPECT_EQ(cache.Size(), 10);
}

TEST(LRUCacheTest, LRUCacheOnEvicted_test)
{
    uint32_t count{0};

    auto onEvicted = [&count](const auto& key, const auto& value) {
        EXPECT_EQ(key, count);
        EXPECT_EQ(value, count + 1);
        ++count;
    };

    auto cache = CreateCache(10, onEvicted);

    EXPECT_EQ(cache.Size(), 10);
    EXPECT_EQ(count, 2);
}

TEST(LRUCacheTest, LRUCacheInsertOrAssign_test)
{
    uint32_t count{0};

    auto onEvicted = [&count]([[maybe_unused]] const auto& key, [[maybe_unused]] const auto& value) {
        ++count;
    };

    auto cache = CreateCache(10, onEvicted);

    cache.InsertOrAssign(2, 22);
    EXPECT_EQ(cache.Get(2), 22);

    cache.InsertOrAssign(33, 34);
    EXPECT_EQ(cache.Get(33), 34);

    EXPECT_EQ(cache.Size(), 10);
    EXPECT_EQ(count, 3);
}

TEST(LRUCacheTest, LRUCacheMove_test)
{
    nador::LRUCache<int, int> cache{10};
    cache.Insert(1, 11);
    auto cache2 = std::move(cache);

    EXPECT_EQ(cache2.Get(1), 11);

    EXPECT_THROW(cache.Get(1), nador::LRUCacheException);

    EXPECT_EQ(cache.Size(), 0);
}

TEST(LRUCacheTest, LRUCacheConstructor_test)
{
    auto f = []{ nador::LRUCache<int, int> cache{0}; };
    EXPECT_THROW(f(), nador::LRUCacheException);
}