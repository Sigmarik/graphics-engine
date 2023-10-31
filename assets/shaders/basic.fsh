#version 330 core

uniform sampler2D albedo;

in vec3 local_pos;
in vec3 world_pos;
in vec4 normal_depth;
in vec2 uv;

layout(location = 0) out vec4 AlbedoOutput;
layout(location = 1) out vec4 NormalDepthOutput;
layout(location = 2) out vec4 SurfaceOutput;
layout(location = 3) out vec4 FinalColor;

void main() {
    AlbedoOutput = vec4(texture(albedo, uv).xyz, 1.0);
    NormalDepthOutput = vec4(0.5 + normal_depth.xyz * 0.5,
                             gl_FragCoord.z);  // gl_FragCoord.z
    SurfaceOutput = vec4(0.1, 0.3, 1.0, 1.0);  // Roughness, Specular, AO
    FinalColor = vec4(0.0, 0.0, 0.0, 1.0);
}
