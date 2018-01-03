//#define STEP_SIZE_REDUCTION 0.95
#define STEP_SIZE_REDUCTION 0.87

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

#define NEXT_R(r_0, r_1, d) STEP_SIZE_REDUCTION*(- r_1 * (r_0 - r_1 - d)/(r_0 - r_1 + d))
layout(index = 0 + 3) subroutine(SphereTrace)
vec3 spheretrace_3(const in Ray r, in float t, in float ft, const in int maxiters)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float pft = 0, dt = 0, nft;
	for(int i = 0; i < maxiters; ++i)
	{
		//dt = ft * (1 - STEP_SIZE_REDUCTION * (pft - ft - dt)/(pft - ft + dt));
		dt = ft + NEXT_R(pft, ft, dt);
		nft = SDF(r,t + dt);
		if(dt < ft + nft)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			dt = ft;
			t += dt;
			ft = SDF(r,t);
		}
		if(IS_CLOSE_TO_SURFACE(ft,t))
			return vec3(t - dt, SDF(r, t - dt), float(i));
		pft = ft;
	}
	return vec3(t, ft, float(maxiters));
}

#define NEXT_R(r_0, r_1, d) STEP_SIZE_REDUCTION*(- r_1 * (r_0 - r_1 - d)/(r_0 - r_1 + d))
layout(index = 0 + 3) subroutine(SphereTrace)
vec3 spheretrace_3(const in Ray r, in float t, in float ft, const in int maxiters)
{
	float rp = 0, rc = ft, rn = 0; //prev, curr, next
	float di = 0;
	for(int i=0; i < maxiters; ++i)
	{
		di = di + NEXT_R(rp, rc, di);
		rn = SDF(r, t + di);
		if(di > rn + rc)
		{
			di = rc;
			rn = SDF(r,t + di);
		}
		if(IS_CLOSE_TO_SURFACE(rn, t + di))
			return vec3(t,rc, float(i));
		t += di;
		rp = rc; rc = rn;
	}
	return vec3(t, rc, float(maxiters));
}