#version 460 core

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
layout (std140, binding = 1) uniform MeshData
{
    VertexMeshData mesh_data[64];
};

layout (std140, binding = 0) uniform CameraData
{
    mat4 projection;
    mat4 view;
    vec4 camera_pos;
    float near;
    float far;
};

//Input
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;

//Output
layout (location = 0) out vec2 vx_texCoords;
layout (location = 1) flat out int vx_drawID;

void main()
{
    vx_texCoords = in_texCoords;
    vx_drawID = gl_DrawID;

    gl_Position = projection * view * mesh_data[gl_DrawID].model * vec4(in_pos, 1.0);
}