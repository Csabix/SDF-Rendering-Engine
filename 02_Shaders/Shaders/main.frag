vec3 camera()
{	//optimized camera
	vec3 w = normalize(cam_eye - cam_at);
	vec3 u = normalize(cross(cam_up, w));
	vec3 ab = vs_cam_coeffs.x * u + vs_cam_coeffs.y * cross(w, u) - w;
	return normalize(ab);
}

vec3 spheretrace_1(const in Ray r, in float t, in float ft, const in int maxiters);

void main()
{
	vec4 in_0 = texture2D(in_nearest_0, vs_tex);
	vec4 in_1 = texture2D(in_linear_1, vs_tex);
	vec4 in_2 = texture2D(in_linear_2, vs_tex);
	
	//actual data mapping follows
	float t = in_0.x;	float ft = in_0.y;	float iternum = in_0.z;
	vec3 n = in_1.xyz;
	vec2 sw = vec2(1 - in_1.w, in_0.w);
	float ao = 1 - in_2.w;

	Ray r = Ray(camera(), cam_eye);
	vec3 depth = vec3(in_0.x, in_0.y, 0);
	
	float pixel_size = t * cam_pixel_growth;
	//t-=pixel_size;

	if(depth.x < 10000)
		depth = spheretrace_1(r, t, 0, 1);

	if(depth.y < 1000 && depth.x < 10000 && !IS_CLOSE_TO_SURFACE(depth.y,depth.y))
	{
		depth = spheretrace(r, depth.x, 0, st_stepcount);
	}

	pixel_size = depth.x * cam_pixel_growth;
	bool depth_changed = depth.x - t > pixel_size*50;
			
	t = depth.x;	ft = depth.y;	iternum += depth.z;
	
	r.p += t*r.v;
	
	
	n = norm(r.p, n, pixel_size * n_epsz_mult);
	sw = shadow(r, n, depth, sw, pixel_size, depth_changed);
	ao = ambient(r.p, n, ao);	//this is the sum of the ambient occulution
	
	vec3 col;
	if(ft > 1000 || t > 10000)	col = skylight(r.p, r.v);
	else			col = shade(r, n, clamp(1 - ao_strength * ao, 0, 1), sw.x);
	
	//reverse data mapping
	out_nearest_0 = vec4(t, ft, depth.z, sw.y);
	out_linear_1 = vec4(n, 1 - sw.x);
	out_linear_2 = vec4(col, 1 - ao);
}