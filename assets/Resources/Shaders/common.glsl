#version 460 core
#define MPE_GLSL_VERSION_DEFINED

//Bindings
#define MPE_CAMERA_BINDING 0
#define MPE_MESH_BINDING 1
#define MPE_LIGHT_BINDING 2

//VERTEX defines
#define MODEL_MATRIX mesh_data[gl_DrawID].model
//FRAGMENT defines
#define DIFFUSE_TEXTURE  textures[mesh_data[vx_DrawID].texture_index0]
#define SPECULAR_TEXTURE textures[mesh_data[vx_DrawID].texture_index1]

//Structs
struct MaterialTransformData
{
    int texture_index0; //diffuse
    int texture_index1; //specular
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

struct PointLight //64 bytes
{
    vec4 position_constant;
    vec4 ambient_linear;
    vec4 diffuse_quadratic;
    vec4 specular;
};

//Uniforms
layout (std140, binding = MPE_CAMERA_BINDING) uniform CameraData
{
    mat4 projection;
    mat4 view;
    vec4 camera_pos;
    float near;
    float far;
};

layout (std140, binding = MPE_MESH_BINDING) uniform MeshData
{
    MaterialTransformData mesh_data[64];
};

layout (std140, binding = MPE_LIGHT_BINDING) uniform LightData
{
    PointLight point_lights[8];
    int num_point_lights;
};

uniform sampler2D textures[8];
uniform samplerCube depth_maps[8];

