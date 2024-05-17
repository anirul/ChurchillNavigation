#include "grid_storage_test.h"

#include "../churchill_library/grid_storage.h"

namespace test {

    TEST_F(GridStorageTest, Simple) {
        GridStorage gs(16, 16);
        gs.Insert(Point{ 1, 2, 1.0, 1.0 });
        gs.Insert(Point{ 2, 3, 2.0, 2.0 });
        gs.Insert(Point{ 3, 4, 3.0, 3.0 });
        gs.Insert(Point{ 4, 5, 4.0, 4.0 });
        gs.Insert(Point{ 5, 6, 5.0, 5.0 });
        gs.Build();
        PriorityList pl(4);
        gs.Query({ -1, -1, 1.5, 1.5 }, pl);
        EXPECT_EQ(pl.Size(), 1);
    }

    TEST_F(GridStorageTest, Second) {
        GridStorage gs(16, 16);
        for (int i = 0; i < 10'000; ++i) {
            gs.Insert(
                Point{
                    static_cast<int8_t>(i),
                    i,
                    static_cast<float>(i) * 0.1f,
                    static_cast<float>(i) * 0.1f
                });
        }
        gs.Build();
        PriorityList pl(4);
        gs.Query({ -1, -1, 1.5, 1.5 }, pl);
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_LT(-1, it->rank);
            EXPECT_GT(4, it->rank);
        }
    }

}