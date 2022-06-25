#version 460 core

//Bindings
#define MPE_CAMERA_BINDING 0
#define MPE_MESH_BINDING 1
#define MPE_LIGHT_BINDING 2

//Defines
#define SET_DRAW_ID vx_DrawID = gl_DrawID
#define DRAW_INDEX vx_DrawID
#define MODEL_MATRIX _mesh_data[DRAW_INDEX].model
#define VIEW_MATRIX _view
#define PROJECTION_MATRIX _projection
#define GET_TEXTURE(n) _textures[n]
#define GET_DEPTH_MAP(n) _depth_maps[n]
#define MATERIAL_AND_TRANSFORM _mesh_data[DRAW_INDEX]
#define DIFFUSE_TEXTURE  GET_TEXTURE(MATERIAL_AND_TRANSFORM.texture_index0)
#define SPECULAR_TEXTURE GET_TEXTURE(MATERIAL_AND_TRANSFORM.texture_index1)
#define SHININESS MATERIAL_AND_TRANSFORM.float0
#define POINT_LIGHT(n) _point_lights[n]
#define POINT_LIGHT_POSITION(l) l.position_constant.xyz
#define POINT_LIGHT_AMBIENT(l) l.ambient_linear.xyz
#define POINT_LIGHT_DIFFUSE(l) l.diffuse_quadratic.xyz
#define POINT_LIGHT_SPECULAR(l) l.specular.xyz
#define POINT_LIGHT_CONSTANT(l) l.position_constant.w
#define POINT_LIGHT_LINEAR(l) l.ambient_linear.w
#define POINT_LIGHT_QUADRATIC(l) l.diffuse_quadratic.w
#define NUM_POINT_LIGHTS _num_point_lights
#define CAMERA_POSITION _camera_pos.xyz
#define CAMERA_NEAR_PLANE _near
#define CAMERA_FAR_PLANE _far

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

    float float0;   //shininess
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
    mat4 _projection;
    mat4 _view;
    vec4 _camera_pos;
    float _near;
    float _far;
};

layout (std140, binding = MPE_MESH_BINDING) uniform MeshData
{
    MaterialTransformData _mesh_data[64];
};

layout (std140, binding = MPE_LIGHT_BINDING) uniform LightData
{
    PointLight _point_lights[8];
    int _num_point_lights;
};

uniform sampler2D _textures[8];
uniform samplerCube _depth_maps[8];

