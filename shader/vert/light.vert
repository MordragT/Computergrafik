
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPosition;

layout (location = 0) uniform mat4 uMvp;

void main() {

    gl_Position = uMvp * vec4(aPosition, 1.0);

}

