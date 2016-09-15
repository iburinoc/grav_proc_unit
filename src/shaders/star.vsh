R"(
#version 330 core

in vec3 position;
in float size;

out vec2 point_coord;

uniform mat4 proj;
uniform mat4 view;

const float DX[] = float[](-1, -1,  1,  1);
const float DY[] = float[]( 1, -1, -1,  1);

void main() {
	vec4 cam_pos = view * vec4(position, 1);

	/* move to corners based on index */
	int ind = gl_VertexID % 4;

	point_coord = vec2(DX[ind], DY[ind]);

	cam_pos.xy -= point_coord * size * 0.5;
	gl_Position = proj * cam_pos;
}
)"
