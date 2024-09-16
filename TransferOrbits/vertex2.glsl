
#version 460 core
layout(location = 0) in vec3 aPos;
out vec3 pos;
uniform float centerX;
uniform float centerY;
uniform float centerZ;
uniform mat4 projection;
uniform mat4 view_direction;
uniform mat4 model;
uniform vec3 camera_global;
layout(std430, binding = 1) buffer rx
{
	float datax[];
};
layout(std430, binding = 2) buffer ry
{
	float datay[];
};
layout(std430, binding = 3) buffer rz
{
	float dataz[];
};

void main() {
	vec3 data = aPos;
	gl_Position = projection * view_direction * vec4(datax[gl_VertexID] + camera_global.x, datay[gl_VertexID] + camera_global.y, dataz[gl_VertexID]+camera_global.z, 1.0);
	pos = vec3(datax[gl_VertexID], datay[gl_VertexID], dataz[gl_VertexID]);
}