//Input
layout (location = 0) in vec3 vx_position;
layout (location = 1) in vec2 vx_texCoords;
layout (location = 2) in vec3 vx_normal;
layout (location = 3) flat in int vx_DrawID;

//Output
out vec4 result;


vec3 ComputePointLight(vec3 position, vec3 normal, vec2 tex_coords, int light_index);
float ComputeShadows(vec3 position, int light_index);

void main()
{
    vec3 normal = normalize(vx_normal);
    
    vec3 res = vec3(0.0);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
        res += ComputePointLight(vx_position, normal, vx_texCoords, i);

    result = vec4(res, 1.0);
}

vec3 ComputePointLight(vec3 position, vec3 normal, vec2 tex_coords, int light_index)
{
    PointLight l = POINT_LIGHT(light_index);

    vec3 camera_direction = normalize(CAMERA_POSITION - position);
    vec3 light_direction = normalize(POINT_LIGHT_POSITION(l) - position);
    vec3 halfway_direction = normalize(light_direction + camera_direction);

    float diffuse_coeff = max(dot(normal, light_direction), 0.0);
    float specular_coeff = pow(max(dot(normal, halfway_direction), 0.0), SHININESS);
    
    float light_distance = distance(POINT_LIGHT_POSITION(l), position);
    float light_attenuation = 1.0 / (
        POINT_LIGHT_CONSTANT(l) +
        POINT_LIGHT_LINEAR(l) * light_distance +
        POINT_LIGHT_QUADRATIC(l) * light_distance * light_distance);

    vec3 diffuse_map = vec3(texture(DIFFUSE_TEXTURE, tex_coords));
    vec3 diffuse = POINT_LIGHT_DIFFUSE(l) * diffuse_coeff * diffuse_map;
    vec3 specular = POINT_LIGHT_SPECULAR(l) * specular_coeff * vec3(texture(SPECULAR_TEXTURE, tex_coords));
    vec3 ambient = POINT_LIGHT_AMBIENT(l) * diffuse_map;

    diffuse *= light_attenuation;
    specular *= light_attenuation;
    ambient *= light_attenuation;
    
    float shadow = ComputeShadows(position, light_index);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ComputeShadows(vec3 position, int light_index)
{
    float shadow = 0.0;
    float bias = 0.15;
    float camera_distance = distance(CAMERA_POSITION, position);
    float disk_radius = (1.0 + (camera_distance / CAMERA_FAR_PLANE)) / 25.0;

    int samples = 20;
    PointLight l = POINT_LIGHT(light_index);
    vec3 from_light = position - POINT_LIGHT_POSITION(l);
    float current_depth = length(from_light);
        
    for (int i = 0; i < samples; i++)
    {
        float closest_depth = texture(GET_DEPTH_MAP(light_index), from_light + gridSamplingDisk[i] * disk_radius).r;
        closest_depth *= CAMERA_FAR_PLANE;   // undo mapping [0;1]
        if(current_depth - bias > closest_depth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}