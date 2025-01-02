#include "level_geometry.h"

#include <math.h>
#include <string.h>

#include <unordered_set>

#include "logger/logger.h"
#include "physics/collider.h"

LevelGeometry::
    LevelGeometry(Box bounding_box, size_t horiz_res, size_t vert_res)
    : colliders_(
          [](const BoxCollider& collider) {
              return collider.get_bounding_box();
          },
          bounding_box,
          bounding_box.get_size() / glm::vec3(horiz_res, vert_res, horiz_res)) {
}

glm::vec3 LevelGeometry::
    get_intersection(const DynamicCollider& collider) const {
    Box object_box = collider.get_bounding_box();

    std::set<BoxCollider> potential_colliders =
        colliders_.find_intersecting(object_box);

    glm::vec3 offset = glm::vec3(0.0, 0.0, 0.0);

    for (const BoxCollider& static_collider : potential_colliders) {
        Intersection intersection = collider.intersect_box(static_collider);

        if (!intersection.overlap) continue;

        offset += intersection.delta;
    }

    return offset;
}

void LevelGeometry::add_collider(const BoxCollider& collider_prototype) {
    Box object_box = collider_prototype.get_bounding_box();
    colliders_.register_object(collider_prototype, object_box);
}
