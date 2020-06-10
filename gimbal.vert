
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 normalMap;
layout(location = 2) in vec2 texPosition;

uniform mat4x4 uMvp;

layout(location = 0) out vec2 vTexPosition;

void main() {

    vTexPosition = texPosition;
    gl_Position = uMvp * vec4(aPosition, 1.0);

}

