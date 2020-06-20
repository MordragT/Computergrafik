#version 430 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(location = 0) uniform float[15] uGaussArray;
layout(location = 15) uniform int uNumber;
layout(location = 16) uniform bool uHorizontal;

layout(binding = 0, rgba8) restrict uniform image2D colorTex;
layout(binding = 1, rgba8) restrict uniform image2D tmpTex;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(tmpTex);

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    if (uHorizontal) {
        for(int i = -uNumber/2; i <= uNumber/2; i += 1) {
            if(pos.x + i >= 0 && pos.x + i <= size.x)
                color += imageLoad(tmpTex, ivec2(pos.x + i, pos.y)) * uGaussArray[i + uNumber/2];
        }
    } else {
        for(int i = -uNumber/2; i <= uNumber/2; i += 1) {
            if(pos.y + i >= 0 && pos.y + i <= size.y)
                color += imageLoad(tmpTex, ivec2(pos.x, pos.y + i)) * uGaussArray[i + uNumber/2];
        }
    }
    imageStore(colorTex, pos, color);
}

