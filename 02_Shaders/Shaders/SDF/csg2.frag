vec3 hsv2rgb(const in vec3 c)
{
    const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 mandel_iq_col(in vec3 p)
{
	vec3 zz = p;
    float m = dot(zz,zz);

    vec4 trap = vec4(abs(zz.xyz),m);
	float dz = 1.0;
    
    
	for( int i=0; i<user_itercount; i++ )
    {
		if( m > 4.0 ) break;
#if 1
        float m2 = m*m;
        float m4 = m2*m2;
		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        float x = zz.x; float x2 = x*x; float x4 = x2*x2;
        float y = zz.y; float y2 = y*y; float y4 = y2*y2;
        float z = zz.z; float z2 = z*z; float z4 = z2*z2;

        float k3 = x2 + z2;
        float k2 = inversesqrt( k3*k3*k3*k3*k3*k3*k3 );
        float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        float k4 = x2 - y2 + z2;

        zz.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        zz.y = p.y + -16.0*y2*k3*k4*k4 + k1*k1;
        zz.z = p.z +  -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;
#else
		dz = 8.0*pow(m,3.5)*dz + 1.0;
        
        float r = length(zz);
        float b = 8.0*acos( clamp(zz.y/r, -1.0, 1.0));
        float a = 8.0*atan( zz.x, zz.z );
        zz = p + pow(r,8.0) * vec3( sin(b)*sin(a), cos(b), sin(b)*cos(a) );
#endif        
        
        trap = min( trap, vec4(abs(zz.xyz),m) );
        m = dot(zz,zz);
    }
    //trap.x = m;
	vec3 col;
	vec3 basecol = hsv2rgb(vec3(length(p)*5, 0.3,0.8));
	col = basecol;
	col = mix( col, vec3(0.7,0.2,0.2), 2*trap.w );
	col = mix( col, vec3(1.0,0.5,0.2), sqrt(trap.y) );
	col = mix( col, vec3(1.0,1.5,1.0)*basecol, trap.z );
	col = mix( col, vec3(1.0,1.0,1.0), sqrt(trap.x) );
	//col = mix( col, basecol , sqrt(trap.x));
    return col;
}

/*vec3 color(in vec3 p, in vec3 v)
{
	if(p.z < -1 || length(p) > 4) return vec3(0.8);
	return mandel_iq_col(p.xzy);
}*/

float mandel_iq( in vec3 p )
{
    vec3 zz = p;
    float m = dot(zz,zz);

	float dz = 1.0;
    
    
	for( int i=0; i< user_itercount; i++ )
    {
	if( m > 4.0 )
            break;
#if 1
        float m2 = m*m;
        float m4 = m2*m2;
		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        float x = zz.x; float x2 = x*x; float x4 = x2*x2;
        float y = zz.y; float y2 = y*y; float y4 = y2*y2;
        float z = zz.z; float z2 = z*z; float z4 = z2*z2;

        float k3 = x2 + z2;
        float k2 = inversesqrt( k3*k3*k3*k3*k3*k3*k3 );
        float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        float k4 = x2 - y2 + z2;

        zz.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        zz.y = p.y + -16.0*y2*k3*k4*k4 + k1*k1;
        zz.z = p.z +  -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;
#else
		dz = 8.0*pow(m,3.5)*dz + 1.0;
        
        float r = length(zz);
        float b = 8.0*acos( clamp(zz.y/r, -1.0, 1.0));
        float a = 8.0*atan( zz.x, zz.z );
        zz = p + pow(r,8.0) * vec3( sin(b)*sin(a), cos(b), sin(b)*cos(a) );
#endif        

        m = dot(zz,zz);
    }

    return 0.25*log(m)*sqrt(m)/dz;
}

float plus(in vec3 p, in vec2 r)
{
	float f = 	cylinder(p.xyz, r.x);
	f = smin(f, cylinder(p.yzx, r.x), r.y);
	f = smin(f, cylinder(p.zxy, r.x), r.y);
	return f;
}

float dice(in vec3 p, in float r)
{
	float f = sphere(p, r*SQRT2);
	f = smax(f, box(p, vec3(r)), 0.035*r);
	return f;
}

float testcube(in vec3 p, in vec3 r)
{
	return  smax(dice(p, r.x), -plus(p, r.yz), r.z);
}

float gizmo(in vec3 p, vec2 t, float k)
{
	float f = 	 torus(p.xyz,t);
	f = smin(f, torus(p.yzx,t),k);
	f = smin(f, torus(p.zxy,t),k);
	return f;
}

vec3 color(in vec3 p, in vec3 v)
{
	vec3 rep = repeat(p, vec3(9));
	float a =  abs(length(rep) - 4);
	if(mandel_iq((p.xzy-vec3(-40,1.1*5,30))*0.2)*5 < 0.1)
		return 5*mandel_iq_col((p.xzy-vec3(-40,1.1*5,30))*0.2);

	vec3 randcol = mix(vec3(0.3,0.3,0.7), vec3(0.65), clamp(a*3,0,1));
	float cube = testcube(p - vec3(10+20*0,10+20*1,8), vec3(8,6,0.5));
	vec3 col = mix(vec3(0.8), vec3(1,0,0), clamp(1-cube,0,1));
	float gizm1 = gizmo(p - vec3(0*20,3*20,8), vec2(8,1.25), 0.75);
	col = mix(col, vec3(0,1,0),  clamp(1-gizm1,0,1));
	float gizm2 = gizmo(p - vec3(0,0*20,8), vec2(8,1.25), 0.75);
	col = mix(col, vec3(0,0,1),  clamp(1-gizm2,0,1));
	return col + randcol*0.5;
}

float viviani(in vec3 p, in float r)
{
	return max(sphere(p, r), cylinder(p + vec3(r*0.5,0,0), r*0.5)); 
}

float sdf(in vec3 p, in vec3 v)
{
	float fv = planeXY(p, v);

	fv = min(fv, gizmo(p - vec3(0,0*20,8), vec2(8,1.25), 0.75));
	fv = min(fv, testcube(p - vec3(10,10+20*0,8), vec3(8,6,0.5)));
	fv = min(fv, gizmo(p - vec3(0,1*20,8), vec2(8,1.25), 0.75));
	fv = min(fv, testcube(p - vec3(10,10+20*1,8), vec3(8,6,0.5)));
	fv = min(fv, gizmo(p - vec3(0,2*20,8), vec2(8,1.25), 0.75));
	fv = min(fv, testcube(p - vec3(10,10+20*2,8), vec3(8,6,0.5)));
	fv = min(fv, gizmo(p - vec3(0,3*20,8), vec2(8,1.25), 0.75));
	//fv = min(fv, testcube(p - vec3(10+20,10+20*3,8), vec3(8,6,0.5)));

	//float vi = viviani(p - vec3(-10,-10,8), 8);
	
	return min(mandel_iq((p.xzy-vec3(-40,1.1*5,30))*0.2)*5, fv);
}
