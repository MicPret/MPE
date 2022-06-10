#version 460 core

//Structs
struct FragmentMeshData
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
uniform sampler2D textures[8];

layout (std140, binding = 1) uniform MeshData
{
    FragmentMeshData mesh_data[64];
};
/*
layout (std140) LightData
{
};
*/

//Input
layout (location = 0) in vec2 vx_texCoords;
layout (location = 1) flat in int vx_drawID;

//Output
out vec4 result;

void main()
{
    int index = mesh_data[vx_drawID].texture_index0;
    result = texture(textures[index], vx_texCoords);
}