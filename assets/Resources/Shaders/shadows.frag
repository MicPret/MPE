in vec4 frag_pos;

uniform vec3 light_pos;

void main()
{
    float light_dist = distance(frag_pos.xyz, light_pos);
    
    light_dist /= CAMERA_FAR_PLANE;
    
    gl_FragDepth = light_dist;
}