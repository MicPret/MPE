//Input
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;

//Output
layout (location = 0) out vec2 vx_texCoords;
layout (location = 1) flat out int vx_DrawID;

void main()
{
    vx_texCoords = in_texCoords;
    vx_DrawID = gl_DrawID;

    gl_Position = projection * view * MODEL_MATRIX * vec4(in_pos, 1.0);
}