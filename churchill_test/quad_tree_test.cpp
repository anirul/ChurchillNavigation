#include "quad_tree_test.h"

#include "../churchill_library/quad_tree.h"

namespace test {

    TEST_F(QuadTreeTest, Simple) {
        QuadTree qt({ -1e10, -1e10, 1e10, 1e10 });
        qt.Insert(Point{ 1, 2, 1.0, 1.0 });
        qt.Insert(Point{ 2, 3, 2.0, 2.0 });
        qt.Insert(Point{ 3, 4, 3.0, 3.0 });
        qt.Insert(Point{ 4, 5, 4.0, 4.0 });
        qt.Insert(Point{ 5, 6, 5.0, 5.0 });
        PriorityList pl(4);
        qt.Query({ -1, -1, 1.5, 1.5 }, pl);
        EXPECT_EQ(pl.Size(), 1);
    }

    TEST_F(QuadTreeTest, Second) {
        QuadTree qt({ -1e10, -1e10, 1e10, 1e10 });
        for (int i = 0; i < 10'000; ++i) {
            qt.Insert(
                Point{
                    static_cast<int8_t>(i),
                    i,
                    static_cast<float>(i) * 0.1f,
                    static_cast<float>(i) * 0.1f
                });
        }
        PriorityList pl(4);
        qt.Query({ -1, -1, 1.5, 1.5 }, pl);
        EXPECT_EQ(pl.Size(), 4);
        for (auto it = pl.Begin(); it != pl.End(); ++it) {
            EXPECT_LT(-1, it->rank);
            EXPECT_GT(4, it->rank);
        }
    }

}