//Input
layout (location = 0) in vec2 vx_texCoords;
layout (location = 1) flat in int vx_DrawID;

//Output
out vec4 result;

void main()
{
    result = texture(DIFFUSE_TEXTURE, vx_texCoords);
}