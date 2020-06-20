#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vTexCoordinates;

layout(location = 0) out vec4 fragColor;

uniform sampler2D uTexture;

void main(void)
{
    vec4 tex = texture(uTexture, vTexCoordinates);
    fragColor = vec4(pow(tex[0], 32), tex[1], tex[2], tex[3]);
    //fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //fragColor = pos;
}
