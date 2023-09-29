/**
 * @file murmur.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Murmur hash algorithm
 * @version 0.1
 * @date 2023-07-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MURMUR_HASH_H
#define MURMUR_HASH_H

#include <stdlib.h>

typedef size_t hash_t;

hash_t murmur_hash(const char* string);

#endif
