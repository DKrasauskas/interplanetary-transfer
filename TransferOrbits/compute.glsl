#version 430 core
precision highp float;

layout(local_size_x = 1, local_size_y = 1, local_size_x = 1) in;

layout(location = 2) uniform float max_p = 1;
layout(location = 3) uniform int min_p = 10;

//define x^3 - 1
//define x = vec2
uniform float x;
uniform float y;
uniform float z;

uniform int id = -1;
int n = -1;

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

float len(vec3 dt, int n) {
	return (dt.x - datax[n]) * (dt.x - datax[n]) + (dt.y - datay[n]) * (dt.y - datay[n]) + (dt.z - dataz[n]) * (dt.z - dataz[n]);
}
int p = 400;
void main() {
	if (len(vec3(x, y, z), id *p + p -1) > 0.0005f) {
		for (int i = id * p; i < id * p + p - 1; i++) {
			datax[i] = datax[i + 1];
			datay[i] = datay[i + 1];
			dataz[i] = dataz[i + 1];
		}
		datax[id * p + p - 1] = x;
		datay[id * p + p - 1] = y;
		dataz[id * p + p - 1] = z;
	}	
	
}