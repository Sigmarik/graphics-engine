/**
 * @file material.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Materials
 * @version 0.1
 * @date 2023-10-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MATERIAL_H
#define MATERIAL_H

enum RenderPass {
    RP_OPAQUE,
    RP_TRANSLUCENT,
    RP_LIGHT_MAP,
    RP_GB_POS,
    RP_GB_NORMAL,
    RP_GB_ALBEDO,
}

#endif
