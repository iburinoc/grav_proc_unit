R"(
#version 330 core

out vec4 colour;

const float ALIAS_CONST = 100;

void main() {
	vec2 rvec = gl_PointCoord * 2 - 1;
	float r2 = dot(rvec, rvec);

	float error = max(r2 - 1, 0);
	error *= ALIAS_CONST;
	if(error >= 1) discard;
	colour = vec4(1, 1, 1, 1 - error);
}
)"
