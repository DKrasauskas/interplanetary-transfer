#pragma once

struct vec3 {
	double x, y, z;
	vec3 operator +(vec3 rhs) {
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}
	vec3 operator *(double rhs) {
		return { x * rhs, y * rhs, z * rhs };
	}
	double len() {
		return x * x + y * y + z * z;
	}
};
struct Conic {
	vec3* conics;
	int size;
};

void init(int size) {
	Conic conic;
	conic.conics = (vec3*)malloc(sizeof(vec3) * size);
	for (int i = 0; i < size; i++) {
		conic.conics[i] = { 0, 0, 0 };
	}
}