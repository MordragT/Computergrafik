#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexPosition;

layout (location = 0) uniform mat4 uMvp;
layout (location = 1) uniform mat4 uModel;
layout (location = 2) uniform mat4 uModelTransposed;

layout (location = 0) out vec3 vFragPos;
layout (location = 1) out vec3 vNormal;
layout (location = 2) out vec2 vTexPosition;

void main() {
    vTexPosition = aTexPosition;

    // Normalize and pass on normals
    vNormal = normalize(vec3(uModelTransposed * vec4(aNormal, 1.0)));

    // Calculate and normalize fragment position
    //vFragPos = normalize( aPosition * uModelMatrix );
    vFragPos = vec3(vec4(aPosition, 1.0) * uModel);
    gl_Position = uMvp * vec4(aPosition, 1.0);
}

