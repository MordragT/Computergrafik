
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout (location = 4) uniform vec3 uBaseColor;

// Location 0 = Output written to screen
layout (location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(uBaseColor, 1.0);
}
