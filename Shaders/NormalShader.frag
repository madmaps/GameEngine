#version 410
uniform sampler2D basic_texture;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D occlusion_map;
in vec3 view_dir_tan;
in vec3 light_dir_tan;
in vec2 texture_coordinates;
vec3 light_position_world = vec3(-1000.0f, -1000.0f, 0.0f);
vec3 Ls = vec3(1.0f, 1.0f, 1.0f);
vec3 Ld = vec3(1.0f, 1.0f, 1.0f);
vec3 La = vec3(1.0f, 1.0f, 1.0f);
vec4 texel = texture(basic_texture, texture_coordinates);
vec4 specularTexel = texture(specular_map, texture_coordinates);
vec4 occlusionTexel = texture(occlusion_map, texture_coordinates);
vec3 Ks = vec3(specularTexel.rgb);
vec3 Kd = vec3(texel.rgb);
vec3 Ka = vec3(occlusionTexel.rgb);
float specular_exponent = 100.0;
out vec4 frag_color;
void main()
{
 	vec3 Ia = La * Ka * Kd;
	vec3 normal_tan = texture(normal_map, texture_coordinates).rgb;
	normal_tan = normalize(normal_tan * 2.0 - 1.0);
	vec3 direction_to_light_tan = normalize(-light_dir_tan);
	float dot_prod = dot(direction_to_light_tan, normal_tan);
	dot_prod = max(dot_prod, 0.0);
	vec3 Id = Kd * Ld * dot_prod;
	vec3 reflection_tan = reflect(normalize(light_dir_tan),normal_tan);
	float dot_prod_specular = dot(reflection_tan, normalize(view_dir_tan));
	dot_prod_specular = max(dot_prod_specular, 0.0);
	float specular_factor = pow(dot_prod_specular, 100.0);
	vec3 Is = Ls * Ks * specular_factor;
	frag_color = vec4(Is + Id + Ia, 1.0);
}
