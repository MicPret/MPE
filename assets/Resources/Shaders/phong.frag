//Input
layout (location = 0) in vec3 vx_position;
layout (location = 1) in vec2 vx_texCoords;
layout (location = 2) in vec3 vx_normal;
layout (location = 3) flat in int vx_DrawID;

//Output
out vec4 result;


vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int light_id);
float ComputeShadows(PointLight light, vec3 pos, int light_id);

void main()
{
    vec3 normal = normalize(vx_normal);
    vec3 view_dir = normalize(camera_pos.xyz - vx_position);
    
    vec3 res = vec3(0.0);

    for (int i = 0; i < num_point_lights; i++)
        res += ComputePointLight(point_lights[i], normal, vx_position, view_dir, i);

    result = vec4(res, 1.0);
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir, int light_id)
{
    vec3 light_pos = light.position_constant.xyz;
    vec3 light_dir = normalize(light_pos - frag_pos);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float shininess = mesh_data[vx_DrawID].float0;
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    float dist = distance(light_pos, frag_pos);
    float attenuation =
        1.0 / (light.position_constant.w + light.ambient_linear.w * dist + light.diffuse_quadratic.w * (dist * dist));
    
    vec3 diffuse = light.diffuse_quadratic.xyz * diff * vec3(texture(DIFFUSE_TEXTURE, vx_texCoords));
    vec3 specular = light.specular.xyz * spec * vec3(texture(SPECULAR_TEXTURE, vx_texCoords));
    vec3 ambient = light.ambient_linear.xyz * vec3(texture(DIFFUSE_TEXTURE, vx_texCoords));

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    float shadow = ComputeShadows(light, vx_position, light_id);

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

float ComputeShadows(PointLight light, vec3 pos, int light_id)
{
    float shadow = 0.0;
    float bias = 0.15;
    float view_distance = distance(camera_pos.xyz, vx_position);
    float disk_radius = (1.0 + (view_distance / far)) / 25.0;

    int samples = 20;
    vec3 to_light = vx_position - light.position_constant.xyz;
    float current_depth = length(to_light);
        
    for (int i = 0; i < samples; i++)
    {
        float closest_depth = texture(depth_maps[light_id], to_light + gridSamplingDisk[i] * disk_radius).r;
        closest_depth *= far;   // undo mapping [0;1]
        if(current_depth - bias > closest_depth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}