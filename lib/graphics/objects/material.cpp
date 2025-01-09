#include "material.h"

#include <stdio.h>

#include "graphics/gl_debug.h"
#include "logger/logger.h"
#include "time/world_timer.h"

void Material::use() const {
    shader_->use();

    poll_gl_errors();

    uniforms_.upload(*shader_);

    shader_->set_uniform_float("WorldTime", (float)WorldTimer::get_time_sec());

    poll_gl_errors();
}
