/**
 * @file box_field.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Box search data structure tests
 * @version 0.1
 * @date 2024-09-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "geometry/box_field.hpp"

using vec3 = glm::vec3;

TEST(BoxField, Basics) {
    BoxField<int> field(Box(vec3(0.0), vec3(10.0)), vec3(1.0));

    field.register_object(42, Box(vec3(2.5), vec3(5.0)));

    std::set<int> hit = field.find_intersecting(Box(vec3(0.0), vec3(1.0)),
                                                IntersectionType::OVERLAP);

    EXPECT_EQ(hit.size(), 1);
    EXPECT_EQ(*hit.begin(), 42);

    std::set<int> miss = field.find_intersecting(
        Box(vec3(0.0, -2.0, 0.0), vec3(1.0)), IntersectionType::OVERLAP);

    EXPECT_EQ(miss.size(), 0);
}

TEST(BoxField, IntersectionTypes) {
    BoxField<int> field(Box(vec3(0.0), vec3(20.0)), vec3(3.0));

    field.register_object(42, Box(vec3(0.0), vec3(2.0)));

    std::set<int> hit = field.find_intersecting(Box(vec3(0.5), vec3(0.1f)),
                                                IntersectionType::UNDERSET);

    EXPECT_EQ(hit.size(), 1);
    EXPECT_EQ(*hit.begin(), 42);

    std::set<int> miss = field.find_intersecting(Box(vec3(0.5), vec3(0.1f)),
                                                 IntersectionType::OVERSET);

    EXPECT_EQ(miss.size(), 0);
}

TEST(BoxField, Movement) {
    BoxField<int> field(Box(vec3(0.0), vec3(20.0)), vec3(3.0));

    field.register_object(42, Box(vec3(0.0), vec3(1.0)));

    std::set<int> hit = field.find_intersecting(Box(vec3(0.5), vec3(0.1f)),
                                                IntersectionType::OVERLAP);

    EXPECT_EQ(hit.size(), 1);
    EXPECT_EQ(*hit.begin(), 42);

    field.move(42, Box(vec3(3.0), vec3(1.0)));

    hit = field.find_intersecting(Box(vec3(3.5), vec3(0.1f)), IntersectionType::
                                                                  OVERLAP);

    EXPECT_EQ(hit.size(), 1);
    EXPECT_EQ(*hit.begin(), 42);

    std::set<int> miss = field.find_intersecting(Box(vec3(0.5), vec3(0.1f)),
                                                 IntersectionType::OVERLAP);

    EXPECT_EQ(miss.size(), 0);
}

static double rand_double() { return (double)rand() * 2.0 / RAND_MAX - 1.0; }

static Box rand_box() {
    vec3 center = vec3(rand_double(), rand_double(), rand_double()) * 12.0f;
    vec3 size =
        (vec3(rand_double(), rand_double(), rand_double()) + vec3(1.0)) * 5.0f;
    return Box(center, size);
}

static void print(const Box& box) {
    printf("Box:\tcenter(%lg, %lg, %lg),\tsize(%lg, %lg, %lg),",
           box.get_center().x, box.get_center().y, box.get_center().z,
           box.get_size().x, box.get_size().y, box.get_size().z);

    vec3 from = box.get_center() - box.get_size() / 2.0f;
    vec3 to = box.get_center() + box.get_size() / 2.0f;

    printf("\tfrom-to(%lg - %lg,\t%lg - %lg,\t%lg - %lg)\n", from.x, to.x,
           from.y, to.y, from.z, to.z);
}

TEST(BoxField, Stress) {
    BoxField<size_t> field(Box(vec3(0.0), vec3(20.0, 15.0, 10.0)),
                           vec3(3.0, 4.0, 5.0));

    const size_t REQUEST_COUNT = 1024;
    const size_t BOX_COUNT = 1024;

    std::vector<Box> boxes(BOX_COUNT, Box());

    for (size_t id = 0; id < boxes.size(); ++id) {
        Box box = rand_box();

        boxes[id] = box;
        field.register_object(id, box);
    }

    for (size_t id = 0; id < REQUEST_COUNT; ++id) {
        Box request = rand_box();

        std::set<size_t> result =
            field.find_intersecting(request, IntersectionType::OVERLAP);

        for (size_t box_id = 0; box_id < boxes.size(); ++box_id) {
            if (!intersect(boxes[box_id], request)) continue;
            EXPECT_TRUE(result.contains(box_id));

            if (!result.contains(box_id)) {
                printf("Request: ");
                print(request);
                printf("Box: ");
                print(boxes[box_id]);
            }

            result.erase(box_id);
        }

        EXPECT_TRUE(result.empty());
    }

    std::vector<Box> history = boxes;

    for (size_t id = 0; id < boxes.size(); ++id) {
        Box box = rand_box();

        boxes[id] = box;
        field.move(id, box);
    }

    for (size_t id = 0; id < REQUEST_COUNT; ++id) {
        Box request = rand_box();

        std::set<size_t> result =
            field.find_intersecting(request, IntersectionType::OVERLAP);

        for (size_t box_id = 0; box_id < boxes.size(); ++box_id) {
            if (!intersect(boxes[box_id], request)) continue;

            EXPECT_TRUE(result.contains(box_id));

            if (!result.contains(box_id)) {
                printf("Request:\t");
                print(request);
                printf("Current:\t");
                print(boxes[box_id]);
                printf("History:\t");
                print(history[box_id]);
            }

            result.erase(box_id);
        }

        EXPECT_TRUE(result.empty());
    }
}
