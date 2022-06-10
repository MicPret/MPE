#version 460 core

in vec4 frag_pos;

//Uniforms
layout (std140, binding = 0) uniform CameraData
{
    mat4 projection;
    mat4 view;
    vec4 camera_pos;
    float near;
    float far;
};
uniform vec3 light_pos;

void main()
{
    float light_dist = distance(frag_pos.xyz, light_pos);
    
    light_dist /= far;
    
    gl_FragDepth = light_dist;
}