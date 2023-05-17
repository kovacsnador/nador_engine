#include <typeinfo>

#include <gtest/gtest.h>

#include "nador/common/ThreadPool.h"

struct ThreadPoolTest : public ::testing::TestWithParam<size_t>
{
};

TEST(ThreadPoolTest, Init_Test)
{
    nador::ThreadPool threadPool(std::thread::hardware_concurrency());

    std::atomic_int32_t count { 0 };

    auto future = threadPool.enqueue(
        [&count](auto plus) {
            count += plus;
            return count.load();
        },
        nador::ETaskPriority::MEDIUM,
        2);

    // wait for the thread
    auto result = future.get();

    EXPECT_TRUE(count.load() == 2 && result == count.load());
}

INSTANTIATE_TEST_CASE_P(ThreadPoolTest, ThreadPoolTest, ::testing::Values(1, 10, 100, 1000));

TEST_P(ThreadPoolTest, NoArgumentAndReturn_Test)
{
    nador::ThreadPool threadPool(std::thread::hardware_concurrency());

    std::atomic_size_t count { 0 };

    std::vector<std::future<void>> futures;

    for (size_t i = 0; i < GetParam(); ++i)
    {
        auto future = threadPool.enqueue([&count] { count++; }, nador::ETaskPriority::VERY_HIGH);
        futures.emplace_back(std::move(future));
    }

    // wait for task
    for (auto& it : futures)
    {
        it.get();
    }

    EXPECT_TRUE(count.load() == GetParam());
}

TEST_P(ThreadPoolTest, Destructor_Test)
{
    auto testFuncion = [count = GetParam()] {
        nador::ThreadPool threadPool(std::thread::hardware_concurrency());

        std::atomic_size_t count { 0 };

        std::vector<std::future<void>> futures;

        for (size_t i = 0; i < GetParam(); ++i)
        {
            auto future = threadPool.enqueue([&count] { count++; }, nador::ETaskPriority::VERY_HIGH);
            futures.emplace_back(std::move(future));
        }
    };

    try
    {
        testFuncion();
    }
    catch (...)
    {
        EXPECT_TRUE(false);
    }
}

TEST_P(ThreadPoolTest, Simple_Test)
{
    size_t  nrTask = GetParam();
    int32_t plus   = 2;

    nador::ThreadPool threadPool(std::thread::hardware_concurrency());

    std::atomic_int32_t count { 0 };

    std::vector<std::future<int32_t>> futures;

    for (size_t i = 0; i < nrTask; ++i)
    {
        auto future = threadPool.enqueue(
            [&count](auto p) {
                count += p;
                return p;
            },
            nador::ETaskPriority::HIGH,
            plus);

        futures.emplace_back(std::move(future));
    }

    for (auto& it : futures)
    {
        // wait for the thread
        EXPECT_EQ(it.get(), plus);
    }

    EXPECT_EQ(count.load(), nrTask * plus);
}

TEST(ThreadPoolTest, BatchAndPriority_Test)
{
    nador::ThreadPool threadPool(std::thread::hardware_concurrency());

    std::mutex mtx;
    uint32_t   count { 0 };

    auto function = [&count, &mtx](nador::ETaskPriority prio) {
        std::scoped_lock lock(mtx);
        if (prio == nador::ETaskPriority::VERY_HIGH)
        {
            EXPECT_TRUE(count < 3);
        }
        else if (prio == nador::ETaskPriority::HIGH)
        {
            EXPECT_TRUE(count >= 3 && count < 6);
        }
        else if (prio == nador::ETaskPriority::MEDIUM)
        {
            EXPECT_TRUE(count >= 6 && count < 9);
        }
        else if (prio == nador::ETaskPriority::LOW)
        {
            EXPECT_TRUE(count >= 9 && count < 12);
        }
        else if (prio == nador::ETaskPriority::VERY_LOW)
        {
            EXPECT_TRUE(count >= 12 && count < 15);
        }

        count++;
    };

    std::vector<nador::ThreadPoolTask> batchTasks;

    auto packagedTaskHelper = [&](size_t nr, nador::ETaskPriority prio) {
        for (size_t i = 0; i < nr; ++i)
        {
            auto pTask = nador::CreatePackagedTask(function, prio);
            batchTasks.emplace_back(nador::ThreadPoolTask(std::move(pTask), prio));
        }
    };

    packagedTaskHelper(3, nador::ETaskPriority::VERY_LOW);
    packagedTaskHelper(3, nador::ETaskPriority::LOW);
    packagedTaskHelper(3, nador::ETaskPriority::MEDIUM);
    packagedTaskHelper(3, nador::ETaskPriority::HIGH);
    packagedTaskHelper(3, nador::ETaskPriority::VERY_HIGH);

    threadPool.wait();

    // run tasks
    threadPool.enqueue(batchTasks);
}

TEST(ThreadPoolTest, TaskConsumption_Test)
{
    nador::ThreadPool threadPool(4);

    auto task = [] { std::this_thread::sleep_for(std::chrono::seconds(1)); };

    threadPool.enqueue(task, nador::ETaskPriority::VERY_HIGH);
    threadPool.enqueue(task, nador::ETaskPriority::VERY_HIGH);
    threadPool.enqueue(task, nador::ETaskPriority::VERY_HIGH);
    threadPool.enqueue(task, nador::ETaskPriority::VERY_HIGH);

    bool stuckTaskInQueue { true };

    auto future = threadPool.enqueue([&stuckTaskInQueue] { stuckTaskInQueue = false; }, nador::ETaskPriority::VERY_HIGH);

    future.wait();

    EXPECT_FALSE(stuckTaskInQueue);
}

TEST_P(ThreadPoolTest, Wait_Test)
{
    nador::ThreadPool threadPool(std::thread::hardware_concurrency());

    auto nrTasks = GetParam();

    std::atomic_int32_t count { 0 };

    auto task = [&count] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++count;
    };

    for(size_t i = 0; i < nrTasks; ++i)
    {
        threadPool.enqueue(task, nador::ETaskPriority::VERY_HIGH);
    }

    threadPool.wait();

    EXPECT_EQ(count.load(), nrTasks);
}

TEST(ThreadPoolTest, InvalidArgumentCtor_Test)
{
    try
    {
        nador::ThreadPool threadPool(0);
        FAIL() << "Expected std::invalid_argument";
    }
    catch (const std::invalid_argument& e)
    {
        EXPECT_TRUE(true);
    }
    catch (...)
    {
        FAIL() << "Expected std::invalid_argument";
    }
}