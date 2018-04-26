#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_text;
layout(location = 3) in vec4 vtangent;
out vec3 view_dir_tan;
out vec3 light_dir_tan;
out vec2 texture_coordinates;
uniform mat4 view, proj, model;
uniform vec3 cameraPosition_world;

void main()
{
	gl_Position = proj * view * model * vec4(vertex_position,1.0);
	texture_coordinates = vertex_text;

	vec3 lightDirection_world = normalize(vec3(-1000.0, -1000.0, 0.0) - vertex_position);
	vec3 bitangent = cross(vertex_normal, vtangent.xyz) * vtangent.w;

	vec3 cameraPosition_local = vec3(inverse(model) * vec4(cameraPosition_world,1.0));
	vec3 lightDirection_local = vec3(inverse(model) * vec4(lightDirection_world,0.0));
	vec3 viewDirection_local = normalize(cameraPosition_local - vertex_position);

	view_dir_tan = vec3(dot(vtangent.xyz, viewDirection_local),dot(bitangent, viewDirection_local), dot(vertex_normal, viewDirection_local));
	light_dir_tan = vec3(dot(vtangent.xyz, lightDirection_local),dot(bitangent,lightDirection_local), dot(vertex_normal, lightDirection_local));
}
