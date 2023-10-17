#include "../lib/data_structures/ptr_map.hpp"

TEST(PtrMap, PushPop) {
    PtrMap<int> map;

    static int* elements[1000];
    static bool listed[1000];

    for (int id = 0; id < 1000; ++id) {
        elements[id] = new int;
        map.push(*elements[id]);
    }

    EXPECT_EQ(map.size(), 1000);

    for (PtrMapIterator<int> iter(map); iter.has_next(); iter++) {
        listed[iter.get()] = true;
    }

    for (unsigned id = 0; id < 1000; ++id) {
        EXPECT_EQ(listed[id], true);
    }
}
