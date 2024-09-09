/**
 * @file box_search.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Box field & tree data structure
 * @version 0.1
 * @date 2024-09-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <math.h>

#include <functional>
#include <map>
#include <set>
#include <vector>

#include "logger/logger.h"
#include "primitives.h"

template <class T>
struct BoxField {
    BoxField(const Box& boundary, const glm::vec3& cell_size);

    void register_object(const T& object, const Box& box);
    void move(const T& object, const Box& new_box);
    void unregister(const T& object);

    std::set<T> find_intersecting(const Box& box,
                                  IntersectionType intersection) const;

    Box get_boundary() const { return boundary_; }

   private:
    struct Container {
        Container(const Box& bounds = Box()) : box(bounds) {}

        Box box;
        std::set<T> content{};
    };

    Container& at(size_t id_x, size_t id_y, size_t id_z);
    const Container& at(size_t id_x, size_t id_y, size_t id_z) const;

    void find_index(const glm::vec3& point, size_t& id_x, size_t& id_y,
                    size_t& id_z) const;

    void for_cell_in_box(const Box& box,
                         std::function<void(Container&)> function);

    void for_cell_in_box(const Box& box,
                         std::function<void(const Container&)> function) const;

    std::map<T, Box> object_boxes_{};

    Box boundary_;
    size_t count_x_, count_y_, count_z_;
    glm::vec3 cell_size_;

    std::vector<Container> field_;

    Container outside_{Box(glm::vec3(INFINITY), glm::vec3(0.0))};
};

template <class T>
inline BoxField<T>::BoxField(const Box& boundary, const glm::vec3& cell_size)
    : boundary_(boundary),
      count_x_((size_t)ceil(boundary_.get_size().x / cell_size.x)),
      count_y_((size_t)ceil(boundary_.get_size().y / cell_size.y)),
      count_z_((size_t)ceil(boundary_.get_size().z / cell_size.z)),
      cell_size_(boundary_.get_size() / glm::
                                            vec3(count_x_, count_y_, count_z_)),
      field_(count_x_ * count_y_ * count_z_, Container()) {
    // clang-format off
    for (size_t id_x = 0; id_x < count_x_; ++id_x) {
    for (size_t id_y = 0; id_y < count_y_; ++id_y) {
    for (size_t id_z = 0; id_z < count_z_; ++id_z) {
        glm::vec3 index = glm::vec3(id_x, id_y, id_z);

        glm::vec3 origin = boundary.get_center() -
                            boundary.get_size() / 2.0f +
                            (index + 0.5f) * cell_size_;

        at(id_x, id_y, id_z).box = Box(origin, cell_size_);
    }
    }
    }
    // clang-format on
}

template <class T>
inline void BoxField<T>::register_object(const T& object, const Box& box) {
    for_cell_in_box(
        box, [&](Container& container) { container.content.insert(object); });

    object_boxes_[object] = box;
}

template <class T>
inline void BoxField<T>::move(const T& object, const Box& new_box) {
    auto found = object_boxes_.find(object);
    if (found == object_boxes_.end()) {
        log_printf(
            ERROR_REPORTS, "error",
            "Attempting to move an unregistered object in a box field.\n");
        return;
    }

    Box old_box = found->second;

    if (old_box == new_box) return;

    for_cell_in_box(old_box, [&](Container& container) {
        if (!intersect(container.box, new_box)) {
            container.content.erase(object);
        }
    });

    for_cell_in_box(new_box, [&](Container& container) {
        if (!intersect(container.box, old_box)) {
            container.content.insert(object);
        }
    });

    object_boxes_[object] = new_box;
}

template <class T>
inline void BoxField<T>::unregister(const T& object) {
    auto found = object_boxes_.find(object);
    if (found == object_boxes_.end()) {
        log_printf(
            ERROR_REPORTS, "error",
            "Attempting to unregister a non-existent object in a box field.\n");
        return;
    }

    Box old_box = found->second;

    for_cell_in_box(old_box, [&](Container& container) {
        container.content.erase(object);
    });

    object_boxes_.erase(found);
}

template <class T>
inline std::set<T> BoxField<T>::
    find_intersecting(const Box& box, IntersectionType intersection) const {
    std::set<T> result;

    for_cell_in_box(box, [&, this](const Container& container) {
        for (const T& object : container.content) {
            if (result.contains(object)) continue;

            Box other_box = object_boxes_.at(object);

            if (match_intersection(box, other_box, intersection)) {
                result.insert(object);
            }
        }
    });

    return result;
}

#define __BOX_SEARCH__FOR_CELL_IN_BOX_IMPL()                                   \
    do {                                                                       \
        size_t start_x = 0, start_y = 0, start_z = 0;                          \
        size_t end_x = 0, end_y = 0, end_z = 0;                                \
                                                                               \
        find_index(box.get_center() - box.get_size() / 2.0f, start_x, start_y, \
                   start_z);                                                   \
                                                                               \
        find_index(box.get_center() + box.get_size() / 2.0f, end_x, end_y,     \
                   end_z);                                                     \
                                                                               \
        for (size_t id_x = start_x; id_x <= end_x; ++id_x) {                   \
            for (size_t id_y = start_y; id_y <= end_y; ++id_y) {               \
                for (size_t id_z = start_z; id_z <= end_z; ++id_z) {           \
                    function(at(id_x, id_y, id_z));                            \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (!boundary_.contains(box)) {                                        \
            function(outside_);                                                \
        }                                                                      \
    } while (0)

template <class T>
inline BoxField<T>::Container& BoxField<T>::
    at(size_t id_x, size_t id_y, size_t id_z) {
    return field_[id_x * count_y_ * count_z_ + id_y * count_z_ + id_z];
}

template <class T>
inline const BoxField<T>::Container& BoxField<T>::
    at(size_t id_x, size_t id_y, size_t id_z) const {
    return field_[id_x * count_y_ * count_z_ + id_y * count_z_ + id_z];
}

static size_t clamp_and_floor(double value, size_t low, size_t high) {
    if (value < (double)low) return low;
    if (value >= (double)high) return high - 1;
    return (size_t)floor(value);
}

template <class T>
inline void BoxField<T>::find_index(const glm::vec3& point, size_t& id_x,
                                    size_t& id_y, size_t& id_z) const {
    glm::vec3 relative = point - boundary_.get_center();
    glm::vec3 from_corner = relative + boundary_.get_size() / 2.0f;
    glm::vec3 indices = from_corner / cell_size_;

    id_x = clamp_and_floor(indices.x, 0, count_x_);
    id_y = clamp_and_floor(indices.y, 0, count_y_);
    id_z = clamp_and_floor(indices.z, 0, count_z_);
}

template <class T>
inline void BoxField<T>::
    for_cell_in_box(const Box& box, std::function<void(Container&)> function) {
    __BOX_SEARCH__FOR_CELL_IN_BOX_IMPL();
}

template <class T>
inline void BoxField<T>::
    for_cell_in_box(const Box& box,
                    std::function<void(const Container&)> function) const {
    __BOX_SEARCH__FOR_CELL_IN_BOX_IMPL();
}
