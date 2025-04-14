//from: https://www.shadertoy.com/view/ttsyRB
vec3 random3(vec3 c) {
    vec3 r = hash33(c);
	return r-0.5;
    //return r *2.0-1.0;
    // return normalize(r-0.5)*.5;
}

const float F3 = 0.3333333;
const float G3 = 0.1666667;

vec4 simplex3d(vec3 p) {
    vec3 s = floor(p + dot(p, vec3(F3)));
    vec3 x = p - s + dot(s, vec3(G3));

    vec3 e = step(vec3(0.0), x - x.yzx);
    vec3 i1 = e*(1.0 - e.zxy);
    vec3 i2 = 1.0 - e.zxy*(1.0 - e);

    vec3 x1 = x - i1 + G3;
    vec3 x2 = x - i2 + 2.0*G3;
    vec3 x3 = x - 1.0 + 3.0*G3;

    vec4 w, d;

    w.x = dot(x, x);
    w.y = dot(x1, x1);
    w.z = dot(x2, x2);
    w.w = dot(x3, x3);
    w = max(0.6 - w, 0.0);

    d.x = dot(random3(s), x);
    d.y = dot(random3(s + i1), x1);
    d.z = dot(random3(s + i2), x2);
    d.w = dot(random3(s + 1.0), x3);

    w *= w;
    w *= w;
    d *= w;

    return 70.0*d;
}

float variations(vec4 n) {
    vec4 an = abs(n);
    vec2 s = vec2(dot(an, vec4(1.)), max(max(max(an.x, an.y), an.z), an.w));

    float t = 0.47;
    //return step( .7, (1.+t)*( (1.-t)+(s.x-s.y/t)*t) );
    return pow( (1.0+t)*( (1.0-t)+(s.x-s.y/t)*t), 2.0);
}

/* const matrices for 3d rotation */
const mat3 rot1 = mat3(-0.37, 0.36, 0.85,-0.14,-0.93, 0.34,0.92, 0.01,0.4);
const mat3 rot2 = mat3(-0.55,-0.39, 0.74, 0.33,-0.91,-0.24,0.77, 0.12,0.63);
const mat3 rot3 = mat3(-0.71, 0.52,-0.47,-0.08,-0.72,-0.68,-0.7,-0.45,0.56);

/* directional artifacts can be reduced by rotating each octave */
float simplex3d_fractal(vec3 p, float scale) {
    p *= scale;
    return   0.5333333*variations( simplex3d(p*rot1) 	 )
			+0.2666667*variations( simplex3d(2.0*p*rot2) )
			+0.1333333*variations( simplex3d(4.0*p*rot3) )
			+0.0666667*variations( simplex3d(8.0*p) 	 );
}

float cosmicWebDensity(vec3 p, float scale){
    // float noise = simplex3d_fractal(p, scale);
    // noise = smoothstep(0.5, 1.0, noise) + noise;
    // noise = smoothstep(0.2, 1.0, noise);
    return simplex3d_fractal(p, scale);
}


float simplex3d_fractal_simple(vec3 p, float scale){
    // return 0.5*variations(simplex3d(p*rot1)) + 0.5*variations(simplex3d(2.0*p));
    return variations(simplex3d(p));
}

float cosmicWebDensitySimple(vec3 p, float scale){
    float noise = simplex3d_fractal(p, scale);
    noise = smoothstep(0.5, 1.0, noise) + noise;
    noise = smoothstep(0.2, 1.0, noise);
    return noise;
}
