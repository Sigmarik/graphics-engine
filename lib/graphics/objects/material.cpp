#include "material.h"

#include <stdio.h>

void Material::use() const {
    static char tex_uniform_name[32] = "";

    shader_.use();
    for (unsigned id = 0; id < TEXTURE_SLOTS_COUNT; ++id) {
        if (textures_[id] == nullptr) continue;

        textures_[id].bind();

        sprintf(tex_uniform_name, "tex%u", id);

        shader_.set_uniform_tex(tex_uniform_name, *textures_[id]);
    }
}
