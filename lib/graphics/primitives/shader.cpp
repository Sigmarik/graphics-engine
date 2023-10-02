#include "shader.h"

#include "io/mmap.h"
#include "logger/logger.h"

Shader::Shader() {}

Shader::Shader(const char* vsh_name, const char* fsh_name) {
    read(vsh_name, fsh_name);
}

Shader::~Shader() { glDeleteProgram(id_); }

static const size_t LOG_SIZE = 512;

void Shader::read(const char* vsh_name, const char* fsh_name) {
    GLuint vsh = 0, fsh = 0;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_VERTEX_SHADER);

    MmapResult vsh_map = map_file(vsh_name, O_RDONLY, PROT_READ, MAP_PRIVATE);
    MmapResult fsh_map = map_file(fsh_name, O_RDONLY, PROT_READ, MAP_PRIVATE);

    glShaderSource(vsh, 1, (const char**)&vsh_map.ptr, NULL);
    glShaderSource(fsh, 1, (const char**)&fsh_map.ptr, NULL);

    glCompileShader(vsh);
    glCompileShader(fsh);

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

    id_ = glCreateProgram();
    glAttachShader(id_, vsh);
    glAttachShader(id_, fsh);
    glLinkProgram(id_);

    glGetProgramiv(fsh, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(fsh, LOG_SIZE, NULL, shader_log);
        log_printf(ERROR_REPORTS, "error",
                   "Failed to link shaders %s (vertex) and %s "
                   "(fragment).\nError:\n%s\n",
                   vsh_name, fsh_name, shader_log);
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);
}

void Shader::setUniformFloat(const char* uniform, float value) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform1f(uni, value);
}

void Shader::setUniformVec2(const char* uniform, const glm::vec2& vector) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform2f(uni, vector.x, vector.y);
}

void Shader::setUniformVec3(const char* uniform, const glm::vec3& vector) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniform2f(uni, vector.x, vector.y);
}

void Shader::setUniformMat4(const char* uniform, const glm::mat4& matrix) {
    GLint uni = glGetUniformLocation(id_, uniform);
    glUniformMatrix4fv(uni, 1, false, &matrix[0][0]);
}
