#version 410

layout(location = 0) in vec2 vertex_position;
uniform vec2 texture_top_left;
uniform vec2 texture_bottom_right;
uniform mat4 model;
uniform vec2 inSize;

out vec2 texture_coordinates;

void main()
{
	gl_Position = model * vec4(vertex_position * inSize, 0.0, 1.0);
   	//gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0f, 1.0f);
    //texture_coordinates = vertex_position;
	texture_coordinates.s = (((vertex_position.x + 1) / 2) * texture_bottom_right.x) + (((vertex_position.x - 1) / -2) * texture_top_left.x);
    texture_coordinates.t = (((vertex_position.y + 1) / 2) * texture_bottom_right.y) + (((vertex_position.y - 1) / -2) * texture_top_left.y);
}
