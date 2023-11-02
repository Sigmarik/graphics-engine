#include "material.h"

#include <stdio.h>

#include "graphics/gl_debug.h"
#include "logger/logger.h"
#include "managers/world_timer.h"

void Material::use() const {
    shader_.use();

    poll_gl_errors();

    for (auto pair : textures_) {
        pair.second->bind();
        shader_.set_uniform_tex(pair.first.data(), *pair.second);
    }

    shader_.set_uniform_float("WorldTime", WorldTimer::get_time_sec());

    poll_gl_errors();
}

void Material::add_texture(const char* uniform, Texture* texture) {
    auto cell = textures_.find(uniform);

    if (cell != textures_.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Material uniform \"%s\" was set twice\n", uniform);
        return;
    }

    textures_.insert({uniform, texture});
}

Texture* Material::get_texture(const char* uniform) const {
    auto cell = textures_.find(uniform);

    if (cell == textures_.end()) return nullptr;

    return cell->second;
}
