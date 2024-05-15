#include "priority_queue_test.h"

#include "../churchill_library/priority_queue.h"

namespace test {

    TEST_F(PriorityQueueTest, PriorityQueueTest_Simple) {
        PriorityList pl(4);
        pl.Insert(Point{ 1, 2, 1.0, 1.0 });
        EXPECT_EQ(pl.Size(), 1);
        pl.Insert(Point{ 2, 3, 2.0, 2.0 });
        EXPECT_EQ(pl.Size(), 2);
        pl.Insert(Point{ 3, 4, 3.0, 3.0 });
        EXPECT_EQ(pl.Size(), 3);
        pl.Insert(Point{ 4, 5, 4.0, 4.0 });
        EXPECT_EQ(pl.Size(), 4);
        pl.Insert(Point{ 5, 6, 5.0, 5.0 });
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_NE(it->rank, 2);
        }
        pl.Insert(Point{ 6, 1, 6.0, 6.0 });
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_NE(it->rank, 2);
        }
    }

    TEST_F(PriorityQueueTest, PriorityQueueTest_Second) {
        PriorityList pl(4);
        for (int i = 0; i < 100; ++i) {
            pl.Insert(Point{ 
                static_cast<int8_t>(i), 
                i, 
                static_cast<float>(i), 
                static_cast<float>(i) });
        }
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_LT(95, it->rank);
        }
        for (int i = 0; i < 500; ++i) {
            pl.Insert(Point{ 
                static_cast<int8_t>(i), 
                1000 - i, 
                static_cast<float>(i), 
                static_cast<float>(i) });
        }
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_LT(995, it->rank);
        }
    }

}