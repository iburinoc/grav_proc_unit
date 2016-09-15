R"(
#version 330 core

in vec2 point_coord;

out vec4 colour;

const float ALIAS_CONST = 100;

void main() {
	vec2 rvec = point_coord;
	float r2 = dot(rvec, rvec);

	float error = max(r2 - 1, 0);
	if(error > 1e-9) discard;
	colour = vec4(1, 1, 1, 1);
	//colour = vec4(point_coord, 0, 1);
}
)"
