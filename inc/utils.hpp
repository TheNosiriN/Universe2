
static bool LoadFile(HXUINTP& bin, const std::string& filepath){
    size_t tsize = 0;
    Files::File file;
    Files::Directory directory = Files::GetProjectDirectory();

    directory.open(file, filepath, Files::HX_FS_READ);
    tsize = file.size();
    bin = hxmalloc(file.size());

    if (file.is_open() && tsize>0){
        file.read(bin, tsize, 0);
    }else{
        return false;
    }
    return true;
}

static uint8_t* LoadTexture(const std::string& name, int width, int height, int channels){
	const char* cname = ("../assets/"+name).c_str();
	return stbi_load(cname, &width, &height, NULL, channels);
}


template<typename T, size_t C>
struct SwapHelper {
public:
	static constexpr size_t Count = C;

	T objects[Count];
	size_t current_index = 0;

	constexpr14 inline void advance(){
		current_index = (current_index+1) % Count;
	}

	constexpr14 inline T& current(){
		return objects[current_index];
	}

	constexpr14 inline T& next(){
		return objects[(current_index + 1) % Count];
	}
	constexpr14 inline T& next(const size_t& amount){
		return objects[(current_index + amount) % Count];
	}

	constexpr14 inline T& previous(){
		return objects[(current_index - 1) % Count];
	}
	constexpr14 inline T& previous(const size_t& amount){
		return objects[(current_index - amount) % Count];
	}
};




template <class DT = std::chrono::milliseconds, class ClockT = std::chrono::steady_clock>
struct ClockTimer {
    using timep_t = typename ClockT::time_point;
    timep_t _start;
    timep_t _end;

    void start() {
        _end = timep_t{};
        _start = ClockT::now();
    }

    void stop() { _end = ClockT::now(); }

    template <class T = DT>
    T duration() const {
        return std::chrono::duration_cast<T>(_end - _start);
    }
};



template<typename T>
struct SpcRange {
    T start;
    T count;

    inline constexpr void zero(const T& st){
        start = st;
        count = 0;
    }
    inline constexpr T abs_next() const {
        return start + count;
    }
    inline constexpr void increment() {
        count += 1;
    }
};



#define saturate(x) clamp(x, 0.0f, 1.0f)


namespace StaticUtils {

    template<typename T, int Total>
    inline static T weight_sum(const std::initializer_list<T> list) noexcept {
        T result = 0;
        for (const T& val : list){ result += val; }
        return result / Total;
    }


    static inline int double_extract_exponent(double num) {
        return (num == 0) ? 0 : (int)std::floor(std::log10(std::fabs(num)));
    }

    static inline float double_extract_mantissa(double num, int exp){
        return float(num / std::pow(10.0, exp));
    }

    static inline vec3 double_extract_mantissa(dvec3 vec, int exp){
        return vec3(vec / std::pow(10.0, exp));
    }


	template<typename T>
	struct LerpValue {
		T current;
		T previous;
	};
	template<typename T>
	static void UpdateLerpVal(LerpValue<T>& val, T lerpspeed){
		val = Mathgl::mix(val.previous, val.current, lerpspeed);
	}


	static uint ConstructBasicSeed(ivec4 p){
		return (uint(p.x)*1973u + uint(p.y)*9277u + uint(p.z)*10195u + uint(p.w)*26699u) | 1u;
	}

	static uint32_t wang_hash(uint& seed){
		seed = (seed ^ 61u) ^ (seed >> 16u);
	    seed *= 9u;
	    seed = seed ^ (seed >> 4);
	    seed *= 0x27d4eb2du;
	    seed = seed ^ (seed >> 15);
	    return seed;
	}

	static float frandom(uint& state){
	    return float(wang_hash(state)) / float(0xffffffffu);
	}

	static vec3 frandom_sphere(uint32_t& seed){
		vec3 x = vec3(
			frandom(seed), frandom(seed), frandom(seed)
		) *2.0f - 1.0f;
		return normalize(x);
	}

	static quat frandom_quaternion(uint32_t& seed){
		float u = frandom(seed);
		float v = frandom(seed);
		float w = frandom(seed);
		float two_pi = Mathgl::pi<float>() * 2.0f;
		return quat(
			sqrt(1-u)*sin(two_pi*v),
			sqrt(1-u)*cos(two_pi*v),
			sqrt(u)*sin(two_pi*w),
			sqrt(u)*cos(two_pi*w)
		);
	}



	static int ind_3Dto1D(ivec3 p, ivec3 bmax) {
	    return (p.z * bmax.x * bmax.y) + (p.y * bmax.x) + p.x;
	}

	static ivec3 ind_1Dto3D(int idx, ivec3 bmax) {
	    int z = idx / (bmax.x * bmax.y);
	    idx -= (z * bmax.x * bmax.y);
	    int y = idx / bmax.x;
	    int x = idx % bmax.x;
	    return ivec3(x, y, z);
	}



	static inline float easeInExpo(float x) {
		return x==0.0f ? 0.0f : pow(2.0f, 10.0f*x - 10.0f);
	}
	static inline float easeOutExpo(float x) {
		return x==1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * x);
	}
	static inline float easeOutInExpo( float t ){
		if( t < 0.5f ) return easeOutExpo(2.0f * t) / 2.0f;
		return easeInExpo(2.0f * t - 1.0f) / 2.0f + 0.5f;
	}



	static dvec3 mulvq(quat q, dvec3 v){		//rotates a 3D vector by a quaternion.
		dvec3 result;
		double axx = q.x * 2.0;
		double ayy = q.y * 2.0;
		double azz = q.z * 2.0;
		double awxx = q.w * axx;
		double awyy = q.w * ayy;
		double awzz = q.w * azz;
		double axxx = q.x * axx;
		double axyy = q.x * ayy;
		double axzz = q.x * azz;
		double ayyy = q.y * ayy;
		double ayzz = q.y * azz;
		double azzz = q.z * azz;
		result.x = ((v.x * ((1.0 - ayyy) - azzz)) + (v.y * (axyy - awzz))) + (v.z * (axzz + awyy));
		result.y = ((v.x * (axyy + awzz)) + (v.y * ((1.0 - axxx) - azzz))) + (v.z * (ayzz - awxx));
		result.z = ((v.x * (axzz - awyy)) + (v.y * (ayzz + awxx))) + (v.z * ((1.0 - axxx) - ayyy));
		return result;
	}

	static quat qaxang(vec3 ax, float ang){	//creates a quaternion from axis and angle.
		float ha = ang * 0.5f;
		float sha = sin(ha);
		return quat(ax.x * sha, ax.y * sha, ax.z * sha, cos(ha));
	}

	static mat2 rotation2D(float angle){
	    float s = sin(angle), c = cos(angle);
	    return mat2( c, -s, s, c );
	}



	// Converts from pure Hue to linear RGB
	static vec3 hue_to_rgb(float hue){
	    float R = abs(hue * 6.0f - 3.0f) - 1.0f;
	    float G = 2.0f - abs(hue * 6.0f - 2.0f);
	    float B = 2.0f - abs(hue * 6.0f - 4.0f);
	    return clamp(vec3(R,G,B), 0.0f, 1.0f);
	}

	// Converts from HSV to linear RGB
	static vec3 hsv_to_rgb(vec3 hsv){
	    vec3 rgb = hue_to_rgb(hsv.x);
	    return ((rgb - 1.0f) * hsv.y + 1.0f) * hsv.z;
	}



	static dvec3 warp_uvsphere_to_vec3(dvec2 uv){
		dvec2 s = (uv-dvec2(0,0.5)) * dvec2(pi<double>(),pi<double>()*2.0);
	    return dvec3(
	        sin(s.x) * cos(s.y),
	        sin(s.x) * sin(s.y),
	        cos(s.x)
	    );
	}
	static dvec2 warp_vec3_to_uvsphere(dvec3 p){
		p = normalize(p);
		dvec2 uv = dvec2(
	        acos(p.z)/pi<double>(),
	        atan(p.y,p.x)/(pi<double>()*2.0) + 0.5
	    );
	    return uv;
	}
}
