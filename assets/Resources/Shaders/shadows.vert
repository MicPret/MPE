#version 460 core

layout (location = 0) in vec3 aPos;

//Structs
struct VertexMeshData
{
    int texture_index0;
    int texture_index1;
    int texture_index2;
    int texture_index3;
    int texture_index4;
    int texture_index5;
    int texture_index6;
    int texture_index7;

    float float0;
    float float1;
    float float2;
    float float3;
    float float4;
    float float5;
    float float6;
    float float7;

    vec4 vecs[8];

    mat4 model;
};

//Uniforms
layout (std140, binding = 0) uniform CameraData
{
    mat4 projection;
    mat4 view;
    vec4 camera_pos;
    float near;
    float far;
};

layout (std140, binding = 1) uniform MeshData
{
    VertexMeshData mesh_data[64];
};

void main()
{
    gl_Position = mesh_data[gl_DrawID].model * vec4(aPos, 1.0);
}