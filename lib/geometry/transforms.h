/**
 * @file transforms.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Geometrical transforms
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

glm::vec3 reflect_plane(const glm::vec3& vector, const glm::vec3& normal);

glm::vec3 to_point(const glm::mat4& transform);
