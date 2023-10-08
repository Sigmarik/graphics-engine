/**
 * @file gl_debug.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Graphics debugging
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GL_DEBUG_H
#define GL_DEBUG_H

#include "graphics/libs.h"
#include "logger/logger.h"

#ifdef _DEBUG
#define poll_gl_errors()                                                 \
    do {                                                                 \
        GLenum error_code = GL_NO_ERROR;                                 \
        while ((error_code = glGetError()) != GL_NO_ERROR) {             \
            const char* error = "";                                      \
            switch (error_code) {                                        \
                case GL_INVALID_ENUM:                                    \
                    error = "INVALID_ENUM";                              \
                    break;                                               \
                case GL_INVALID_VALUE:                                   \
                    error = "INVALID_VALUE";                             \
                    break;                                               \
                case GL_INVALID_OPERATION:                               \
                    error = "INVALID_OPERATION";                         \
                    break;                                               \
                case GL_OUT_OF_MEMORY:                                   \
                    error = "OUT_OF_MEMORY";                             \
                    break;                                               \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                   \
                    error = "INVALID_FRAMEBUFFER_OPERATION";             \
                    break;                                               \
                default:                                                 \
                    error = "UNKNOWN_ERROR";                             \
                    break;                                               \
            }                                                            \
            log_printf(ERROR_REPORTS, "error", "GL error: %s\n", error); \
        }                                                                \
    } while (0)
#else
#define poll_gl_errors \
    do {               \
    } while (0)
#endif

#endif
