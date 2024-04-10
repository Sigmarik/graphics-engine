/**
 * @file scene_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scene importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "external_level.h"
#include "managers/importer.h"

XML_BASED_IMPORTER(ExternalLevel, "level") {
    // TODO: Implement

    return new Asset<ExternalLevel>(ExternalLevel::Factory());
}
