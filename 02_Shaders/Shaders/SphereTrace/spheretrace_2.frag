#define TRACE_OR 1.6

layout(index = 0 + 2) subroutine(SphereTrace)
vec3 spheretrace_2(const in Ray r, in float t, in float ft, const in int maxiters)
{
	float rc = ft, rn;
	float di = ft; int k=0;
	for(int i = 0; i < maxiters; ++i)
	{
		di = rc * TRACE_OR;
		rn = SDF(r, t + di);
		if(di > rc + rn)
		{
			di = rc; ++k;
			rn = SDF(r, t + di);
		}
		if(IS_CLOSE_TO_SURFACE(rn, t + di))
			return vec3(t, rc, float(k)/float(i));
		t += di;
		rc = rn;
	}
	return vec3(t, rc, float(k)/float(maxiters));
}

/*{
	float pft = 0, dt = 0, nft;
	for(int i = 0; i < maxiters; ++i)
	{
		dt = ft * TRACE_OR;
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
			return vec3(t-dt, SDF(r, t-dt), float(i));
		pft = ft;
	}
	return vec3(t, ft, float(maxiters));
}*/