#version 410
in vec3 texCoords;
uniform samplerCube cube_texture;
out vec4 frag_color;
void main()
{
	frag_color = texture(cube_texture, texCoords);
}
