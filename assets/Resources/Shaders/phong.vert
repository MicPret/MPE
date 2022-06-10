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

//Input
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;
layout (location = 2) in vec3 in_normal;

//Output
layout (location = 0) out vec3 vx_position;
layout (location = 1) out vec2 vx_texCoords;
layout (location = 2) out vec3 vx_normal;
layout (location = 3) flat out int vx_drawID;

void main()
{
    mat4 model = mesh_data[gl_DrawID].model;
    vx_texCoords = in_texCoords;
    vx_normal = mat3(transpose(inverse(model))) * in_normal;
    vx_drawID = gl_DrawID;
    vx_position = (model * vec4(in_pos, 1.0)).xyz;
    gl_Position = projection * view * vec4(vx_position, 1.0);
}