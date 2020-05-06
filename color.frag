#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vUvCoordinates;

layout(location = 0) out vec4 fragColor;

uniform float uAlpha;
uniform sampler2D tex;
uniform float uvCoordinatesAdd;

void main(void)
{
    fragColor = vec4(vColor, uAlpha);
}
