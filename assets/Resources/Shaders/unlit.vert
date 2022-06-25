//Input
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;

//Output
layout (location = 0) out vec2 vx_texCoords;
layout (location = 1) flat out int vx_DrawID;

void main()
{
    SET_DRAW_ID;
    vx_texCoords = in_texCoords;

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * vec4(in_pos, 1.0);
}