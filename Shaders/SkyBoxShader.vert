#version 410
in vec3 vp;
uniform mat4 proj_mat, camera_rotation;
out vec3 texCoords;
void main()
{
	texCoords = vp;
	gl_Position = proj_mat * camera_rotation * vec4(vp, 1.0);
}
