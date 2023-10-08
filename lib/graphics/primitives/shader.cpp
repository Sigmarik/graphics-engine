#include "shader.h"

#include "graphics/gl_debug.h"
#include "io/mmap.h"
#include "logger/logger.h"

Shader::Shader() {}

Shader::Shader(const char* vsh_name, const char* fsh_name) {
    read(vsh_name, fsh_name);
}

static const size_t LOG_SIZE = 512;

void Shader::read(const char* vsh_name, const char* fsh_name) {
    GLuint vsh = 0, fsh = 0;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);

    poll_gl_errors();

    MmapResult vsh_map = map_file(vsh_name, O_RDONLY, PROT_READ, MAP_PRIVATE);
    MmapResult fsh_map = map_file(fsh_name, O_RDONLY, PROT_READ, MAP_PRIVATE);

    glShaderSource(vsh, 1, (const char**)&vsh_map.ptr, NULL);
    glShaderSource(fsh, 1, (const char**)&fsh_map.ptr, NULL);

    poll_gl_errors();

    glCompileShader(vsh);
    glCompileShader(fsh);

    poll_gl_errors();

    close(vsh_map.fd);
    close(fsh_map.fd);

    int success = 0;
    static char shader_log[LOG_SIZE] = "";

    glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vsh, LOG_SIZE, NULL, shader_log);
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load vertex shader %s.\nError:\n%s\n", vsh_name,
                   shader_log);
    }

    glGetShaderiv(fsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fsh, LOG_SIZE, NULL, shader_log);
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load fragment shader %s.\nError:\n%s\n", fsh_name,
                   shader_log);
    }

    poll_gl_errors();

    id_ = glCreateProgram();

    poll_gl_errors();

    glAttachShader(id_, vsh);
    glAttachShader(id_, fsh);

    poll_gl_errors();

    glLinkProgram(id_);

    poll_gl_errors();

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id_, LOG_SIZE, NULL, shader_log);
        log_printf(ERROR_REPORTS, "error",
                   "Failed to link shaders %s (vertex) and %s "
                   "(fragment).\nError:\n%s\n",
                   vsh_name, fsh_name, shader_log);
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);

    poll_gl_errors();
}

void Shader::set_uniform_float(const char* uniform, float value) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform1f(uni, value);
}

void Shader::set_uniform_vec2(const char* uniform, const glm::vec2& vector) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform2f(uni, vector.x, vector.y);
}

void Shader::set_uniform_vec3(const char* uniform, const glm::vec3& vector) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform2f(uni, vector.x, vector.y);
}

void Shader::set_uniform_mat4(const char* uniform, const glm::mat4& matrix) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniformMatrix4fv(uni, 1, false, &matrix[0][0]);
}

void Shader::set_uniform_img(const char* uniform, const Texture& texture) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform1i(uni, texture.get_slot());
}
