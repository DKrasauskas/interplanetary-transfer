
#version 460 core
layout(location = 0) in vec3 aPos;
out vec3 pos;
uniform float x;
uniform float y;
uniform float z;
uniform vec3 offset;
uniform mat4 projection;
uniform mat4 view_direction;
uniform mat4 model;
uniform vec3 camera_global;
void main() {
	vec3 data = aPos * 0.02f;
	data += camera_global;
	gl_Position = projection * view_direction  * vec4(data + vec3(x, y, z), 1.0);
	pos = vec3(aPos.x, aPos.y, -1.0f);/// color_map(lininterp(data[gl_VertexID]));
}
