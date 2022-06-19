//Input
layout (location = 0) in vec2 vx_texCoords;
layout (location = 1) flat in int vx_DrawID;

//Output
out vec4 result;

void main()
{
    int index = mesh_data[vx_DrawID].texture_index0;
    result = texture(textures[index], vx_texCoords);
}