/**
 * @file generic_meta_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief GenericMeta importer
 * @version 0.1
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "generic_meta.h"
#include "managers/importer.h"

XML_BASED_IMPORTER(ExternalLevel::Metadata, "generic_meta") {
    return new Asset<GenericMeta>(data);
}
