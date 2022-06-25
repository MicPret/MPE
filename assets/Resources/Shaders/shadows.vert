layout (location = 0) in vec3 in_pos;

void main()
{
    int vx_DrawID;
    SET_DRAW_ID;
    gl_Position = MODEL_MATRIX * vec4(in_pos, 1.0);
}