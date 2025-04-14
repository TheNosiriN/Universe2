#define PI 3.14159265358979323846
#define EPSILON 0.001
#define POSITIVE_INFINITY uintBitsToFloat(0x7F800000)
#define NEGATIVE_INFINITY uintBitsToFloat(0xFF800000)
#define saturate(x) clamp(x, 0.0f, 1.0f)


#ifdef HX_VERTEX_SHADER
	#define c_attrib out
#endif
#ifdef HX_GEOMETRY_SHADER
	#define c_attrib out
#endif
#ifdef HX_FRAGMENT_SHADER
	#define c_attrib in
#endif


#include "hash.glsl"


float dot2(vec3 v){ return dot(v,v); }
float dot2(vec2 v){ return dot(v,v); }
float max3(vec3 v){ return max(v.x, max(v.y, v.z)); }
float min3(vec3 v){ return min(v.x, min(v.y, v.z)); }

float remap(float v, float oldmn, float oldmx, float newmn, float newmx){
    return ((v - oldmn) * (newmx - newmn)) / (oldmx - oldmn) + newmn;
}


vec3 construct_cuber_vertices(int vertexIndex){
    int tri = vertexIndex / 3;
    int idx = vertexIndex % 3;
    int face = tri / 2;
    int top = tri % 2;

    int dir = face % 3;
    int pos = face / 3;

    int nz = dir >> 1;
    int ny = dir & 1;
    int nx = 1 ^ (ny | nz);

    vec3 d = vec3(nx, ny, nz);
    float flip = 1 - 2 * pos;

    vec3 n = flip * d;
    vec3 u = -d.yzx;
    vec3 v = flip * d.zxy;

    float mirror = -1 + 2 * top;
    return n + mirror*(1-2*(idx&1))*u + mirror*(1-2*(idx>>1))*v;
}



/// Cube: triangle strip version
const vec3 triangle_strip_cube_vertices[8] = vec3[](
	vec3(-1.0, -1.0,  1.0),
    vec3(1.0, -1.0,  1.0),
    vec3(-1.0,  1.0,  1.0),
    vec3(1.0,  1.0,  1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, -1.0, -1.0),
    vec3(-1.0,  1.0, -1.0),
    vec3(1.0,  1.0, -1.0)
);

const uint triangle_strip_cube_indices[14] = uint[](
	0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
);

/// Quad
const vec2 triangle_strip_quad_vertices[6] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);

const vec2 triangle_vertices[3] = vec2[](
    vec2(0.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 0.0)
);



float easeInCirc(float x) {
    float s = sqrt(1.0 - (x*x));
    return 1.0 - (isnan(s) ? 0.0 : s);
}
float easeInExpo(float x) {
	return x==0.0f ? 0.0f : pow(2.0f, 10.0f*x - 10.0f);
}
float easeInExpoFac(float x, float fac) {
	return x==0.0f ? 0.0f : pow(fac, 10.0f*x - 10.0f);
}



// float hash13(vec3 p){
// 	uvec3 q = uvec3(ivec3(p)) * uvec3(1597334673U, 3812015801U, 2798796415U);
// 	uint n = (q.x ^ q.y ^ q.z) * 1597334673U;
// 	return float(n) * 2.328306437080797e-10;
// }
// vec3 hash33(vec3 p){
// 	uvec3 q = uvec3(ivec3(p)) * uvec3(1597334673U, 3812015801U, 2798796415U);
// 	q = (q.x ^ q.y ^ q.z)*uvec3(1597334673U, 3812015801U, 2798796415U);
// 	return vec3(q) * 2.328306437080797e-10;
// }





vec3 mulvq(vec4 q, vec3 v){		//rotates a 3D vector by a quaternion.
	vec3 result;
	float axx = q.x * 2.0;
	float ayy = q.y * 2.0;
	float azz = q.z * 2.0;
	float awxx = q.a * axx;
	float awyy = q.a * ayy;
	float awzz = q.a * azz;
	float axxx = q.x * axx;
	float axyy = q.x * ayy;
	float axzz = q.x * azz;
	float ayyy = q.y * ayy;
	float ayzz = q.y * azz;
	float azzz = q.z * azz;
	result.x = ((v.x * ((1.0 - ayyy) - azzz)) + (v.y * (axyy - awzz))) + (v.z * (axzz + awyy));
	result.y = ((v.x * (axyy + awzz)) + (v.y * ((1.0 - axxx) - azzz))) + (v.z * (ayzz - awxx));
	result.z = ((v.x * (axzz - awyy)) + (v.y * (ayzz + awxx))) + (v.z * ((1.0 - axxx) - ayyy));
	return result;
}

vec4 qaxang(vec3 ax, float ang){	//creates a quaternion from axis and angle.
	float ha = ang * 0.5f;
	float sha = sin(ha);
	return vec4(ax.x * sha, ax.y * sha, ax.z * sha, cos(ha));
}

vec3 rndUnitSphere(float u, float v) {
    float theta = u * 2.0 * PI; // Azimuthal angle in [0, 2*PI]
    float phi = acos(2.0 * v - 1.0); // Polar angle in [0, PI]

    float sinTheta = sin(theta);
    float cosTheta = cos(theta);
    float sinPhi = sin(phi);
    float cosPhi = cos(phi);

    return vec3(
        sinPhi * cosTheta,
        sinPhi * sinTheta,
        cosPhi
    );
}





float ibox( in vec3 ro, in vec3 rd, in vec2 distBound, in vec3 boxSize, out bool inside, out bool hit, out vec2 tfn) {
    vec3 m = sign(rd)/max(abs(rd), 1e-8);
    vec3 n = m*ro;
    vec3 k = abs(m)*boxSize;

    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

	float tN = max( max( t1.x, t1.y ), t1.z );
	float tF = min( min( t2.x, t2.y ), t2.z );
    tfn = vec2(tN, tF);

    hit = true;

    if (tN > tF || tF <= 0.) {
        hit = false;
        return 0.;
    } else {
        if (tN >= distBound.x && tN <= distBound.y) {
        	inside = false;
            return tN;
        } else if (tF >= distBound.x && tF <= distBound.y) {
        	inside = true;
            return tF; // this means inside
        } else {
            hit = false;
            return 0.;
        }
    }
}


float isphere(vec3 eye, vec3 dir, float rad, out bool inside, out bool hit, out vec2 tfn){
    float a = dot(dir, dir);
    float b = 2.*dot(eye,dir);
    float c = dot(eye,eye)-rad*rad;
    float d = b*b-4.*a*c;

    if (d < 0.0){
        hit = false;
    }else{
        hit = true;
    }

    tfn.x = (-b-sqrt(d))/(2.*a);
    tfn.y = (-b+sqrt(d))/(2.*a);

    if (tfn.x < 0.){
        inside = true;
        return tfn.y;
    }else{
        inside = false;
        return tfn.x;
    }
}


// f(x) = (|x|² + R² - r²)² - 4·R²·|xy|² = 0
float itorus(in vec3 ro, in vec3 rd, in vec2 tor, out bool inside, out bool hit, out vec2 tfn){
    float po = 1.0;

    float Ra2 = tor.x*tor.x;
    float ra2 = tor.y*tor.y;

    float m = dot(ro,ro);
    float n = dot(ro,rd);

    // bounding sphere
    {
        float h = n*n - m + (tor.x+tor.y)*(tor.x+tor.y);
        if( h<0.0 ){
            hit = false;
            return -1.0;
        }
	//float t = -n-sqrt(h); // could use this to compute intersections from ro+t*rd
    }

	// find quartic equation
    float k = (m - ra2 - Ra2)/2.0;
    float k3 = n;
    float k2 = n*n + Ra2*rd.z*rd.z + k;
    float k1 = k*n + Ra2*ro.z*rd.z;
    float k0 = k*k + Ra2*ro.z*ro.z - Ra2*ra2;

    #if 1
    // prevent |c1| from being too close to zero
    if( abs(k3*(k3*k3 - k2) + k1) < 0.01 )
    {
        po = -1.0;
        float tmp=k1; k1=k3; k3=tmp;
        k0 = 1.0/k0;
        k1 = k1*k0;
        k2 = k2*k0;
        k3 = k3*k0;
    }
	#endif

    float c2 = 2.0*k2 - 3.0*k3*k3;
    float c1 = k3*(k3*k3 - k2) + k1;
    float c0 = k3*(k3*(-3.0*k3*k3 + 4.0*k2) - 8.0*k1) + 4.0*k0;


    c2 /= 3.0;
    c1 *= 2.0;
    c0 /= 3.0;

    float Q = c2*c2 + c0;
    float R = 3.0*c0*c2 - c2*c2*c2 - c1*c1;


    float h = R*R - Q*Q*Q;
    float z = 0.0;
    if( h < 0.0 )
    {
    	// 4 intersections
        float sQ = sqrt(Q);
        z = 2.0*sQ*cos( acos(R/(sQ*Q)) / 3.0 );
    }
    else
    {
        // 2 intersections
        float sQ = pow( sqrt(h) + abs(R), 1.0/3.0 );
        z = sign(R)*abs( sQ + Q/sQ );
    }
    z = c2 - z;

    float d1 = z   - 3.0*c2;
    float d2 = z*z - 3.0*c0;
    if( abs(d1) < 1.0e-4 )
    {
        if( d2 < 0.0 ){ hit = false; return -1.0; }
        d2 = sqrt(d2);
    }
    else
    {
        if( d1 < 0.0 ){ hit = false; return -1.0; }
        d1 = sqrt( d1/2.0 );
        d2 = c1/d1;
    }

    //----------------------------------

    float result = 1e32;

    h = d1*d1 - z + d2;
    if( h > 0.0 )
    {
        h = sqrt(h);
        float t1 = -d1 - h - k3; t1 = (po<0.0)?2.0/t1:t1;
        float t2 = -d1 + h - k3; t2 = (po<0.0)?2.0/t2:t2;
        tfn = vec2(t1, t2);
        if( t1 > 0.0 ) result=t1;
        if( t2 > 0.0 ) result=min(result,t2);
    }

    h = d1*d1 - z - d2;
    if( h > 0.0 )
    {
        h = sqrt(h);
        float t1 = d1 - h - k3;  t1 = (po<0.0)?2.0/t1:t1;
        float t2 = d1 + h - k3;  t2 = (po<0.0)?2.0/t2:t2;
        tfn = vec2(t1, t2);
        if( t1 > 0.0 ) result=min(result,t1);
        if( t2 > 0.0 ) result=min(result,t2);
    }

    hit = result > 0.0;
    inside = result <= 0.0;
    return result;
}

// df(x)/dx
vec3 ntorus( in vec3 pos, vec2 tor ){
	return normalize( pos*(dot(pos,pos)- tor.y*tor.y - tor.x*tor.x*vec3(1.0,1.0,-1.0)));
}




int ind_3Dto1D(ivec3 p, ivec3 bmax) {
    return (p.z * bmax.x * bmax.y) + (p.y * bmax.x) + p.x;
}

ivec3 ind_1Dto3D(int idx, ivec3 bmax) {
    int z = idx / (bmax.x * bmax.y);
    idx -= (z * bmax.x * bmax.y);
    int y = idx / bmax.x;
    int x = idx % bmax.x;
    return ivec3(x, y, z);
}
