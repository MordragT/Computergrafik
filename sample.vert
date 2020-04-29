#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 uvCoordinates;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vUvCoordinates;

void main(void)
{
    gl_Position = vec4(aPosition, 1.0);
    vColor = aColor;
    vUvCoordinates = uvCoordinates;
}
