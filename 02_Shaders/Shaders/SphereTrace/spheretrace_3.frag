#define STEP_SIZE_REDUCTION 0.95
//#define STEP_SIZE_REDUCTION 0.87

//Performs a sphere-step
// r a ray on wich along the tracing happens
// c0, c1 are the previous cones
// the result is the next cone
// A
/*
vec2 sphere_step(const in Ray r, in vec2 c0, in vec2 c1)
{
	float dt = c0.y * (1 - STEP_SIZE_REDUCTION * (c1.y - c0.y - (c1.x - c0.x))/(c1.y - c0.y + (c1.x - c0.x)));
	float ft = sdf(RAY(r, c0.x + dt));
	if(dt < c0.y + c1.y)
		return vec2();
}*/

layout(index = 0 + 3) subroutine(SphereTrace)
vec3 spheretrace_3(const in Ray r, in float t, in float ft, const in int maxiters)
{
	float rp = 0, rc = ft, rn = 0; //prev, curr, next
	float di = 0; int k = 0;
	for(int i=0; i < maxiters; ++i)
	{
		di = rc + STEP_SIZE_REDUCTION * rc * max( (di - rp + rc) / (di + rp - rc), 0.6);
		rn = SDF(r, t + di);
		if(di > rc + rn)
		{
			di = rc; ++k;
			rn = SDF(r, t + di);
		}
		if(IS_CLOSE_TO_SURFACE(rn, t + di))
			return vec3(t, rc, float(k)/float(i));
		t += di;
		rp = rc; rc = rn;
	}
	return vec3(t, rc, float(k)/float(maxiters));
}