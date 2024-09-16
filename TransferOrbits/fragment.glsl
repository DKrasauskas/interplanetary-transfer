#version 330 core
#define SMOOTH 0

out vec4 FragColor;
in vec3 pos;

uniform sampler2D tex;

uniform float id;
vec4 color(float id) {
	if (id > 0.0f) {
		if (id > 1.0f) {
			if (id > 2.0f) {
				if (id > 3.0f) {
					if (id > 4.0f) {
						return vec4(0.749, 0.592, 0.4, 1.0);
					}
					return vec4(0.321, 0.317, 0.352, 1.0);
				}
				return vec4(193.0f / 255.0f, 68.0f / 255.0f, 14.0f / 255.0f, 1.0f);
			}
			return vec4(1, 1, 0, 1);
		}
		return vec4(1, 1, 1, 1);
	}
	return vec4(0, 0, 1, 1);
}
void main()
{
	
	
	FragColor = color(id);
	
}
