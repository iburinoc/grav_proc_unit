R"(

__constant const float G = 6.67408e-11f * 1e10f;
__constant const float Geps = 1e-1f;

static float3 grav_accel(float3 delta, float mass) {
	float l = length(delta);
	float d = l*l + Geps * Geps;

	return (float)-G * mass / d / l * delta;
}

__kernel void gr_step(
		const int N,
		const float dt,
		const float ratio,

		__global const float3 *p0,
		__global const float3 *v0,
		__global const float3 *p1,
		__global const float3 *v1,

		__global const float *m,
		__global const uchar *active,

		__global float3 *p2,
		__global float3 *v2,
		__global float3 *va,
		__global float3 *aa
		) {

	const int i = get_global_id(0);

	if(i >= N) return;
	if(!active[i]) return;

	float3 pos = p1[i];

	float3 acc = (float3)(.0f, .0f, .0f);
	for(int j = 0; j < N; j++) {
		if(i == j || !active[j]) continue;

		acc += grav_accel(pos - p1[j], m[j]);
	}

	p2[i] = p0[i] + v1[i] * dt;
	v2[i] = v0[i] + acc * dt;

	va[i] += v0[i] * ratio;
	aa[i] += acc * ratio;
}

__kernel void gr_end(
		const int N,
		const float dt,
		__global const bool *active,

		__global const float3 *va,
		__global const float3 *aa,

		__global float3 *pos,
		__global float3 *vel
		) {

	const int i = get_global_id(0);

	if(i >= N) return;
	if(!active[i]) return;

	pos[i] = pos[i] + dt * va[i] / 6.0f;
	vel[i] = vel[i] + dt * aa[i] / 6.0f;
}
)"
