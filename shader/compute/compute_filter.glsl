#version 430 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

// Ignore the following line, it 's not relevant for now
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(binding = 0, rgba8) restrict uniform image2D someImage;

void main() {

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = imageLoad(someImage,pos);

    color = color * vec4(1.0, 0.0, 1.0, 1.0);

    imageStore(someImage, pos, color);
}
