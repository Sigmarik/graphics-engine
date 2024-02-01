#include "level_geometry.h"

#include <math.h>
#include <string.h>

#include <unordered_set>

#include "logger/logger.h"
#include "physics/collider.h"

LevelGeometry::LevelGeometry(Box bounding_box, size_t horiz_res,
                             size_t vert_res)
    : horiz_res_(horiz_res),
      vert_res_(vert_res),
      boundary_(bounding_box),
      cells_(new LevelGeometry::LevelCell[horiz_res * horiz_res * vert_res]) {}

LevelGeometry::LevelGeometry(const LevelGeometry& geometry)
    : horiz_res_(geometry.horiz_res_),
      vert_res_(geometry.vert_res_),
      boundary_(geometry.boundary_),
      cells_(
          new LevelGeometry::LevelCell[horiz_res_ * horiz_res_ * vert_res_]) {
    for (const BoxCollider& collider : geometry.colliders_) {
        add_collider(collider);
    }
}

LevelGeometry::~LevelGeometry() { delete[] cells_; }

glm::vec3 LevelGeometry::get_intersection(
    const DynamicCollider& collider) const {
    Box object_box = collider.get_bounding_box();

    static std::unordered_set<const BoxCollider*> potential_colliders;

    potential_colliders.clear();

    if (!boundary_.contains(object_box)) {
        potential_colliders.insert(global_colliders_.begin(),
                                   global_colliders_.end());
    }

    IndexBox indices = find_indices(object_box);

    // clang-format off
    for (size_t id_z = indices.low_z; id_z <= indices.high_z; ++id_z) {
    for (size_t id_y = indices.low_y; id_y <= indices.high_y; ++id_y) {
    for (size_t id_x = indices.low_x; id_x <= indices.high_x; ++id_x) {
        const LevelCell& cell = cells_[get_cell_id(id_x, id_y, id_z)];

        potential_colliders.insert(cell.colliders.begin(), 
                                   cell.colliders.end());
    }}}
    // clang-format on

    glm::vec3 offset = glm::vec3(0.0, 0.0, 0.0);

    for (const BoxCollider* box : potential_colliders) {
        Intersection intersection = collider.intersect_box(*box);

        if (!intersection.overlap) continue;

        offset += intersection.delta;
    }

    return offset;
}

void LevelGeometry::add_collider(const BoxCollider& collider_prototype) {
    colliders_.push_back(collider_prototype);

    const BoxCollider& collider = colliders_[colliders_.size() - 1];

    Box object_box = collider.get_bounding_box();

    IndexBox indices = find_indices(object_box);

    // clang-format off
    for (size_t id_x = indices.low_x; id_x <= indices.high_x; ++id_x) {
    for (size_t id_y = indices.low_y; id_y <= indices.high_y; ++id_y) {
    for (size_t id_z = indices.low_z; id_z <= indices.high_z; ++id_z) {
        cells_[get_cell_id(id_x, id_y, id_z)].colliders.push_back(&collider);
    }}}
    // clang-format on

    if (!boundary_.contains(object_box)) {
        global_colliders_.push_back(&collider);
    }
}

size_t clamp(long int index, size_t size) {
    if (index < 0) return 0;
    if ((size_t)index >= size) return size - 1;

    return (size_t)index;
}

LevelGeometry::IndexBox LevelGeometry::find_indices(const Box& box) const {
    IndexBox indices = {};

    glm::vec3 low_pos = box.local(glm::vec3(-1.0, -1.0, -1.0));
    glm::vec3 high_pos = box.local(glm::vec3(1.0, 1.0, 1.0));

    glm::vec3 rel_low = low_pos - boundary_.local(glm::vec3(-1.0, -1.0, -1.0));
    glm::vec3 rel_high =
        high_pos - boundary_.local(glm::vec3(-1.0, -1.0, -1.0));

    rel_low /= boundary_.get_size();
    rel_high /= boundary_.get_size();

    long int x_low = (long int)floor(rel_low.x * (float)horiz_res_);
    long int y_low = (long int)floor(rel_low.y * (float)vert_res_);
    long int z_low = (long int)floor(rel_low.z * (float)horiz_res_);

    long int x_high = (long int)ceil(rel_high.x * (float)horiz_res_);
    long int y_high = (long int)ceil(rel_high.y * (float)vert_res_);
    long int z_high = (long int)ceil(rel_high.z * (float)horiz_res_);

    indices.low_x = clamp(x_low, horiz_res_);
    indices.low_y = clamp(y_low, vert_res_);
    indices.low_z = clamp(z_low, horiz_res_);

    indices.high_x = clamp(x_high, horiz_res_);
    indices.high_y = clamp(y_high, vert_res_);
    indices.high_z = clamp(z_high, horiz_res_);

    return indices;
}

size_t LevelGeometry::get_cell_id(size_t id_x, size_t id_y, size_t id_z) const {
    return id_y * horiz_res_ * horiz_res_ + id_z * horiz_res_ + id_x;
}
