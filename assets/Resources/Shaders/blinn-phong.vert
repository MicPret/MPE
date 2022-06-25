//Input
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;
layout (location = 2) in vec3 in_normal;

//Output
layout (location = 0) out vec3 vx_position;
layout (location = 1) out vec2 vx_texCoords;
layout (location = 2) out vec3 vx_normal;
layout (location = 3) flat out int vx_DrawID;

void main()
{
    SET_DRAW_ID;
    vx_texCoords = in_texCoords;
    vx_normal = mat3(transpose(inverse(MODEL_MATRIX))) * in_normal;
    vx_position = (MODEL_MATRIX * vec4(in_pos, 1.0)).xyz;
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * vec4(vx_position, 1.0);
}