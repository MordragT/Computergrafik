
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vTexPosition;

uniform vec4 uRingColor;
uniform sampler2D uTexture;

// Location 0 = Output written to screen
layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = texture(uTexture, vTexPosition) * uRingColor;
}
