/**
 * @file component_importer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Helper header for component importers
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "external_level.h"
#include "geometry/transforms.h"
#include "managers/importer.h"
#include "xml/data_extractors.h"

using Factory = ExternalLevel::Factory;
using Producer = Factory::Producer;

#define PRODUCER \
    [=](const glm::mat4& parent_tform) -> Subcomponent<SceneComponent>
