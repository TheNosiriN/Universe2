


#ifndef HEXO_GRAPHICS_H
#define HEXO_GRAPHICS_H






#ifndef HEXO_DEFINITIONS_H
#define HEXO_DEFINITIONS_H



#if !defined(NOMINMAX)
	#define NOMINMAX
#endif
#define _CRT_SECURE_NO_WARNINGS





#include <cstdio>      // vsnprintf, sscanf, printf
#include <cmath>
#include <cstdlib>
#include <cstring>


#if !defined(alloca)
	#if defined(__GLIBC__) || defined(__sun) || defined(__CYGWIN__)
		#include <alloca.h>     // alloca
	#elif defined(_WIN32)
		#include <malloc.h>     // alloca
		#if !defined(alloca)
			#define alloca _alloca  // for clang with MS Codegen
		#endif
	#else
		#include <stdlib.h>     // alloca
	#endif
#endif


#ifdef _MSC_VER
	#define __HXCPP_VER _MSVC_LANG
#else
	#define __HXCPP_VER __cplusplus
#endif





#include <string>
#include <chrono>
#include <utility>
#include <limits>

#include <atomic>
#include <functional>

#include <thread> /// this is used by HXDedicatedThreadTicker
#include <vector>	/// this is used in GraphicsEngine and AssetExports as ShaderIntermediateBinary
#include <fstream>
#include <iostream>




#ifndef _MSC_VER
	#include <cxxabi.h>
#endif

#ifndef HEXO_RUNTIME_NO_EXCEPTIONS
	#include <stdexcept>
#endif

#ifndef HEXO_BUILD_STATIC
	#include <type_traits>
#endif


#define hxmalloc malloc
#define hxmemset memset
#define hxmemcpy memcpy
#define hxmemmove memmove

#if __HXCPP_VER >= 201703L
	#define constexpr17 constexpr
#else
	#define constexpr17
#endif

#if __HXCPP_VER >= 201402L
	#define constexpr14 constexpr
#else
	#define constexpr14
#endif

#if __HXCPP_VER >= 201103L
	#define constexpr11 constexpr
#else
	#define constexpr11
#endif


#ifdef _MSC_VER
	#define hx_noinline __declspec(noinline)
#else
	#define hx_noinline __attribute__((noinline))
#endif


#ifdef _MSC_VER
	#define hx_forceinline __forceinline
#else
	#define hx_forceinline __attribute__((always_inline))
#endif







#ifdef UNICODE
	#define HX_FS_WIDESTRING
#endif



#ifdef HEXO_BUILD_STATIC
	#undef DEBUG
	#undef _DEBUG
	#define HEXO_GRAPHICS_NO_TRACKING
	#define HEXO_THREADING_NO_TRACKING
#endif



#if defined(DEBUG) || defined(_DEBUG) || defined(HEXO_BUILD_DEBUG)
	#define HEXO_ENGINE_ENABLE_ERRORS
    #define HEXO_THREADING_ENABLE_ERRORS
    #define HEXO_GRAPHICS_ENABLE_ERRORS
    #define HEXO_INPUT_ENABLE_ERRORS
    #define HEXO_AUDIO_ENABLE_ERRORS
#else
	#undef HEXO_ENGINE_ENABLE_ERRORS
	#undef HEXO_THREADING_ENABLE_ERRORS
	#undef HEXO_GRAPHICS_ENABLE_ERRORS
	#undef HEXO_INPUT_ENABLE_ERRORS
	#undef HEXO_AUDIO_ENABLE_ERRORS
#endif




#ifdef HEXO_PLATFORM_OS_WINDOWS
	#include <windows.h>
	#include <windowsx.h>
#endif











namespace Hexo {


	// By: TheNosiriN
	// HXUINTP can only be used with integral types
	template<typename Size, bool = (std::is_integral<Size>::value==true)>
	struct HXSIZEP_t;

	template<typename Size>
	struct HXSIZEP_t<Size, true> {
	    Size _value_;

	    // converters
	    template<
	        typename T,
	        typename = typename std::enable_if<
	            std::is_trivially_constructible<Size, typename std::remove_cv<T>::type>::value == false
	            && (sizeof(T) >= sizeof(Size))
	        >::type
	    >
	    constexpr inline operator T() const noexcept {
	        return (T)(_value_);
	    }

	    constexpr inline operator Size() const noexcept {
	        return _value_;
	    }

	    // constructors
	    constexpr inline HXSIZEP_t() noexcept : _value_(0) {}

	    template<typename T, typename = typename std::enable_if<sizeof(T) <= sizeof(Size)>::type>
	    constexpr inline HXSIZEP_t(T&& v) noexcept : _value_((Size)(v)) {}

		// custom arithmetic
		template<typename Int, typename = typename std::enable_if<std::is_integral<Int>::value == true>::type>
		constexpr inline HXSIZEP_t operator + (const Int& b) const noexcept {
			return HXSIZEP_t(_value_ + b);
		}

		template<typename Int, typename = typename std::enable_if<std::is_integral<Int>::value == true>::type>
		constexpr inline HXSIZEP_t operator * (const Int& b) const noexcept {
			return HXSIZEP_t(_value_ * b);
		}

		template<typename Int, typename = typename std::enable_if<std::is_integral<Int>::value == true>::type>
		constexpr inline HXSIZEP_t operator - (const Int& b) const noexcept {
			return HXSIZEP_t(_value_ - b);
		}

		// stream ops
		template<typename Stream_t>
	    constexpr inline friend Stream_t& operator<<(Stream_t& os, const HXSIZEP_t& p){ return os << p._value_; }

	    template<typename Stream_t>
	    constexpr inline friend Stream_t& operator>>(Stream_t& is, HXSIZEP_t& p){ return is >> p._value_; }

	};


	using HXUINTP = HXSIZEP_t<size_t>;		// specialized for size type
	using HXUINTP32 = HXSIZEP_t<uint32_t>;	// specialized for uint32_t
	////







    typedef int32_t HXSIZE_I32;
    typedef int16_t HXSIZE_I16;
    typedef int8_t HXSIZE_I8;

    typedef uint32_t HXSIZE_U32;
    typedef uint16_t HXSIZE_U16;
    typedef uint8_t HXSIZE_U8;

    typedef size_t HXSIZE;
    typedef HXSIZE_I32 HXSIZE_I;
	// typedef size_t HXUINTP;

	struct HXPTRPAIR {
		HXUINTP ptr;
		HXSIZE size;
	};


	#ifdef HX_FS_WIDESTRING
		typedef std::wstring HXFileString;
		typedef wchar_t HXFileChar;
		#define HX_MKFSTR(str) L ## str
	#else
		typedef std::string HXFileString;
		typedef char HXFileChar;
		#define HX_MKFSTR(str) str
	#endif

}











#ifndef HEXO_UTILS_H
#define HEXO_UTILS_H







#if (defined(HEXO_PLATFORM_OS_WINDOWS_11) || defined(HEXO_PLATFORM_OS_WINDOWS_10) || defined(HEXO_PLATFORM_OS_WINDOWS))
	#define HXENTRY_API extern "C" __declspec(dllexport)
#else
	#define HXENTRY_API
#endif



#ifdef _MSC_VER
#    if (_MSC_VER >= 1800)
#        define __alignas_is_defined 1
#    endif
#    if (_MSC_VER >= 1900)
#        define __alignof_is_defined 1
#    endif
#else
#    include <stdalign.h>   // __alignas/of_is_defined directly from the implementation
#endif

#ifdef __alignas_is_defined
#    define ALIGN(X) alignas(X)
#else
#    ifdef __GNUG__
#        define ALIGN(X) __attribute__ ((aligned(X)))
#    elif defined(_MSC_VER)
#        define ALIGN(X) __declspec(align(X))
#    else
#        error Unknown compiler, unknown alignment attribute!
#    endif
#endif

#ifdef __alignof_is_defined
#    define ALIGNOF(X) alignof(x)
#else
#    ifdef __GNUG__
#        define ALIGNOF(X) __alignof__ (X)
#    elif defined(_MSC_VER)
#        define ALIGNOF(X) __alignof(X)
#    else
#        error Unknown compiler, unknown alignment attribute!
#    endif
#endif


#define HX_PLACEMENT_NEW(address, type) new(address) type


#define HX_MAKE_NON_COPYABLE_TYPE(type) type(const type&)=delete; \
type& operator=(const type&)=delete;


#define HX_CHECK_FLAG(p, flag, action) if (p & flag){ action; }


#define MKSTR(x) #x
#define __RAW_STRING__(x) R ## #x
#define MKRWSTR(x) __RAW_STRING__((x))


#define HX_PACK_STRUCT(str) _Pragma("pack(push, 1)") str _Pragma("pack(pop)")


#define HX_LENGTH_C_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))


#define HX_NULL_ID_MAKE_ID(i) (i+1)
#define HX_NULL_ID_GET_ID(i) (i-1)


#define HX_MEM_ALIGN(x, multiple) (multiple * Mathgl::ceil(float(x)/multiple))








namespace Hexo {

	namespace Utils {


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//////
		////// Null pointer function tester
		//////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		#define HX_TEST_FUNCTION(F, ...) (F==nullptr ? Hexo::Utils::__ReturnDefaultTypeForFunction(F, __VA_ARGS__) : F(__VA_ARGS__))

		/// If its less than C++17 standard
		#if __HXCPP_VER >= 201703L
			template<typename F, typename... P>
			using safe_result_of = std::invoke_result<F, P...>;
		#else
			template<typename F, typename... P>
			using safe_result_of = std::result_of<F(P...)>;
		#endif

		template<typename F, typename... P>
		constexpr inline typename safe_result_of<F, P...>::type __ReturnDefaultTypeForFunction(F&& a, P&&... v) {
			return typename safe_result_of<F, P...>::type{};
		}

        /////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//////
		////// some string utils
		//////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

        /// see: https://web.archive.org/web/20151229003112/http://blogs.msdn.com/b/jeuge/archive/2005/06/08/hakmem-bit-count.aspx
        constexpr inline int CountNumberOfBits(unsigned int u){
            unsigned int uCount = u - ((u >> 1) & 033333333333) - ((u >> 2) & 011111111111);
            return ((uCount + (uCount >> 3)) & 030707070707) % 63;
        }

		/////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//////
		////// some string utils
		//////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		// inline std::string loadfile(const HXFileString& f){
		// 	std::ifstream file(f, std::ios::in | std::ios::binary);
	    //     return std::string(
	    //         std::istreambuf_iterator<char>( file ),
	    //         std::istreambuf_iterator<char>()
	    //     );
		// }

		inline std::string wide2str(const std::wstring& wide) noexcept {
			std::string str(wide.length(), 0);
			for (size_t i=0; i<wide.length(); (str[i] = char(wide[i])), ++i);
			return str;
		}

		inline std::wstring str2wide(const std::string& str) noexcept {
			std::wstring wide(str.length(), 0);
			for (size_t i=0; i<str.length(); (wide[i] = wchar_t(str[i])), ++i);
			return wide;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// Default Placeholder Allocator
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

	    template <typename T>
	    struct __DefaultAllocator{

	        constexpr14 inline T* allocate(const size_t& n) {
	            return static_cast<T*>(__allocate(n, sizeof(T)));
	        }

	        constexpr14 hx_noinline void deallocate(T* p, const size_t& n) noexcept {
	            std::free(p);
	        }

	        constexpr14 hx_noinline void* __allocate(const size_t& size, const size_t& stride) noexcept {
	            if (size > std::numeric_limits<size_t>::max() / stride)return nullptr; /// issue a HXRC_FATAL message

				void* p = hxmalloc(size*stride);
				if (!p){} /// issue a HXRC_FATAL message

				return p;
	        }

	    };

	    template <class T, class U>
	    inline bool operator==(const __DefaultAllocator<T>&, const __DefaultAllocator<U>&) { return true; }
	    template <class T, class U>
	    inline bool operator!=(const __DefaultAllocator<T>&, const __DefaultAllocator<U>&) { return false; }

		/////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        ////// Author: Fabio Alemagna <personal@fabioalemagna.net>
        ////// Source: https://github.com/falemagn/fameta-counter
        ////// Inspired by Filip Roséen's work. See https://stackoverflow.com/questions/60082260/c-compile-time-counters-revisited
        /////////////////////////////////////////////////////////////////////////////////////////////////////

		namespace fameta {
		    struct default_counter_tag{};

		    template <int StartN, int StartValue = 0, int Step = 1, typename Tag = default_counter_tag>
		    class counter {
		    #if defined(__INTEL_COMPILER) || defined(_MSC_VER) || !defined(__cpp_decltype_auto)
		        template <int N>
		        struct slot {
		            #if defined(__INTEL_COMPILER)
		            #   pragma warning push
		            #   pragma warning disable 1624
		            #elif defined(__GNUC__) && !defined(__clang__)
		            #   pragma GCC diagnostic push
		            #   pragma GCC diagnostic ignored "-Wnon-template-friend"
		            #endif
		            friend constexpr int slot_value(slot<N>);
		            #if defined(__INTEL_COMPILER)
		            #   pragma warning pop
		            #elif defined(__GNUC__) && !defined(__clang__)
		            #   pragma GCC diagnostic pop
		            #endif
		        };

		        template <int N, int I>
		        struct writer {
		            friend constexpr int slot_value(slot<N>) {
		                return I;
		            }

		            static constexpr int value = I;
		        };

		        template <int N, int R = slot_value(slot<N>())>
		        static constexpr int reader(int, slot<N>) {
		            return R;
		        };
		    #else
		        template <int N>
		        struct slot {
		            #if defined(__GNUC__) && !defined(__clang__)
		            #   pragma GCC diagnostic push
		            #   pragma GCC diagnostic ignored "-Wnon-template-friend"
		            #endif
		            friend constexpr auto slot_value(slot<N>);
		            #if defined(__GNUC__) && !defined(__clang__)
		            #   pragma GCC diagnostic pop
		            #endif
		        };

		        template <int N, int I>
		        struct writer {
		            friend constexpr auto slot_value(slot<N>) {
		                return I;
		            }

		            static constexpr int value = I;
		        };

		        template <int N, typename = decltype(slot_value(slot<N>()))>
		        static constexpr int reader(int, slot<N>, int R = slot_value(slot<N>())) {
		            return R;
		        }
		    #endif

		        static_assert(sizeof(writer<StartN, StartValue-Step>), "Base case");

		        template <int N>
		        static constexpr int reader(float, slot<N>, int R = reader(0, slot<N-1>())) {
		            return R;
		        }

		    public:

		    #if !defined(__clang_major__) || __clang_major__ > 7
		        template <int N>
		        static constexpr int next(int R = writer<N, reader(0, slot<N-1>())+Step>::value) {
		            return R;
		        }

		        template <int N>
		        static constexpr int current(int R = reader(0, slot<N-1>())) {
		            return R;
		        }
		    #else
		        template <int N>
		        static constexpr int next(int R = writer<N, reader(0, slot<N>())+Step>::value) {
		            return R;
		        }

		        template <int N>
		        static constexpr int current(int R = reader(0, slot<N>())) {
		            return R;
		        }
		    #endif
		    };
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// Typename string demangler
        ////// NOTE: The result of this function is compiler implementation defined
        /////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T>
    	inline std::string type_to_string() {
            using TR = typename std::remove_reference<T>::type;

		#ifndef _MSC_VER
			std::string name = abi::__cxa_demangle(typeid(TR).name(), NULL, NULL, NULL);
		#else
			std::string name = typeid(TR).name();
		#endif

            if (std::is_const<TR>::value) name += " const";
            if (std::is_volatile<TR>::value) name += " volatile";
            if (std::is_lvalue_reference<T>::value) name += "&";
            else if (std::is_rvalue_reference<T>::value) name += "&&";
            return name;
    	}

        /////////////////////////////////////////////////////////////////////////////////////////////////////










		//////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// Function + data pair
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        template<typename Func>
        struct HXFunctionPair {
            Func func;
            HXUINTP data;

            HXFunctionPair() : func(NULL), data(NULL) {}
            HXFunctionPair(const Func& f) : func(f), data(NULL){}
            HXFunctionPair(const Func& f, const HXUINTP& d) : func(f), data(d) {}

            constexpr14 HXFunctionPair& operator=(const Func& other){
                func = other;
                data = NULL;
                return *this;
            }

            constexpr14 inline operator Func() {
    	        return func;
    	    }

            template<typename... Args>
            constexpr14 inline typename safe_result_of<Func, Args...>::type operator()(Args&&... a) {
    	        return func(a...);
    	    }
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////



	}
}











#ifndef HEXO_UTILS_TYPED_ARRAY_H
#define HEXO_UTILS_TYPED_ARRAY_H



namespace Hexo {
	namespace Utils {


		template<typename T, size_t N>
		struct TypedArray {
			using value_type = T;
			using size_type = size_t;

		private:
			value_type m_data[N];

		public:
			constexpr inline value_type& operator[](const size_type& i) {
				return m_data[i];
			}
			constexpr inline value_type* operator+(const size_type& i) {
				return m_data+i;
			}

			constexpr inline value_type* at(const size_type& i) {
				if (i >= N)return nullptr;
				return m_data+i;
			}


			constexpr inline value_type* data() const { return m_data; }
			constexpr inline size_type size() const { return N; }
			constexpr inline size_type stride() const { return sizeof(value_type); }


            template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == true, void>::type
			__ConstructInPlace(const size_type& ptr, Args&&... a) {
				HX_PLACEMENT_NEW(m_data+ptr, NT(std::forward<NT>(a)...));
			}

			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == false, void>::type
			__ConstructInPlace(const size_type& ptr, Args&&... a) {
				if (std::is_default_constructible<NT>::value){
					HX_PLACEMENT_NEW(m_data+ptr, NT{a...});
				}else{
					/// at this point, the object can never be constructed with the params the user gave it
					/// so just run its trivial default constructor
					HX_PLACEMENT_NEW(m_data+ptr, NT{});
				}
			}


            template<typename... Args>
            constexpr14 inline void emplace(size_type index, Args&&... a){
                __ConstructInPlace<T>(index, a...);
            }


			constexpr14 inline void clear() {
				for (size_type i=0; i<N; ++i){
					m_data[i].~value_type();
				}
			}

		};


	}
}




#endif /* end of include guard: HEXO_UTILS_TYPED_ARRAY_H */




#ifndef HEXO_UTILS_TYPED_VECTOR_H
#define HEXO_UTILS_TYPED_VECTOR_H


#include <type_traits>
#include <typeindex>


namespace Hexo {
	namespace Utils {



		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////// MiniTypedVector
		////// This vector is used when there is no need for speed on push_back and erase but still need for locality
		////// e.g. Resource management. This vector trusts that the allocator will handle its memory when speed is needed
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T, typename U = __DefaultAllocator<uint8_t>>
		struct mini_typed_vector {
			using _Uint = size_t;
			using size_type = _Uint;
			using alloc_type = U;
			using value_type = T;

		private:
			value_type* m_data = nullptr;
			_Uint m_size = 0;

			alloc_type alloc;

		public:

			constexpr inline value_type& operator[](const _Uint& i) const {
				return m_data[i];
			}
			constexpr inline value_type* operator+(const _Uint& i) const {
				return m_data+i;
			}

			constexpr inline value_type* at(const _Uint& i) const {
				if (!m_data || i >= m_size)return nullptr;
				return m_data + i;
			}


			constexpr14 inline void transfer(value_type** out) noexcept { (*out) = m_data; m_data=nullptr, m_size=0; }
			constexpr inline value_type* data() const { return this->m_data; }
			constexpr inline _Uint size() const { return this->m_size; }
			constexpr inline _Uint stride() const { return sizeof(value_type); }
			constexpr inline _Uint last_index() const { return (this->m_size>0) ? (this->m_size-1) : 0; }
			constexpr inline bool empty() const { return this->m_size == 0; }
            inline value_type& last() { return this->m_data[this->m_size-1]; }


			/// constructors
			constexpr14 mini_typed_vector() : m_size(0), m_data(nullptr), alloc() {}
			constexpr14 mini_typed_vector(const alloc_type& a) : m_size(0), m_data(nullptr), alloc(a) {}
			~mini_typed_vector(){ clear(); }

			/// copy semantics
			constexpr14 mini_typed_vector(const mini_typed_vector& other) : m_size(other.m_size), alloc(other.alloc){
				m_data = (value_type*)alloc.allocate(other.m_size*sizeof(value_type));
				__SafeCopy(m_data, other.m_data, other.m_size);
			}
			constexpr14 mini_typed_vector& operator=(const mini_typed_vector& other){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_size*sizeof(value_type));
				}

				m_size = other.m_size;
				alloc = other.alloc;
				m_data = (value_type*)alloc.allocate(other.m_size*sizeof(value_type));
				__SafeCopy(m_data, other.m_data, other.m_size);

				return *this;
			}

			/// move semantics
			constexpr14 mini_typed_vector(mini_typed_vector&& other) : m_size(other.m_size), alloc(other.alloc){
				other.transfer(&this->m_data);
			}
			constexpr14 mini_typed_vector& operator=(mini_typed_vector&& other){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_size*sizeof(value_type));
				}

				m_size = other.m_size;
				alloc = other.alloc;
				other.transfer(&this->m_data);
				return *this;
			}


			constexpr14 void reset(){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_size*sizeof(value_type));
				}
				m_data = nullptr;
				m_size = 0;
			}


			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == true, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count){
				hxmemcpy(dst, src, count*sizeof(NT));
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == false, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count){
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{src[i]}); }
			}


			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == true, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count){
				hxmemmove(dst, src, count*sizeof(NT));
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == false, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count){
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{ std::move(src[i]) }); }
			}


			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == true, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) {
				HX_PLACEMENT_NEW(m_data+ptr, NT(std::forward<NT>(a)...));
			}

			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == false, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) {
				if (std::is_default_constructible<NT>::value){
					HX_PLACEMENT_NEW(m_data+ptr, NT{a...});
				}else{
					/// at this point, the object can never be constructed with the params the user gave it
					/// so just run its trivial default constructor
					HX_PLACEMENT_NEW(m_data+ptr, NT{});
				}
			}


			constexpr14 bool __Reallocate(const _Uint& count){
				value_type* ndata = (value_type*)(alloc.allocate((m_size+count)*sizeof(value_type)));
				if (!ndata)return false;

				if (m_data && ndata!=m_data){
					__SafeMove(ndata, m_data, m_size);
				}

				alloc.deallocate((uint8_t*)(m_data), m_size*sizeof(value_type));
				m_data = ndata;
				return true;
			}


            template<typename... Args>
			constexpr14 inline _Uint emplace_mult(const _Uint& pos, const _Uint& count, Args&&... a){
				if (!__Reallocate(count))return 0;

				__SafeCopy(m_data+pos+count, m_data+pos, m_size-pos);

				for (_Uint i=0; i<count; ++i){
					__ConstructInPlace<T>(pos+i, a...);
				}
				m_size += count;
				return pos;
			}

			template<typename... Args>
			constexpr14 _Uint emplace(const _Uint& pos, Args&&... a){
				return emplace_mult(pos, 1, a...);
			}

			constexpr14 inline _Uint insert_mult(const _Uint& pos, const _Uint& count, T* data){
				if (!__Reallocate(count))return 0;

				__SafeCopy(m_data + (pos+count), m_data+pos, m_size-pos);
				__SafeCopy(m_data + pos, data, 1);

				m_size += count;
				return pos;
			}

			constexpr14 _Uint insert(const _Uint& pos, T&& data){
				return insert_mult(pos, 1, &data);
			}


			template<typename... Args>
			constexpr14 _Uint emplace_back(Args&&... a){
				if (!__Reallocate(1))return 0;

				__ConstructInPlace<T>(m_size, a...);
				m_size += 1;
				return m_size-1;
			}

			constexpr14 _Uint push_back(const value_type& obj){
				if (!__Reallocate(1))return 0;

				__SafeCopy<value_type>(m_data+m_size, &obj, 1);
				m_size += 1;
				return m_size-1;
			}


            constexpr14 inline _Uint push_back_raw(const value_type* obj){
				if (!__Reallocate(1))return 0;
				hxmemcpy(m_data+m_size, obj, sizeof(value_type));
				m_size += 1;
				return m_size-1;
			}


			/// TODO: FIX THIS! IT IS LEAKING MEMORY!!!
			constexpr14 bool eraseFast(const _Uint& i){
				if (!m_data || i >= m_size)return false;

				value_type* ndata = (value_type*)(alloc.allocate((m_size-1)*sizeof(value_type)));
				if (!ndata)return false;

				if (m_data && ndata!=m_data){
					__SafeCopy(ndata, m_data, i);
					__SafeCopy(ndata, m_data+i+1, m_size-(i+1));
				}

				alloc.deallocate((uint8_t*)(m_data), m_size*sizeof(value_type));
				m_data = ndata;
				m_size -= 1;
				return true;
			}
			///

			constexpr14 bool erase(const _Uint& i){
				T* p = at(i);
				if (p){ (*p).~value_type(); }
				return eraseFast(i);
			}



			constexpr14 inline void clearFast(){
				reset();
			}

			constexpr14 void clear(){
				for (_Uint i=0; i<m_size; ++i){
					m_data[i].~value_type();
				}
				reset();
			}

		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////












		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//////
		////// TypedVector
		////// This vector is used as a replacement to the common std::vector
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T, typename U = __DefaultAllocator<uint8_t>>
		struct typed_vector {
			using _Uint = size_t;
			using size_type = _Uint;
			using alloc_type = U;
			using value_type = T;

		private:
			value_type* m_data = nullptr;
			_Uint m_size = 0;
			_Uint m_realsize = 0;

			alloc_type alloc;

		public:

			constexpr inline value_type& operator[](const _Uint& i) const {
				return m_data[i];
			}
			constexpr inline value_type* operator+(const _Uint& i) const {
				return m_data+i;
			}


			struct Iterator {
				typed_vector* parent = nullptr;
				_Uint index = 0;

				constexpr14 inline Iterator& operator++(){
					if (parent){ index += 1; }
					return *this;
				}
				constexpr14 inline Iterator& operator--(){
					if (parent){ index -= (index==0 ? 0 : 1); }
					return *this;
				}
				constexpr14 inline Iterator operator++(int){
					Iterator t = *this;
					++*this; return t;
				}
				constexpr14 inline Iterator operator--(int){
					Iterator t = *this;
					--*this; return t;
				}
				constexpr inline Iterator operator+(const _Uint& num) const {
					return Iterator{ this->parent, this->index+num };
				}
				constexpr inline Iterator operator-(const _Uint& num) const {
					return Iterator{ this->parent, this->index-num };
				}
				constexpr14 inline Iterator& operator+=(const _Uint& num){
					this->index += num;
					return *this;
				}
				constexpr14 inline Iterator& operator-=(const _Uint& num){
					this->index -= num;
					return *this;
				}

				constexpr inline bool operator!=(const Iterator& t) const { return (this->index != t.index); }
				constexpr inline bool operator==(const Iterator& t) const { return (this->index == t.index); }

				constexpr inline value_type* operator*() const {
					if (parent){ return parent->at(index); }
					return nullptr;
				}

			};

			constexpr14 inline Iterator begin() { return Iterator{ this, 0 }; }
			constexpr14 inline Iterator end() { return Iterator{ this, this->m_size }; }


			/// constructors
			constexpr14 typed_vector() : m_size(0), m_realsize(0), m_data(nullptr), alloc() {}
			constexpr14 typed_vector(const alloc_type& a) : m_size(0), m_realsize(0), m_data(nullptr), alloc(a) {}
			~typed_vector(){ clear(); reset(); }

			/// copy semantics
			constexpr14 typed_vector(const typed_vector& other) : m_size(other.m_size), m_realsize(other.m_size), alloc(other.alloc){
				m_data = (value_type*)alloc.allocate(other.m_size*sizeof(value_type));
				__SafeCopy(m_data, other.m_data, other.m_size);
			}
			constexpr14 typed_vector& operator=(const typed_vector& other){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_realsize*sizeof(value_type));
				}

				m_size = other.m_size;
				m_realsize = other.m_size;
				alloc = other.alloc;
				m_data = (value_type*)alloc.allocate(other.m_size*sizeof(value_type));
				__SafeCopy(m_data, other.m_data, other.m_size);

				return *this;
			}

			/// move semantics
			constexpr14 typed_vector(typed_vector&& other) : m_size(other.m_size), m_realsize(other.m_size), alloc(other.alloc){
				other.transfer(&this->m_data);
			}
			constexpr14 typed_vector& operator=(typed_vector&& other){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_realsize*sizeof(value_type));
				}

				m_size = other.m_size;
				m_realsize = other.m_size;
				alloc = other.alloc;
				other.transfer(&this->m_data);
				return *this;
			}


			constexpr14 void reset(){
				if (m_data){
					alloc.deallocate((uint8_t*)(m_data), m_realsize*sizeof(value_type));
				}
				m_data = nullptr;
				m_realsize = 0;
				m_size = 0;
			}


			constexpr inline value_type* __GetNoCheck(const _Uint& i) const {
				return m_data + i;
			}

			constexpr inline bool __ValidateIndex(const _Uint& i) const {
				return !(!m_data || i>=m_size);
			}

			constexpr inline value_type* at(const _Uint& i) const {
				if (!m_data || i >= m_size)return nullptr;
				return m_data + i;
			}



			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == true, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				hxmemcpy(dst, src, count*sizeof(NT));
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == false, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{src[i]}); }
			}


			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == true, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				hxmemmove(dst, src, count*sizeof(NT));
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == false, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{ std::move(src[i]) }); }
			}


			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == true, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) const {
				HX_PLACEMENT_NEW(m_data+ptr, NT(std::forward<NT>(a)...));
			}

			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == false, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) const {
				if (std::is_default_constructible<NT>::value){
					HX_PLACEMENT_NEW(m_data+ptr, NT{a...});
				}else{
					/// at this point, the object can never be constructed with the params the user gave it
					/// so just run its trivial default constructor
					HX_PLACEMENT_NEW(m_data+ptr, NT{});
				}
			}


			constexpr14 bool __CheckForReallocate(const _Uint& count){
				if (m_size+count >= m_realsize){
					/// python 3 list: https://github.com/python/cpython/blob/2.6/Objects/listobject.c#L48
					_Uint newsize = m_size+count;
					m_realsize += (newsize >> 3) + (newsize < 9 ? 3 : 6);
					///

					value_type* ndata = (value_type*)(alloc.allocate(m_realsize*sizeof(T)));
					if (!ndata)return false;

					if (m_data && ndata!=m_data){
						__SafeMove(ndata, m_data, m_size);
						alloc.deallocate((uint8_t*)(m_data), m_realsize*sizeof(T));
					}

					m_data = ndata;
				}
				return true;
			}


			constexpr14 bool reserve(const _Uint& size){
				if (m_size+size <= m_realsize)return true;

				m_realsize += size - (m_realsize-m_size);

				value_type* ndata = (value_type*)(alloc.allocate(m_realsize*sizeof(T)));
				if (!ndata)return false;

				if (m_data && ndata != m_data){
					__SafeMove(ndata, m_data, m_size);
				}

				alloc.deallocate((uint8_t*)(m_data), m_realsize*sizeof(T));
				m_data = ndata;
				return true;
			}



			constexpr14 inline void transfer(value_type** out) noexcept { (*out) = m_data; m_data=nullptr, m_size=0, m_realsize=0; }
			constexpr inline value_type* data() const { return this->m_data; }
			constexpr inline _Uint size() const { return this->m_size; }
			constexpr inline _Uint stride() const { return sizeof(value_type); }
			constexpr inline _Uint capacity() const { return this->m_realsize; }
			constexpr inline _Uint last_index() const { return (this->m_size>0) ? (this->m_size-1) : 0; }
			constexpr inline bool empty() const { return this->m_size == 0; }
            inline value_type& last() { return this->m_data[this->m_size-1]; }



			template<typename... Args>
			constexpr14 inline _Uint emplace_mult(const _Uint& pos, const _Uint& count, Args&&... a){
				if (!__CheckForReallocate(count))return 0;

				__SafeCopy(m_data+pos+count, m_data+pos, m_size-pos);

				for (_Uint i=0; i<count; ++i){
					__ConstructInPlace<T>(pos+i, a...);
				}
				m_size += count;
				return pos;
			}

			template<typename... Args>
			constexpr14 _Uint emplace(const _Uint& pos, Args&&... a){
				return emplace_mult(pos, 1, a...);
			}

			constexpr14 inline _Uint insert_mult(const _Uint& pos, const _Uint& count, T* data){
				if (!__CheckForReallocate(count))return 0;

				__SafeCopy(m_data + (pos+count), m_data+pos, m_size-pos);
				__SafeCopy(m_data + pos, data, 1);

				m_size += count;
				return pos;
			}

			constexpr14 inline _Uint insert(const _Uint& pos, T&& data){
				return insert_mult(pos, 1, &data);
			}


			template<typename... Args>
			constexpr14 _Uint emplace_back(Args&&... a){
				if (!__CheckForReallocate(1))return 0;

				__ConstructInPlace<T>(m_size, a...);
				m_size += 1;
				return m_size-1;
			}

			constexpr14 _Uint push_back(const value_type& obj){
				if (!__CheckForReallocate(1))return 0;

				__SafeCopy<value_type>(m_data+m_size, &obj, 1);
				m_size += 1;
				return m_size-1;
			}

            constexpr14 inline _Uint push_back_raw(const value_type* obj){
				if (!__CheckForReallocate(1))return 0;
				hxmemcpy(m_data+m_size, obj, sizeof(value_type));
				m_size += 1;
				return m_size-1;
			}

			template<typename... M>
			constexpr14 inline _Uint push_back_mult(M&&... objs){
				if (!reserve( sizeof...(objs) ))return 0;

				size_type index = this->size();
				int dummy[] = { ((void) push_back(objs), 0)... };
				return index;
			}

			constexpr14 inline _Uint push_back_array(const value_type* arr, const size_type& sz){
				if (!reserve(sz))return 0;

				size_type index = this->size();
				for (size_type i=0; i<sz; ++i){ push_back(arr[i]); }

				return index;
			}



			constexpr14 void eraseFast(const _Uint& i){
				if (!m_data || i >= m_size)return;
				__SafeMove(__GetNoCheck(i), __GetNoCheck(i+1), m_size-1-i);
				m_size -= 1;
			}

			constexpr14 void erase(const _Uint& i) {
				T* p = at(i);
				if (p){ (*p).~value_type(); }
				eraseFast(i);
			}

			constexpr14 void erase(const Iterator& t) {
				erase(t.index);
			}



			constexpr14 inline void clearFast(){
				m_size = 0;
			}

			constexpr14 void clear(){
				for (_Uint i=0; i<m_size; ++i){
					m_data[i].~value_type();
				}
				clearFast();
			}


		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////












		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////// TypelessVector
		////// This vector is used to bypass static typesafety.
		////// The user must ensure that each object inserted is of the same stride and similar (but not the same) type at runtime.
		////// For library builders: to ensure runtime typesafety use the TypesafeTypelessVector
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		struct __NoTypeChecker {
			template<typename T> constexpr14 inline void init_checker() const {}
			template<typename T> constexpr14 inline bool compare_type() const { return true; }
			constexpr14 inline void reset_checker() const {}
		};

		struct DefaultTypeChecker {
			std::type_index sign;

			DefaultTypeChecker() : sign(typeid(void)) {}

			template<typename T> constexpr14 inline void init_checker(){ sign = std::type_index(typeid(std::remove_cv<T>::type)); }
			template<typename T> constexpr14 inline bool compare_type() const { return (std::type_index(typeid(std::remove_cv<T>::type)) == sign); }
			inline void reset_checker(){ sign = std::type_index(typeid(void)); }
		};




		template<typename TypeChecker = __NoTypeChecker, typename U = __DefaultAllocator<uint8_t>>
		struct typeless_vector : TypeChecker {
			using _Uint = size_t;
			using size_type = _Uint;
			using alloc_type = U;
			using value_type = uint8_t;
			using byte_type = value_type;

		private:
			value_type* m_data = nullptr;
			_Uint m_stride = 0;
			_Uint m_size = 0;
			_Uint m_realsize = 0;

			alloc_type alloc;

		public:

			constexpr inline void* operator[](const _Uint& i) const {
				return m_data + i*m_stride;
			}

			template<typename T>
			constexpr inline T& operator[](const _Uint& i) const {
				return *cast<T>( m_data + i*m_stride );
			}

			constexpr inline value_type* operator+(const _Uint& i) const {
				return m_data+i;
			}



			struct Iterator {
				typeless_vector* parent = nullptr;
				_Uint index = 0;

				constexpr14 inline Iterator& operator++(){
					if (parent){ index += 1; }
					return *this;
				}
				constexpr14 inline Iterator& operator--(){
					if (parent){ index -= (index==0 ? 0 : 1); }
					return *this;
				}
				constexpr14 inline Iterator operator++(int){
					Iterator t = *this;
					++*this; return t;
				}
				constexpr14 inline Iterator operator--(int){
					Iterator t = *this;
					--*this; return t;
				}
				constexpr inline Iterator operator+(const _Uint& num) const {
					return Iterator{ this->parent, this->index+num };
				}
				constexpr inline Iterator operator-(const _Uint& num) const {
					return Iterator{ this->parent, this->index-num };
				}
				constexpr14 inline Iterator& operator+=(const _Uint& num){
					this->index += num;
					return *this;
				}
				constexpr14 inline Iterator& operator-=(const _Uint& num){
					this->index -= num;
					return *this;
				}

				constexpr inline bool operator!=(const Iterator& t) const { return (this->index != t.index); }
				constexpr inline bool operator==(const Iterator& t) const { return (this->index == t.index); }

				constexpr inline void* operator*() const {
					if (parent){ return parent->at(index); }
					return nullptr;
				}

			};

			constexpr14 inline Iterator begin() { return Iterator{ this, 0 }; }
			constexpr14 inline Iterator end() { return Iterator{ this, this->m_size }; }



			/// constructors
			constexpr14 typeless_vector() : m_stride(0), m_size(0), m_realsize(0), m_data(nullptr), alloc() {}
			constexpr14 typeless_vector(const alloc_type& a) : m_stride(0), m_size(0), m_realsize(0), m_data(nullptr), alloc(a) {}

			constexpr14 typeless_vector(const _Uint& stride){
				init_raw(stride);
			}
			constexpr14 typeless_vector(const _Uint& stride, const alloc_type& a) : alloc(a) {
				init_raw(stride);
			}

			template<typename T>
			constexpr14 typeless_vector(T&&){
				__Init<T>();
			}
			template<typename T>
			constexpr14 typeless_vector(T&&, const alloc_type& a) : alloc(a) {
				__Init<T>();
			}

			~typeless_vector(){ reset(); }


			/// copy semantics
			constexpr14 typeless_vector(const typeless_vector& other) : m_size(other.m_size), m_stride(other.m_stride), m_realsize(other.m_size), alloc(other.alloc){
				m_data = alloc.allocate(other.m_size*other.m_stride);
				__SafeCopy(m_data, other.m_data, other.m_size);
			}
			constexpr14 typeless_vector& operator=(const typeless_vector& other){
				reset();

				m_size = other.m_size;
				m_realsize = other.m_size;
				m_stride = other.m_stride;
				alloc = other.alloc;
				m_data = alloc.allocate(other.m_size*other.m_stride);
				__SafeCopy(m_data, other.m_data, other.m_size);

				return *this;
			}


			/// move semantics
			constexpr14 typeless_vector(typeless_vector&& other) : m_size(other.m_size), m_stride(other.m_stride), m_realsize(other.m_size), alloc(other.alloc){
				other.transfer(&this->m_data);
			}
			constexpr14 typeless_vector& operator=(typeless_vector&& other){
				reset();

				m_size = other.m_size;
				m_realsize = other.m_size;
				m_stride = other.m_stride;
				alloc = other.alloc;
				other.transfer(&this->m_data);
				return *this;
			}



			template<typename T>
			constexpr14 bool __Init(){
				this->template init_checker<T>();
				m_stride = sizeof(T);
				m_size = 0;
				m_realsize = 0;
				m_data = nullptr;

				if (m_stride == 0)return false;
				return true;
			}

			template<typename T>
			constexpr14 inline bool init(){
				return __Init<T>();
			}
			template<typename T>
			constexpr14 inline bool init(T&&){
				return __Init<T>();
			}

			constexpr14 bool init_raw(const _Uint& stride){
				m_stride = stride;
				m_size = 0;
				m_realsize = 0;
				m_data = nullptr;

				if (stride == 0)return false;
				return true;
			}

			constexpr14 void reset(){
				if (m_data){
					alloc.deallocate(m_data, m_realsize*m_stride);
				}
				m_data = nullptr;
				m_realsize = 0;
				m_size = 0;
				this->reset_checker();
			}




			template <typename T>
			constexpr inline bool __check_type_helper(T*, std::true_type) const noexcept {
				return sizeof(T) == m_stride;
			}

			template <typename T>
			constexpr inline bool __check_type_helper(T*, std::false_type) const noexcept {
				return this->template compare_type<T>();
			}

			/// please don't ever use this function if you are not using a TypeChecker
			template<typename T>
			constexpr inline bool check_type(T* t) const noexcept {
				return __check_type_helper(t, std::is_same<TypeChecker, __NoTypeChecker>::value);
			}




			template<typename T>
			inline typename std::enable_if<std::is_same<TypeChecker, __NoTypeChecker>::value == false, T*>::type
			cast(void* p) const {
				if ( this->template compare_type<T>() == false ){
					/// throw some error or call quits cuz the user violated typesafety
					#ifndef HEXO_RUNTIME_NO_EXCEPTIONS
					throw std::runtime_error("typeless_vector's TypeChecker detected a different type from the one given at last init/construct");
					#endif
					return nullptr;
				}
				return (T*)p;
			}

			template<typename T>
			inline typename std::enable_if<std::is_same<TypeChecker, __NoTypeChecker>::value == true, T*>::type
			cast(void* p) const {
				return (T*)p;
			}




			constexpr inline void* __GetNoCheck(const _Uint& i) const {
				return m_data + i*m_stride;
			}

			constexpr inline bool __ValidateIndex(const _Uint& i) const {
				return !(!m_data || i>=m_size);
			}

			constexpr inline void* at(const _Uint& i) const {
				if (!m_data || i >= m_size)return nullptr;
				return m_data + i*m_stride;
			}

			template<typename T>
			constexpr inline T& at(const _Uint& i) const {
				return *(cast<T>(at(i)));
			}



			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == true, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				hxmemcpy(dst, src, count*m_stride);
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == false, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{cast<NT>(src[i])}); }
			}


			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == true, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				hxmemmove(dst, src, count*m_stride);
			}
			template<typename NT = value_type>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == false, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{ std::move(cast<NT>(src[i])) }); }
			}


			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == true, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) const {
				HX_PLACEMENT_NEW(cast<NT>(m_data)+ptr, NT(std::forward<NT>(a)...));
			}

			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == false, void>::type
			__ConstructInPlace(_Uint ptr, Args&&... a) const {
				if (std::is_default_constructible<NT>::value){
					HX_PLACEMENT_NEW(cast<NT>(m_data)+ptr, NT{a...});
				}else{
					/// at this point, the object can never be constructed with the params the user gave it
					/// so just run its trivial default constructor
					HX_PLACEMENT_NEW(cast<NT>(m_data)+ptr, NT{});
				}
			}


			template<typename NT = value_type>
			constexpr14 bool __CheckForReallocate(const _Uint& count){
				if (m_size+count >= m_realsize){
					/// python 3 list: https://github.com/python/cpython/blob/2.6/Objects/listobject.c#L48
					_Uint newsize = m_size+count;
					m_realsize += (newsize >> 3) + (newsize < 9 ? 3 : 6);
					///
					value_type* ndata = alloc.allocate(m_realsize*m_stride);
					if (!ndata)return false;

					if (m_data && ndata!=m_data){
						__SafeMove<NT>(ndata, m_data, m_size);
						alloc.deallocate(m_data, m_realsize*m_stride);
					}

					m_data = ndata;
				}
				return true;
			}

			template<typename NT = value_type>
			constexpr14 bool reserve(const _Uint& size){
				if (m_size+size <= m_realsize)return true;

				m_realsize += size - (m_realsize-m_size);

				value_type* ndata = alloc.allocate(m_realsize*m_stride);
				if (!ndata)return false;

				if (m_data && ndata != m_data){
					__SafeMove<NT>(ndata, m_data, m_size);
				}
				m_data = ndata;

				return true;
			}



			constexpr14 inline void transfer(void** out) noexcept { (*out) = m_data; m_data=nullptr, m_size=0, m_realsize=0; }
			constexpr inline void* data() const { return this->m_data; }

			template<typename T>
			constexpr inline T* data() const { return cast<T>(m_data); }

			constexpr inline _Uint size() const { return this->m_size; }
			constexpr inline _Uint stride() const { return this->m_stride; }
			constexpr inline _Uint capacity() const { return this->m_realsize; }
			constexpr inline _Uint last_index() const { return (this->m_size>0) ? (this->m_size-1) : 0; }
			constexpr inline bool empty() const { return this->m_size == 0; }
            template<typename T> inline T& last() { return this->at(this->m_size-1); }
            inline void* last() { return this->at(this->m_size-1); }




			template<typename T, typename... Args>
			constexpr14 _Uint emplace_mult(const _Uint& pos, const _Uint& count, Args&&... a){
				if (!__CheckForReallocate(count))return 0;

				T* rdata = cast<T>(m_data);
				__SafeCopy(rdata+pos+count, rdata+pos, m_size-pos);

				for (_Uint i=0; i<count; ++i){
					__ConstructInPlace<T>(pos+i, a...);
				}
				m_size += count;
				return pos;
			}

			template<typename T, typename... Args>
			constexpr14 inline _Uint emplace(const _Uint& pos, Args&&... a){
				return emplace_mult<T>(pos, 1, a...);
			}

			template<typename T>
			constexpr14 _Uint insert_mult(const _Uint& pos, const _Uint& count, T* data){
				if (!__CheckForReallocate(count))return 0;

				uint8_t* rdata = (uint8_t*)cast<T>(m_data);
				__SafeCopy(rdata + (pos+count)*m_stride, rdata + pos*m_stride, m_size-pos);
				__SafeCopy(rdata + pos*m_stride, data, 1);

				m_size += count;
				return pos;
			}

			constexpr14 _Uint insert_mult_raw(const _Uint& pos, const _Uint& count, void* data){
				if (!__CheckForReallocate(count))return 0;

				hxmemcpy(m_data + (pos+count)*m_stride, m_data + pos*m_stride, (m_size-pos)*m_stride);
				hxmemcpy(m_data + pos*m_stride, data, m_stride);

				m_size += count;
				return pos;
			}

			template<typename T>
			constexpr14 inline _Uint insert(const _Uint& pos, T&& data){
				return insert_mult(pos, 1, &data);
			}


			template<typename T, typename... Args>
			constexpr14 _Uint emplace_back(Args&&... a){
				if (!__CheckForReallocate(1))return 0;

				__ConstructInPlace<T>(m_size, a...);
				m_size += 1;
				return m_size-1;
			}

			template<typename T>
			constexpr14 _Uint push_back(const T& obj){
				if (!__CheckForReallocate(1))return 0;
				__SafeCopy<T>(m_data + m_size*m_stride, &obj, 1);
				m_size += 1;
				return m_size-1;
			}

            constexpr14 inline _Uint push_back_raw(const void* obj, const size_t& sz){
				if (!__CheckForReallocate(1))return 0;
				hxmemcpy(m_data + (m_size*m_stride), obj, std::min<size_t>(m_stride, sz));
				m_size += 1;
				return m_size-1;
			}

			template<typename... M>
			constexpr14 inline _Uint push_back_mult(M&&... objs){
				if (!reserve<std::tuple_element<0, std::tuple<M...>>::type>( sizeof...(objs) ))return 0;

				size_type index = this->size();
				int dummy[] = { ((void) push_back<M>(objs), 0)... };
				return index;
			}

			template<typename T>
			constexpr14 inline _Uint push_back_array(const T* arr, const size_type& sz){
				if (!reserve<T>(sz))return 0;

				size_type index = this->size();
				for (size_type i=0; i<sz; ++i){ push_back<T>(arr[i]); }

				return index;
			}



			constexpr14 void erase(const _Uint& i){
				if (!m_data || i >= m_size)return;
				hxmemmove(
					__GetNoCheck(i), __GetNoCheck(i+1), m_stride*(m_size-1-i)
				);
				m_size -= 1;
			}

			template<typename T>
			constexpr14 void erase(const _Uint& i) {
				if (!m_data || i >= m_size)return;
				T* p = cast<T>(at(i));
				if (p){ (*p).~T(); }
				__SafeMove<>(__GetNoCheck(i), __GetNoCheck(i+1), m_size-1-i);
				m_size -= 1;
			}

			constexpr14 inline void erase(const Iterator& t) {
				erase(t.index);
			}

			template<typename T>
			constexpr14 inline void erase(const Iterator& t) {
				erase<T>(t.index);
			}


			constexpr14 inline void clearFast(){
				m_size = 0;
			}

			constexpr14 inline void clear(){
				hxmemset(m_data, 0, m_stride*m_size);
				m_size = 0;
			}

			template<typename T>
			constexpr14 void clear(){
				for (_Uint i=0; i<m_size; ++i){
					at<T>(i).~T();
				}
				clear();
			}


		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////







		template<typename T, typename A = __DefaultAllocator<uint8_t>>
		using MiniTypedVector = mini_typed_vector<T, A>;

		template<typename T, typename A = __DefaultAllocator<uint8_t>>
		using TypedVector = typed_vector<T, A>;

		template<typename T = __DefaultAllocator<uint8_t>>
		using TypelessVector = typeless_vector<__NoTypeChecker, T>;

		template<typename T = __DefaultAllocator<uint8_t>>
		using TypesafeTypelessVector = typeless_vector<DefaultTypeChecker, T>;


	}
}




#endif /* end of include guard: HEXO_UTILS_TYPED_VECTOR_H */


#ifndef HEXO_UTILS_TYPED_LINKED_LIST_H
#define HEXO_UTILS_TYPED_LINKED_LIST_H



namespace Hexo {
	namespace Utils {


		//////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
		////// TypelessLinkedList (TrulyTypeless)
        ////// This is used when storing polymorphic objects. It is preferred over vector because either way
        ////// the element is a pointer and will be dereferenced, so the benefit of vector does not come into play.
		////// Instead of storing nested pointers, why not directly store the polymorphic data in the node itself?
        /////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename U>
		struct typless_linked_list {
			using _Uint = size_t;
			using size_type = _Uint;
			using alloc_type = U;

			struct Node {
				Node* next;
				size_type size;
			};

			template<typename T>
			struct NodeData : Node {
				T data;
			};

			using BasicNode = NodeData<uint8_t>;


		private:
			alloc_type alloc;
			Node* m_first;
			Node* m_last;
			_Uint m_size;


		public:
			constexpr14 typless_linked_list() : alloc(), m_first(nullptr), m_last(nullptr){}
			constexpr14 typless_linked_list(const alloc_type& a) : alloc(a), m_first(nullptr), m_last(nullptr){}
			~typless_linked_list(){ clear(); }


			constexpr14 inline void* operator[](const _Uint& i) const {
				return &(at(i).data);
			}
			constexpr14 inline void* operator+(const _Uint& i) const {
				return &(at(i).data);
			}



			constexpr14 inline void transfer(Node** out) noexcept { (*out) = m_first; m_first = nullptr; m_size = 0; }
			constexpr inline Node* first() const { return m_first; }
			constexpr inline Node* last() const { return m_last; }
			constexpr inline _Uint size() const { return m_size; }
			constexpr inline _Uint last_index() const { return (m_size>0) ? (m_size-1) : 0; }
			constexpr inline bool empty() const { return m_size == 0; }


			constexpr14 inline Node* at(const _Uint& i) const {
				Node* p = m_first;
				_Uint ti = 0;
				while (p != nullptr){
					if (ti == i){ return p; }
					p = p->next;
					ti += 1;
				}
				return p;
			}




			template<typename NT>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == true, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				hxmemcpy(dst, src, count*sizeof(NT));
			}
			template<typename NT>
			constexpr14 inline typename std::enable_if<(std::is_trivially_copyable<NT>::value || !std::is_copy_constructible<NT>::value) == false, void>::type
			__SafeCopy(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{src[i]}); }
			}


			template<typename NT>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == true, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				hxmemmove(dst, src, count*sizeof(NT));
			}
			template<typename NT>
			constexpr14 inline typename std::enable_if<(std::is_trivially_move_assignable<NT>::value || !std::is_move_constructible<NT>::value) == false, void>::type
			__SafeMove(NT* dst, const NT* src, const size_type& count) const {
				for (size_t i=0; i<count; ++i){ HX_PLACEMENT_NEW(dst+i, NT{ std::move(src[i]) }); }
			}


			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == true, void>::type
			__ConstructInPlace(NT* ptr, Args&&... a) const {
				HX_PLACEMENT_NEW(ptr, NT(std::forward<NT>(a)...));
			}

			template<typename NT, typename... Args>
			constexpr14 inline typename std::enable_if<std::is_constructible<NT, Args...>::value == false, void>::type
			__ConstructInPlace(NT* ptr, Args&&... a) const {
				if (std::is_default_constructible<NT>::value){
					HX_PLACEMENT_NEW(ptr, NT{a...});
				}else{
					/// at this point, the object can never be constructed with the params the user gave it
					/// so just run its trivial default constructor
					HX_PLACEMENT_NEW(ptr, NT{});
				}
			}


			template<typename T>
			constexpr14 NodeData<T>* __AllocateSpace(){
				NodeData<T>* t = (NodeData<T>*)alloc.allocate(sizeof(T) + sizeof(Node));
				t->next = nullptr;
				t->size = sizeof(T);
				return t;
			}



			template<typename T, typename... Args>
			constexpr14 _Uint emplace_back(Args&&... a){
				NodeData<T>* t = __AllocateSpace();
				if (!t)return 0;

				__ConstructInPlace<T>(&(t->data), a...);

				if (!m_first){
					m_first = t;
					m_last = m_first;
				}else{
					m_last->next = t;
				}

				return m_size++;
			}


			template<typename T>
			constexpr14 _Uint push_back(const T& obj){
				NodeData<T>* t = __AllocateSpace();
				if (!t)return 0;

				__SafeCopy<T>(&(t->data), &obj, 1);

				if (!m_first){
					m_first = t;
					m_last = m_first;
				}else{
					m_last->next = t;
				}

				return m_size++;
			}



			constexpr14 void eraseFast(const _Uint& i){
				if (!m_size)return;

				if (i == m_size-1){
					// m_last
					/// TODO:
					return;
				}
			}

			template<typename T>
			constexpr14 void erase(const _Uint& i){
				/// TODO:
			}



			constexpr14 void clear(){
				Node* p = m_first;
				Node* nt = nullptr;
				while (p != nullptr){
					nt = p->next;
					if (p){ alloc.deallocate(p, p->size + sizeof(Node)); }
					p = nt;
				}

				m_first = nullptr;
				m_last = nullptr;
				m_size = 0;
			}



		};


		template<typename U>
		using TyplessLinkedList = typless_linked_list<U>;

	}
}



#endif /* end of include guard: HEXO_UTILS_TYPED_LINKED_LIST_H */




#ifndef HEXO_UTILS_TYPED_QUEUE_H
#define HEXO_UTILS_TYPED_QUEUE_H



namespace Hexo {
	namespace Utils {

        template<typename T, typename U = Utils::__DefaultAllocator<uint8_t>, typename C = TypedVector<T, U>>
		struct TypedQueue {
		    using value_type = T;
		    using container_type = C;
		    using size_type = size_t;

		private:
		    container_type data;

		public:
		    TypedQueue(){}
			TypedQueue(const typename container_type::alloc_type& a) : data(a) {}
		    ~TypedQueue(){}

		    constexpr inline size_type size() const noexcept { return data.size(); }
		    constexpr inline bool empty() const noexcept { return data.empty(); }

		    constexpr14 inline void push(const value_type& d){
		        data.push_back(d);
		    }

		    template<typename... Args>
		    constexpr14 inline void emplace(Args&&... a){
		        data.emplace_back(a...);
		    }

		    constexpr14 inline value_type* front() const {
		        if (data.empty())return nullptr;
		        return &(data[0]);
		    }

		    constexpr14 inline value_type* back() const {
		        if (data.empty())return nullptr;
		        return data + (data.last_index());
		    }

		    constexpr14 inline void pop_front(){
				data.erase(0);
		    }

		    constexpr14 inline void pop_back(){
				data.erase(data.last_index());
		    }

		    constexpr14 inline void pop(){
		        pop_front();
		    }

		};


        // /// this is a fixed size FILO queue (First In Last Out)
        // template<typename T, HXSIZE RSize, typename C = TypedArray<T, S>>
		// struct TypedFixedQueue {
        //     using value_type = T;
        //     using container_type = C;
        //     using size_type = size_t;
        //
        // private:
        //     container_type data;
        //     size_type pointer;
        //     size_type size;
        //
        // public:
        //     TypedQueue() : data(), pointer(0), size(0){}
        //     ~TypedQueue(){}
        //
        //     constexpr inline size_type size() const noexcept { return size; }
		//     constexpr inline bool empty() const noexcept { return size == 0; }
        //
		//     constexpr14 inline void push(const value_type& d){
		//         data[pointer] = d;
        //         pointer = (pointer+1) % RSize;
        //         size++;
		//     }
        //
		//     template<typename... Args>
		//     constexpr14 inline void emplace(Args&&... a){
		//         data.emplace(pointer, a);
        //         pointer = (pointer+1) % RSize;
        //         size++;
		//     }
        //
		//     constexpr14 inline value_type* front() const {
		//         if (empty())return nullptr;
		//         return &(data[0]);
		//     }
        //
		//     constexpr14 inline value_type* back() const {
		//         if (empty())return nullptr;
		//         return data + (data.last_index());
		//     }
        //
		//     constexpr14 inline void pop(){
        //
		//     }
        // };



	}

}



#endif /* end of include guard: HEXO_UTILS_TYPED_QUEUE_H */




#ifndef HEXO_UTILS_TYPED_SPARSESET_H
#define HEXO_UTILS_TYPED_SPARSESET_H




namespace Hexo {

	namespace Utils {

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////// TypedSparseSet
		////// A Sparse set is a key value pair data structure that does not require hashing.
		////// It works by keeping a dense array of data and a sparse array of keys
		////// The elements in the sparse array are indices to the elements in the dense array
		////// The benefit here is search time and cache locality
		////// The downside is storage, because the sparse will have serious fragmentation
		////// so it is recommended not to repeatedly iterate over the sparse array.
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T, typename U = Utils::__DefaultAllocator<uint8_t>, typename Prec_t = size_t>
		struct TypedSparseSet {
		    using value_type = T;
		    using size_type = Prec_t;
			using key_type = size_type;
			using alloc_type = U;

		    template<typename R>
		    using container_type = Utils::TypedVector<R, U>;


		    struct Data {
		        value_type data;
		        size_type id;
		    };


		private:
		    container_type<Data> m_dense;
		    container_type<size_type> m_sparse;


		public:
		    constexpr14 TypedSparseSet() : m_dense(), m_sparse() {}
			constexpr14 TypedSparseSet(const alloc_type& a) : m_dense(a), m_sparse(a) {}
		    ~TypedSparseSet(){ clear(); }


			constexpr inline size_type full_size() const noexcept { return m_sparse.size(); }
			constexpr inline size_type size() const noexcept { return m_dense.size(); }
			constexpr inline size_type* sparse() noexcept { return m_sparse.data(); }
			constexpr inline Data* dense() noexcept { return m_dense.data(); }
		    constexpr inline Data* data() noexcept { return m_dense.data(); }
			constexpr inline bool empty() const { return m_dense.empty(); }


		    constexpr inline value_type& operator[](const key_type& i) const {
		        return (*at(i));
		    }

		    constexpr inline value_type* operator+(const key_type& i) const {
		        return at(i);
		    }

			template<typename F, typename... Args>
			constexpr14 inline void iterate(F&& func, Args&&... a){
				for (size_t i=0; i<m_dense.size(); ++i){
					func(m_dense[i].data, i, a...);
				}
			}


		    constexpr14 key_type push_back(const value_type& a){
				/// The counter is the size of the m_sparse array because the array never shrinks
				size_type Counter = m_sparse.size();

		        m_dense.push_back(Data{a, Counter});
		        m_sparse.push_back( m_dense.size() );

		        return key_type{Counter+1};
		    }

		    template<typename... Args>
		    constexpr14 key_type emplace_back(Args&&... a){
				size_type Counter = m_sparse.size();

		        m_dense.push_back(Data{std::forward<Args>(a)..., Counter});
		        m_sparse.push_back( m_dense.size() );

		        return key_type{Counter+1};
		    }

		    constexpr inline bool validate(const key_type& p) const noexcept {
		        if (p < 1)return false;
		        size_type in = m_sparse[p-1];
		        if (!in)return false;

		        return true;
		    }

		    constexpr inline value_type* at(const key_type& p) const {
		        if (p < 1)return nullptr;
		        size_type in = m_sparse[p-1];
		        if (!in)return nullptr;

		        return &(m_dense[in-1].data);
		    }

		    constexpr14 void erase(const key_type& p){
		        if (p < 1)return;
		        size_type in = m_sparse[p-1];
		        if (!in)return;

		        in -= 1;
		        const Data& d = m_dense[in];
				size_type lst = m_dense.last_index();

		        m_sparse[d.id] = 0;
		        m_sparse[m_dense[lst].id] = in+1;

		        d.data.~value_type();
		        std::swap( m_dense[in], m_dense[lst] );
		        m_dense.erase(lst);
		    }

			constexpr14 inline void clear() noexcept {
				m_dense.clear();
				m_sparse.clear();
			}

		};


	}


}





#endif /* end of include guard: HEXO_UTILS_TYPED_SPARSESET_H */




#ifndef HEXO_UTILS_TYPED_HASHMAP_H
#define HEXO_UTILS_TYPED_HASHMAP_H



#include <unordered_map>

namespace Hexo {
	namespace Utils {


		/// TODO: MAKE THIS!!!
		template<typename T, typename U>
		using TypedHashMap = std::unordered_map<T, U>;

		template<typename T>
		using StringHashMap = TypedHashMap<std::string, T>;

		template<typename T>
		using IntHashMap = TypedHashMap<HXSIZE, T>;

	}
}




#endif /* end of include guard: HEXO_UTILS_TYPED_HASHMAP_H */




#ifndef HEXO_UTILS_BITSET_H
#define HEXO_UTILS_BITSET_H




namespace Hexo {
	namespace Utils {



		template<HXSIZE N, typename TN = HXSIZE, typename prec_t = TN>
		struct Bitset {
		    using size_type = HXSIZE;
		    using value_type = TN;
			static constexpr const HXSIZE BitsPerChunk = std::numeric_limits<prec_t>::digits;

		private:
		    value_type bits[size_type(N/BitsPerChunk)+1];


		public:
		    struct reference {
		        Bitset<N, value_type, prec_t>* p;
		        size_type index;

		        constexpr14 reference(Bitset<N, value_type, prec_t>* _p, const size_type& _i) : p(_p), index(_i) {}

		        constexpr operator bool() const { return p->test(index); }
		        constexpr reference& operator=(bool b) noexcept { p->set(index, b); return *this; }
		        constexpr reference& operator=(const reference& b) noexcept { p->set(index, b); return *this; }

		    };



		    constexpr14 Bitset(){ reset(); }
			

		    constexpr inline size_type size() const noexcept { return N; }
		    constexpr14 inline void reset() noexcept {
		        // for (size_type i=0; i<(sizeof(bits)/sizeof(size_type)); bits[i]=0, ++i);
				hxmemset(&bits, 0, sizeof(bits));
		    }


		    constexpr14 inline reference operator[](const size_type& i) noexcept {
		        return reference(this, i);
		    }
		    constexpr inline bool operator[](const size_type& i) const noexcept {
		        return test(i);
		    }


		    constexpr inline bool test(const size_type& i) const noexcept {
		        if (i >= N)return false;
		        size_type chunkIndex = size_type(i/BitsPerChunk);
		        size_type index = (BitsPerChunk-1) - (i - (chunkIndex*BitsPerChunk));
		        return bits[chunkIndex] & (index == BitsPerChunk ? index : size_type(1)<<index);
		    }

		    constexpr14 inline void set(const size_type& i, bool v) noexcept {
		        if (i >= N)return;
		        size_type chunkIndex = size_type(i/BitsPerChunk);
		        size_type index = (BitsPerChunk-1) - (i - (chunkIndex*BitsPerChunk));

		        if (v){
		            bits[chunkIndex] |= (prec_t)(index == BitsPerChunk ? index : size_type(1)<<index);
		        }else{
		            bits[chunkIndex] &= ~(prec_t)(index == BitsPerChunk ? index : size_type(1)<<index);
		        }
		    }

		};




		template<typename Container = TypedVector<HXSIZE>, typename prec_t = typename Container::value_type>
		struct DynamicBitset {
		    using size_type = HXSIZE;
		    using value_type = typename Container::value_type;
			using container_type = Container;
			static constexpr const HXSIZE BitsPerChunk = std::numeric_limits<prec_t>::digits;

		private:
		    container_type bits;
		    size_type lastIndex;


		public:
		    struct reference {
		        DynamicBitset<container_type, prec_t>* p;
		        size_type index;

		        constexpr14 reference(DynamicBitset<container_type, prec_t>* _p, const size_type& _i) : p(_p), index(_i) {}

		        constexpr operator bool() const { return p->test(index); }
		        constexpr reference& operator=(bool b) noexcept { p->set(index, b); return *this; }
		        constexpr reference& operator=(const reference& b) noexcept { p->set(index, b); return *this; }

		    };



		    constexpr14 DynamicBitset() : lastIndex(0) {}
			constexpr14 DynamicBitset(const typename container_type::alloc_type& a) : lastIndex(0), bits(a) {}
		    ~DynamicBitset() = default;

		    constexpr inline size_type size() const noexcept { return bits.size()/BitsPerChunk; }
		    constexpr14 inline void reset() noexcept {
				lastIndex = 0;
				bits.clearFast();
			}


		    constexpr14 inline reference operator[](const size_type& i) noexcept {
		        return reference(this, i);
		    }
		    constexpr inline bool operator[](const size_type& i) const noexcept {
		        return test(i);
		    }


		    constexpr inline bool test(const size_type& i) const noexcept {
		        if (i >= bits.size()*BitsPerChunk)return false;
		        size_type chunkIndex = size_type(i/BitsPerChunk);
		        size_type index = (BitsPerChunk-1) - (i - (chunkIndex*BitsPerChunk));
		        return bits[chunkIndex] & (index == BitsPerChunk ? index : size_type(1)<<index);
		    }

		    constexpr14 inline void set(const size_type& i, bool v) noexcept {
		        if (i >= bits.size()*BitsPerChunk)return;
		        size_type chunkIndex = size_type(i/BitsPerChunk);
		        size_type index = (BitsPerChunk-1) - (i - (chunkIndex*BitsPerChunk));

		        if (v){
		            bits[chunkIndex] |= (prec_t)(index == BitsPerChunk ? index : size_type(1)<<index);
		        }else{
		            bits[chunkIndex] &= ~(prec_t)(index == BitsPerChunk ? index : size_type(1)<<index);
		        }
		    }





		    constexpr14 inline size_type push_back(bool v = false) {
		        if (lastIndex >= (bits.size()*BitsPerChunk) ){
		            bits.push_back(size_type(0));
		        }

		        set(lastIndex, v);
		        lastIndex += 1;
		        return lastIndex-1;
		    }

			constexpr14 inline bool reserve(const size_type i){
				return bits.reserve(size_type(i/BitsPerChunk)+1);
			}

		};




		template<HXSIZE N, typename prec_t = HXSIZE>
		using AtomicBitset = Bitset<N, std::atomic<prec_t>, prec_t>;


		// template<typename prec_t = HXSIZE, typename A = __DefaultAllocator, typename Container = TypedVector<std::atomic<prec_t>, A>, HXSIZE B = std::numeric_limits<HXSIZE>::digits>
		// using AtomicDynamicBitset = DynamicBitset<Container, B>;



	}
}





#endif /* end of include guard: HEXO_UTILS_BITSET_H */





#ifndef HEXO_UTILS_SPINLOCK_H
#define HEXO_UTILS_SPINLOCK_H





namespace Hexo {
	namespace Utils {


		struct Spinlock {
		    std::atomic<bool> locked;

		    Spinlock(){ locked = false; }
		    ~Spinlock(){ locked = false; }

		    Spinlock(const Spinlock&) = delete;
		    Spinlock& operator=(Spinlock const&) = delete;

		    inline void lock() noexcept {
		        for(;;){
		            if (locked == false)break;
		        }
		        locked = true;
		    }

		    inline void unlock() noexcept {
		        locked = false;
		    }

		};


	}
}





#endif /* end of include guard: HEXO_UTILS_SPINLOCK_H */








#endif /* end of include guard: HEXO_UTILS_H */







#endif /* end of include guard: HEXO_DEFINITIONS_H */



#ifndef HEXO_HXRC_H
#define HEXO_HXRC_H


#include <functional>


namespace Hexo {
	typedef uint8_t HXRC;




	enum : HXRC {
		HXRC_OK			= 0,
		HXRC_INFO		= 1,
		HXRC_NOT_OK		= 2,
		HXRC_WARNING	= 3,
		HXRC_FATAL		= 4,
	};


	struct HXRC_STATE {
		HXRC_STATE(const std::string& s, const HXRC& c) : ErrorString(s), Code(c) {}
		std::string ErrorString = nullptr;
		HXRC Code = 0;
	};


	struct HXRC_APP {
		std::function<void(HXRC_STATE)> func;
	};


	struct HXRC_CATCHER {
		std::string pushed;
		HXRC level;
	};


	struct HXRC_SYS {
		Utils::TypedVector<HXRC_APP> Apps;
		HXRC_CATCHER Catcher;

		template<typename F>
		inline size_t AddApp(F&& func) noexcept {
			return Apps.emplace_back(func) + 1;
		}

		static inline HXRC_SYS* Get() noexcept {
			static HXRC_SYS h;
			return &h;
		}

		inline HXRC_APP* GetApp(size_t i) const noexcept {
			return Apps+(i-1);
		}
	};



	template<typename F>
	static inline size_t HXRC_REGISTER_APP(F&& func) noexcept {
		return HXRC_SYS::Get()->AddApp(func);
	}



	static inline HXRC HXRC_BUILD(size_t app, const std::string& string, HXRC code) noexcept {
		if (app >= 1){
			// const HXRC_SYS* p = HXRC_SYS::Get()->Apps[i-1];
			// HXRC_SYS::Get()->Apps.size();
			if (HXRC_SYS::Get()->Apps[app-1].func){
				HXRC_SYS::Get()->Apps[app-1].func(HXRC_STATE{string, code});
			}
		}
		return code;
	}


	static inline HXRC HXRC_PUSH(const std::string& string, HXRC code) noexcept {
		auto sys = HXRC_SYS::Get();

		sys->Catcher.pushed += (string+"\n");
		HXRC prev = sys->Catcher.level;

		sys->Catcher.level = (prev > code ? prev : code);
		return sys->Catcher.level;
	}


	static inline HXRC HXRC_MERGE(const HXRC& a, const HXRC& b) noexcept {
		return a > b ? a : b;
	}




	#define HXRC_FAILED(x) (x != HXRC_OK)
	#define HXRC_SUCCESS(x) (x == HXRC_OK)

	#define HXRC_PRINT(string, app) HXRC_BUILD(app, string, HXRC_INFO);

	#define HXRC_MERGE_RETURN(x, y) HXRC_MERGE(x,y); HXRC_RETURN(x);

	#define HXRC_RETURN_VOID(x) if (x != HXRC_OK)return;
	#define HXRC_RETURN_BOOL(x) if (x != HXRC_OK)return false;
	#define HXRC_RETURN(x) { HXRC c=x; if (c != HXRC_OK)return c; }

	#define HXRC_RETURN_INFO(string, app) return HXRC_BUILD(app, string, HXRC_INFO);
	#define HXRC_RETURN_FATAL(string, app) return HXRC_BUILD(app, string, HXRC_FATAL);
	#define HXRC_RETURN_WARNING(string, app) return HXRC_BUILD(app, string, HXRC_WARNING);

	#define HXRC_RETURN_BOOL_INFO(string, app) HXRC_BUILD(app, string, HXRC_INFO);return false;
	#define HXRC_RETURN_BOOL_FATAL(string, app) HXRC_BUILD(app, string, HXRC_FATAL);return false;
	#define HXRC_RETURN_BOOL_WARNING(string, app) HXRC_BUILD(app, string, HXRC_WARNING);return false;

	#define HXRC_ASSERT(x, string, app, code) if (!(x))return HXRC_BUILD(app, string, code);
	#define HXRC_ASSERT_INFO(x, string, app) if (!(x))return HXRC_BUILD(app, string, HXRC_INFO);
	#define HXRC_ASSERT_FATAL(x, string, app) if (!(x))return HXRC_BUILD(app, string, HXRC_FATAL);
	#define HXRC_ASSERT_WARNING(x, string, app) if (!(x))return HXRC_BUILD(app, string, HXRC_WARNING);

    #define HXRC_ASSERT_RETURN_BOOL(x, string, app, code) if (!(x)){HXRC_BUILD(app, string, code);return false;}
    #define HXRC_ASSERT_INFO_RETURN_BOOL(x, string, app) HXRC_ASSERT_RETURN_BOOL(x, string, app, HXRC_INFO);
    #define HXRC_ASSERT_FATAL_RETURN_BOOL(x, string, app) HXRC_ASSERT_RETURN_BOOL(x, string, app, HXRC_FATAL);
    #define HXRC_ASSERT_WARNING_RETURN_BOOL(x, string, app) HXRC_ASSERT_RETURN_BOOL(x, string, app, HXRC_WARNING);

	#define HXRC_NO_RETURN_INFO(string, app) HXRC_BUILD(app, string, HXRC_INFO);
	#define HXRC_NO_RETURN_FATAL(string, app) HXRC_BUILD(app, string, HXRC_FATAL);
	#define HXRC_NO_RETURN_WARNING(string, app) HXRC_BUILD(app, string, HXRC_WARNING);

	#define HXRC_ASSERT_NO_RETURN(x, string, app, code) if (!(x))HXRC_BUILD(app, string, code);
	#define HXRC_ASSERT_INFO_NO_RETURN(x, string, app) if (!(x))HXRC_BUILD(app, string, HXRC_INFO);
	#define HXRC_ASSERT_FATAL_NO_RETURN(x, string, app) if (!(x))HXRC_BUILD(app, string, HXRC_FATAL);
	#define HXRC_ASSERT_WARNING_NO_RETURN(x, string, app) if (!(x))HXRC_BUILD(app, string, HXRC_WARNING);

	#define HXRC_PUSH_NO_RETURN(x, code) HXRC_PUSH(string, code);
	#define HXRC_PUSH_RETURN(string, code) return HXRC_PUSH(string, code);
	#define HXRC_PUSH_RETURN_ASSERT(x, string, code) if (!(x))return HXRC_PUSH(string, code);

};




#endif /* end of include guard: HEXO_HXRC_H */





#ifndef HEXO_MATH_H
#define HEXO_MATH_H




#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/reciprocal.hpp> // glm::cot
#include <glm/gtx/quaternion.hpp> // glm::quat
#include <glm/gtx/rotate_vector.hpp> // glm::rotate
#include <glm/gtc/type_precision.hpp> // glm::f64vec3
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/matrix_decompose.hpp> // glm::decompose






namespace Hexo {
	namespace Mathgl = glm;


namespace Math {







#ifndef HEXO_MATH_VECTOR_H
#define HEXO_MATH_VECTOR_H






template<typename F>
struct HXVector4 {
	F x, y, z, w;

	//constructors
	constexpr14 HXVector4(){ x=0; y=0; z=0; w=0; }
	constexpr14 HXVector4(F f){ x=f; y=f; z=f; w=f; }
	constexpr14 HXVector4(F _x, F _y, F _z, F _w){ x=_x; y=_y; z=_z; w=_w; }
	constexpr14 HXVector4(const HXVector4<F>& v){ x=v.x; y=v.y; z=v.z; w=v.w; }


	std::string tostring() const noexcept {
			return
			"x: "+std::to_string(x)+
			" y: "+std::to_string(y)+
			" z: "+std::to_string(z)+
			" w: "+std::to_string(w);
	}


	//Vector3s
	friend constexpr14 inline HXVector4<F> operator + (const HXVector4<F>& v1, const HXVector4<F>& v2) { return HXVector4<F>(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w); }
	friend constexpr14 inline HXVector4<F> operator - (const HXVector4<F>& v1, const HXVector4<F>& v2) { return HXVector4<F>(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w); }
	friend constexpr14 inline HXVector4<F> operator * (const HXVector4<F>& v1, const HXVector4<F>& v2) { return HXVector4<F>(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w); }
	friend constexpr14 inline HXVector4<F> operator / (const HXVector4<F>& v1, const HXVector4<F>& v2) { return HXVector4<F>(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w); }

	friend constexpr14 HXVector4<F> operator + (const HXVector4<F>& v, const F f) { return HXVector4<F>(v.x+f, v.y+f, v.z+f, v.w+f); }
	friend constexpr14 HXVector4<F> operator - (const HXVector4<F>& v, const F f) { return HXVector4<F>(v.x-f, v.y-f, v.z-f, v.w-f); }
	friend constexpr14 HXVector4<F> operator * (const HXVector4<F>& v, const F f) { return HXVector4<F>(v.x*f, v.y*f, v.z*f, v.w*f); }
	friend constexpr14 HXVector4<F> operator / (const HXVector4<F>& v, const F f) { return HXVector4<F>(v.x/f, v.y/f, v.z/f, v.w/f); }

	friend HXVector4<F> operator + (const F f, const HXVector4<F>& v){ return HXVector4<F>(f+v.x, f+v.y, f+v.z, f+v.w); }
	friend HXVector4<F> operator - (const F f, const HXVector4<F>& v){ return HXVector4<F>(f-v.x, f-v.y, f-v.z, f-v.w); }
	friend HXVector4<F> operator * (const F f, const HXVector4<F>& v){ return HXVector4<F>(f*v.x, f*v.y, f*v.z, f*v.w); }
	friend HXVector4<F> operator / (const F f, const HXVector4<F>& v){ return HXVector4<F>(f/v.x, f/v.y, f/v.z, f/v.w); }

	//booleans
	bool operator > (const HXVector4<F>& v){ return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w); }
	bool operator < (const HXVector4<F>& v){ return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w); }
	bool operator <= (const HXVector4<F>& v){ return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w); }
	bool operator >= (const HXVector4<F>& v){ return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w); }
	bool operator == (const HXVector4<F>& v){ return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator != (const HXVector4<F>& v){ return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w); }

	bool operator > (const F v){ return (x > v) && (y > v) && (z > v) && (w > v); }
	bool operator < (const F v){ return (x < v) && (y < v) && (z < v) && (w < v); }
	bool operator <= (const F v){ return (x <= v) && (y <= v) && (z <= v) && (w <= v); }
	bool operator >= (const F v){ return (x >= v) && (y >= v) && (z >= v) && (w >= v); }
	bool operator == (const F v){ return (x == v) && (y == v) && (z == v) && (w == v); }
	bool operator != (const F v){ return (x != v) || (y != v) || (z != v) || (w != v); }

	//voids
	void operator += (const HXVector4<F>& v){ x += v.x; y += v.y; z += v.z; w += v.w; }
	void operator -= (const HXVector4<F>& v){ x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator *= (const HXVector4<F>& v){ x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
	void operator /= (const HXVector4<F>& v){ x /= v.x; y /= v.y; z /= v.z; w /= v.w; }

	void operator += (const F v){ x += v; y += v; z += v; w += v; }
	void operator -= (const F v){ x -= v; y -= v; z -= v; w -= v; }
	void operator *= (const F v){ x *= v; y *= v; z *= v; w *= v; }
	void operator /= (const F v){ x /= v; y /= v; z /= v; w /= v; }


};








template<typename F>
struct HXVector3 {
	F x, y, z;

	//constructors
	HXVector3(){ x=0; y=0; z=0; }
	HXVector3(F f){ x=f; y=f; z=f; }
	HXVector3(F _x, F _y, F _z){ x=_x; y=_y; z=_z; }
	HXVector3(const HXVector3<F>& v){ x=v.x; y=v.y; z=v.z; }


	inline std::string tostring(){
			return
			"x: "+std::to_string(x)+
			" y: "+std::to_string(y)+
			" z: "+std::to_string(z);
	}


	//Vector3s
	friend HXVector3<F> operator + (const HXVector3<F>& v1, const HXVector3<F>& v2){ return HXVector3<F>(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }
	friend HXVector3<F> operator - (const HXVector3<F>& v1, const HXVector3<F>& v2){ return HXVector3<F>(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }
	friend HXVector3<F> operator * (const HXVector3<F>& v1, const HXVector3<F>& v2){ return HXVector3<F>(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z); }
	friend HXVector3<F> operator / (const HXVector3<F>& v1, const HXVector3<F>& v2){ return HXVector3<F>(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z); }

	friend HXVector3<F> operator + (const HXVector3<F>& v, const F f){ return HXVector3<F>(v.x+f, v.y+f, v.z+f); }
	friend HXVector3<F> operator - (const HXVector3<F>& v, const F f){ return HXVector3<F>(v.x-f, v.y-f, v.z-f); }
	friend HXVector3<F> operator * (const HXVector3<F>& v, const F f){ return HXVector3<F>(v.x*f, v.y*f, v.z*f); }
	friend HXVector3<F> operator / (const HXVector3<F>& v, const F f){ return HXVector3<F>(v.x/f, v.y/f, v.z/f); }

	friend HXVector3<F> operator + (const F f, const HXVector3<F>& v){ return HXVector3<F>(f+v.x, f+v.y, f+v.z); }
	friend HXVector3<F> operator - (const F f, const HXVector3<F>& v){ return HXVector3<F>(f-v.x, f-v.y, f-v.z); }
	friend HXVector3<F> operator * (const F f, const HXVector3<F>& v){ return HXVector3<F>(f*v.x, f*v.y, f*v.z); }
	friend HXVector3<F> operator / (const F f, const HXVector3<F>& v){ return HXVector3<F>(f/v.x, f/v.y, f/v.z); }

	//booleans
	bool operator > (const HXVector3<F>& v){ return (x > v.x) && (y > v.y) && (z > v.z); }
	bool operator < (const HXVector3<F>& v){ return (x < v.x) && (y < v.y) && (z < v.z); }
	bool operator <= (const HXVector3<F>& v){ return (x <= v.x) && (y <= v.y) && (z <= v.z); }
	bool operator >= (const HXVector3<F>& v){ return (x >= v.x) && (y >= v.y) && (z >= v.z); }
	bool operator == (const HXVector3<F>& v){ return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator != (const HXVector3<F>& v){ return (x != v.x) || (y != v.y) || (z != v.z); }

	bool operator > (const F v){ return (x > v) && (y > v) && (z > v); }
	bool operator < (const F v){ return (x < v) && (y < v) && (z < v); }
	bool operator <= (const F v){ return (x <= v) && (y <= v) && (z <= v); }
	bool operator >= (const F v){ return (x >= v) && (y >= v) && (z >= v); }
	bool operator == (const F v){ return (x == v) && (y == v) && (z == v); }
	bool operator != (const F v){ return (x != v) || (y != v) || (z != v); }

	//voids
	void operator += (const HXVector3<F>& v){ x += v.x; y += v.y; z += v.z; }
	void operator -= (const HXVector3<F>& v){ x -= v.x; y -= v.y; z -= v.z; }
	void operator *= (const HXVector3<F>& v){ x *= v.x; y *= v.y; z *= v.z; }
	void operator /= (const HXVector3<F>& v){ x /= v.x; y /= v.y; z /= v.z; }

	void operator += (const F v){ x += v; y += v; z += v; }
	void operator -= (const F v){ x -= v; y -= v; z -= v; }
	void operator *= (const F v){ x *= v; y *= v; z *= v; }
	void operator /= (const F v){ x /= v; y /= v; z /= v; }


};









template<typename F>
struct HXVector2 {
	F x, y;

	//constructors
	constexpr14 HXVector2(){ x=0; y=0; }
	constexpr14 HXVector2(F f){ x=f; y=f; }
	constexpr14 HXVector2(F _x, F _y){ x=_x; y=_y; }
	constexpr14 HXVector2(const HXVector2<F>& v){ x=v.x; y=v.y; }


	inline std::string tostring() {
			return
			"x: "+std::to_string(x)+
			" y: "+std::to_string(y);
	}


	//Vector3s
	friend constexpr14 inline HXVector2<F> operator + (const HXVector2<F>& v1, const HXVector2<F>& v2){ return HXVector2<F>(v1.x+v2.x, v1.y+v2.y); }
	friend constexpr14 inline HXVector2<F> operator - (const HXVector2<F>& v1, const HXVector2<F>& v2){ return HXVector2<F>(v1.x-v2.x, v1.y-v2.y); }
	friend constexpr14 inline HXVector2<F> operator * (const HXVector2<F>& v1, const HXVector2<F>& v2){ return HXVector2<F>(v1.x*v2.x, v1.y*v2.y); }
	friend constexpr14 inline HXVector2<F> operator / (const HXVector2<F>& v1, const HXVector2<F>& v2){ return HXVector2<F>(v1.x/v2.x, v1.y/v2.y); }

	friend constexpr14 inline HXVector2<F> operator + (const HXVector2<F>& v, const F f){ return HXVector2<F>(v.x+f, v.y+f); }
	friend constexpr14 inline HXVector2<F> operator - (const HXVector2<F>& v, const F f){ return HXVector2<F>(v.x-f, v.y-f); }
	friend constexpr14 inline HXVector2<F> operator * (const HXVector2<F>& v, const F f){ return HXVector2<F>(v.x*f, v.y*f); }
	friend constexpr14 inline HXVector2<F> operator / (const HXVector2<F>& v, const F f){ return HXVector2<F>(v.x/f, v.y/f); }

	friend constexpr14 inline HXVector2<F> operator + (const F f, const HXVector2<F>& v){ return HXVector2<F>(f+v.x, f+v.y); }
	friend constexpr14 inline HXVector2<F> operator - (const F f, const HXVector2<F>& v){ return HXVector2<F>(f-v.x, f-v.y); }
	friend constexpr14 inline HXVector2<F> operator * (const F f, const HXVector2<F>& v){ return HXVector2<F>(f*v.x, f*v.y); }
	friend constexpr14 inline HXVector2<F> operator / (const F f, const HXVector2<F>& v){ return HXVector2<F>(f/v.x, f/v.y); }

	//booleans
	constexpr14 inline bool operator > (const HXVector2<F>& v){ return (x > v.x) && (y > v.y); }
	constexpr14 inline bool operator < (const HXVector2<F>& v){ return (x < v.x) && (y < v.y); }
	constexpr14 inline bool operator <= (const HXVector2<F>& v){ return (x <= v.x) && (y <= v.y); }
	constexpr14 inline bool operator >= (const HXVector2<F>& v){ return (x >= v.x) && (y >= v.y); }
	constexpr14 inline bool operator == (const HXVector2<F>& v){ return (x == v.x) && (y == v.y); }
	constexpr14 inline bool operator != (const HXVector2<F>& v){ return (x != v.x) || (y != v.y); }

	constexpr14 inline bool operator > (const F v){ return (x > v) && (y > v); }
	constexpr14 inline bool operator < (const F v){ return (x < v) && (y < v); }
	constexpr14 inline bool operator <= (const F v){ return (x <= v) && (y <= v); }
	constexpr14 inline bool operator >= (const F v){ return (x >= v) && (y >= v); }
	constexpr14 inline bool operator == (const F v){ return (x == v) && (y == v); }
	constexpr14 inline bool operator != (const F v){ return (x != v) || (y != v); }

	//voids
	constexpr14 inline void operator += (const HXVector2<F>& v){ x += v.x; y += v.y; }
	constexpr14 inline void operator -= (const HXVector2<F>& v){ x -= v.x; y -= v.y; }
	constexpr14 inline void operator *= (const HXVector2<F>& v){ x *= v.x; y *= v.y; }
	constexpr14 inline void operator /= (const HXVector2<F>& v){ x /= v.x; y /= v.y; }

	constexpr14 inline void operator += (const F v){ x += v; y += v; }
	constexpr14 inline void operator -= (const F v){ x -= v; y -= v; }
	constexpr14 inline void operator *= (const F v){ x *= v; y *= v; }
	constexpr14 inline void operator /= (const F v){ x /= v; y /= v; }


};











struct HXColor {
	float r=0, g=0, b=0, a=0;

	HXColor(){ r=0; g=0; b=0; a=0; }
	HXColor(float x){ r=x; g=x; b=x; a=x; }
	HXColor(float _r, float _g, float _b){ r=_r; g=_g; b=_b; a=1; }
	HXColor(float _r, float _g, float _b, float _a){ r=_r; g=_g; b=_b; a=_a; }


	//casts
	operator float*() const {
		return new float[4]{r, g, b, a};
	}


	//HXColor and floats
	friend HXColor operator + (const HXColor& c1, const HXColor& c2){ return HXColor(c1.r+c2.r, c1.g+c2.g, c1.b+c2.b, c1.a+c2.a); }
	friend HXColor operator - (const HXColor& c1, const HXColor& c2){ return HXColor(c1.r-c2.r, c1.g-c2.g, c1.b-c2.b, c1.a-c2.a); }
	friend HXColor operator * (const HXColor& c1, const HXColor& c2){ return HXColor(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b, c1.a*c2.a); }
	friend HXColor operator / (const HXColor& c1, const HXColor& c2){ return HXColor(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b, c1.a/c2.a); }

	friend HXColor operator + (const HXColor& c, const float f){ return HXColor(c.r+f, c.g+f, c.b+f, c.a+f); }
	friend HXColor operator - (const HXColor& c, const float f){ return HXColor(c.r-f, c.g-f, c.b-f, c.a-f); }
	friend HXColor operator * (const HXColor& c, const float f){ return HXColor(c.r*f, c.g*f, c.b*f, c.a*f); }
	friend HXColor operator / (const HXColor& c, const float f){ return HXColor(c.r/f, c.g/f, c.b/f, c.a/f); }

	friend HXColor operator + (const float f, const HXColor& c){ return HXColor(f+c.r, f+c.g, f+c.b, f+c.a); }
	friend HXColor operator - (const float f, const HXColor& c){ return HXColor(f-c.r, f-c.g, f-c.b, f-c.a); }
	friend HXColor operator * (const float f, const HXColor& c){ return HXColor(f*c.r, f*c.g, f*c.b, f*c.a); }
	friend HXColor operator / (const float f, const HXColor& c){ return HXColor(f/c.r, f/c.g, f/c.b, f/c.a); }

	//booleans
	bool operator <= (const HXColor& c){ return (r <= c.r) && (g <= c.g) && (b <= c.b) && (a <= c.a); }
	bool operator >= (const HXColor& c){ return (r >= c.r) && (g >= c.g) && (b >= c.b) && (a >= c.a); }
	bool operator == (const HXColor& c){ return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a); }
	bool operator != (const HXColor& c){ return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a); }
	bool operator < (const HXColor& c){ return (r < c.r) && (g < c.g) && (b < c.b) && (a < c.a); }
	bool operator > (const HXColor& c){ return (r > c.r) && (g > c.g) && (b > c.b) && (a > c.a); }

	bool operator <= (const float v){ return (r <= v) && (g <= v) && (b <= v) && (a <= v); }
	bool operator >= (const float v){ return (r >= v) && (g >= v) && (b >= v) && (a >= v); }
	bool operator == (const float v){ return (r == v) && (g == v) && (b == v) && (a == v); }
	bool operator != (const float v){ return (r != v) || (g != v) || (b != v) || (a != v); }
	bool operator < (const float v){ return (r < v) && (g < v) && (b < v) && (a < v); }
	bool operator > (const float v){ return (r > v) && (g > v) && (b > v) && (a > v); }

	//voids
	void operator += (const HXColor& c){ r += c.r; g += c.g; b += c.b; a += c.a; }
	void operator -= (const HXColor& c){ r -= c.r; g -= c.g; b -= c.b; a -= c.a; }
	void operator *= (const HXColor& c){ r *= c.r; g *= c.g; b *= c.b; a *= c.a; }
	void operator /= (const HXColor& c){ r /= c.r; g /= c.g; b /= c.b; a /= c.a; }

	void operator += (const float v){ r += v; g += v; b += v; a += v; }
	void operator -= (const float v){ r -= v; g -= v; b -= v; a -= v; }
	void operator *= (const float v){ r *= v; g *= v; b *= v; a *= v; }
	void operator /= (const float v){ r /= v; g /= v; b /= v; a /= v; }



	static inline HXColor To8Bits(HXColor c){ return c*255; }
	static inline HXColor From8Bits(uint8_t r, uint8_t g, uint8_t b){ return HXColor(r,g,b,1)/255; }
	static inline HXColor From8Bits(uint8_t r, uint8_t g, uint8_t b, uint8_t a){ return HXColor(r,g,b,a)/255; }

	static inline HXColor White(){ return HXColor(1); }
	static inline HXColor Black(){ return HXColor(0); }
	static inline HXColor Red(){ return HXColor(1,0,0); }
	static inline HXColor Blue(){ return HXColor(0,0,1); }
	static inline HXColor Green(){ return HXColor(0,1,0); }

};









#endif /* end of include guard: HEXO_MATH_VECTOR_H */




#ifndef HEXO_MATH_MATRIX
#define HEXO_MATH_MATRIX



template<typename T, unsigned int W, unsigned int H>
struct HXMatrix {
    T m[W*H];

    HXMatrix(){ for (size_t i=0; i<(W*H); m[i]=0, ++i); }
    HXMatrix(const T* _m){ for (size_t i=0; i<(W*H); m[i]=_m[i], ++i); }


    inline const T* getpointer() const { return this->m; }

    inline std::string tostring(){
        std::string out = "";
        for (size_t i=0; i<H; i++){
            for (size_t j=0; j<W; j++){
                if (j != 0) out += ", ";
                out += std::to_string(m[j + i*W]);
            }
            out += "\n";
        }
        return out;
    }

};




#endif /* end of include guard: HEXO_MATH_MATRIX */


	typedef HXVector4<float> Vector4;
	typedef HXVector4<HXSIZE_I> Vector4Int;
	typedef HXVector4<HXSIZE> Vector4Uint;

	typedef HXVector3<float> Vector3;
	typedef HXVector3<HXSIZE_I> Vector3Int;
	typedef HXVector3<HXSIZE> Vector3Uint;

	typedef HXVector2<float> Vector2;
	typedef HXVector2<HXSIZE_I> Vector2Int;
	typedef HXVector2<HXSIZE> Vector2Uint;

    typedef HXVector2<HXSIZE_U8> Vector2U8;
    typedef HXVector2<HXSIZE_U16> Vector2U16;
    typedef HXVector2<HXSIZE_U32> Vector2U32;

    typedef HXVector2<HXSIZE_I8> Vector2I8;
    typedef HXVector2<HXSIZE_I16> Vector2I16;
    typedef HXVector2<HXSIZE_I32> Vector2I32;

	typedef HXMatrix<float, 4, 4> Matrix4f;



	struct Bounds2 {
		Vector2 Position, Dimension;

		Bounds2(){ Position = Vector2(0); Dimension = Vector2(1); }
		Bounds2(Vector2 pos){ Position = pos; Dimension = Vector2(1); }
		Bounds2(Vector2 pos, Vector2 dim){ Position = pos; Dimension = dim; }
		Bounds2(const Bounds2& b){ Position = b.Position; Dimension = b.Dimension; }
		Bounds2(float _x, float _y){ Position = Vector2(_x,_y); Dimension = Vector2(1); }
		Bounds2(float _x, float _y, float _w, float _h){ Position = Vector2(_x,_y); Dimension = Vector2(_w,_h); }

	};








	static const float PI = 3.1415926536f;
	static const char* AlphaNumerics = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	static float DegToRad(float a){ return a * (PI/180.0f); }
	static float RadToDeg(float a){ return a * (180.0f/PI); }

	template<typename T>
	static T max(const T& n1, const T& n2){ return (n1 > n2) ? n1 : n2; }
	template<typename T>
	static T min(const T& n1, const T& n2){ return (n1 < n2) ? n1 : n2; }
	template<typename T>
	static T clamp(const T& value, const T& min, const T& max){ return Math::max(Math::min(value, max), min); }
	template<typename T>
	static T saturate(const T& value){ return clamp(value, T(0), T(1)); }

	static float remp(float value, float OldMin, float OldMax, float NewMin, float NewMax){
	    return (((value - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin;
	}


	//Random
	static float RandomSeed = 1.0f;
	static const uint32_t M1 = 1597334677U;     //1719413*929
	static const uint32_t M2 = 3812015801U;     //140473*2467*11
	static const uint32_t M3 = 3299493293U;     //467549*7057
	static const uint32_t M4 = 3738282773U;
	static const float F0 = 1.0f/float(0xffffffffU);

	static uint32_t IHash(uint32_t n){ return n*(n^(n>>15)); }

	static uint32_t ICoords1(float p){ return uint32_t(p)*M1; }
	static uint32_t ICoords2(Vector2 p){ return (uint32_t(p.x)*M1)^(uint32_t(p.y)*M2); }
	static uint32_t ICoords3(Vector3 p){ return (uint32_t(p.x)*M1)^(uint32_t(p.y)*M2)^(uint32_t(p.z)*M3); }
	// static uint32_t ICoords4(Vector4 p){ return (uint32_t)(p.x*M1)^(uint32_t)(p.y*M2)^(uint32_t)(p.z*M3)^(uint32_t)(p.w*M4); }

	static float Hash11(float n){ return IHash(ICoords1(n)) * F0; }

	static Vector2 Hash12(float n){ return Vector2(IHash(ICoords1(n)*0x1U)*F0, IHash(ICoords1(n)*0x3fffU)*F0); }
	static float Hash21(Vector2 n){ return IHash(ICoords2(n)) * F0; }

	static Vector3 Hash13(float n){ return Vector3(IHash(ICoords1(n)*0x1U)*F0, IHash(ICoords1(n)*0x1ffU)*F0, IHash(ICoords1(n)*0x3ffffU)*F0); }
	static float Hash31(Vector3 n){ return IHash(ICoords3(n)) * F0; }

	// static Vector4 Hash14(float n){ return Vector4(IHash(ICoords1(n)*0x1U)*F0, IHash(ICoords1(n)*0x1ffU)*F0, IHash(ICoords1(n)*0x3ffffU)*F0, IHash(ICoords1(n)*0x1fffffU)*F0); }
	// static float Hash41(Vector4 n){ return IHash(ICoords4(n)) * F0; }


	static float Random(){
		RandomSeed = Hash11(RandomSeed);
		return RandomSeed;
	}
	static Vector2 RandomVector2(){
		Vector2 r = Hash12(RandomSeed);
		RandomSeed = ICoords2(r) * F0;
		return r;
	}
	static Vector3 RandomVector3(){
		Vector3 r = Hash13(RandomSeed);
		RandomSeed = ICoords3(r) * F0;
		return r;
	}
	// static Vector4 RandomVector4(){
	// 	Vector4 r = Hash14(RandomSeed);
	// 	RandomSeed = ICoords4(r) * F0;
	// 	return r;
	// }

	static float Random(float min, float max){
		return min + (Random() * abs(min-max));
	}
	static Vector2 RandomVector2(float min, float max){
		return min + (RandomVector2() * abs(min-max));
	}
	static Vector3 RandomVector3(float min, float max){
		return min + (RandomVector3() * abs(min-max));
	}
	// static Vector4 RandomVector4(float min, float max){
	// 	return min + (RandomVector4() * abs(min-max));
	// }





}
}





#endif /* end of include guard: HEXO_MATH_H */



#ifdef HEXO_GRAPHICS_RHI_GLES3
/*

    OpenGL ES loader generated by glad 0.1.35 on Thu Dec 23 20:00:56 2021.

    Language/Generator: C/C++
    Specification: gl
    APIs: gles2=3.2
    Profile: core
    Extensions:
        GL_AMD_compressed_3DC_texture,
        GL_AMD_compressed_ATC_texture,
        GL_AMD_framebuffer_multisample_advanced,
        GL_AMD_performance_monitor,
        GL_AMD_program_binary_Z400,
        GL_ANDROID_extension_pack_es31a,
        GL_ANGLE_depth_texture,
        GL_ANGLE_framebuffer_blit,
        GL_ANGLE_framebuffer_multisample,
        GL_ANGLE_instanced_arrays,
        GL_ANGLE_pack_reverse_row_order,
        GL_ANGLE_program_binary,
        GL_ANGLE_texture_compression_dxt3,
        GL_ANGLE_texture_compression_dxt5,
        GL_ANGLE_texture_usage,
        GL_ANGLE_translated_shader_source,
        GL_APPLE_clip_distance,
        GL_APPLE_color_buffer_packed_float,
        GL_APPLE_copy_texture_levels,
        GL_APPLE_framebuffer_multisample,
        GL_APPLE_rgb_422,
        GL_APPLE_sync,
        GL_APPLE_texture_format_BGRA8888,
        GL_APPLE_texture_max_level,
        GL_APPLE_texture_packed_float,
        GL_ARM_mali_program_binary,
        GL_ARM_mali_shader_binary,
        GL_ARM_rgba8,
        GL_ARM_shader_framebuffer_fetch,
        GL_ARM_shader_framebuffer_fetch_depth_stencil,
        GL_ARM_texture_unnormalized_coordinates,
        GL_DMP_program_binary,
        GL_DMP_shader_binary,
        GL_EXT_EGL_image_array,
        GL_EXT_EGL_image_storage,
        GL_EXT_EGL_image_storage_compression,
        GL_EXT_YUV_target,
        GL_EXT_base_instance,
        GL_EXT_blend_func_extended,
        GL_EXT_blend_minmax,
        GL_EXT_buffer_storage,
        GL_EXT_clear_texture,
        GL_EXT_clip_control,
        GL_EXT_clip_cull_distance,
        GL_EXT_color_buffer_float,
        GL_EXT_color_buffer_half_float,
        GL_EXT_conservative_depth,
        GL_EXT_copy_image,
        GL_EXT_debug_label,
        GL_EXT_debug_marker,
        GL_EXT_depth_clamp,
        GL_EXT_discard_framebuffer,
        GL_EXT_disjoint_timer_query,
        GL_EXT_draw_buffers,
        GL_EXT_draw_buffers_indexed,
        GL_EXT_draw_elements_base_vertex,
        GL_EXT_draw_instanced,
        GL_EXT_draw_transform_feedback,
        GL_EXT_external_buffer,
        GL_EXT_float_blend,
        GL_EXT_geometry_point_size,
        GL_EXT_geometry_shader,
        GL_EXT_gpu_shader5,
        GL_EXT_instanced_arrays,
        GL_EXT_map_buffer_range,
        GL_EXT_memory_object,
        GL_EXT_memory_object_fd,
        GL_EXT_memory_object_win32,
        GL_EXT_multi_draw_arrays,
        GL_EXT_multi_draw_indirect,
        GL_EXT_multisampled_compatibility,
        GL_EXT_multisampled_render_to_texture,
        GL_EXT_multisampled_render_to_texture2,
        GL_EXT_multiview_draw_buffers,
        GL_EXT_multiview_tessellation_geometry_shader,
        GL_EXT_multiview_texture_multisample,
        GL_EXT_multiview_timer_query,
        GL_EXT_occlusion_query_boolean,
        GL_EXT_polygon_offset_clamp,
        GL_EXT_post_depth_coverage,
        GL_EXT_primitive_bounding_box,
        GL_EXT_protected_textures,
        GL_EXT_pvrtc_sRGB,
        GL_EXT_raster_multisample,
        GL_EXT_read_format_bgra,
        GL_EXT_render_snorm,
        GL_EXT_robustness,
        GL_EXT_sRGB,
        GL_EXT_sRGB_write_control,
        GL_EXT_semaphore,
        GL_EXT_semaphore_fd,
        GL_EXT_semaphore_win32,
        GL_EXT_separate_shader_objects,
        GL_EXT_shader_framebuffer_fetch,
        GL_EXT_shader_framebuffer_fetch_non_coherent,
        GL_EXT_shader_group_vote,
        GL_EXT_shader_implicit_conversions,
        GL_EXT_shader_integer_mix,
        GL_EXT_shader_io_blocks,
        GL_EXT_shader_non_constant_global_initializers,
        GL_EXT_shader_pixel_local_storage,
        GL_EXT_shader_pixel_local_storage2,
        GL_EXT_shader_texture_lod,
        GL_EXT_shadow_samplers,
        GL_EXT_sparse_texture,
        GL_EXT_sparse_texture2,
        GL_EXT_tessellation_point_size,
        GL_EXT_tessellation_shader,
        GL_EXT_texture_border_clamp,
        GL_EXT_texture_buffer,
        GL_EXT_texture_compression_astc_decode_mode,
        GL_EXT_texture_compression_bptc,
        GL_EXT_texture_compression_dxt1,
        GL_EXT_texture_compression_rgtc,
        GL_EXT_texture_compression_s3tc,
        GL_EXT_texture_compression_s3tc_srgb,
        GL_EXT_texture_cube_map_array,
        GL_EXT_texture_filter_anisotropic,
        GL_EXT_texture_filter_minmax,
        GL_EXT_texture_format_BGRA8888,
        GL_EXT_texture_format_sRGB_override,
        GL_EXT_texture_mirror_clamp_to_edge,
        GL_EXT_texture_norm16,
        GL_EXT_texture_query_lod,
        GL_EXT_texture_rg,
        GL_EXT_texture_sRGB_R8,
        GL_EXT_texture_sRGB_RG8,
        GL_EXT_texture_sRGB_decode,
        GL_EXT_texture_shadow_lod,
        GL_EXT_texture_storage,
        GL_EXT_texture_storage_compression,
        GL_EXT_texture_type_2_10_10_10_REV,
        GL_EXT_texture_view,
        GL_EXT_unpack_subimage,
        GL_EXT_win32_keyed_mutex,
        GL_EXT_window_rectangles,
        GL_FJ_shader_binary_GCCSO,
        GL_IMG_bindless_texture,
        GL_IMG_framebuffer_downsample,
        GL_IMG_multisampled_render_to_texture,
        GL_IMG_program_binary,
        GL_IMG_read_format,
        GL_IMG_shader_binary,
        GL_IMG_texture_compression_pvrtc,
        GL_IMG_texture_compression_pvrtc2,
        GL_IMG_texture_filter_cubic,
        GL_INTEL_blackhole_render,
        GL_INTEL_conservative_rasterization,
        GL_INTEL_framebuffer_CMAA,
        GL_INTEL_performance_query,
        GL_KHR_blend_equation_advanced,
        GL_KHR_blend_equation_advanced_coherent,
        GL_KHR_context_flush_control,
        GL_KHR_debug,
        GL_KHR_no_error,
        GL_KHR_parallel_shader_compile,
        GL_KHR_robust_buffer_access_behavior,
        GL_KHR_robustness,
        GL_KHR_shader_subgroup,
        GL_KHR_texture_compression_astc_hdr,
        GL_KHR_texture_compression_astc_ldr,
        GL_KHR_texture_compression_astc_sliced_3d,
        GL_MESA_bgra,
        GL_MESA_framebuffer_flip_x,
        GL_MESA_framebuffer_flip_y,
        GL_MESA_framebuffer_swap_xy,
        GL_MESA_program_binary_formats,
        GL_MESA_shader_integer_functions,
        GL_NVX_blend_equation_advanced_multi_draw_buffers,
        GL_NV_bindless_texture,
        GL_NV_blend_equation_advanced,
        GL_NV_blend_equation_advanced_coherent,
        GL_NV_blend_minmax_factor,
        GL_NV_clip_space_w_scaling,
        GL_NV_compute_shader_derivatives,
        GL_NV_conditional_render,
        GL_NV_conservative_raster,
        GL_NV_conservative_raster_pre_snap,
        GL_NV_conservative_raster_pre_snap_triangles,
        GL_NV_copy_buffer,
        GL_NV_coverage_sample,
        GL_NV_depth_nonlinear,
        GL_NV_draw_buffers,
        GL_NV_draw_instanced,
        GL_NV_draw_vulkan_image,
        GL_NV_explicit_attrib_location,
        GL_NV_fbo_color_attachments,
        GL_NV_fence,
        GL_NV_fill_rectangle,
        GL_NV_fragment_coverage_to_color,
        GL_NV_fragment_shader_barycentric,
        GL_NV_fragment_shader_interlock,
        GL_NV_framebuffer_blit,
        GL_NV_framebuffer_mixed_samples,
        GL_NV_framebuffer_multisample,
        GL_NV_generate_mipmap_sRGB,
        GL_NV_geometry_shader_passthrough,
        GL_NV_gpu_shader5,
        GL_NV_image_formats,
        GL_NV_instanced_arrays,
        GL_NV_internalformat_sample_query,
        GL_NV_memory_attachment,
        GL_NV_memory_object_sparse,
        GL_NV_mesh_shader,
        GL_NV_non_square_matrices,
        GL_NV_path_rendering,
        GL_NV_path_rendering_shared_edge,
        GL_NV_pixel_buffer_object,
        GL_NV_polygon_mode,
        GL_NV_primitive_shading_rate,
        GL_NV_read_buffer,
        GL_NV_read_buffer_front,
        GL_NV_read_depth,
        GL_NV_read_depth_stencil,
        GL_NV_read_stencil,
        GL_NV_representative_fragment_test,
        GL_NV_sRGB_formats,
        GL_NV_sample_locations,
        GL_NV_sample_mask_override_coverage,
        GL_NV_scissor_exclusive,
        GL_NV_shader_atomic_fp16_vector,
        GL_NV_shader_noperspective_interpolation,
        GL_NV_shader_subgroup_partitioned,
        GL_NV_shader_texture_footprint,
        GL_NV_shading_rate_image,
        GL_NV_shadow_samplers_array,
        GL_NV_shadow_samplers_cube,
        GL_NV_stereo_view_rendering,
        GL_NV_texture_border_clamp,
        GL_NV_texture_compression_s3tc_update,
        GL_NV_texture_npot_2D_mipmap,
        GL_NV_timeline_semaphore,
        GL_NV_viewport_array,
        GL_NV_viewport_array2,
        GL_NV_viewport_swizzle,
        GL_OES_EGL_image,
        GL_OES_EGL_image_external,
        GL_OES_EGL_image_external_essl3,
        GL_OES_compressed_ETC1_RGB8_sub_texture,
        GL_OES_compressed_ETC1_RGB8_texture,
        GL_OES_compressed_paletted_texture,
        GL_OES_copy_image,
        GL_OES_depth24,
        GL_OES_depth32,
        GL_OES_depth_texture,
        GL_OES_draw_buffers_indexed,
        GL_OES_draw_elements_base_vertex,
        GL_OES_element_index_uint,
        GL_OES_fbo_render_mipmap,
        GL_OES_fragment_precision_high,
        GL_OES_geometry_point_size,
        GL_OES_geometry_shader,
        GL_OES_get_program_binary,
        GL_OES_gpu_shader5,
        GL_OES_mapbuffer,
        GL_OES_packed_depth_stencil,
        GL_OES_primitive_bounding_box,
        GL_OES_required_internalformat,
        GL_OES_rgb8_rgba8,
        GL_OES_sample_shading,
        GL_OES_sample_variables,
        GL_OES_shader_image_atomic,
        GL_OES_shader_io_blocks,
        GL_OES_shader_multisample_interpolation,
        GL_OES_standard_derivatives,
        GL_OES_stencil1,
        GL_OES_stencil4,
        GL_OES_surfaceless_context,
        GL_OES_tessellation_point_size,
        GL_OES_tessellation_shader,
        GL_OES_texture_3D,
        GL_OES_texture_border_clamp,
        GL_OES_texture_buffer,
        GL_OES_texture_compression_astc,
        GL_OES_texture_cube_map_array,
        GL_OES_texture_float,
        GL_OES_texture_float_linear,
        GL_OES_texture_half_float,
        GL_OES_texture_half_float_linear,
        GL_OES_texture_npot,
        GL_OES_texture_stencil8,
        GL_OES_texture_storage_multisample_2d_array,
        GL_OES_texture_view,
        GL_OES_vertex_array_object,
        GL_OES_vertex_half_float,
        GL_OES_vertex_type_10_10_10_2,
        GL_OES_viewport_array,
        GL_OVR_multiview,
        GL_OVR_multiview2,
        GL_OVR_multiview_multisampled_render_to_texture,
        GL_QCOM_YUV_texture_gather,
        GL_QCOM_alpha_test,
        GL_QCOM_binning_control,
        GL_QCOM_driver_control,
        GL_QCOM_extended_get,
        GL_QCOM_extended_get2,
        GL_QCOM_frame_extrapolation,
        GL_QCOM_framebuffer_foveated,
        GL_QCOM_motion_estimation,
        GL_QCOM_perfmon_global_mode,
        GL_QCOM_render_shared_exponent,
        GL_QCOM_shader_framebuffer_fetch_noncoherent,
        GL_QCOM_shader_framebuffer_fetch_rate,
        GL_QCOM_shading_rate,
        GL_QCOM_texture_foveated,
        GL_QCOM_texture_foveated2,
        GL_QCOM_texture_foveated_subsampled_layout,
        GL_QCOM_tiled_rendering,
        GL_QCOM_writeonly_rendering,
        GL_VIV_shader_binary
    Loader: True
    Local files: False
    Omit khrplatform: False
    Reproducible: False

    Commandline:
        --profile="core" --api="gles2=3.2" --generator="c" --spec="gl" --extensions="GL_AMD_compressed_3DC_texture,GL_AMD_compressed_ATC_texture,GL_AMD_framebuffer_multisample_advanced,GL_AMD_performance_monitor,GL_AMD_program_binary_Z400,GL_ANDROID_extension_pack_es31a,GL_ANGLE_depth_texture,GL_ANGLE_framebuffer_blit,GL_ANGLE_framebuffer_multisample,GL_ANGLE_instanced_arrays,GL_ANGLE_pack_reverse_row_order,GL_ANGLE_program_binary,GL_ANGLE_texture_compression_dxt3,GL_ANGLE_texture_compression_dxt5,GL_ANGLE_texture_usage,GL_ANGLE_translated_shader_source,GL_APPLE_clip_distance,GL_APPLE_color_buffer_packed_float,GL_APPLE_copy_texture_levels,GL_APPLE_framebuffer_multisample,GL_APPLE_rgb_422,GL_APPLE_sync,GL_APPLE_texture_format_BGRA8888,GL_APPLE_texture_max_level,GL_APPLE_texture_packed_float,GL_ARM_mali_program_binary,GL_ARM_mali_shader_binary,GL_ARM_rgba8,GL_ARM_shader_framebuffer_fetch,GL_ARM_shader_framebuffer_fetch_depth_stencil,GL_ARM_texture_unnormalized_coordinates,GL_DMP_program_binary,GL_DMP_shader_binary,GL_EXT_EGL_image_array,GL_EXT_EGL_image_storage,GL_EXT_EGL_image_storage_compression,GL_EXT_YUV_target,GL_EXT_base_instance,GL_EXT_blend_func_extended,GL_EXT_blend_minmax,GL_EXT_buffer_storage,GL_EXT_clear_texture,GL_EXT_clip_control,GL_EXT_clip_cull_distance,GL_EXT_color_buffer_float,GL_EXT_color_buffer_half_float,GL_EXT_conservative_depth,GL_EXT_copy_image,GL_EXT_debug_label,GL_EXT_debug_marker,GL_EXT_depth_clamp,GL_EXT_discard_framebuffer,GL_EXT_disjoint_timer_query,GL_EXT_draw_buffers,GL_EXT_draw_buffers_indexed,GL_EXT_draw_elements_base_vertex,GL_EXT_draw_instanced,GL_EXT_draw_transform_feedback,GL_EXT_external_buffer,GL_EXT_float_blend,GL_EXT_geometry_point_size,GL_EXT_geometry_shader,GL_EXT_gpu_shader5,GL_EXT_instanced_arrays,GL_EXT_map_buffer_range,GL_EXT_memory_object,GL_EXT_memory_object_fd,GL_EXT_memory_object_win32,GL_EXT_multi_draw_arrays,GL_EXT_multi_draw_indirect,GL_EXT_multisampled_compatibility,GL_EXT_multisampled_render_to_texture,GL_EXT_multisampled_render_to_texture2,GL_EXT_multiview_draw_buffers,GL_EXT_multiview_tessellation_geometry_shader,GL_EXT_multiview_texture_multisample,GL_EXT_multiview_timer_query,GL_EXT_occlusion_query_boolean,GL_EXT_polygon_offset_clamp,GL_EXT_post_depth_coverage,GL_EXT_primitive_bounding_box,GL_EXT_protected_textures,GL_EXT_pvrtc_sRGB,GL_EXT_raster_multisample,GL_EXT_read_format_bgra,GL_EXT_render_snorm,GL_EXT_robustness,GL_EXT_sRGB,GL_EXT_sRGB_write_control,GL_EXT_semaphore,GL_EXT_semaphore_fd,GL_EXT_semaphore_win32,GL_EXT_separate_shader_objects,GL_EXT_shader_framebuffer_fetch,GL_EXT_shader_framebuffer_fetch_non_coherent,GL_EXT_shader_group_vote,GL_EXT_shader_implicit_conversions,GL_EXT_shader_integer_mix,GL_EXT_shader_io_blocks,GL_EXT_shader_non_constant_global_initializers,GL_EXT_shader_pixel_local_storage,GL_EXT_shader_pixel_local_storage2,GL_EXT_shader_texture_lod,GL_EXT_shadow_samplers,GL_EXT_sparse_texture,GL_EXT_sparse_texture2,GL_EXT_tessellation_point_size,GL_EXT_tessellation_shader,GL_EXT_texture_border_clamp,GL_EXT_texture_buffer,GL_EXT_texture_compression_astc_decode_mode,GL_EXT_texture_compression_bptc,GL_EXT_texture_compression_dxt1,GL_EXT_texture_compression_rgtc,GL_EXT_texture_compression_s3tc,GL_EXT_texture_compression_s3tc_srgb,GL_EXT_texture_cube_map_array,GL_EXT_texture_filter_anisotropic,GL_EXT_texture_filter_minmax,GL_EXT_texture_format_BGRA8888,GL_EXT_texture_format_sRGB_override,GL_EXT_texture_mirror_clamp_to_edge,GL_EXT_texture_norm16,GL_EXT_texture_query_lod,GL_EXT_texture_rg,GL_EXT_texture_sRGB_R8,GL_EXT_texture_sRGB_RG8,GL_EXT_texture_sRGB_decode,GL_EXT_texture_shadow_lod,GL_EXT_texture_storage,GL_EXT_texture_storage_compression,GL_EXT_texture_type_2_10_10_10_REV,GL_EXT_texture_view,GL_EXT_unpack_subimage,GL_EXT_win32_keyed_mutex,GL_EXT_window_rectangles,GL_FJ_shader_binary_GCCSO,GL_IMG_bindless_texture,GL_IMG_framebuffer_downsample,GL_IMG_multisampled_render_to_texture,GL_IMG_program_binary,GL_IMG_read_format,GL_IMG_shader_binary,GL_IMG_texture_compression_pvrtc,GL_IMG_texture_compression_pvrtc2,GL_IMG_texture_filter_cubic,GL_INTEL_blackhole_render,GL_INTEL_conservative_rasterization,GL_INTEL_framebuffer_CMAA,GL_INTEL_performance_query,GL_KHR_blend_equation_advanced,GL_KHR_blend_equation_advanced_coherent,GL_KHR_context_flush_control,GL_KHR_debug,GL_KHR_no_error,GL_KHR_parallel_shader_compile,GL_KHR_robust_buffer_access_behavior,GL_KHR_robustness,GL_KHR_shader_subgroup,GL_KHR_texture_compression_astc_hdr,GL_KHR_texture_compression_astc_ldr,GL_KHR_texture_compression_astc_sliced_3d,GL_MESA_bgra,GL_MESA_framebuffer_flip_x,GL_MESA_framebuffer_flip_y,GL_MESA_framebuffer_swap_xy,GL_MESA_program_binary_formats,GL_MESA_shader_integer_functions,GL_NVX_blend_equation_advanced_multi_draw_buffers,GL_NV_bindless_texture,GL_NV_blend_equation_advanced,GL_NV_blend_equation_advanced_coherent,GL_NV_blend_minmax_factor,GL_NV_clip_space_w_scaling,GL_NV_compute_shader_derivatives,GL_NV_conditional_render,GL_NV_conservative_raster,GL_NV_conservative_raster_pre_snap,GL_NV_conservative_raster_pre_snap_triangles,GL_NV_copy_buffer,GL_NV_coverage_sample,GL_NV_depth_nonlinear,GL_NV_draw_buffers,GL_NV_draw_instanced,GL_NV_draw_vulkan_image,GL_NV_explicit_attrib_location,GL_NV_fbo_color_attachments,GL_NV_fence,GL_NV_fill_rectangle,GL_NV_fragment_coverage_to_color,GL_NV_fragment_shader_barycentric,GL_NV_fragment_shader_interlock,GL_NV_framebuffer_blit,GL_NV_framebuffer_mixed_samples,GL_NV_framebuffer_multisample,GL_NV_generate_mipmap_sRGB,GL_NV_geometry_shader_passthrough,GL_NV_gpu_shader5,GL_NV_image_formats,GL_NV_instanced_arrays,GL_NV_internalformat_sample_query,GL_NV_memory_attachment,GL_NV_memory_object_sparse,GL_NV_mesh_shader,GL_NV_non_square_matrices,GL_NV_path_rendering,GL_NV_path_rendering_shared_edge,GL_NV_pixel_buffer_object,GL_NV_polygon_mode,GL_NV_primitive_shading_rate,GL_NV_read_buffer,GL_NV_read_buffer_front,GL_NV_read_depth,GL_NV_read_depth_stencil,GL_NV_read_stencil,GL_NV_representative_fragment_test,GL_NV_sRGB_formats,GL_NV_sample_locations,GL_NV_sample_mask_override_coverage,GL_NV_scissor_exclusive,GL_NV_shader_atomic_fp16_vector,GL_NV_shader_noperspective_interpolation,GL_NV_shader_subgroup_partitioned,GL_NV_shader_texture_footprint,GL_NV_shading_rate_image,GL_NV_shadow_samplers_array,GL_NV_shadow_samplers_cube,GL_NV_stereo_view_rendering,GL_NV_texture_border_clamp,GL_NV_texture_compression_s3tc_update,GL_NV_texture_npot_2D_mipmap,GL_NV_timeline_semaphore,GL_NV_viewport_array,GL_NV_viewport_array2,GL_NV_viewport_swizzle,GL_OES_EGL_image,GL_OES_EGL_image_external,GL_OES_EGL_image_external_essl3,GL_OES_compressed_ETC1_RGB8_sub_texture,GL_OES_compressed_ETC1_RGB8_texture,GL_OES_compressed_paletted_texture,GL_OES_copy_image,GL_OES_depth24,GL_OES_depth32,GL_OES_depth_texture,GL_OES_draw_buffers_indexed,GL_OES_draw_elements_base_vertex,GL_OES_element_index_uint,GL_OES_fbo_render_mipmap,GL_OES_fragment_precision_high,GL_OES_geometry_point_size,GL_OES_geometry_shader,GL_OES_get_program_binary,GL_OES_gpu_shader5,GL_OES_mapbuffer,GL_OES_packed_depth_stencil,GL_OES_primitive_bounding_box,GL_OES_required_internalformat,GL_OES_rgb8_rgba8,GL_OES_sample_shading,GL_OES_sample_variables,GL_OES_shader_image_atomic,GL_OES_shader_io_blocks,GL_OES_shader_multisample_interpolation,GL_OES_standard_derivatives,GL_OES_stencil1,GL_OES_stencil4,GL_OES_surfaceless_context,GL_OES_tessellation_point_size,GL_OES_tessellation_shader,GL_OES_texture_3D,GL_OES_texture_border_clamp,GL_OES_texture_buffer,GL_OES_texture_compression_astc,GL_OES_texture_cube_map_array,GL_OES_texture_float,GL_OES_texture_float_linear,GL_OES_texture_half_float,GL_OES_texture_half_float_linear,GL_OES_texture_npot,GL_OES_texture_stencil8,GL_OES_texture_storage_multisample_2d_array,GL_OES_texture_view,GL_OES_vertex_array_object,GL_OES_vertex_half_float,GL_OES_vertex_type_10_10_10_2,GL_OES_viewport_array,GL_OVR_multiview,GL_OVR_multiview2,GL_OVR_multiview_multisampled_render_to_texture,GL_QCOM_YUV_texture_gather,GL_QCOM_alpha_test,GL_QCOM_binning_control,GL_QCOM_driver_control,GL_QCOM_extended_get,GL_QCOM_extended_get2,GL_QCOM_frame_extrapolation,GL_QCOM_framebuffer_foveated,GL_QCOM_motion_estimation,GL_QCOM_perfmon_global_mode,GL_QCOM_render_shared_exponent,GL_QCOM_shader_framebuffer_fetch_noncoherent,GL_QCOM_shader_framebuffer_fetch_rate,GL_QCOM_shading_rate,GL_QCOM_texture_foveated,GL_QCOM_texture_foveated2,GL_QCOM_texture_foveated_subsampled_layout,GL_QCOM_tiled_rendering,GL_QCOM_writeonly_rendering,GL_VIV_shader_binary"
    Online:
        Too many extensions
*/


#ifndef __glad_h_
#define __glad_h_

#ifdef __gl2_h_
#error OpenGL ES 2 header already included, remove this include, glad already provides it
#endif
#define __gl2_h_

#ifdef __gl3_h_
#error OpenGL ES 3 header already included, remove this include, glad already provides it
#endif
#define __gl3_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define APIENTRY __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct gladGLversionStruct {
    int major;
    int minor;
};

typedef void* (* GLADloadproc)(const char *name);

#ifndef GLAPI
# if defined(GLAD_GLAPI_EXPORT)
#  if defined(_WIN32) || defined(__CYGWIN__)
#   if defined(GLAD_GLAPI_EXPORT_BUILD)
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllexport)) extern
#    else
#     define GLAPI __declspec(dllexport) extern
#    endif
#   else
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllimport)) extern
#    else
#     define GLAPI __declspec(dllimport) extern
#    endif
#   endif
#  elif defined(__GNUC__) && defined(GLAD_GLAPI_EXPORT_BUILD)
#   define GLAPI __attribute__ ((visibility ("default"))) extern
#  else
#   define GLAPI extern
#  endif
# else
#  define GLAPI extern
# endif
#endif

GLAPI struct gladGLversionStruct GLVersion;
GLAPI int gladLoadGLES2Loader(GLADloadproc);

#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Khronos platform-specific types and definitions.
 *
 * The master copy of khrplatform.h is maintained in the Khronos EGL
 * Registry repository at https://github.com/KhronosGroup/EGL-Registry
 * The last semantic modification to khrplatform.h was at commit ID:
 *      67a3e0864c2d75ea5287b9f3d2eb74a745936692
 *
 * Adopters may modify this file to suit their platform. Adopters are
 * encouraged to submit platform specific modifications to the Khronos
 * group so that they can be included in future versions of this file.
 * Please submit changes by filing pull requests or issues on
 * the EGL Registry repository linked above.
 *
 *
 * See the Implementer's Guidelines for information about where this file
 * should be located on your system and for more details of its use:
 *    http://www.khronos.org/registry/implementers_guide.pdf
 *
 * This file should be included as
 *        #include <KHR/khrplatform.h>
 * by Khronos client API header files that use its types and defines.
 *
 * The types in khrplatform.h should only be used to define API-specific types.
 *
 * Types defined in khrplatform.h:
 *    khronos_int8_t              signed   8  bit
 *    khronos_uint8_t             unsigned 8  bit
 *    khronos_int16_t             signed   16 bit
 *    khronos_uint16_t            unsigned 16 bit
 *    khronos_int32_t             signed   32 bit
 *    khronos_uint32_t            unsigned 32 bit
 *    khronos_int64_t             signed   64 bit
 *    khronos_uint64_t            unsigned 64 bit
 *    khronos_intptr_t            signed   same number of bits as a pointer
 *    khronos_uintptr_t           unsigned same number of bits as a pointer
 *    khronos_ssize_t             signed   size
 *    khronos_usize_t             unsigned size
 *    khronos_float_t             signed   32 bit floating point
 *    khronos_time_ns_t           unsigned 64 bit time in nanoseconds
 *    khronos_utime_nanoseconds_t unsigned time interval or absolute time in
 *                                         nanoseconds
 *    khronos_stime_nanoseconds_t signed time interval in nanoseconds
 *    khronos_boolean_enum_t      enumerated boolean type. This should
 *      only be used as a base type when a client API's boolean type is
 *      an enum. Client APIs which use an integer or other type for
 *      booleans cannot use this as the base type for their boolean.
 *
 * Tokens defined in khrplatform.h:
 *
 *    KHRONOS_FALSE, KHRONOS_TRUE Enumerated boolean false/true values.
 *
 *    KHRONOS_SUPPORT_INT64 is 1 if 64 bit integers are supported; otherwise 0.
 *    KHRONOS_SUPPORT_FLOAT is 1 if floats are supported; otherwise 0.
 *
 * Calling convention macros defined in this file:
 *    KHRONOS_APICALL
 *    KHRONOS_APIENTRY
 *    KHRONOS_APIATTRIBUTES
 *
 * These may be used in function prototypes as:
 *
 *      KHRONOS_APICALL void KHRONOS_APIENTRY funcname(
 *                                  int arg1,
 *                                  int arg2) KHRONOS_APIATTRIBUTES;
 */

#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
#   define KHRONOS_STATIC 1
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APICALL
 *-------------------------------------------------------------------------
 * This precedes the return type of the function in the function prototype.
 */
#if defined(KHRONOS_STATIC)
    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
     * header compatible with static linking. */
#   define KHRONOS_APICALL
#elif defined(_WIN32)
#   define KHRONOS_APICALL __declspec(dllimport)
#elif defined (__SYMBIAN32__)
#   define KHRONOS_APICALL IMPORT_C
#elif defined(__ANDROID__)
#   define KHRONOS_APICALL __attribute__((visibility("default")))
#else
#   define KHRONOS_APICALL
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRY
 *-------------------------------------------------------------------------
 * This follows the return type of the function  and precedes the function
 * name in the function prototype.
 */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
    /* Win32 but not WinCE */
#   define KHRONOS_APIENTRY __stdcall
#else
#   define KHRONOS_APIENTRY
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIATTRIBUTES
 *-------------------------------------------------------------------------
 * This follows the closing parenthesis of the function prototype arguments.
 */
#if defined (__ARMCC_2__)
#define KHRONOS_APIATTRIBUTES __softfp
#else
#define KHRONOS_APIATTRIBUTES
#endif

/*-------------------------------------------------------------------------
 * basic type definitions
 *-----------------------------------------------------------------------*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__GNUC__) || defined(__SCO__) || defined(__USLC__)


/*
 * Using <stdint.h>
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1
/*
 * To support platform where unsigned long cannot be used interchangeably with
 * inptr_t (e.g. CHERI-extended ISAs), we can use the stdint.h intptr_t.
 * Ideally, we could just use (u)intptr_t everywhere, but this could result in
 * ABI breakage if khronos_uintptr_t is changed from unsigned long to
 * unsigned long long or similar (this results in different C++ name mangling).
 * To avoid changes for existing platforms, we restrict usage of intptr_t to
 * platforms where the size of a pointer is larger than the size of long.
 */
#if defined(__SIZEOF_LONG__) && defined(__SIZEOF_POINTER__)
#if __SIZEOF_POINTER__ > __SIZEOF_LONG__
#define KHRONOS_USE_INTPTR_T
#endif
#endif

#elif defined(__VMS ) || defined(__sgi)

/*
 * Using <inttypes.h>
 */
#include <inttypes.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)

/*
 * Win32
 */
typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(__sun__) || defined(__digital__)

/*
 * Sun or Digital
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#if defined(__arch64__) || defined(_LP64)
typedef long int                khronos_int64_t;
typedef unsigned long int       khronos_uint64_t;
#else
typedef long long int           khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;
#endif /* __arch64__ */
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif 0

/*
 * Hypothetical platform with no float or int64 support
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#define KHRONOS_SUPPORT_INT64   0
#define KHRONOS_SUPPORT_FLOAT   0

#else

/*
 * Generic fallback
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#endif


/*
 * Types that are (so far) the same on all platforms
 */
typedef signed   char          khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;

/*
 * Types that differ between LLP64 and LP64 architectures - in LLP64,
 * pointers are 64 bits, but 'long' is still 32 bits. Win64 appears
 * to be the only LLP64 architecture in current use.
 */
#ifdef KHRONOS_USE_INTPTR_T
typedef intptr_t               khronos_intptr_t;
typedef uintptr_t              khronos_uintptr_t;
#elif defined(_WIN64)
typedef signed   long long int khronos_intptr_t;
typedef unsigned long long int khronos_uintptr_t;
#else
typedef signed   long  int     khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
#endif

#if defined(_WIN64)
typedef signed   long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
#else
typedef signed   long  int     khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;
#endif

#if KHRONOS_SUPPORT_FLOAT
/*
 * Float type
 */
typedef          float         khronos_float_t;
#endif

#if KHRONOS_SUPPORT_INT64
/* Time types
 *
 * These types can be used to represent a time interval in nanoseconds or
 * an absolute Unadjusted System Time.  Unadjusted System Time is the number
 * of nanoseconds since some arbitrary system event (e.g. since the last
 * time the system booted).  The Unadjusted System Time is an unsigned
 * 64 bit value that wraps back to 0 every 584 years.  Time intervals
 * may be either signed or unsigned.
 */
typedef khronos_uint64_t       khronos_utime_nanoseconds_t;
typedef khronos_int64_t        khronos_stime_nanoseconds_t;
#endif

/*
 * Dummy value used to pad enum types to 32 bits.
 */
#ifndef KHRONOS_MAX_ENUM
#define KHRONOS_MAX_ENUM 0x7FFFFFFF
#endif

/*
 * Enumerated boolean type
 *
 * Values other than zero should be considered to be true.  Therefore
 * comparisons should not be made against KHRONOS_TRUE.
 */
typedef enum {
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE  = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
} khronos_boolean_enum_t;

#endif /* __khrplatform_h_ */

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (APIENTRY *GLVULKANPROCNV)(void);
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_FUNC_ADD 0x8006
#define GL_BLEND_EQUATION 0x8009
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_BLEND_COLOR 0x8005
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_TEXTURE_2D 0x0DE1
#define GL_CULL_FACE 0x0B44
#define GL_BLEND 0x0BE2
#define GL_DITHER 0x0BD0
#define GL_STENCIL_TEST 0x0B90
#define GL_DEPTH_TEST 0x0B71
#define GL_SCISSOR_TEST 0x0C11
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_OUT_OF_MEMORY 0x0505
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_LINE_WIDTH 0x0B21
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_FIXED 0x140C
#define GL_DEPTH_COMPONENT 0x1902
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#define GL_SHADER_TYPE 0x8B4F
#define GL_DELETE_STATUS 0x8B80
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_INVERT 0x150A
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE 0x1702
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_CUBE 0x8B60
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_SHADER_COMPILER 0x8DFA
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGB565 0x8D62
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_STENCIL_INDEX8 0x8D48
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_NONE 0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_READ_BUFFER 0x0C02
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_RED 0x1903
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_MAX_VARYING_COMPONENTS 0x8B4B
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
#define GL_HALF_FLOAT 0x140B
#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_RG8 0x822B
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37
#define GL_COPY_READ_BUFFER_BINDING 0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING 0x8F37
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFF
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_ANY_SAMPLES_PASSED 0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_SAMPLER_BINDING 0x8919
#define GL_RGB10_A2UI 0x906F
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_INT_2_10_10_10_REV 0x8D9F
#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_NUM_SAMPLE_COUNTS 0x9380
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43
#define GL_MAX_UNIFORM_LOCATIONS 0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318
#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_NAME_LENGTH 0x92F9
#define GL_TYPE 0x92FA
#define GL_ARRAY_SIZE 0x92FB
#define GL_OFFSET 0x92FC
#define GL_BLOCK_INDEX 0x92FD
#define GL_ARRAY_STRIDE 0x92FE
#define GL_MATRIX_STRIDE 0x92FF
#define GL_IS_ROW_MAJOR 0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_LOCATION 0x930E
#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA
#define GL_STENCIL_INDEX 0x1901
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_VERTEX_BINDING_BUFFER 0x8F4F
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_MAX_VERTEX_ATTRIB_STRIDE 0x82E5
#define GL_MULTISAMPLE_LINE_WIDTH_RANGE 0x9381
#define GL_MULTISAMPLE_LINE_WIDTH_GRANULARITY 0x9382
#define GL_MULTIPLY 0x9294
#define GL_SCREEN 0x9295
#define GL_OVERLAY 0x9296
#define GL_DARKEN 0x9297
#define GL_LIGHTEN 0x9298
#define GL_COLORDODGE 0x9299
#define GL_COLORBURN 0x929A
#define GL_HARDLIGHT 0x929B
#define GL_SOFTLIGHT 0x929C
#define GL_DIFFERENCE 0x929E
#define GL_EXCLUSION 0x92A0
#define GL_HSL_HUE 0x92AD
#define GL_HSL_SATURATION 0x92AE
#define GL_HSL_COLOR 0x92AF
#define GL_HSL_LUMINOSITY 0x92B0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_VERTEX_ARRAY 0x8074
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_UNDEFINED_VERTEX 0x8260
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_PRIMITIVE_BOUNDING_BOX 0x92BE
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT 0x00000004
#define GL_CONTEXT_FLAGS 0x821E
#define GL_LOSE_CONTEXT_ON_RESET 0x8252
#define GL_GUILTY_CONTEXT_RESET 0x8253
#define GL_INNOCENT_CONTEXT_RESET 0x8254
#define GL_UNKNOWN_CONTEXT_RESET 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY 0x8256
#define GL_NO_RESET_NOTIFICATION 0x8261
#define GL_CONTEXT_LOST 0x0507
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_PATCHES 0x000E
#define GL_PATCH_VERTICES 0x8E72
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_ISOLINES 0x8E7A
#define GL_QUADS 0x0007
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_IS_PER_PATCH 0x92E7
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_TEXTURE_BUFFER_BINDING 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_IMAGE_BUFFER 0x9051
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_COMPRESSED_RGBA_ASTC_4x4 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12 0x93DD
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#ifndef GL_ES_VERSION_2_0
#define GL_ES_VERSION_2_0 1
GLAPI int GLAD_GL_ES_VERSION_2_0;
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
GLAPI PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
#define glActiveTexture glad_glActiveTexture
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
GLAPI PFNGLATTACHSHADERPROC glad_glAttachShader;
#define glAttachShader glad_glAttachShader
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar *name);
GLAPI PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation;
#define glBindAttribLocation glad_glBindAttribLocation
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
GLAPI PFNGLBINDBUFFERPROC glad_glBindBuffer;
#define glBindBuffer glad_glBindBuffer
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
GLAPI PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
#define glBindFramebuffer glad_glBindFramebuffer
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
GLAPI PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
#define glBindRenderbuffer glad_glBindRenderbuffer
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
GLAPI PFNGLBINDTEXTUREPROC glad_glBindTexture;
#define glBindTexture glad_glBindTexture
typedef void (APIENTRYP PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLBLENDCOLORPROC glad_glBlendColor;
#define glBlendColor glad_glBlendColor
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);
GLAPI PFNGLBLENDEQUATIONPROC glad_glBlendEquation;
#define glBlendEquation glad_glBlendEquation
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
GLAPI PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate;
#define glBlendEquationSeparate glad_glBlendEquationSeparate
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
GLAPI PFNGLBLENDFUNCPROC glad_glBlendFunc;
#define glBlendFunc glad_glBlendFunc
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLAPI PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate;
#define glBlendFuncSeparate glad_glBlendFuncSeparate
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLAPI PFNGLBUFFERDATAPROC glad_glBufferData;
#define glBufferData glad_glBufferData
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GLAPI PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
#define glBufferSubData glad_glBufferSubData
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
GLAPI PFNGLCLEARPROC glad_glClear;
#define glClear glad_glClear
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLCLEARCOLORPROC glad_glClearColor;
#define glClearColor glad_glClearColor
typedef void (APIENTRYP PFNGLCLEARDEPTHFPROC)(GLfloat d);
GLAPI PFNGLCLEARDEPTHFPROC glad_glClearDepthf;
#define glClearDepthf glad_glClearDepthf
typedef void (APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
GLAPI PFNGLCLEARSTENCILPROC glad_glClearStencil;
#define glClearStencil glad_glClearStencil
typedef void (APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI PFNGLCOLORMASKPROC glad_glColorMask;
#define glColorMask glad_glColorMask
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
GLAPI PFNGLCOMPILESHADERPROC glad_glCompileShader;
#define glCompileShader glad_glCompileShader
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;
#define glCompressedTexImage2D glad_glCompressedTexImage2D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;
#define glCompressedTexSubImage2D glad_glCompressedTexSubImage2D
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;
#define glCopyTexImage2D glad_glCopyTexImage2D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;
#define glCopyTexSubImage2D glad_glCopyTexSubImage2D
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
GLAPI PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
#define glCreateProgram glad_glCreateProgram
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
GLAPI PFNGLCREATESHADERPROC glad_glCreateShader;
#define glCreateShader glad_glCreateShader
typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
GLAPI PFNGLCULLFACEPROC glad_glCullFace;
#define glCullFace glad_glCullFace
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
GLAPI PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
#define glDeleteBuffers glad_glDeleteBuffers
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
GLAPI PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
#define glDeleteFramebuffers glad_glDeleteFramebuffers
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
GLAPI PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
#define glDeleteProgram glad_glDeleteProgram
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint *renderbuffers);
GLAPI PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;
#define glDeleteRenderbuffers glad_glDeleteRenderbuffers
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
GLAPI PFNGLDELETESHADERPROC glad_glDeleteShader;
#define glDeleteShader glad_glDeleteShader
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
GLAPI PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
#define glDeleteTextures glad_glDeleteTextures
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
GLAPI PFNGLDEPTHFUNCPROC glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
typedef void (APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
GLAPI PFNGLDEPTHMASKPROC glad_glDepthMask;
#define glDepthMask glad_glDepthMask
typedef void (APIENTRYP PFNGLDEPTHRANGEFPROC)(GLfloat n, GLfloat f);
GLAPI PFNGLDEPTHRANGEFPROC glad_glDepthRangef;
#define glDepthRangef glad_glDepthRangef
typedef void (APIENTRYP PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
GLAPI PFNGLDETACHSHADERPROC glad_glDetachShader;
#define glDetachShader glad_glDetachShader
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
GLAPI PFNGLDISABLEPROC glad_glDisable;
#define glDisable glad_glDisable
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
GLAPI PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
GLAPI PFNGLDRAWARRAYSPROC glad_glDrawArrays;
#define glDrawArrays glad_glDrawArrays
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
GLAPI PFNGLDRAWELEMENTSPROC glad_glDrawElements;
#define glDrawElements glad_glDrawElements
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
GLAPI PFNGLENABLEPROC glad_glEnable;
#define glEnable glad_glEnable
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
typedef void (APIENTRYP PFNGLFINISHPROC)(void);
GLAPI PFNGLFINISHPROC glad_glFinish;
#define glFinish glad_glFinish
typedef void (APIENTRYP PFNGLFLUSHPROC)(void);
GLAPI PFNGLFLUSHPROC glad_glFlush;
#define glFlush glad_glFlush
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
#define glFramebufferRenderbuffer glad_glFramebufferRenderbuffer
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
#define glFramebufferTexture2D glad_glFramebufferTexture2D
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
GLAPI PFNGLFRONTFACEPROC glad_glFrontFace;
#define glFrontFace glad_glFrontFace
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
GLAPI PFNGLGENBUFFERSPROC glad_glGenBuffers;
#define glGenBuffers glad_glGenBuffers
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
GLAPI PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
#define glGenerateMipmap glad_glGenerateMipmap
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
GLAPI PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
#define glGenFramebuffers glad_glGenFramebuffers
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint *renderbuffers);
GLAPI PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
#define glGenRenderbuffers glad_glGenRenderbuffers
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
GLAPI PFNGLGENTEXTURESPROC glad_glGenTextures;
#define glGenTextures glad_glGenTextures
typedef void (APIENTRYP PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib;
#define glGetActiveAttrib glad_glGetActiveAttrib
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform;
#define glGetActiveUniform glad_glGetActiveUniform
typedef void (APIENTRYP PFNGLGETATTACHEDSHADERSPROC)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
GLAPI PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders;
#define glGetAttachedShaders glad_glGetAttachedShaders
typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation;
#define glGetAttribLocation glad_glGetAttribLocation
typedef void (APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
GLAPI PFNGLGETBOOLEANVPROC glad_glGetBooleanv;
#define glGetBooleanv glad_glGetBooleanv
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;
#define glGetBufferParameteriv glad_glGetBufferParameteriv
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
GLAPI PFNGLGETERRORPROC glad_glGetError;
#define glGetError glad_glGetError
typedef void (APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
GLAPI PFNGLGETFLOATVPROC glad_glGetFloatv;
#define glGetFloatv glad_glGetFloatv
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
GLAPI PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv;
#define glGetFramebufferAttachmentParameteriv glad_glGetFramebufferAttachmentParameteriv
typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
GLAPI PFNGLGETINTEGERVPROC glad_glGetIntegerv;
#define glGetIntegerv glad_glGetIntegerv
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
GLAPI PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
#define glGetProgramiv glad_glGetProgramiv
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
#define glGetProgramInfoLog glad_glGetProgramInfoLog
typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv;
#define glGetRenderbufferParameteriv glad_glGetRenderbufferParameteriv
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
GLAPI PFNGLGETSHADERIVPROC glad_glGetShaderiv;
#define glGetShaderiv glad_glGetShaderiv
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
#define glGetShaderInfoLog glad_glGetShaderInfoLog
typedef void (APIENTRYP PFNGLGETSHADERPRECISIONFORMATPROC)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
GLAPI PFNGLGETSHADERPRECISIONFORMATPROC glad_glGetShaderPrecisionFormat;
#define glGetShaderPrecisionFormat glad_glGetShaderPrecisionFormat
typedef void (APIENTRYP PFNGLGETSHADERSOURCEPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLAPI PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource;
#define glGetShaderSource glad_glGetShaderSource
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
GLAPI PFNGLGETSTRINGPROC glad_glGetString;
#define glGetString glad_glGetString
typedef void (APIENTRYP PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;
#define glGetTexParameterfv glad_glGetTexParameterfv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;
#define glGetTexParameteriv glad_glGetTexParameteriv
typedef void (APIENTRYP PFNGLGETUNIFORMFVPROC)(GLuint program, GLint location, GLfloat *params);
GLAPI PFNGLGETUNIFORMFVPROC glad_glGetUniformfv;
#define glGetUniformfv glad_glGetUniformfv
typedef void (APIENTRYP PFNGLGETUNIFORMIVPROC)(GLuint program, GLint location, GLint *params);
GLAPI PFNGLGETUNIFORMIVPROC glad_glGetUniformiv;
#define glGetUniformiv glad_glGetUniformiv
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
#define glGetUniformLocation glad_glGetUniformLocation
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBFVPROC)(GLuint index, GLenum pname, GLfloat *params);
GLAPI PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv;
#define glGetVertexAttribfv glad_glGetVertexAttribfv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVPROC)(GLuint index, GLenum pname, GLint *params);
GLAPI PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv;
#define glGetVertexAttribiv glad_glGetVertexAttribiv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index, GLenum pname, void **pointer);
GLAPI PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv;
#define glGetVertexAttribPointerv glad_glGetVertexAttribPointerv
typedef void (APIENTRYP PFNGLHINTPROC)(GLenum target, GLenum mode);
GLAPI PFNGLHINTPROC glad_glHint;
#define glHint glad_glHint
typedef GLboolean (APIENTRYP PFNGLISBUFFERPROC)(GLuint buffer);
GLAPI PFNGLISBUFFERPROC glad_glIsBuffer;
#define glIsBuffer glad_glIsBuffer
typedef GLboolean (APIENTRYP PFNGLISENABLEDPROC)(GLenum cap);
GLAPI PFNGLISENABLEDPROC glad_glIsEnabled;
#define glIsEnabled glad_glIsEnabled
typedef GLboolean (APIENTRYP PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
GLAPI PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer;
#define glIsFramebuffer glad_glIsFramebuffer
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC)(GLuint program);
GLAPI PFNGLISPROGRAMPROC glad_glIsProgram;
#define glIsProgram glad_glIsProgram
typedef GLboolean (APIENTRYP PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
GLAPI PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer;
#define glIsRenderbuffer glad_glIsRenderbuffer
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC)(GLuint shader);
GLAPI PFNGLISSHADERPROC glad_glIsShader;
#define glIsShader glad_glIsShader
typedef GLboolean (APIENTRYP PFNGLISTEXTUREPROC)(GLuint texture);
GLAPI PFNGLISTEXTUREPROC glad_glIsTexture;
#define glIsTexture glad_glIsTexture
typedef void (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
GLAPI PFNGLLINEWIDTHPROC glad_glLineWidth;
#define glLineWidth glad_glLineWidth
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
GLAPI PFNGLLINKPROGRAMPROC glad_glLinkProgram;
#define glLinkProgram glad_glLinkProgram
typedef void (APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
GLAPI PFNGLPIXELSTOREIPROC glad_glPixelStorei;
#define glPixelStorei glad_glPixelStorei
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
GLAPI PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;
#define glPolygonOffset glad_glPolygonOffset
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
GLAPI PFNGLREADPIXELSPROC glad_glReadPixels;
#define glReadPixels glad_glReadPixels
typedef void (APIENTRYP PFNGLRELEASESHADERCOMPILERPROC)(void);
GLAPI PFNGLRELEASESHADERCOMPILERPROC glad_glReleaseShaderCompiler;
#define glReleaseShaderCompiler glad_glReleaseShaderCompiler
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
#define glRenderbufferStorage glad_glRenderbufferStorage
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
GLAPI PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;
#define glSampleCoverage glad_glSampleCoverage
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSORPROC glad_glScissor;
#define glScissor glad_glScissor
typedef void (APIENTRYP PFNGLSHADERBINARYPROC)(GLsizei count, const GLuint *shaders, GLenum binaryFormat, const void *binary, GLsizei length);
GLAPI PFNGLSHADERBINARYPROC glad_glShaderBinary;
#define glShaderBinary glad_glShaderBinary
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
GLAPI PFNGLSHADERSOURCEPROC glad_glShaderSource;
#define glShaderSource glad_glShaderSource
typedef void (APIENTRYP PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLSTENCILFUNCPROC glad_glStencilFunc;
#define glStencilFunc glad_glStencilFunc
typedef void (APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face, GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate;
#define glStencilFuncSeparate glad_glStencilFuncSeparate
typedef void (APIENTRYP PFNGLSTENCILMASKPROC)(GLuint mask);
GLAPI PFNGLSTENCILMASKPROC glad_glStencilMask;
#define glStencilMask glad_glStencilMask
typedef void (APIENTRYP PFNGLSTENCILMASKSEPARATEPROC)(GLenum face, GLuint mask);
GLAPI PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate;
#define glStencilMaskSeparate glad_glStencilMaskSeparate
typedef void (APIENTRYP PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
GLAPI PFNGLSTENCILOPPROC glad_glStencilOp;
#define glStencilOp glad_glStencilOp
typedef void (APIENTRYP PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
GLAPI PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate;
#define glStencilOpSeparate glad_glStencilOpSeparate
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
#define glTexImage2D glad_glTexImage2D
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
GLAPI PFNGLTEXPARAMETERFPROC glad_glTexParameterf;
#define glTexParameterf glad_glTexParameterf
typedef void (APIENTRYP PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;
#define glTexParameterfv glad_glTexParameterfv
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
#define glTexParameteri glad_glTexParameteri
typedef void (APIENTRYP PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;
#define glTexParameteriv glad_glTexParameteriv
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;
#define glTexSubImage2D glad_glTexSubImage2D
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
GLAPI PFNGLUNIFORM1FPROC glad_glUniform1f;
#define glUniform1f glad_glUniform1f
typedef void (APIENTRYP PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM1FVPROC glad_glUniform1fv;
#define glUniform1fv glad_glUniform1fv
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
GLAPI PFNGLUNIFORM1IPROC glad_glUniform1i;
#define glUniform1i glad_glUniform1i
typedef void (APIENTRYP PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM1IVPROC glad_glUniform1iv;
#define glUniform1iv glad_glUniform1iv
typedef void (APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
GLAPI PFNGLUNIFORM2FPROC glad_glUniform2f;
#define glUniform2f glad_glUniform2f
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM2FVPROC glad_glUniform2fv;
#define glUniform2fv glad_glUniform2fv
typedef void (APIENTRYP PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
GLAPI PFNGLUNIFORM2IPROC glad_glUniform2i;
#define glUniform2i glad_glUniform2i
typedef void (APIENTRYP PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM2IVPROC glad_glUniform2iv;
#define glUniform2iv glad_glUniform2iv
typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI PFNGLUNIFORM3FPROC glad_glUniform3f;
#define glUniform3f glad_glUniform3f
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM3FVPROC glad_glUniform3fv;
#define glUniform3fv glad_glUniform3fv
typedef void (APIENTRYP PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
GLAPI PFNGLUNIFORM3IPROC glad_glUniform3i;
#define glUniform3i glad_glUniform3i
typedef void (APIENTRYP PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM3IVPROC glad_glUniform3iv;
#define glUniform3iv glad_glUniform3iv
typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI PFNGLUNIFORM4FPROC glad_glUniform4f;
#define glUniform4f glad_glUniform4f
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM4FVPROC glad_glUniform4fv;
#define glUniform4fv glad_glUniform4fv
typedef void (APIENTRYP PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI PFNGLUNIFORM4IPROC glad_glUniform4i;
#define glUniform4i glad_glUniform4i
typedef void (APIENTRYP PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM4IVPROC glad_glUniform4iv;
#define glUniform4iv glad_glUniform4iv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv;
#define glUniformMatrix2fv glad_glUniformMatrix2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;
#define glUniformMatrix3fv glad_glUniformMatrix3fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
#define glUniformMatrix4fv glad_glUniformMatrix4fv
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
GLAPI PFNGLUSEPROGRAMPROC glad_glUseProgram;
#define glUseProgram glad_glUseProgram
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC)(GLuint program);
GLAPI PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram;
#define glValidateProgram glad_glValidateProgram
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
GLAPI PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f;
#define glVertexAttrib1f glad_glVertexAttrib1f
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv;
#define glVertexAttrib1fv glad_glVertexAttrib1fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FPROC)(GLuint index, GLfloat x, GLfloat y);
GLAPI PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f;
#define glVertexAttrib2f glad_glVertexAttrib2f
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv;
#define glVertexAttrib2fv glad_glVertexAttrib2fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f;
#define glVertexAttrib3f glad_glVertexAttrib3f
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv;
#define glVertexAttrib3fv glad_glVertexAttrib3fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f;
#define glVertexAttrib4f glad_glVertexAttrib4f
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv;
#define glVertexAttrib4fv glad_glVertexAttrib4fv
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
#define glVertexAttribPointer glad_glVertexAttribPointer
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLVIEWPORTPROC glad_glViewport;
#define glViewport glad_glViewport
#endif
#ifndef GL_ES_VERSION_3_0
#define GL_ES_VERSION_3_0 1
GLAPI int GLAD_GL_ES_VERSION_3_0;
typedef void (APIENTRYP PFNGLREADBUFFERPROC)(GLenum src);
GLAPI PFNGLREADBUFFERPROC glad_glReadBuffer;
#define glReadBuffer glad_glReadBuffer
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
GLAPI PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements;
#define glDrawRangeElements glad_glDrawRangeElements
typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE3DPROC glad_glTexImage3D;
#define glTexImage3D glad_glTexImage3D
typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D;
#define glTexSubImage3D glad_glTexSubImage3D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D;
#define glCopyTexSubImage3D glad_glCopyTexSubImage3D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D;
#define glCompressedTexImage3D glad_glCompressedTexImage3D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D;
#define glCompressedTexSubImage3D glad_glCompressedTexSubImage3D
typedef void (APIENTRYP PFNGLGENQUERIESPROC)(GLsizei n, GLuint *ids);
GLAPI PFNGLGENQUERIESPROC glad_glGenQueries;
#define glGenQueries glad_glGenQueries
typedef void (APIENTRYP PFNGLDELETEQUERIESPROC)(GLsizei n, const GLuint *ids);
GLAPI PFNGLDELETEQUERIESPROC glad_glDeleteQueries;
#define glDeleteQueries glad_glDeleteQueries
typedef GLboolean (APIENTRYP PFNGLISQUERYPROC)(GLuint id);
GLAPI PFNGLISQUERYPROC glad_glIsQuery;
#define glIsQuery glad_glIsQuery
typedef void (APIENTRYP PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
GLAPI PFNGLBEGINQUERYPROC glad_glBeginQuery;
#define glBeginQuery glad_glBeginQuery
typedef void (APIENTRYP PFNGLENDQUERYPROC)(GLenum target);
GLAPI PFNGLENDQUERYPROC glad_glEndQuery;
#define glEndQuery glad_glEndQuery
typedef void (APIENTRYP PFNGLGETQUERYIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETQUERYIVPROC glad_glGetQueryiv;
#define glGetQueryiv glad_glGetQueryiv
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint *params);
GLAPI PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv;
#define glGetQueryObjectuiv glad_glGetQueryObjectuiv
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)(GLenum target);
GLAPI PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer;
#define glUnmapBuffer glad_glUnmapBuffer
typedef void (APIENTRYP PFNGLGETBUFFERPOINTERVPROC)(GLenum target, GLenum pname, void **params);
GLAPI PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv;
#define glGetBufferPointerv glad_glGetBufferPointerv
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum *bufs);
GLAPI PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;
#define glDrawBuffers glad_glDrawBuffers
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv;
#define glUniformMatrix2x3fv glad_glUniformMatrix2x3fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv;
#define glUniformMatrix3x2fv glad_glUniformMatrix3x2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv;
#define glUniformMatrix2x4fv glad_glUniformMatrix2x4fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv;
#define glUniformMatrix4x2fv glad_glUniformMatrix4x2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv;
#define glUniformMatrix3x4fv glad_glUniformMatrix3x4fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv;
#define glUniformMatrix4x3fv glad_glUniformMatrix4x3fv
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer;
#define glBlitFramebuffer glad_glBlitFramebuffer
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;
#define glRenderbufferStorageMultisample glad_glRenderbufferStorageMultisample
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer;
#define glFramebufferTextureLayer glad_glFramebufferTextureLayer
typedef void * (APIENTRYP PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange;
#define glMapBufferRange glad_glMapBufferRange
typedef void (APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
GLAPI PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange;
#define glFlushMappedBufferRange glad_glFlushMappedBufferRange
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
GLAPI PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
#define glBindVertexArray glad_glBindVertexArray
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
GLAPI PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
#define glDeleteVertexArrays glad_glDeleteVertexArrays
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
GLAPI PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
#define glGenVertexArrays glad_glGenVertexArrays
typedef GLboolean (APIENTRYP PFNGLISVERTEXARRAYPROC)(GLuint array);
GLAPI PFNGLISVERTEXARRAYPROC glad_glIsVertexArray;
#define glIsVertexArray glad_glIsVertexArray
typedef void (APIENTRYP PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint *data);
GLAPI PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v;
#define glGetIntegeri_v glad_glGetIntegeri_v
typedef void (APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKPROC)(GLenum primitiveMode);
GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback;
#define glBeginTransformFeedback glad_glBeginTransformFeedback
typedef void (APIENTRYP PFNGLENDTRANSFORMFEEDBACKPROC)(void);
GLAPI PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback;
#define glEndTransformFeedback glad_glEndTransformFeedback
typedef void (APIENTRYP PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange;
#define glBindBufferRange glad_glBindBufferRange
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
GLAPI PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase;
#define glBindBufferBase glad_glBindBufferBase
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings;
#define glTransformFeedbackVaryings glad_glTransformFeedbackVaryings
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying;
#define glGetTransformFeedbackVarying glad_glGetTransformFeedbackVarying
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer;
#define glVertexAttribIPointer glad_glVertexAttribIPointer
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIIVPROC)(GLuint index, GLenum pname, GLint *params);
GLAPI PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv;
#define glGetVertexAttribIiv glad_glGetVertexAttribIiv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIUIVPROC)(GLuint index, GLenum pname, GLuint *params);
GLAPI PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv;
#define glGetVertexAttribIuiv glad_glGetVertexAttribIuiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IPROC)(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i;
#define glVertexAttribI4i glad_glVertexAttribI4i
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui;
#define glVertexAttribI4ui glad_glVertexAttribI4ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv;
#define glVertexAttribI4iv glad_glVertexAttribI4iv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv;
#define glVertexAttribI4uiv glad_glVertexAttribI4uiv
typedef void (APIENTRYP PFNGLGETUNIFORMUIVPROC)(GLuint program, GLint location, GLuint *params);
GLAPI PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv;
#define glGetUniformuiv glad_glGetUniformuiv
typedef GLint (APIENTRYP PFNGLGETFRAGDATALOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation;
#define glGetFragDataLocation glad_glGetFragDataLocation
typedef void (APIENTRYP PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
GLAPI PFNGLUNIFORM1UIPROC glad_glUniform1ui;
#define glUniform1ui glad_glUniform1ui
typedef void (APIENTRYP PFNGLUNIFORM2UIPROC)(GLint location, GLuint v0, GLuint v1);
GLAPI PFNGLUNIFORM2UIPROC glad_glUniform2ui;
#define glUniform2ui glad_glUniform2ui
typedef void (APIENTRYP PFNGLUNIFORM3UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI PFNGLUNIFORM3UIPROC glad_glUniform3ui;
#define glUniform3ui glad_glUniform3ui
typedef void (APIENTRYP PFNGLUNIFORM4UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI PFNGLUNIFORM4UIPROC glad_glUniform4ui;
#define glUniform4ui glad_glUniform4ui
typedef void (APIENTRYP PFNGLUNIFORM1UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM1UIVPROC glad_glUniform1uiv;
#define glUniform1uiv glad_glUniform1uiv
typedef void (APIENTRYP PFNGLUNIFORM2UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM2UIVPROC glad_glUniform2uiv;
#define glUniform2uiv glad_glUniform2uiv
typedef void (APIENTRYP PFNGLUNIFORM3UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM3UIVPROC glad_glUniform3uiv;
#define glUniform3uiv glad_glUniform3uiv
typedef void (APIENTRYP PFNGLUNIFORM4UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM4UIVPROC glad_glUniform4uiv;
#define glUniform4uiv glad_glUniform4uiv
typedef void (APIENTRYP PFNGLCLEARBUFFERIVPROC)(GLenum buffer, GLint drawbuffer, const GLint *value);
GLAPI PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv;
#define glClearBufferiv glad_glClearBufferiv
typedef void (APIENTRYP PFNGLCLEARBUFFERUIVPROC)(GLenum buffer, GLint drawbuffer, const GLuint *value);
GLAPI PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv;
#define glClearBufferuiv glad_glClearBufferuiv
typedef void (APIENTRYP PFNGLCLEARBUFFERFVPROC)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
GLAPI PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv;
#define glClearBufferfv glad_glClearBufferfv
typedef void (APIENTRYP PFNGLCLEARBUFFERFIPROC)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GLAPI PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi;
#define glClearBufferfi glad_glClearBufferfi
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
GLAPI PFNGLGETSTRINGIPROC glad_glGetStringi;
#define glGetStringi glad_glGetStringi
typedef void (APIENTRYP PFNGLCOPYBUFFERSUBDATAPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLAPI PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData;
#define glCopyBufferSubData glad_glCopyBufferSubData
typedef void (APIENTRYP PFNGLGETUNIFORMINDICESPROC)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
GLAPI PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices;
#define glGetUniformIndices glad_glGetUniformIndices
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMSIVPROC)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
GLAPI PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv;
#define glGetActiveUniformsiv glad_glGetActiveUniformsiv
typedef GLuint (APIENTRYP PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar *uniformBlockName);
GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex;
#define glGetUniformBlockIndex glad_glGetUniformBlockIndex
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv;
#define glGetActiveUniformBlockiv glad_glGetActiveUniformBlockiv
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName;
#define glGetActiveUniformBlockName glad_glGetActiveUniformBlockName
typedef void (APIENTRYP PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
GLAPI PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding;
#define glUniformBlockBinding glad_glUniformBlockBinding
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
GLAPI PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;
#define glDrawArraysInstanced glad_glDrawArraysInstanced
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
GLAPI PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced;
#define glDrawElementsInstanced glad_glDrawElementsInstanced
typedef GLsync (APIENTRYP PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
GLAPI PFNGLFENCESYNCPROC glad_glFenceSync;
#define glFenceSync glad_glFenceSync
typedef GLboolean (APIENTRYP PFNGLISSYNCPROC)(GLsync sync);
GLAPI PFNGLISSYNCPROC glad_glIsSync;
#define glIsSync glad_glIsSync
typedef void (APIENTRYP PFNGLDELETESYNCPROC)(GLsync sync);
GLAPI PFNGLDELETESYNCPROC glad_glDeleteSync;
#define glDeleteSync glad_glDeleteSync
typedef GLenum (APIENTRYP PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync;
#define glClientWaitSync glad_glClientWaitSync
typedef void (APIENTRYP PFNGLWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLWAITSYNCPROC glad_glWaitSync;
#define glWaitSync glad_glWaitSync
typedef void (APIENTRYP PFNGLGETINTEGER64VPROC)(GLenum pname, GLint64 *data);
GLAPI PFNGLGETINTEGER64VPROC glad_glGetInteger64v;
#define glGetInteger64v glad_glGetInteger64v
typedef void (APIENTRYP PFNGLGETSYNCIVPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
GLAPI PFNGLGETSYNCIVPROC glad_glGetSynciv;
#define glGetSynciv glad_glGetSynciv
typedef void (APIENTRYP PFNGLGETINTEGER64I_VPROC)(GLenum target, GLuint index, GLint64 *data);
GLAPI PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v;
#define glGetInteger64i_v glad_glGetInteger64i_v
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERI64VPROC)(GLenum target, GLenum pname, GLint64 *params);
GLAPI PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v;
#define glGetBufferParameteri64v glad_glGetBufferParameteri64v
typedef void (APIENTRYP PFNGLGENSAMPLERSPROC)(GLsizei count, GLuint *samplers);
GLAPI PFNGLGENSAMPLERSPROC glad_glGenSamplers;
#define glGenSamplers glad_glGenSamplers
typedef void (APIENTRYP PFNGLDELETESAMPLERSPROC)(GLsizei count, const GLuint *samplers);
GLAPI PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers;
#define glDeleteSamplers glad_glDeleteSamplers
typedef GLboolean (APIENTRYP PFNGLISSAMPLERPROC)(GLuint sampler);
GLAPI PFNGLISSAMPLERPROC glad_glIsSampler;
#define glIsSampler glad_glIsSampler
typedef void (APIENTRYP PFNGLBINDSAMPLERPROC)(GLuint unit, GLuint sampler);
GLAPI PFNGLBINDSAMPLERPROC glad_glBindSampler;
#define glBindSampler glad_glBindSampler
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIPROC)(GLuint sampler, GLenum pname, GLint param);
GLAPI PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri;
#define glSamplerParameteri glad_glSamplerParameteri
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv;
#define glSamplerParameteriv glad_glSamplerParameteriv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFPROC)(GLuint sampler, GLenum pname, GLfloat param);
GLAPI PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf;
#define glSamplerParameterf glad_glSamplerParameterf
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, const GLfloat *param);
GLAPI PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv;
#define glSamplerParameterfv glad_glSamplerParameterfv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv;
#define glGetSamplerParameteriv glad_glGetSamplerParameteriv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, GLfloat *params);
GLAPI PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv;
#define glGetSamplerParameterfv glad_glGetSamplerParameterfv
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
GLAPI PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;
#define glVertexAttribDivisor glad_glVertexAttribDivisor
typedef void (APIENTRYP PFNGLBINDTRANSFORMFEEDBACKPROC)(GLenum target, GLuint id);
GLAPI PFNGLBINDTRANSFORMFEEDBACKPROC glad_glBindTransformFeedback;
#define glBindTransformFeedback glad_glBindTransformFeedback
typedef void (APIENTRYP PFNGLDELETETRANSFORMFEEDBACKSPROC)(GLsizei n, const GLuint *ids);
GLAPI PFNGLDELETETRANSFORMFEEDBACKSPROC glad_glDeleteTransformFeedbacks;
#define glDeleteTransformFeedbacks glad_glDeleteTransformFeedbacks
typedef void (APIENTRYP PFNGLGENTRANSFORMFEEDBACKSPROC)(GLsizei n, GLuint *ids);
GLAPI PFNGLGENTRANSFORMFEEDBACKSPROC glad_glGenTransformFeedbacks;
#define glGenTransformFeedbacks glad_glGenTransformFeedbacks
typedef GLboolean (APIENTRYP PFNGLISTRANSFORMFEEDBACKPROC)(GLuint id);
GLAPI PFNGLISTRANSFORMFEEDBACKPROC glad_glIsTransformFeedback;
#define glIsTransformFeedback glad_glIsTransformFeedback
typedef void (APIENTRYP PFNGLPAUSETRANSFORMFEEDBACKPROC)(void);
GLAPI PFNGLPAUSETRANSFORMFEEDBACKPROC glad_glPauseTransformFeedback;
#define glPauseTransformFeedback glad_glPauseTransformFeedback
typedef void (APIENTRYP PFNGLRESUMETRANSFORMFEEDBACKPROC)(void);
GLAPI PFNGLRESUMETRANSFORMFEEDBACKPROC glad_glResumeTransformFeedback;
#define glResumeTransformFeedback glad_glResumeTransformFeedback
typedef void (APIENTRYP PFNGLGETPROGRAMBINARYPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
GLAPI PFNGLGETPROGRAMBINARYPROC glad_glGetProgramBinary;
#define glGetProgramBinary glad_glGetProgramBinary
typedef void (APIENTRYP PFNGLPROGRAMBINARYPROC)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
GLAPI PFNGLPROGRAMBINARYPROC glad_glProgramBinary;
#define glProgramBinary glad_glProgramBinary
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERIPROC)(GLuint program, GLenum pname, GLint value);
GLAPI PFNGLPROGRAMPARAMETERIPROC glad_glProgramParameteri;
#define glProgramParameteri glad_glProgramParameteri
typedef void (APIENTRYP PFNGLINVALIDATEFRAMEBUFFERPROC)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
GLAPI PFNGLINVALIDATEFRAMEBUFFERPROC glad_glInvalidateFramebuffer;
#define glInvalidateFramebuffer glad_glInvalidateFramebuffer
typedef void (APIENTRYP PFNGLINVALIDATESUBFRAMEBUFFERPROC)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLINVALIDATESUBFRAMEBUFFERPROC glad_glInvalidateSubFramebuffer;
#define glInvalidateSubFramebuffer glad_glInvalidateSubFramebuffer
typedef void (APIENTRYP PFNGLTEXSTORAGE2DPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLTEXSTORAGE2DPROC glad_glTexStorage2D;
#define glTexStorage2D glad_glTexStorage2D
typedef void (APIENTRYP PFNGLTEXSTORAGE3DPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI PFNGLTEXSTORAGE3DPROC glad_glTexStorage3D;
#define glTexStorage3D glad_glTexStorage3D
typedef void (APIENTRYP PFNGLGETINTERNALFORMATIVPROC)(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint *params);
GLAPI PFNGLGETINTERNALFORMATIVPROC glad_glGetInternalformativ;
#define glGetInternalformativ glad_glGetInternalformativ
#endif
#ifndef GL_ES_VERSION_3_1
#define GL_ES_VERSION_3_1 1
GLAPI int GLAD_GL_ES_VERSION_3_1;
typedef void (APIENTRYP PFNGLDISPATCHCOMPUTEPROC)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GLAPI PFNGLDISPATCHCOMPUTEPROC glad_glDispatchCompute;
#define glDispatchCompute glad_glDispatchCompute
typedef void (APIENTRYP PFNGLDISPATCHCOMPUTEINDIRECTPROC)(GLintptr indirect);
GLAPI PFNGLDISPATCHCOMPUTEINDIRECTPROC glad_glDispatchComputeIndirect;
#define glDispatchComputeIndirect glad_glDispatchComputeIndirect
typedef void (APIENTRYP PFNGLDRAWARRAYSINDIRECTPROC)(GLenum mode, const void *indirect);
GLAPI PFNGLDRAWARRAYSINDIRECTPROC glad_glDrawArraysIndirect;
#define glDrawArraysIndirect glad_glDrawArraysIndirect
typedef void (APIENTRYP PFNGLDRAWELEMENTSINDIRECTPROC)(GLenum mode, GLenum type, const void *indirect);
GLAPI PFNGLDRAWELEMENTSINDIRECTPROC glad_glDrawElementsIndirect;
#define glDrawElementsIndirect glad_glDrawElementsIndirect
typedef void (APIENTRYP PFNGLFRAMEBUFFERPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLFRAMEBUFFERPARAMETERIPROC glad_glFramebufferParameteri;
#define glFramebufferParameteri glad_glFramebufferParameteri
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETFRAMEBUFFERPARAMETERIVPROC glad_glGetFramebufferParameteriv;
#define glGetFramebufferParameteriv glad_glGetFramebufferParameteriv
typedef void (APIENTRYP PFNGLGETPROGRAMINTERFACEIVPROC)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
GLAPI PFNGLGETPROGRAMINTERFACEIVPROC glad_glGetProgramInterfaceiv;
#define glGetProgramInterfaceiv glad_glGetProgramInterfaceiv
typedef GLuint (APIENTRYP PFNGLGETPROGRAMRESOURCEINDEXPROC)(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI PFNGLGETPROGRAMRESOURCEINDEXPROC glad_glGetProgramResourceIndex;
#define glGetProgramResourceIndex glad_glGetProgramResourceIndex
typedef void (APIENTRYP PFNGLGETPROGRAMRESOURCENAMEPROC)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
GLAPI PFNGLGETPROGRAMRESOURCENAMEPROC glad_glGetProgramResourceName;
#define glGetProgramResourceName glad_glGetProgramResourceName
typedef void (APIENTRYP PFNGLGETPROGRAMRESOURCEIVPROC)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLint *params);
GLAPI PFNGLGETPROGRAMRESOURCEIVPROC glad_glGetProgramResourceiv;
#define glGetProgramResourceiv glad_glGetProgramResourceiv
typedef GLint (APIENTRYP PFNGLGETPROGRAMRESOURCELOCATIONPROC)(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI PFNGLGETPROGRAMRESOURCELOCATIONPROC glad_glGetProgramResourceLocation;
#define glGetProgramResourceLocation glad_glGetProgramResourceLocation
typedef void (APIENTRYP PFNGLUSEPROGRAMSTAGESPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
GLAPI PFNGLUSEPROGRAMSTAGESPROC glad_glUseProgramStages;
#define glUseProgramStages glad_glUseProgramStages
typedef void (APIENTRYP PFNGLACTIVESHADERPROGRAMPROC)(GLuint pipeline, GLuint program);
GLAPI PFNGLACTIVESHADERPROGRAMPROC glad_glActiveShaderProgram;
#define glActiveShaderProgram glad_glActiveShaderProgram
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROGRAMVPROC)(GLenum type, GLsizei count, const GLchar *const*strings);
GLAPI PFNGLCREATESHADERPROGRAMVPROC glad_glCreateShaderProgramv;
#define glCreateShaderProgramv glad_glCreateShaderProgramv
typedef void (APIENTRYP PFNGLBINDPROGRAMPIPELINEPROC)(GLuint pipeline);
GLAPI PFNGLBINDPROGRAMPIPELINEPROC glad_glBindProgramPipeline;
#define glBindProgramPipeline glad_glBindProgramPipeline
typedef void (APIENTRYP PFNGLDELETEPROGRAMPIPELINESPROC)(GLsizei n, const GLuint *pipelines);
GLAPI PFNGLDELETEPROGRAMPIPELINESPROC glad_glDeleteProgramPipelines;
#define glDeleteProgramPipelines glad_glDeleteProgramPipelines
typedef void (APIENTRYP PFNGLGENPROGRAMPIPELINESPROC)(GLsizei n, GLuint *pipelines);
GLAPI PFNGLGENPROGRAMPIPELINESPROC glad_glGenProgramPipelines;
#define glGenProgramPipelines glad_glGenProgramPipelines
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPIPELINEPROC)(GLuint pipeline);
GLAPI PFNGLISPROGRAMPIPELINEPROC glad_glIsProgramPipeline;
#define glIsProgramPipeline glad_glIsProgramPipeline
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEIVPROC)(GLuint pipeline, GLenum pname, GLint *params);
GLAPI PFNGLGETPROGRAMPIPELINEIVPROC glad_glGetProgramPipelineiv;
#define glGetProgramPipelineiv glad_glGetProgramPipelineiv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IPROC)(GLuint program, GLint location, GLint v0);
GLAPI PFNGLPROGRAMUNIFORM1IPROC glad_glProgramUniform1i;
#define glProgramUniform1i glad_glProgramUniform1i
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IPROC)(GLuint program, GLint location, GLint v0, GLint v1);
GLAPI PFNGLPROGRAMUNIFORM2IPROC glad_glProgramUniform2i;
#define glProgramUniform2i glad_glProgramUniform2i
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
GLAPI PFNGLPROGRAMUNIFORM3IPROC glad_glProgramUniform3i;
#define glProgramUniform3i glad_glProgramUniform3i
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI PFNGLPROGRAMUNIFORM4IPROC glad_glProgramUniform4i;
#define glProgramUniform4i glad_glProgramUniform4i
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIPROC)(GLuint program, GLint location, GLuint v0);
GLAPI PFNGLPROGRAMUNIFORM1UIPROC glad_glProgramUniform1ui;
#define glProgramUniform1ui glad_glProgramUniform1ui
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1);
GLAPI PFNGLPROGRAMUNIFORM2UIPROC glad_glProgramUniform2ui;
#define glProgramUniform2ui glad_glProgramUniform2ui
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI PFNGLPROGRAMUNIFORM3UIPROC glad_glProgramUniform3ui;
#define glProgramUniform3ui glad_glProgramUniform3ui
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI PFNGLPROGRAMUNIFORM4UIPROC glad_glProgramUniform4ui;
#define glProgramUniform4ui glad_glProgramUniform4ui
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FPROC)(GLuint program, GLint location, GLfloat v0);
GLAPI PFNGLPROGRAMUNIFORM1FPROC glad_glProgramUniform1f;
#define glProgramUniform1f glad_glProgramUniform1f
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
GLAPI PFNGLPROGRAMUNIFORM2FPROC glad_glProgramUniform2f;
#define glProgramUniform2f glad_glProgramUniform2f
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI PFNGLPROGRAMUNIFORM3FPROC glad_glProgramUniform3f;
#define glProgramUniform3f glad_glProgramUniform3f
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI PFNGLPROGRAMUNIFORM4FPROC glad_glProgramUniform4f;
#define glProgramUniform4f glad_glProgramUniform4f
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IVPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM1IVPROC glad_glProgramUniform1iv;
#define glProgramUniform1iv glad_glProgramUniform1iv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IVPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM2IVPROC glad_glProgramUniform2iv;
#define glProgramUniform2iv glad_glProgramUniform2iv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IVPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM3IVPROC glad_glProgramUniform3iv;
#define glProgramUniform3iv glad_glProgramUniform3iv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IVPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM4IVPROC glad_glProgramUniform4iv;
#define glProgramUniform4iv glad_glProgramUniform4iv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM1UIVPROC glad_glProgramUniform1uiv;
#define glProgramUniform1uiv glad_glProgramUniform1uiv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM2UIVPROC glad_glProgramUniform2uiv;
#define glProgramUniform2uiv glad_glProgramUniform2uiv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM3UIVPROC glad_glProgramUniform3uiv;
#define glProgramUniform3uiv glad_glProgramUniform3uiv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIVPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM4UIVPROC glad_glProgramUniform4uiv;
#define glProgramUniform4uiv glad_glProgramUniform4uiv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM1FVPROC glad_glProgramUniform1fv;
#define glProgramUniform1fv glad_glProgramUniform1fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM2FVPROC glad_glProgramUniform2fv;
#define glProgramUniform2fv glad_glProgramUniform2fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM3FVPROC glad_glProgramUniform3fv;
#define glProgramUniform3fv glad_glProgramUniform3fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FVPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM4FVPROC glad_glProgramUniform4fv;
#define glProgramUniform4fv glad_glProgramUniform4fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2FVPROC glad_glProgramUniformMatrix2fv;
#define glProgramUniformMatrix2fv glad_glProgramUniformMatrix2fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3FVPROC glad_glProgramUniformMatrix3fv;
#define glProgramUniformMatrix3fv glad_glProgramUniformMatrix3fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4FVPROC glad_glProgramUniformMatrix4fv;
#define glProgramUniformMatrix4fv glad_glProgramUniformMatrix4fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glad_glProgramUniformMatrix2x3fv;
#define glProgramUniformMatrix2x3fv glad_glProgramUniformMatrix2x3fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glad_glProgramUniformMatrix3x2fv;
#define glProgramUniformMatrix3x2fv glad_glProgramUniformMatrix3x2fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glad_glProgramUniformMatrix2x4fv;
#define glProgramUniformMatrix2x4fv glad_glProgramUniformMatrix2x4fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glad_glProgramUniformMatrix4x2fv;
#define glProgramUniformMatrix4x2fv glad_glProgramUniformMatrix4x2fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glad_glProgramUniformMatrix3x4fv;
#define glProgramUniformMatrix3x4fv glad_glProgramUniformMatrix3x4fv
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glad_glProgramUniformMatrix4x3fv;
#define glProgramUniformMatrix4x3fv glad_glProgramUniformMatrix4x3fv
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPIPELINEPROC)(GLuint pipeline);
GLAPI PFNGLVALIDATEPROGRAMPIPELINEPROC glad_glValidateProgramPipeline;
#define glValidateProgramPipeline glad_glValidateProgramPipeline
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEINFOLOGPROC)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETPROGRAMPIPELINEINFOLOGPROC glad_glGetProgramPipelineInfoLog;
#define glGetProgramPipelineInfoLog glad_glGetProgramPipelineInfoLog
typedef void (APIENTRYP PFNGLBINDIMAGETEXTUREPROC)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GLAPI PFNGLBINDIMAGETEXTUREPROC glad_glBindImageTexture;
#define glBindImageTexture glad_glBindImageTexture
typedef void (APIENTRYP PFNGLGETBOOLEANI_VPROC)(GLenum target, GLuint index, GLboolean *data);
GLAPI PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v;
#define glGetBooleani_v glad_glGetBooleani_v
typedef void (APIENTRYP PFNGLMEMORYBARRIERPROC)(GLbitfield barriers);
GLAPI PFNGLMEMORYBARRIERPROC glad_glMemoryBarrier;
#define glMemoryBarrier glad_glMemoryBarrier
typedef void (APIENTRYP PFNGLMEMORYBARRIERBYREGIONPROC)(GLbitfield barriers);
GLAPI PFNGLMEMORYBARRIERBYREGIONPROC glad_glMemoryBarrierByRegion;
#define glMemoryBarrierByRegion glad_glMemoryBarrierByRegion
typedef void (APIENTRYP PFNGLTEXSTORAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI PFNGLTEXSTORAGE2DMULTISAMPLEPROC glad_glTexStorage2DMultisample;
#define glTexStorage2DMultisample glad_glTexStorage2DMultisample
typedef void (APIENTRYP PFNGLGETMULTISAMPLEFVPROC)(GLenum pname, GLuint index, GLfloat *val);
GLAPI PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv;
#define glGetMultisamplefv glad_glGetMultisamplefv
typedef void (APIENTRYP PFNGLSAMPLEMASKIPROC)(GLuint maskNumber, GLbitfield mask);
GLAPI PFNGLSAMPLEMASKIPROC glad_glSampleMaski;
#define glSampleMaski glad_glSampleMaski
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv;
#define glGetTexLevelParameteriv glad_glGetTexLevelParameteriv
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv;
#define glGetTexLevelParameterfv glad_glGetTexLevelParameterfv
typedef void (APIENTRYP PFNGLBINDVERTEXBUFFERPROC)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GLAPI PFNGLBINDVERTEXBUFFERPROC glad_glBindVertexBuffer;
#define glBindVertexBuffer glad_glBindVertexBuffer
typedef void (APIENTRYP PFNGLVERTEXATTRIBFORMATPROC)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GLAPI PFNGLVERTEXATTRIBFORMATPROC glad_glVertexAttribFormat;
#define glVertexAttribFormat glad_glVertexAttribFormat
typedef void (APIENTRYP PFNGLVERTEXATTRIBIFORMATPROC)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI PFNGLVERTEXATTRIBIFORMATPROC glad_glVertexAttribIFormat;
#define glVertexAttribIFormat glad_glVertexAttribIFormat
typedef void (APIENTRYP PFNGLVERTEXATTRIBBINDINGPROC)(GLuint attribindex, GLuint bindingindex);
GLAPI PFNGLVERTEXATTRIBBINDINGPROC glad_glVertexAttribBinding;
#define glVertexAttribBinding glad_glVertexAttribBinding
typedef void (APIENTRYP PFNGLVERTEXBINDINGDIVISORPROC)(GLuint bindingindex, GLuint divisor);
GLAPI PFNGLVERTEXBINDINGDIVISORPROC glad_glVertexBindingDivisor;
#define glVertexBindingDivisor glad_glVertexBindingDivisor
#endif
#ifndef GL_ES_VERSION_3_2
#define GL_ES_VERSION_3_2 1
GLAPI int GLAD_GL_ES_VERSION_3_2;
typedef void (APIENTRYP PFNGLBLENDBARRIERPROC)(void);
GLAPI PFNGLBLENDBARRIERPROC glad_glBlendBarrier;
#define glBlendBarrier glad_glBlendBarrier
typedef void (APIENTRYP PFNGLCOPYIMAGESUBDATAPROC)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GLAPI PFNGLCOPYIMAGESUBDATAPROC glad_glCopyImageSubData;
#define glCopyImageSubData glad_glCopyImageSubData
typedef void (APIENTRYP PFNGLDEBUGMESSAGECONTROLPROC)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI PFNGLDEBUGMESSAGECONTROLPROC glad_glDebugMessageControl;
#define glDebugMessageControl glad_glDebugMessageControl
typedef void (APIENTRYP PFNGLDEBUGMESSAGEINSERTPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GLAPI PFNGLDEBUGMESSAGEINSERTPROC glad_glDebugMessageInsert;
#define glDebugMessageInsert glad_glDebugMessageInsert
typedef void (APIENTRYP PFNGLDEBUGMESSAGECALLBACKPROC)(GLDEBUGPROC callback, const void *userParam);
GLAPI PFNGLDEBUGMESSAGECALLBACKPROC glad_glDebugMessageCallback;
#define glDebugMessageCallback glad_glDebugMessageCallback
typedef GLuint (APIENTRYP PFNGLGETDEBUGMESSAGELOGPROC)(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
GLAPI PFNGLGETDEBUGMESSAGELOGPROC glad_glGetDebugMessageLog;
#define glGetDebugMessageLog glad_glGetDebugMessageLog
typedef void (APIENTRYP PFNGLPUSHDEBUGGROUPPROC)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
GLAPI PFNGLPUSHDEBUGGROUPPROC glad_glPushDebugGroup;
#define glPushDebugGroup glad_glPushDebugGroup
typedef void (APIENTRYP PFNGLPOPDEBUGGROUPPROC)(void);
GLAPI PFNGLPOPDEBUGGROUPPROC glad_glPopDebugGroup;
#define glPopDebugGroup glad_glPopDebugGroup
typedef void (APIENTRYP PFNGLOBJECTLABELPROC)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
GLAPI PFNGLOBJECTLABELPROC glad_glObjectLabel;
#define glObjectLabel glad_glObjectLabel
typedef void (APIENTRYP PFNGLGETOBJECTLABELPROC)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI PFNGLGETOBJECTLABELPROC glad_glGetObjectLabel;
#define glGetObjectLabel glad_glGetObjectLabel
typedef void (APIENTRYP PFNGLOBJECTPTRLABELPROC)(const void *ptr, GLsizei length, const GLchar *label);
GLAPI PFNGLOBJECTPTRLABELPROC glad_glObjectPtrLabel;
#define glObjectPtrLabel glad_glObjectPtrLabel
typedef void (APIENTRYP PFNGLGETOBJECTPTRLABELPROC)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI PFNGLGETOBJECTPTRLABELPROC glad_glGetObjectPtrLabel;
#define glGetObjectPtrLabel glad_glGetObjectPtrLabel
typedef void (APIENTRYP PFNGLGETPOINTERVPROC)(GLenum pname, void **params);
GLAPI PFNGLGETPOINTERVPROC glad_glGetPointerv;
#define glGetPointerv glad_glGetPointerv
typedef void (APIENTRYP PFNGLENABLEIPROC)(GLenum target, GLuint index);
GLAPI PFNGLENABLEIPROC glad_glEnablei;
#define glEnablei glad_glEnablei
typedef void (APIENTRYP PFNGLDISABLEIPROC)(GLenum target, GLuint index);
GLAPI PFNGLDISABLEIPROC glad_glDisablei;
#define glDisablei glad_glDisablei
typedef void (APIENTRYP PFNGLBLENDEQUATIONIPROC)(GLuint buf, GLenum mode);
GLAPI PFNGLBLENDEQUATIONIPROC glad_glBlendEquationi;
#define glBlendEquationi glad_glBlendEquationi
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIPROC)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLAPI PFNGLBLENDEQUATIONSEPARATEIPROC glad_glBlendEquationSeparatei;
#define glBlendEquationSeparatei glad_glBlendEquationSeparatei
typedef void (APIENTRYP PFNGLBLENDFUNCIPROC)(GLuint buf, GLenum src, GLenum dst);
GLAPI PFNGLBLENDFUNCIPROC glad_glBlendFunci;
#define glBlendFunci glad_glBlendFunci
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIPROC)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLAPI PFNGLBLENDFUNCSEPARATEIPROC glad_glBlendFuncSeparatei;
#define glBlendFuncSeparatei glad_glBlendFuncSeparatei
typedef void (APIENTRYP PFNGLCOLORMASKIPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI PFNGLCOLORMASKIPROC glad_glColorMaski;
#define glColorMaski glad_glColorMaski
typedef GLboolean (APIENTRYP PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
GLAPI PFNGLISENABLEDIPROC glad_glIsEnabledi;
#define glIsEnabledi glad_glIsEnabledi
typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex;
#define glDrawElementsBaseVertex glad_glDrawElementsBaseVertex
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex;
#define glDrawRangeElementsBaseVertex glad_glDrawRangeElementsBaseVertex
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex;
#define glDrawElementsInstancedBaseVertex glad_glDrawElementsInstancedBaseVertex
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture;
#define glFramebufferTexture glad_glFramebufferTexture
typedef void (APIENTRYP PFNGLPRIMITIVEBOUNDINGBOXPROC)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
GLAPI PFNGLPRIMITIVEBOUNDINGBOXPROC glad_glPrimitiveBoundingBox;
#define glPrimitiveBoundingBox glad_glPrimitiveBoundingBox
typedef GLenum (APIENTRYP PFNGLGETGRAPHICSRESETSTATUSPROC)(void);
GLAPI PFNGLGETGRAPHICSRESETSTATUSPROC glad_glGetGraphicsResetStatus;
#define glGetGraphicsResetStatus glad_glGetGraphicsResetStatus
typedef void (APIENTRYP PFNGLREADNPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GLAPI PFNGLREADNPIXELSPROC glad_glReadnPixels;
#define glReadnPixels glad_glReadnPixels
typedef void (APIENTRYP PFNGLGETNUNIFORMFVPROC)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI PFNGLGETNUNIFORMFVPROC glad_glGetnUniformfv;
#define glGetnUniformfv glad_glGetnUniformfv
typedef void (APIENTRYP PFNGLGETNUNIFORMIVPROC)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI PFNGLGETNUNIFORMIVPROC glad_glGetnUniformiv;
#define glGetnUniformiv glad_glGetnUniformiv
typedef void (APIENTRYP PFNGLGETNUNIFORMUIVPROC)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI PFNGLGETNUNIFORMUIVPROC glad_glGetnUniformuiv;
#define glGetnUniformuiv glad_glGetnUniformuiv
typedef void (APIENTRYP PFNGLMINSAMPLESHADINGPROC)(GLfloat value);
GLAPI PFNGLMINSAMPLESHADINGPROC glad_glMinSampleShading;
#define glMinSampleShading glad_glMinSampleShading
typedef void (APIENTRYP PFNGLPATCHPARAMETERIPROC)(GLenum pname, GLint value);
GLAPI PFNGLPATCHPARAMETERIPROC glad_glPatchParameteri;
#define glPatchParameteri glad_glPatchParameteri
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv;
#define glTexParameterIiv glad_glTexParameterIiv
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, const GLuint *params);
GLAPI PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv;
#define glTexParameterIuiv glad_glTexParameterIuiv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv;
#define glGetTexParameterIiv glad_glGetTexParameterIiv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, GLuint *params);
GLAPI PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv;
#define glGetTexParameterIuiv glad_glGetTexParameterIuiv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv;
#define glSamplerParameterIiv glad_glSamplerParameterIiv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, const GLuint *param);
GLAPI PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv;
#define glSamplerParameterIuiv glad_glSamplerParameterIuiv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv;
#define glGetSamplerParameterIiv glad_glGetSamplerParameterIiv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, GLuint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv;
#define glGetSamplerParameterIuiv glad_glGetSamplerParameterIuiv
typedef void (APIENTRYP PFNGLTEXBUFFERPROC)(GLenum target, GLenum internalformat, GLuint buffer);
GLAPI PFNGLTEXBUFFERPROC glad_glTexBuffer;
#define glTexBuffer glad_glTexBuffer
typedef void (APIENTRYP PFNGLTEXBUFFERRANGEPROC)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI PFNGLTEXBUFFERRANGEPROC glad_glTexBufferRange;
#define glTexBufferRange glad_glTexBufferRange
typedef void (APIENTRYP PFNGLTEXSTORAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI PFNGLTEXSTORAGE3DMULTISAMPLEPROC glad_glTexStorage3DMultisample;
#define glTexStorage3DMultisample glad_glTexStorage3DMultisample
#endif
#define GL_3DC_X_AMD 0x87F9
#define GL_3DC_XY_AMD 0x87FA
#define GL_ATC_RGB_AMD 0x8C92
#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD 0x8C93
#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD 0x87EE
#define GL_RENDERBUFFER_STORAGE_SAMPLES_AMD 0x91B2
#define GL_MAX_COLOR_FRAMEBUFFER_SAMPLES_AMD 0x91B3
#define GL_MAX_COLOR_FRAMEBUFFER_STORAGE_SAMPLES_AMD 0x91B4
#define GL_MAX_DEPTH_STENCIL_FRAMEBUFFER_SAMPLES_AMD 0x91B5
#define GL_NUM_SUPPORTED_MULTISAMPLE_MODES_AMD 0x91B6
#define GL_SUPPORTED_MULTISAMPLE_MODES_AMD 0x91B7
#define GL_COUNTER_TYPE_AMD 0x8BC0
#define GL_COUNTER_RANGE_AMD 0x8BC1
#define GL_UNSIGNED_INT64_AMD 0x8BC2
#define GL_PERCENTAGE_AMD 0x8BC3
#define GL_PERFMON_RESULT_AVAILABLE_AMD 0x8BC4
#define GL_PERFMON_RESULT_SIZE_AMD 0x8BC5
#define GL_PERFMON_RESULT_AMD 0x8BC6
#define GL_Z400_BINARY_AMD 0x8740
#define GL_DEPTH_STENCIL_OES 0x84F9
#define GL_UNSIGNED_INT_24_8_OES 0x84FA
#define GL_DEPTH_COMPONENT32_OES 0x81A7
#define GL_DEPTH24_STENCIL8_OES 0x88F0
#define GL_READ_FRAMEBUFFER_ANGLE 0x8CA8
#define GL_DRAW_FRAMEBUFFER_ANGLE 0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING_ANGLE 0x8CA6
#define GL_READ_FRAMEBUFFER_BINDING_ANGLE 0x8CAA
#define GL_RENDERBUFFER_SAMPLES_ANGLE 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_ANGLE 0x8D56
#define GL_MAX_SAMPLES_ANGLE 0x8D57
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE 0x88FE
#define GL_PACK_REVERSE_ROW_ORDER_ANGLE 0x93A4
#define GL_PROGRAM_BINARY_ANGLE 0x93A6
#define GL_COMPRESSED_RGBA_S3TC_DXT3_ANGLE 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE 0x83F3
#define GL_TEXTURE_USAGE_ANGLE 0x93A2
#define GL_FRAMEBUFFER_ATTACHMENT_ANGLE 0x93A3
#define GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE 0x93A0
#define GL_MAX_CLIP_DISTANCES_APPLE 0x0D32
#define GL_CLIP_DISTANCE0_APPLE 0x3000
#define GL_CLIP_DISTANCE1_APPLE 0x3001
#define GL_CLIP_DISTANCE2_APPLE 0x3002
#define GL_CLIP_DISTANCE3_APPLE 0x3003
#define GL_CLIP_DISTANCE4_APPLE 0x3004
#define GL_CLIP_DISTANCE5_APPLE 0x3005
#define GL_CLIP_DISTANCE6_APPLE 0x3006
#define GL_CLIP_DISTANCE7_APPLE 0x3007
#define GL_RENDERBUFFER_SAMPLES_APPLE 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_APPLE 0x8D56
#define GL_MAX_SAMPLES_APPLE 0x8D57
#define GL_READ_FRAMEBUFFER_APPLE 0x8CA8
#define GL_DRAW_FRAMEBUFFER_APPLE 0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING_APPLE 0x8CA6
#define GL_READ_FRAMEBUFFER_BINDING_APPLE 0x8CAA
#define GL_RGB_422_APPLE 0x8A1F
#define GL_UNSIGNED_SHORT_8_8_APPLE 0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE 0x85BB
#define GL_RGB_RAW_422_APPLE 0x8A51
#define GL_SYNC_OBJECT_APPLE 0x8A53
#define GL_MAX_SERVER_WAIT_TIMEOUT_APPLE 0x9111
#define GL_OBJECT_TYPE_APPLE 0x9112
#define GL_SYNC_CONDITION_APPLE 0x9113
#define GL_SYNC_STATUS_APPLE 0x9114
#define GL_SYNC_FLAGS_APPLE 0x9115
#define GL_SYNC_FENCE_APPLE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE 0x9117
#define GL_UNSIGNALED_APPLE 0x9118
#define GL_SIGNALED_APPLE 0x9119
#define GL_ALREADY_SIGNALED_APPLE 0x911A
#define GL_TIMEOUT_EXPIRED_APPLE 0x911B
#define GL_CONDITION_SATISFIED_APPLE 0x911C
#define GL_WAIT_FAILED_APPLE 0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT_APPLE 0x00000001
#define GL_TIMEOUT_IGNORED_APPLE 0xFFFFFFFFFFFFFFFF
#define GL_BGRA_EXT 0x80E1
#define GL_BGRA8_EXT 0x93A1
#define GL_TEXTURE_MAX_LEVEL_APPLE 0x813D
#define GL_UNSIGNED_INT_10F_11F_11F_REV_APPLE 0x8C3B
#define GL_UNSIGNED_INT_5_9_9_9_REV_APPLE 0x8C3E
#define GL_R11F_G11F_B10F_APPLE 0x8C3A
#define GL_RGB9_E5_APPLE 0x8C3D
#define GL_MALI_PROGRAM_BINARY_ARM 0x8F61
#define GL_MALI_SHADER_BINARY_ARM 0x8F60
#define GL_FETCH_PER_SAMPLE_ARM 0x8F65
#define GL_FRAGMENT_SHADER_FRAMEBUFFER_FETCH_MRT_ARM 0x8F66
#define GL_TEXTURE_UNNORMALIZED_COORDINATES_ARM 0x8F6A
#define GL_SMAPHS30_PROGRAM_BINARY_DMP 0x9251
#define GL_SMAPHS_PROGRAM_BINARY_DMP 0x9252
#define GL_DMP_PROGRAM_BINARY_DMP 0x9253
#define GL_SHADER_BINARY_DMP 0x9250
#define GL_SURFACE_COMPRESSION_EXT 0x96C0
#define GL_SURFACE_COMPRESSION_FIXED_RATE_NONE_EXT 0x96C1
#define GL_SURFACE_COMPRESSION_FIXED_RATE_DEFAULT_EXT 0x96C2
#define GL_SAMPLER_EXTERNAL_2D_Y2Y_EXT 0x8BE7
#define GL_TEXTURE_EXTERNAL_OES 0x8D65
#define GL_TEXTURE_BINDING_EXTERNAL_OES 0x8D67
#define GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES 0x8D68
#define GL_SRC1_COLOR_EXT 0x88F9
#define GL_SRC1_ALPHA_EXT 0x8589
#define GL_ONE_MINUS_SRC1_COLOR_EXT 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA_EXT 0x88FB
#define GL_SRC_ALPHA_SATURATE_EXT 0x0308
#define GL_LOCATION_INDEX_EXT 0x930F
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS_EXT 0x88FC
#define GL_MIN_EXT 0x8007
#define GL_MAX_EXT 0x8008
#define GL_FUNC_ADD_EXT 0x8006
#define GL_BLEND_EQUATION_EXT 0x8009
#define GL_MAP_PERSISTENT_BIT_EXT 0x0040
#define GL_MAP_COHERENT_BIT_EXT 0x0080
#define GL_DYNAMIC_STORAGE_BIT_EXT 0x0100
#define GL_CLIENT_STORAGE_BIT_EXT 0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE_EXT 0x821F
#define GL_BUFFER_STORAGE_FLAGS_EXT 0x8220
#define GL_LOWER_LEFT_EXT 0x8CA1
#define GL_UPPER_LEFT_EXT 0x8CA2
#define GL_NEGATIVE_ONE_TO_ONE_EXT 0x935E
#define GL_ZERO_TO_ONE_EXT 0x935F
#define GL_CLIP_ORIGIN_EXT 0x935C
#define GL_CLIP_DEPTH_MODE_EXT 0x935D
#define GL_MAX_CLIP_DISTANCES_EXT 0x0D32
#define GL_MAX_CULL_DISTANCES_EXT 0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES_EXT 0x82FA
#define GL_CLIP_DISTANCE0_EXT 0x3000
#define GL_CLIP_DISTANCE1_EXT 0x3001
#define GL_CLIP_DISTANCE2_EXT 0x3002
#define GL_CLIP_DISTANCE3_EXT 0x3003
#define GL_CLIP_DISTANCE4_EXT 0x3004
#define GL_CLIP_DISTANCE5_EXT 0x3005
#define GL_CLIP_DISTANCE6_EXT 0x3006
#define GL_CLIP_DISTANCE7_EXT 0x3007
#define GL_RGBA16F_EXT 0x881A
#define GL_RGB16F_EXT 0x881B
#define GL_RG16F_EXT 0x822F
#define GL_R16F_EXT 0x822D
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT 0x8211
#define GL_UNSIGNED_NORMALIZED_EXT 0x8C17
#define GL_PROGRAM_PIPELINE_OBJECT_EXT 0x8A4F
#define GL_PROGRAM_OBJECT_EXT 0x8B40
#define GL_SHADER_OBJECT_EXT 0x8B48
#define GL_BUFFER_OBJECT_EXT 0x9151
#define GL_QUERY_OBJECT_EXT 0x9153
#define GL_VERTEX_ARRAY_OBJECT_EXT 0x9154
#define GL_DEPTH_CLAMP_EXT 0x864F
#define GL_COLOR_EXT 0x1800
#define GL_DEPTH_EXT 0x1801
#define GL_STENCIL_EXT 0x1802
#define GL_QUERY_COUNTER_BITS_EXT 0x8864
#define GL_CURRENT_QUERY_EXT 0x8865
#define GL_QUERY_RESULT_EXT 0x8866
#define GL_QUERY_RESULT_AVAILABLE_EXT 0x8867
#define GL_TIME_ELAPSED_EXT 0x88BF
#define GL_TIMESTAMP_EXT 0x8E28
#define GL_GPU_DISJOINT_EXT 0x8FBB
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_MAX_DRAW_BUFFERS_EXT 0x8824
#define GL_DRAW_BUFFER0_EXT 0x8825
#define GL_DRAW_BUFFER1_EXT 0x8826
#define GL_DRAW_BUFFER2_EXT 0x8827
#define GL_DRAW_BUFFER3_EXT 0x8828
#define GL_DRAW_BUFFER4_EXT 0x8829
#define GL_DRAW_BUFFER5_EXT 0x882A
#define GL_DRAW_BUFFER6_EXT 0x882B
#define GL_DRAW_BUFFER7_EXT 0x882C
#define GL_DRAW_BUFFER8_EXT 0x882D
#define GL_DRAW_BUFFER9_EXT 0x882E
#define GL_DRAW_BUFFER10_EXT 0x882F
#define GL_DRAW_BUFFER11_EXT 0x8830
#define GL_DRAW_BUFFER12_EXT 0x8831
#define GL_DRAW_BUFFER13_EXT 0x8832
#define GL_DRAW_BUFFER14_EXT 0x8833
#define GL_DRAW_BUFFER15_EXT 0x8834
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_GEOMETRY_SHADER_BIT_EXT 0x00000004
#define GL_GEOMETRY_LINKED_VERTICES_OUT_EXT 0x8916
#define GL_GEOMETRY_LINKED_INPUT_TYPE_EXT 0x8917
#define GL_GEOMETRY_LINKED_OUTPUT_TYPE_EXT 0x8918
#define GL_GEOMETRY_SHADER_INVOCATIONS_EXT 0x887F
#define GL_LAYER_PROVOKING_VERTEX_EXT 0x825E
#define GL_LINES_ADJACENCY_EXT 0x000A
#define GL_LINE_STRIP_ADJACENCY_EXT 0x000B
#define GL_TRIANGLES_ADJACENCY_EXT 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0x000D
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS_EXT 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8A32
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS_EXT 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_EXT 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS_EXT 0x8E5A
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_EXT 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS_EXT 0x92D5
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS_EXT 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_EXT 0x90D7
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_UNDEFINED_VERTEX_EXT 0x8260
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_FRAMEBUFFER_DEFAULT_LAYERS_EXT 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS_EXT 0x9317
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_REFERENCED_BY_GEOMETRY_SHADER_EXT 0x9309
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_EXT 0x88FE
#define GL_MAP_READ_BIT_EXT 0x0001
#define GL_MAP_WRITE_BIT_EXT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT_EXT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT_EXT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT 0x0020
#define GL_TEXTURE_TILING_EXT 0x9580
#define GL_DEDICATED_MEMORY_OBJECT_EXT 0x9581
#define GL_PROTECTED_MEMORY_OBJECT_EXT 0x959B
#define GL_NUM_TILING_TYPES_EXT 0x9582
#define GL_TILING_TYPES_EXT 0x9583
#define GL_OPTIMAL_TILING_EXT 0x9584
#define GL_LINEAR_TILING_EXT 0x9585
#define GL_NUM_DEVICE_UUIDS_EXT 0x9596
#define GL_DEVICE_UUID_EXT 0x9597
#define GL_DRIVER_UUID_EXT 0x9598
#define GL_UUID_SIZE_EXT 16
#define GL_HANDLE_TYPE_OPAQUE_FD_EXT 0x9586
#define GL_HANDLE_TYPE_OPAQUE_WIN32_EXT 0x9587
#define GL_HANDLE_TYPE_OPAQUE_WIN32_KMT_EXT 0x9588
#define GL_DEVICE_LUID_EXT 0x9599
#define GL_DEVICE_NODE_MASK_EXT 0x959A
#define GL_LUID_SIZE_EXT 8
#define GL_HANDLE_TYPE_D3D12_TILEPOOL_EXT 0x9589
#define GL_HANDLE_TYPE_D3D12_RESOURCE_EXT 0x958A
#define GL_HANDLE_TYPE_D3D11_IMAGE_EXT 0x958B
#define GL_HANDLE_TYPE_D3D11_IMAGE_KMT_EXT 0x958C
#define GL_MULTISAMPLE_EXT 0x809D
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT 0x8D6C
#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT 0x8D57
#define GL_COLOR_ATTACHMENT_EXT 0x90F0
#define GL_MULTIVIEW_EXT 0x90F1
#define GL_DRAW_BUFFER_EXT 0x0C01
#define GL_READ_BUFFER_EXT 0x0C02
#define GL_MAX_MULTIVIEW_BUFFERS_EXT 0x90F2
#define GL_ANY_SAMPLES_PASSED_EXT 0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE_EXT 0x8D6A
#define GL_POLYGON_OFFSET_CLAMP_EXT 0x8E1B
#define GL_PRIMITIVE_BOUNDING_BOX_EXT 0x92BE
#define GL_CONTEXT_FLAG_PROTECTED_CONTENT_BIT_EXT 0x00000010
#define GL_TEXTURE_PROTECTED_EXT 0x8BFA
#define GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT 0x8A54
#define GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT 0x8A55
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT 0x8A56
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT 0x8A57
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV2_IMG 0x93F0
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV2_IMG 0x93F1
#define GL_RASTER_MULTISAMPLE_EXT 0x9327
#define GL_RASTER_SAMPLES_EXT 0x9328
#define GL_MAX_RASTER_SAMPLES_EXT 0x9329
#define GL_RASTER_FIXED_SAMPLE_LOCATIONS_EXT 0x932A
#define GL_MULTISAMPLE_RASTERIZATION_ALLOWED_EXT 0x932B
#define GL_EFFECTIVE_RASTER_SAMPLES_EXT 0x932C
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT 0x8366
#define GL_R16_SNORM_EXT 0x8F98
#define GL_RG16_SNORM_EXT 0x8F99
#define GL_RGBA16_SNORM_EXT 0x8F9B
#define GL_GUILTY_CONTEXT_RESET_EXT 0x8253
#define GL_INNOCENT_CONTEXT_RESET_EXT 0x8254
#define GL_UNKNOWN_CONTEXT_RESET_EXT 0x8255
#define GL_CONTEXT_ROBUST_ACCESS_EXT 0x90F3
#define GL_RESET_NOTIFICATION_STRATEGY_EXT 0x8256
#define GL_LOSE_CONTEXT_ON_RESET_EXT 0x8252
#define GL_NO_RESET_NOTIFICATION_EXT 0x8261
#define GL_SRGB_EXT 0x8C40
#define GL_SRGB_ALPHA_EXT 0x8C42
#define GL_SRGB8_ALPHA8_EXT 0x8C43
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT 0x8210
#define GL_FRAMEBUFFER_SRGB_EXT 0x8DB9
#define GL_LAYOUT_GENERAL_EXT 0x958D
#define GL_LAYOUT_COLOR_ATTACHMENT_EXT 0x958E
#define GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT 0x958F
#define GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT 0x9590
#define GL_LAYOUT_SHADER_READ_ONLY_EXT 0x9591
#define GL_LAYOUT_TRANSFER_SRC_EXT 0x9592
#define GL_LAYOUT_TRANSFER_DST_EXT 0x9593
#define GL_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_EXT 0x9530
#define GL_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_EXT 0x9531
#define GL_HANDLE_TYPE_D3D12_FENCE_EXT 0x9594
#define GL_D3D12_FENCE_VALUE_EXT 0x9595
#define GL_ACTIVE_PROGRAM_EXT 0x8B8D
#define GL_VERTEX_SHADER_BIT_EXT 0x00000001
#define GL_FRAGMENT_SHADER_BIT_EXT 0x00000002
#define GL_ALL_SHADER_BITS_EXT 0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE_EXT 0x8258
#define GL_PROGRAM_PIPELINE_BINDING_EXT 0x825A
#define GL_FRAGMENT_SHADER_DISCARDS_SAMPLES_EXT 0x8A52
#define GL_MAX_SHADER_PIXEL_LOCAL_STORAGE_FAST_SIZE_EXT 0x8F63
#define GL_MAX_SHADER_PIXEL_LOCAL_STORAGE_SIZE_EXT 0x8F67
#define GL_SHADER_PIXEL_LOCAL_STORAGE_EXT 0x8F64
#define GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_FAST_SIZE_EXT 0x9650
#define GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_SIZE_EXT 0x9651
#define GL_FRAMEBUFFER_INCOMPLETE_INSUFFICIENT_SHADER_COMBINED_LOCAL_STORAGE_EXT 0x9652
#define GL_TEXTURE_COMPARE_MODE_EXT 0x884C
#define GL_TEXTURE_COMPARE_FUNC_EXT 0x884D
#define GL_COMPARE_REF_TO_TEXTURE_EXT 0x884E
#define GL_SAMPLER_2D_SHADOW_EXT 0x8B62
#define GL_TEXTURE_SPARSE_EXT 0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_EXT 0x91A7
#define GL_NUM_SPARSE_LEVELS_EXT 0x91AA
#define GL_NUM_VIRTUAL_PAGE_SIZES_EXT 0x91A8
#define GL_VIRTUAL_PAGE_SIZE_X_EXT 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_EXT 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_EXT 0x9197
#define GL_TEXTURE_CUBE_MAP_ARRAY_OES 0x9009
#define GL_MAX_SPARSE_TEXTURE_SIZE_EXT 0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT 0x919A
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT 0x91A9
#define GL_PATCHES_EXT 0x000E
#define GL_PATCH_VERTICES_EXT 0x8E72
#define GL_TESS_CONTROL_OUTPUT_VERTICES_EXT 0x8E75
#define GL_TESS_GEN_MODE_EXT 0x8E76
#define GL_TESS_GEN_SPACING_EXT 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER_EXT 0x8E78
#define GL_TESS_GEN_POINT_MODE_EXT 0x8E79
#define GL_ISOLINES_EXT 0x8E7A
#define GL_QUADS_EXT 0x0007
#define GL_FRACTIONAL_ODD_EXT 0x8E7B
#define GL_FRACTIONAL_EVEN_EXT 0x8E7C
#define GL_MAX_PATCH_VERTICES_EXT 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL_EXT 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS_EXT 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS_EXT 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS_EXT 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS_EXT 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS_EXT 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS_EXT 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS_EXT 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS_EXT 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS_EXT 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS_EXT 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS_EXT 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS_EXT 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS_EXT 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS_EXT 0x8E1F
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS_EXT 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS_EXT 0x92CE
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS_EXT 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS_EXT 0x92D4
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS_EXT 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS_EXT 0x90CC
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS_EXT 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS_EXT 0x90D9
#define GL_IS_PER_PATCH_EXT 0x92E7
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER_EXT 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER_EXT 0x9308
#define GL_TESS_CONTROL_SHADER_EXT 0x8E88
#define GL_TESS_EVALUATION_SHADER_EXT 0x8E87
#define GL_TESS_CONTROL_SHADER_BIT_EXT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT_EXT 0x00000010
#define GL_TEXTURE_BORDER_COLOR_EXT 0x1004
#define GL_CLAMP_TO_BORDER_EXT 0x812D
#define GL_TEXTURE_BUFFER_EXT 0x8C2A
#define GL_TEXTURE_BUFFER_BINDING_EXT 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_EXT 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_EXT 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT 0x8C2D
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT_EXT 0x919F
#define GL_SAMPLER_BUFFER_EXT 0x8DC2
#define GL_INT_SAMPLER_BUFFER_EXT 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT 0x8DD8
#define GL_IMAGE_BUFFER_EXT 0x9051
#define GL_INT_IMAGE_BUFFER_EXT 0x905C
#define GL_UNSIGNED_INT_IMAGE_BUFFER_EXT 0x9067
#define GL_TEXTURE_BUFFER_OFFSET_EXT 0x919D
#define GL_TEXTURE_BUFFER_SIZE_EXT 0x919E
#define GL_TEXTURE_ASTC_DECODE_PRECISION_EXT 0x8F69
#define GL_COMPRESSED_RGBA_BPTC_UNORM_EXT 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_EXT 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_EXT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_EXT 0x8E8F
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#define GL_TEXTURE_CUBE_MAP_ARRAY_EXT 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_EXT 0x900A
#define GL_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_EXT 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900F
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT 0x9054
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x905F
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_REDUCTION_MODE_EXT 0x9366
#define GL_WEIGHTED_AVERAGE_EXT 0x9367
#define GL_TEXTURE_FORMAT_SRGB_OVERRIDE_EXT 0x8FBF
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_R16_EXT 0x822A
#define GL_RG16_EXT 0x822C
#define GL_RGBA16_EXT 0x805B
#define GL_RGB16_EXT 0x8054
#define GL_RGB16_SNORM_EXT 0x8F9A
#define GL_RED_EXT 0x1903
#define GL_RG_EXT 0x8227
#define GL_R8_EXT 0x8229
#define GL_RG8_EXT 0x822B
#define GL_SR8_EXT 0x8FBD
#define GL_SRG8_EXT 0x8FBE
#define GL_TEXTURE_SRGB_DECODE_EXT 0x8A48
#define GL_DECODE_EXT 0x8A49
#define GL_SKIP_DECODE_EXT 0x8A4A
#define GL_TEXTURE_IMMUTABLE_FORMAT_EXT 0x912F
#define GL_ALPHA8_EXT 0x803C
#define GL_LUMINANCE8_EXT 0x8040
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#define GL_RGBA32F_EXT 0x8814
#define GL_RGB32F_EXT 0x8815
#define GL_ALPHA32F_EXT 0x8816
#define GL_LUMINANCE32F_EXT 0x8818
#define GL_LUMINANCE_ALPHA32F_EXT 0x8819
#define GL_ALPHA16F_EXT 0x881C
#define GL_LUMINANCE16F_EXT 0x881E
#define GL_LUMINANCE_ALPHA16F_EXT 0x881F
#define GL_RGB10_A2_EXT 0x8059
#define GL_RGB10_EXT 0x8052
#define GL_R32F_EXT 0x822E
#define GL_RG32F_EXT 0x8230
#define GL_NUM_SURFACE_COMPRESSION_FIXED_RATES_EXT 0x8F6E
#define GL_SURFACE_COMPRESSION_FIXED_RATE_1BPC_EXT 0x96C4
#define GL_SURFACE_COMPRESSION_FIXED_RATE_2BPC_EXT 0x96C5
#define GL_SURFACE_COMPRESSION_FIXED_RATE_3BPC_EXT 0x96C6
#define GL_SURFACE_COMPRESSION_FIXED_RATE_4BPC_EXT 0x96C7
#define GL_SURFACE_COMPRESSION_FIXED_RATE_5BPC_EXT 0x96C8
#define GL_SURFACE_COMPRESSION_FIXED_RATE_6BPC_EXT 0x96C9
#define GL_SURFACE_COMPRESSION_FIXED_RATE_7BPC_EXT 0x96CA
#define GL_SURFACE_COMPRESSION_FIXED_RATE_8BPC_EXT 0x96CB
#define GL_SURFACE_COMPRESSION_FIXED_RATE_9BPC_EXT 0x96CC
#define GL_SURFACE_COMPRESSION_FIXED_RATE_10BPC_EXT 0x96CD
#define GL_SURFACE_COMPRESSION_FIXED_RATE_11BPC_EXT 0x96CE
#define GL_SURFACE_COMPRESSION_FIXED_RATE_12BPC_EXT 0x96CF
#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT 0x8368
#define GL_TEXTURE_VIEW_MIN_LEVEL_EXT 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS_EXT 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER_EXT 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS_EXT 0x82DE
#define GL_UNPACK_ROW_LENGTH_EXT 0x0CF2
#define GL_UNPACK_SKIP_ROWS_EXT 0x0CF3
#define GL_UNPACK_SKIP_PIXELS_EXT 0x0CF4
#define GL_INCLUSIVE_EXT 0x8F10
#define GL_EXCLUSIVE_EXT 0x8F11
#define GL_WINDOW_RECTANGLE_EXT 0x8F12
#define GL_WINDOW_RECTANGLE_MODE_EXT 0x8F13
#define GL_MAX_WINDOW_RECTANGLES_EXT 0x8F14
#define GL_NUM_WINDOW_RECTANGLES_EXT 0x8F15
#define GL_GCCSO_SHADER_BINARY_FJ 0x9260
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_AND_DOWNSAMPLE_IMG 0x913C
#define GL_NUM_DOWNSAMPLE_SCALES_IMG 0x913D
#define GL_DOWNSAMPLE_SCALES_IMG 0x913E
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SCALE_IMG 0x913F
#define GL_RENDERBUFFER_SAMPLES_IMG 0x9133
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_IMG 0x9134
#define GL_MAX_SAMPLES_IMG 0x9135
#define GL_TEXTURE_SAMPLES_IMG 0x9136
#define GL_SGX_PROGRAM_BINARY_IMG 0x9130
#define GL_BGRA_IMG 0x80E1
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_IMG 0x8365
#define GL_SGX_BINARY_IMG 0x8C0A
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8C03
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG 0x9137
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG 0x9138
#define GL_CUBIC_IMG 0x9139
#define GL_CUBIC_MIPMAP_NEAREST_IMG 0x913A
#define GL_CUBIC_MIPMAP_LINEAR_IMG 0x913B
#define GL_BLACKHOLE_RENDER_INTEL 0x83FC
#define GL_CONSERVATIVE_RASTERIZATION_INTEL 0x83FE
#define GL_PERFQUERY_SINGLE_CONTEXT_INTEL 0x00000000
#define GL_PERFQUERY_GLOBAL_CONTEXT_INTEL 0x00000001
#define GL_PERFQUERY_WAIT_INTEL 0x83FB
#define GL_PERFQUERY_FLUSH_INTEL 0x83FA
#define GL_PERFQUERY_DONOT_FLUSH_INTEL 0x83F9
#define GL_PERFQUERY_COUNTER_EVENT_INTEL 0x94F0
#define GL_PERFQUERY_COUNTER_DURATION_NORM_INTEL 0x94F1
#define GL_PERFQUERY_COUNTER_DURATION_RAW_INTEL 0x94F2
#define GL_PERFQUERY_COUNTER_THROUGHPUT_INTEL 0x94F3
#define GL_PERFQUERY_COUNTER_RAW_INTEL 0x94F4
#define GL_PERFQUERY_COUNTER_TIMESTAMP_INTEL 0x94F5
#define GL_PERFQUERY_COUNTER_DATA_UINT32_INTEL 0x94F8
#define GL_PERFQUERY_COUNTER_DATA_UINT64_INTEL 0x94F9
#define GL_PERFQUERY_COUNTER_DATA_FLOAT_INTEL 0x94FA
#define GL_PERFQUERY_COUNTER_DATA_DOUBLE_INTEL 0x94FB
#define GL_PERFQUERY_COUNTER_DATA_BOOL32_INTEL 0x94FC
#define GL_PERFQUERY_QUERY_NAME_LENGTH_MAX_INTEL 0x94FD
#define GL_PERFQUERY_COUNTER_NAME_LENGTH_MAX_INTEL 0x94FE
#define GL_PERFQUERY_COUNTER_DESC_LENGTH_MAX_INTEL 0x94FF
#define GL_PERFQUERY_GPA_EXTENDED_COUNTERS_INTEL 0x9500
#define GL_MULTIPLY_KHR 0x9294
#define GL_SCREEN_KHR 0x9295
#define GL_OVERLAY_KHR 0x9296
#define GL_DARKEN_KHR 0x9297
#define GL_LIGHTEN_KHR 0x9298
#define GL_COLORDODGE_KHR 0x9299
#define GL_COLORBURN_KHR 0x929A
#define GL_HARDLIGHT_KHR 0x929B
#define GL_SOFTLIGHT_KHR 0x929C
#define GL_DIFFERENCE_KHR 0x929E
#define GL_EXCLUSION_KHR 0x92A0
#define GL_HSL_HUE_KHR 0x92AD
#define GL_HSL_SATURATION_KHR 0x92AE
#define GL_HSL_COLOR_KHR 0x92AF
#define GL_HSL_LUMINOSITY_KHR 0x92B0
#define GL_BLEND_ADVANCED_COHERENT_KHR 0x9285
#define GL_CONTEXT_RELEASE_BEHAVIOR 0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH 0x82FC
#define GL_CONTEXT_RELEASE_BEHAVIOR_KHR 0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_KHR 0x82FC
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_KHR 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_KHR 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_KHR 0x8245
#define GL_DEBUG_SOURCE_API_KHR 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_KHR 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_KHR 0x8249
#define GL_DEBUG_SOURCE_APPLICATION_KHR 0x824A
#define GL_DEBUG_SOURCE_OTHER_KHR 0x824B
#define GL_DEBUG_TYPE_ERROR_KHR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY_KHR 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_KHR 0x8250
#define GL_DEBUG_TYPE_OTHER_KHR 0x8251
#define GL_DEBUG_TYPE_MARKER_KHR 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP_KHR 0x8269
#define GL_DEBUG_TYPE_POP_GROUP_KHR 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION_KHR 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH_KHR 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH_KHR 0x826D
#define GL_BUFFER_KHR 0x82E0
#define GL_SHADER_KHR 0x82E1
#define GL_PROGRAM_KHR 0x82E2
#define GL_VERTEX_ARRAY_KHR 0x8074
#define GL_QUERY_KHR 0x82E3
#define GL_PROGRAM_PIPELINE_KHR 0x82E4
#define GL_SAMPLER_KHR 0x82E6
#define GL_MAX_LABEL_LENGTH_KHR 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH_KHR 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_KHR 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_KHR 0x9145
#define GL_DEBUG_SEVERITY_HIGH_KHR 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_KHR 0x9147
#define GL_DEBUG_SEVERITY_LOW_KHR 0x9148
#define GL_DEBUG_OUTPUT_KHR 0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT_KHR 0x00000002
#define GL_STACK_OVERFLOW_KHR 0x0503
#define GL_STACK_UNDERFLOW_KHR 0x0504
#define GL_DISPLAY_LIST 0x82E7
#define GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR 0x00000008
#define GL_MAX_SHADER_COMPILER_THREADS_KHR 0x91B0
#define GL_COMPLETION_STATUS_KHR 0x91B1
#define GL_CONTEXT_ROBUST_ACCESS 0x90F3
#define GL_CONTEXT_ROBUST_ACCESS_KHR 0x90F3
#define GL_LOSE_CONTEXT_ON_RESET_KHR 0x8252
#define GL_GUILTY_CONTEXT_RESET_KHR 0x8253
#define GL_INNOCENT_CONTEXT_RESET_KHR 0x8254
#define GL_UNKNOWN_CONTEXT_RESET_KHR 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_KHR 0x8256
#define GL_NO_RESET_NOTIFICATION_KHR 0x8261
#define GL_CONTEXT_LOST_KHR 0x0507
#define GL_SUBGROUP_SIZE_KHR 0x9532
#define GL_SUBGROUP_SUPPORTED_STAGES_KHR 0x9533
#define GL_SUBGROUP_SUPPORTED_FEATURES_KHR 0x9534
#define GL_SUBGROUP_QUAD_ALL_STAGES_KHR 0x9535
#define GL_SUBGROUP_FEATURE_BASIC_BIT_KHR 0x00000001
#define GL_SUBGROUP_FEATURE_VOTE_BIT_KHR 0x00000002
#define GL_SUBGROUP_FEATURE_ARITHMETIC_BIT_KHR 0x00000004
#define GL_SUBGROUP_FEATURE_BALLOT_BIT_KHR 0x00000008
#define GL_SUBGROUP_FEATURE_SHUFFLE_BIT_KHR 0x00000010
#define GL_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT_KHR 0x00000020
#define GL_SUBGROUP_FEATURE_CLUSTERED_BIT_KHR 0x00000040
#define GL_SUBGROUP_FEATURE_QUAD_BIT_KHR 0x00000080
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD
#define GL_BGR_EXT 0x80E0
#define GL_FRAMEBUFFER_FLIP_X_MESA 0x8BBC
#define GL_FRAMEBUFFER_FLIP_Y_MESA 0x8BBB
#define GL_FRAMEBUFFER_SWAP_XY_MESA 0x8BBD
#define GL_PROGRAM_BINARY_FORMAT_MESA 0x875F
#define GL_BLEND_OVERLAP_NV 0x9281
#define GL_BLEND_PREMULTIPLIED_SRC_NV 0x9280
#define GL_BLUE_NV 0x1905
#define GL_COLORBURN_NV 0x929A
#define GL_COLORDODGE_NV 0x9299
#define GL_CONJOINT_NV 0x9284
#define GL_CONTRAST_NV 0x92A1
#define GL_DARKEN_NV 0x9297
#define GL_DIFFERENCE_NV 0x929E
#define GL_DISJOINT_NV 0x9283
#define GL_DST_ATOP_NV 0x928F
#define GL_DST_IN_NV 0x928B
#define GL_DST_NV 0x9287
#define GL_DST_OUT_NV 0x928D
#define GL_DST_OVER_NV 0x9289
#define GL_EXCLUSION_NV 0x92A0
#define GL_GREEN_NV 0x1904
#define GL_HARDLIGHT_NV 0x929B
#define GL_HARDMIX_NV 0x92A9
#define GL_HSL_COLOR_NV 0x92AF
#define GL_HSL_HUE_NV 0x92AD
#define GL_HSL_LUMINOSITY_NV 0x92B0
#define GL_HSL_SATURATION_NV 0x92AE
#define GL_INVERT_OVG_NV 0x92B4
#define GL_INVERT_RGB_NV 0x92A3
#define GL_LIGHTEN_NV 0x9298
#define GL_LINEARBURN_NV 0x92A5
#define GL_LINEARDODGE_NV 0x92A4
#define GL_LINEARLIGHT_NV 0x92A7
#define GL_MINUS_CLAMPED_NV 0x92B3
#define GL_MINUS_NV 0x929F
#define GL_MULTIPLY_NV 0x9294
#define GL_OVERLAY_NV 0x9296
#define GL_PINLIGHT_NV 0x92A8
#define GL_PLUS_CLAMPED_ALPHA_NV 0x92B2
#define GL_PLUS_CLAMPED_NV 0x92B1
#define GL_PLUS_DARKER_NV 0x9292
#define GL_PLUS_NV 0x9291
#define GL_RED_NV 0x1903
#define GL_SCREEN_NV 0x9295
#define GL_SOFTLIGHT_NV 0x929C
#define GL_SRC_ATOP_NV 0x928E
#define GL_SRC_IN_NV 0x928A
#define GL_SRC_NV 0x9286
#define GL_SRC_OUT_NV 0x928C
#define GL_SRC_OVER_NV 0x9288
#define GL_UNCORRELATED_NV 0x9282
#define GL_VIVIDLIGHT_NV 0x92A6
#define GL_XOR_NV 0x1506
#define GL_BLEND_ADVANCED_COHERENT_NV 0x9285
#define GL_FACTOR_MIN_AMD 0x901C
#define GL_FACTOR_MAX_AMD 0x901D
#define GL_VIEWPORT_POSITION_W_SCALE_NV 0x937C
#define GL_VIEWPORT_POSITION_W_SCALE_X_COEFF_NV 0x937D
#define GL_VIEWPORT_POSITION_W_SCALE_Y_COEFF_NV 0x937E
#define GL_QUERY_WAIT_NV 0x8E13
#define GL_QUERY_NO_WAIT_NV 0x8E14
#define GL_QUERY_BY_REGION_WAIT_NV 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT_NV 0x8E16
#define GL_CONSERVATIVE_RASTERIZATION_NV 0x9346
#define GL_SUBPIXEL_PRECISION_BIAS_X_BITS_NV 0x9347
#define GL_SUBPIXEL_PRECISION_BIAS_Y_BITS_NV 0x9348
#define GL_MAX_SUBPIXEL_PRECISION_BIAS_BITS_NV 0x9349
#define GL_CONSERVATIVE_RASTER_MODE_PRE_SNAP_NV 0x9550
#define GL_CONSERVATIVE_RASTER_MODE_NV 0x954D
#define GL_CONSERVATIVE_RASTER_MODE_POST_SNAP_NV 0x954E
#define GL_CONSERVATIVE_RASTER_MODE_PRE_SNAP_TRIANGLES_NV 0x954F
#define GL_COPY_READ_BUFFER_NV 0x8F36
#define GL_COPY_WRITE_BUFFER_NV 0x8F37
#define GL_COVERAGE_COMPONENT_NV 0x8ED0
#define GL_COVERAGE_COMPONENT4_NV 0x8ED1
#define GL_COVERAGE_ATTACHMENT_NV 0x8ED2
#define GL_COVERAGE_BUFFERS_NV 0x8ED3
#define GL_COVERAGE_SAMPLES_NV 0x8ED4
#define GL_COVERAGE_ALL_FRAGMENTS_NV 0x8ED5
#define GL_COVERAGE_EDGE_FRAGMENTS_NV 0x8ED6
#define GL_COVERAGE_AUTOMATIC_NV 0x8ED7
#define GL_COVERAGE_BUFFER_BIT_NV 0x00008000
#define GL_DEPTH_COMPONENT16_NONLINEAR_NV 0x8E2C
#define GL_MAX_DRAW_BUFFERS_NV 0x8824
#define GL_DRAW_BUFFER0_NV 0x8825
#define GL_DRAW_BUFFER1_NV 0x8826
#define GL_DRAW_BUFFER2_NV 0x8827
#define GL_DRAW_BUFFER3_NV 0x8828
#define GL_DRAW_BUFFER4_NV 0x8829
#define GL_DRAW_BUFFER5_NV 0x882A
#define GL_DRAW_BUFFER6_NV 0x882B
#define GL_DRAW_BUFFER7_NV 0x882C
#define GL_DRAW_BUFFER8_NV 0x882D
#define GL_DRAW_BUFFER9_NV 0x882E
#define GL_DRAW_BUFFER10_NV 0x882F
#define GL_DRAW_BUFFER11_NV 0x8830
#define GL_DRAW_BUFFER12_NV 0x8831
#define GL_DRAW_BUFFER13_NV 0x8832
#define GL_DRAW_BUFFER14_NV 0x8833
#define GL_DRAW_BUFFER15_NV 0x8834
#define GL_COLOR_ATTACHMENT0_NV 0x8CE0
#define GL_COLOR_ATTACHMENT1_NV 0x8CE1
#define GL_COLOR_ATTACHMENT2_NV 0x8CE2
#define GL_COLOR_ATTACHMENT3_NV 0x8CE3
#define GL_COLOR_ATTACHMENT4_NV 0x8CE4
#define GL_COLOR_ATTACHMENT5_NV 0x8CE5
#define GL_COLOR_ATTACHMENT6_NV 0x8CE6
#define GL_COLOR_ATTACHMENT7_NV 0x8CE7
#define GL_COLOR_ATTACHMENT8_NV 0x8CE8
#define GL_COLOR_ATTACHMENT9_NV 0x8CE9
#define GL_COLOR_ATTACHMENT10_NV 0x8CEA
#define GL_COLOR_ATTACHMENT11_NV 0x8CEB
#define GL_COLOR_ATTACHMENT12_NV 0x8CEC
#define GL_COLOR_ATTACHMENT13_NV 0x8CED
#define GL_COLOR_ATTACHMENT14_NV 0x8CEE
#define GL_COLOR_ATTACHMENT15_NV 0x8CEF
#define GL_MAX_COLOR_ATTACHMENTS_NV 0x8CDF
#define GL_ALL_COMPLETED_NV 0x84F2
#define GL_FENCE_STATUS_NV 0x84F3
#define GL_FENCE_CONDITION_NV 0x84F4
#define GL_FILL_RECTANGLE_NV 0x933C
#define GL_FRAGMENT_COVERAGE_TO_COLOR_NV 0x92DD
#define GL_FRAGMENT_COVERAGE_COLOR_NV 0x92DE
#define GL_READ_FRAMEBUFFER_NV 0x8CA8
#define GL_DRAW_FRAMEBUFFER_NV 0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING_NV 0x8CA6
#define GL_READ_FRAMEBUFFER_BINDING_NV 0x8CAA
#define GL_COVERAGE_MODULATION_TABLE_NV 0x9331
#define GL_COLOR_SAMPLES_NV 0x8E20
#define GL_DEPTH_SAMPLES_NV 0x932D
#define GL_STENCIL_SAMPLES_NV 0x932E
#define GL_MIXED_DEPTH_SAMPLES_SUPPORTED_NV 0x932F
#define GL_MIXED_STENCIL_SAMPLES_SUPPORTED_NV 0x9330
#define GL_COVERAGE_MODULATION_NV 0x9332
#define GL_COVERAGE_MODULATION_TABLE_SIZE_NV 0x9333
#define GL_RENDERBUFFER_SAMPLES_NV 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_NV 0x8D56
#define GL_MAX_SAMPLES_NV 0x8D57
#define GL_INT64_NV 0x140E
#define GL_UNSIGNED_INT64_NV 0x140F
#define GL_INT8_NV 0x8FE0
#define GL_INT8_VEC2_NV 0x8FE1
#define GL_INT8_VEC3_NV 0x8FE2
#define GL_INT8_VEC4_NV 0x8FE3
#define GL_INT16_NV 0x8FE4
#define GL_INT16_VEC2_NV 0x8FE5
#define GL_INT16_VEC3_NV 0x8FE6
#define GL_INT16_VEC4_NV 0x8FE7
#define GL_INT64_VEC2_NV 0x8FE9
#define GL_INT64_VEC3_NV 0x8FEA
#define GL_INT64_VEC4_NV 0x8FEB
#define GL_UNSIGNED_INT8_NV 0x8FEC
#define GL_UNSIGNED_INT8_VEC2_NV 0x8FED
#define GL_UNSIGNED_INT8_VEC3_NV 0x8FEE
#define GL_UNSIGNED_INT8_VEC4_NV 0x8FEF
#define GL_UNSIGNED_INT16_NV 0x8FF0
#define GL_UNSIGNED_INT16_VEC2_NV 0x8FF1
#define GL_UNSIGNED_INT16_VEC3_NV 0x8FF2
#define GL_UNSIGNED_INT16_VEC4_NV 0x8FF3
#define GL_UNSIGNED_INT64_VEC2_NV 0x8FF5
#define GL_UNSIGNED_INT64_VEC3_NV 0x8FF6
#define GL_UNSIGNED_INT64_VEC4_NV 0x8FF7
#define GL_FLOAT16_NV 0x8FF8
#define GL_FLOAT16_VEC2_NV 0x8FF9
#define GL_FLOAT16_VEC3_NV 0x8FFA
#define GL_FLOAT16_VEC4_NV 0x8FFB
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_NV 0x88FE
#define GL_MULTISAMPLES_NV 0x9371
#define GL_SUPERSAMPLE_SCALE_X_NV 0x9372
#define GL_SUPERSAMPLE_SCALE_Y_NV 0x9373
#define GL_CONFORMANT_NV 0x9374
#define GL_ATTACHED_MEMORY_OBJECT_NV 0x95A4
#define GL_ATTACHED_MEMORY_OFFSET_NV 0x95A5
#define GL_MEMORY_ATTACHABLE_ALIGNMENT_NV 0x95A6
#define GL_MEMORY_ATTACHABLE_SIZE_NV 0x95A7
#define GL_MEMORY_ATTACHABLE_NV 0x95A8
#define GL_DETACHED_MEMORY_INCARNATION_NV 0x95A9
#define GL_DETACHED_TEXTURES_NV 0x95AA
#define GL_DETACHED_BUFFERS_NV 0x95AB
#define GL_MAX_DETACHED_TEXTURES_NV 0x95AC
#define GL_MAX_DETACHED_BUFFERS_NV 0x95AD
#define GL_MESH_SHADER_NV 0x9559
#define GL_TASK_SHADER_NV 0x955A
#define GL_MAX_MESH_UNIFORM_BLOCKS_NV 0x8E60
#define GL_MAX_MESH_TEXTURE_IMAGE_UNITS_NV 0x8E61
#define GL_MAX_MESH_IMAGE_UNIFORMS_NV 0x8E62
#define GL_MAX_MESH_UNIFORM_COMPONENTS_NV 0x8E63
#define GL_MAX_MESH_ATOMIC_COUNTER_BUFFERS_NV 0x8E64
#define GL_MAX_MESH_ATOMIC_COUNTERS_NV 0x8E65
#define GL_MAX_MESH_SHADER_STORAGE_BLOCKS_NV 0x8E66
#define GL_MAX_COMBINED_MESH_UNIFORM_COMPONENTS_NV 0x8E67
#define GL_MAX_TASK_UNIFORM_BLOCKS_NV 0x8E68
#define GL_MAX_TASK_TEXTURE_IMAGE_UNITS_NV 0x8E69
#define GL_MAX_TASK_IMAGE_UNIFORMS_NV 0x8E6A
#define GL_MAX_TASK_UNIFORM_COMPONENTS_NV 0x8E6B
#define GL_MAX_TASK_ATOMIC_COUNTER_BUFFERS_NV 0x8E6C
#define GL_MAX_TASK_ATOMIC_COUNTERS_NV 0x8E6D
#define GL_MAX_TASK_SHADER_STORAGE_BLOCKS_NV 0x8E6E
#define GL_MAX_COMBINED_TASK_UNIFORM_COMPONENTS_NV 0x8E6F
#define GL_MAX_MESH_WORK_GROUP_INVOCATIONS_NV 0x95A2
#define GL_MAX_TASK_WORK_GROUP_INVOCATIONS_NV 0x95A3
#define GL_MAX_MESH_TOTAL_MEMORY_SIZE_NV 0x9536
#define GL_MAX_TASK_TOTAL_MEMORY_SIZE_NV 0x9537
#define GL_MAX_MESH_OUTPUT_VERTICES_NV 0x9538
#define GL_MAX_MESH_OUTPUT_PRIMITIVES_NV 0x9539
#define GL_MAX_TASK_OUTPUT_COUNT_NV 0x953A
#define GL_MAX_DRAW_MESH_TASKS_COUNT_NV 0x953D
#define GL_MAX_MESH_VIEWS_NV 0x9557
#define GL_MESH_OUTPUT_PER_VERTEX_GRANULARITY_NV 0x92DF
#define GL_MESH_OUTPUT_PER_PRIMITIVE_GRANULARITY_NV 0x9543
#define GL_MAX_MESH_WORK_GROUP_SIZE_NV 0x953B
#define GL_MAX_TASK_WORK_GROUP_SIZE_NV 0x953C
#define GL_MESH_WORK_GROUP_SIZE_NV 0x953E
#define GL_TASK_WORK_GROUP_SIZE_NV 0x953F
#define GL_MESH_VERTICES_OUT_NV 0x9579
#define GL_MESH_PRIMITIVES_OUT_NV 0x957A
#define GL_MESH_OUTPUT_TYPE_NV 0x957B
#define GL_UNIFORM_BLOCK_REFERENCED_BY_MESH_SHADER_NV 0x959C
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TASK_SHADER_NV 0x959D
#define GL_REFERENCED_BY_MESH_SHADER_NV 0x95A0
#define GL_REFERENCED_BY_TASK_SHADER_NV 0x95A1
#define GL_MESH_SHADER_BIT_NV 0x00000040
#define GL_TASK_SHADER_BIT_NV 0x00000080
#define GL_MESH_SUBROUTINE_NV 0x957C
#define GL_TASK_SUBROUTINE_NV 0x957D
#define GL_MESH_SUBROUTINE_UNIFORM_NV 0x957E
#define GL_TASK_SUBROUTINE_UNIFORM_NV 0x957F
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_MESH_SHADER_NV 0x959E
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TASK_SHADER_NV 0x959F
#define GL_FLOAT_MAT2x3_NV 0x8B65
#define GL_FLOAT_MAT2x4_NV 0x8B66
#define GL_FLOAT_MAT3x2_NV 0x8B67
#define GL_FLOAT_MAT3x4_NV 0x8B68
#define GL_FLOAT_MAT4x2_NV 0x8B69
#define GL_FLOAT_MAT4x3_NV 0x8B6A
#define GL_PATH_FORMAT_SVG_NV 0x9070
#define GL_PATH_FORMAT_PS_NV 0x9071
#define GL_STANDARD_FONT_NAME_NV 0x9072
#define GL_SYSTEM_FONT_NAME_NV 0x9073
#define GL_FILE_NAME_NV 0x9074
#define GL_PATH_STROKE_WIDTH_NV 0x9075
#define GL_PATH_END_CAPS_NV 0x9076
#define GL_PATH_INITIAL_END_CAP_NV 0x9077
#define GL_PATH_TERMINAL_END_CAP_NV 0x9078
#define GL_PATH_JOIN_STYLE_NV 0x9079
#define GL_PATH_MITER_LIMIT_NV 0x907A
#define GL_PATH_DASH_CAPS_NV 0x907B
#define GL_PATH_INITIAL_DASH_CAP_NV 0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV 0x907D
#define GL_PATH_DASH_OFFSET_NV 0x907E
#define GL_PATH_CLIENT_LENGTH_NV 0x907F
#define GL_PATH_FILL_MODE_NV 0x9080
#define GL_PATH_FILL_MASK_NV 0x9081
#define GL_PATH_FILL_COVER_MODE_NV 0x9082
#define GL_PATH_STROKE_COVER_MODE_NV 0x9083
#define GL_PATH_STROKE_MASK_NV 0x9084
#define GL_COUNT_UP_NV 0x9088
#define GL_COUNT_DOWN_NV 0x9089
#define GL_PATH_OBJECT_BOUNDING_BOX_NV 0x908A
#define GL_CONVEX_HULL_NV 0x908B
#define GL_BOUNDING_BOX_NV 0x908D
#define GL_TRANSLATE_X_NV 0x908E
#define GL_TRANSLATE_Y_NV 0x908F
#define GL_TRANSLATE_2D_NV 0x9090
#define GL_TRANSLATE_3D_NV 0x9091
#define GL_AFFINE_2D_NV 0x9092
#define GL_AFFINE_3D_NV 0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV 0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV 0x9098
#define GL_UTF8_NV 0x909A
#define GL_UTF16_NV 0x909B
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV 0x909C
#define GL_PATH_COMMAND_COUNT_NV 0x909D
#define GL_PATH_COORD_COUNT_NV 0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV 0x909F
#define GL_PATH_COMPUTED_LENGTH_NV 0x90A0
#define GL_PATH_FILL_BOUNDING_BOX_NV 0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV 0x90A2
#define GL_SQUARE_NV 0x90A3
#define GL_ROUND_NV 0x90A4
#define GL_TRIANGULAR_NV 0x90A5
#define GL_BEVEL_NV 0x90A6
#define GL_MITER_REVERT_NV 0x90A7
#define GL_MITER_TRUNCATE_NV 0x90A8
#define GL_SKIP_MISSING_GLYPH_NV 0x90A9
#define GL_USE_MISSING_GLYPH_NV 0x90AA
#define GL_PATH_ERROR_POSITION_NV 0x90AB
#define GL_ACCUM_ADJACENT_PAIRS_NV 0x90AD
#define GL_ADJACENT_PAIRS_NV 0x90AE
#define GL_FIRST_TO_REST_NV 0x90AF
#define GL_PATH_GEN_MODE_NV 0x90B0
#define GL_PATH_GEN_COEFF_NV 0x90B1
#define GL_PATH_GEN_COMPONENTS_NV 0x90B3
#define GL_PATH_STENCIL_FUNC_NV 0x90B7
#define GL_PATH_STENCIL_REF_NV 0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV 0x90B9
#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV 0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV 0x90BE
#define GL_PATH_COVER_DEPTH_FUNC_NV 0x90BF
#define GL_PATH_DASH_OFFSET_RESET_NV 0x90B4
#define GL_MOVE_TO_RESETS_NV 0x90B5
#define GL_MOVE_TO_CONTINUES_NV 0x90B6
#define GL_CLOSE_PATH_NV 0x00
#define GL_MOVE_TO_NV 0x02
#define GL_RELATIVE_MOVE_TO_NV 0x03
#define GL_LINE_TO_NV 0x04
#define GL_RELATIVE_LINE_TO_NV 0x05
#define GL_HORIZONTAL_LINE_TO_NV 0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV 0x07
#define GL_VERTICAL_LINE_TO_NV 0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV 0x09
#define GL_QUADRATIC_CURVE_TO_NV 0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV 0x0B
#define GL_CUBIC_CURVE_TO_NV 0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV 0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0F
#define GL_SMOOTH_CUBIC_CURVE_TO_NV 0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV 0x11
#define GL_SMALL_CCW_ARC_TO_NV 0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV 0x13
#define GL_SMALL_CW_ARC_TO_NV 0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV 0x15
#define GL_LARGE_CCW_ARC_TO_NV 0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV 0x17
#define GL_LARGE_CW_ARC_TO_NV 0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV 0x19
#define GL_RESTART_PATH_NV 0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV 0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV 0xF4
#define GL_RECT_NV 0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV 0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV 0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV 0xFC
#define GL_ARC_TO_NV 0xFE
#define GL_RELATIVE_ARC_TO_NV 0xFF
#define GL_BOLD_BIT_NV 0x01
#define GL_ITALIC_BIT_NV 0x02
#define GL_GLYPH_WIDTH_BIT_NV 0x01
#define GL_GLYPH_HEIGHT_BIT_NV 0x02
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV 0x04
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV 0x08
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV 0x10
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV 0x80
#define GL_GLYPH_HAS_KERNING_BIT_NV 0x100
#define GL_FONT_X_MIN_BOUNDS_BIT_NV 0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV 0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV 0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV 0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV 0x00100000
#define GL_FONT_ASCENDER_BIT_NV 0x00200000
#define GL_FONT_DESCENDER_BIT_NV 0x00400000
#define GL_FONT_HEIGHT_BIT_NV 0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV 0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV 0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV 0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV 0x10000000
#define GL_ROUNDED_RECT_NV 0xE8
#define GL_RELATIVE_ROUNDED_RECT_NV 0xE9
#define GL_ROUNDED_RECT2_NV 0xEA
#define GL_RELATIVE_ROUNDED_RECT2_NV 0xEB
#define GL_ROUNDED_RECT4_NV 0xEC
#define GL_RELATIVE_ROUNDED_RECT4_NV 0xED
#define GL_ROUNDED_RECT8_NV 0xEE
#define GL_RELATIVE_ROUNDED_RECT8_NV 0xEF
#define GL_RELATIVE_RECT_NV 0xF7
#define GL_FONT_GLYPHS_AVAILABLE_NV 0x9368
#define GL_FONT_TARGET_UNAVAILABLE_NV 0x9369
#define GL_FONT_UNAVAILABLE_NV 0x936A
#define GL_FONT_UNINTELLIGIBLE_NV 0x936B
#define GL_CONIC_CURVE_TO_NV 0x1A
#define GL_RELATIVE_CONIC_CURVE_TO_NV 0x1B
#define GL_FONT_NUM_GLYPH_INDICES_BIT_NV 0x20000000
#define GL_STANDARD_FONT_FORMAT_NV 0x936C
#define GL_2_BYTES_NV 0x1407
#define GL_3_BYTES_NV 0x1408
#define GL_4_BYTES_NV 0x1409
#define GL_EYE_LINEAR_NV 0x2400
#define GL_OBJECT_LINEAR_NV 0x2401
#define GL_CONSTANT_NV 0x8576
#define GL_PATH_FOG_GEN_MODE_NV 0x90AC
#define GL_PRIMARY_COLOR 0x8577
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D
#define GL_PATH_GEN_COLOR_FORMAT_NV 0x90B2
#define GL_PATH_PROJECTION_NV 0x1701
#define GL_PATH_MODELVIEW_NV 0x1700
#define GL_PATH_MODELVIEW_STACK_DEPTH_NV 0x0BA3
#define GL_PATH_MODELVIEW_MATRIX_NV 0x0BA6
#define GL_PATH_MAX_MODELVIEW_STACK_DEPTH_NV 0x0D36
#define GL_PATH_TRANSPOSE_MODELVIEW_MATRIX_NV 0x84E3
#define GL_PATH_PROJECTION_STACK_DEPTH_NV 0x0BA4
#define GL_PATH_PROJECTION_MATRIX_NV 0x0BA7
#define GL_PATH_MAX_PROJECTION_STACK_DEPTH_NV 0x0D38
#define GL_PATH_TRANSPOSE_PROJECTION_MATRIX_NV 0x84E4
#define GL_FRAGMENT_INPUT_NV 0x936D
#define GL_SHARED_EDGE_NV 0xC0
#define GL_PIXEL_PACK_BUFFER_NV 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_NV 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_NV 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_NV 0x88EF
#define GL_POLYGON_MODE_NV 0x0B40
#define GL_POLYGON_OFFSET_POINT_NV 0x2A01
#define GL_POLYGON_OFFSET_LINE_NV 0x2A02
#define GL_POINT_NV 0x1B00
#define GL_LINE_NV 0x1B01
#define GL_FILL_NV 0x1B02
#define GL_SHADING_RATE_IMAGE_PER_PRIMITIVE_NV 0x95B1
#define GL_SHADING_RATE_IMAGE_PALETTE_COUNT_NV 0x95B2
#define GL_READ_BUFFER_NV 0x0C02
#define GL_REPRESENTATIVE_FRAGMENT_TEST_NV 0x937F
#define GL_SLUMINANCE_NV 0x8C46
#define GL_SLUMINANCE_ALPHA_NV 0x8C44
#define GL_SRGB8_NV 0x8C41
#define GL_SLUMINANCE8_NV 0x8C47
#define GL_SLUMINANCE8_ALPHA8_NV 0x8C45
#define GL_COMPRESSED_SRGB_S3TC_DXT1_NV 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_NV 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_NV 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_NV 0x8C4F
#define GL_ETC1_SRGB8_NV 0x88EE
#define GL_SAMPLE_LOCATION_SUBPIXEL_BITS_NV 0x933D
#define GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_NV 0x933E
#define GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_NV 0x933F
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_NV 0x9340
#define GL_SAMPLE_LOCATION_NV 0x8E50
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_NV 0x9341
#define GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_NV 0x9342
#define GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_NV 0x9343
#define GL_SCISSOR_TEST_EXCLUSIVE_NV 0x9555
#define GL_SCISSOR_BOX_EXCLUSIVE_NV 0x9556
#define GL_SUBGROUP_FEATURE_PARTITIONED_BIT_NV 0x00000100
#define GL_SHADING_RATE_IMAGE_NV 0x9563
#define GL_SHADING_RATE_NO_INVOCATIONS_NV 0x9564
#define GL_SHADING_RATE_1_INVOCATION_PER_PIXEL_NV 0x9565
#define GL_SHADING_RATE_1_INVOCATION_PER_1X2_PIXELS_NV 0x9566
#define GL_SHADING_RATE_1_INVOCATION_PER_2X1_PIXELS_NV 0x9567
#define GL_SHADING_RATE_1_INVOCATION_PER_2X2_PIXELS_NV 0x9568
#define GL_SHADING_RATE_1_INVOCATION_PER_2X4_PIXELS_NV 0x9569
#define GL_SHADING_RATE_1_INVOCATION_PER_4X2_PIXELS_NV 0x956A
#define GL_SHADING_RATE_1_INVOCATION_PER_4X4_PIXELS_NV 0x956B
#define GL_SHADING_RATE_2_INVOCATIONS_PER_PIXEL_NV 0x956C
#define GL_SHADING_RATE_4_INVOCATIONS_PER_PIXEL_NV 0x956D
#define GL_SHADING_RATE_8_INVOCATIONS_PER_PIXEL_NV 0x956E
#define GL_SHADING_RATE_16_INVOCATIONS_PER_PIXEL_NV 0x956F
#define GL_SHADING_RATE_IMAGE_BINDING_NV 0x955B
#define GL_SHADING_RATE_IMAGE_TEXEL_WIDTH_NV 0x955C
#define GL_SHADING_RATE_IMAGE_TEXEL_HEIGHT_NV 0x955D
#define GL_SHADING_RATE_IMAGE_PALETTE_SIZE_NV 0x955E
#define GL_MAX_COARSE_FRAGMENT_SAMPLES_NV 0x955F
#define GL_SHADING_RATE_SAMPLE_ORDER_DEFAULT_NV 0x95AE
#define GL_SHADING_RATE_SAMPLE_ORDER_PIXEL_MAJOR_NV 0x95AF
#define GL_SHADING_RATE_SAMPLE_ORDER_SAMPLE_MAJOR_NV 0x95B0
#define GL_SAMPLER_2D_ARRAY_SHADOW_NV 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW_NV 0x8DC5
#define GL_TEXTURE_BORDER_COLOR_NV 0x1004
#define GL_CLAMP_TO_BORDER_NV 0x812D
#define GL_TIMELINE_SEMAPHORE_VALUE_NV 0x9595
#define GL_SEMAPHORE_TYPE_NV 0x95B3
#define GL_SEMAPHORE_TYPE_BINARY_NV 0x95B4
#define GL_SEMAPHORE_TYPE_TIMELINE_NV 0x95B5
#define GL_MAX_TIMELINE_SEMAPHORE_VALUE_DIFFERENCE_NV 0x95B6
#define GL_MAX_VIEWPORTS_NV 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS_NV 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE_NV 0x825D
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX_NV 0x825F
#define GL_VIEWPORT_SWIZZLE_POSITIVE_X_NV 0x9350
#define GL_VIEWPORT_SWIZZLE_NEGATIVE_X_NV 0x9351
#define GL_VIEWPORT_SWIZZLE_POSITIVE_Y_NV 0x9352
#define GL_VIEWPORT_SWIZZLE_NEGATIVE_Y_NV 0x9353
#define GL_VIEWPORT_SWIZZLE_POSITIVE_Z_NV 0x9354
#define GL_VIEWPORT_SWIZZLE_NEGATIVE_Z_NV 0x9355
#define GL_VIEWPORT_SWIZZLE_POSITIVE_W_NV 0x9356
#define GL_VIEWPORT_SWIZZLE_NEGATIVE_W_NV 0x9357
#define GL_VIEWPORT_SWIZZLE_X_NV 0x9358
#define GL_VIEWPORT_SWIZZLE_Y_NV 0x9359
#define GL_VIEWPORT_SWIZZLE_Z_NV 0x935A
#define GL_VIEWPORT_SWIZZLE_W_NV 0x935B
#define GL_SAMPLER_EXTERNAL_OES 0x8D66
#define GL_ETC1_RGB8_OES 0x8D64
#define GL_PALETTE4_RGB8_OES 0x8B90
#define GL_PALETTE4_RGBA8_OES 0x8B91
#define GL_PALETTE4_R5_G6_B5_OES 0x8B92
#define GL_PALETTE4_RGBA4_OES 0x8B93
#define GL_PALETTE4_RGB5_A1_OES 0x8B94
#define GL_PALETTE8_RGB8_OES 0x8B95
#define GL_PALETTE8_RGBA8_OES 0x8B96
#define GL_PALETTE8_R5_G6_B5_OES 0x8B97
#define GL_PALETTE8_RGBA4_OES 0x8B98
#define GL_PALETTE8_RGB5_A1_OES 0x8B99
#define GL_DEPTH_COMPONENT24_OES 0x81A6
#define GL_GEOMETRY_SHADER_OES 0x8DD9
#define GL_GEOMETRY_SHADER_BIT_OES 0x00000004
#define GL_GEOMETRY_LINKED_VERTICES_OUT_OES 0x8916
#define GL_GEOMETRY_LINKED_INPUT_TYPE_OES 0x8917
#define GL_GEOMETRY_LINKED_OUTPUT_TYPE_OES 0x8918
#define GL_GEOMETRY_SHADER_INVOCATIONS_OES 0x887F
#define GL_LAYER_PROVOKING_VERTEX_OES 0x825E
#define GL_LINES_ADJACENCY_OES 0x000A
#define GL_LINE_STRIP_ADJACENCY_OES 0x000B
#define GL_TRIANGLES_ADJACENCY_OES 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY_OES 0x000D
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_OES 0x8DDF
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS_OES 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_OES 0x8A32
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS_OES 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_OES 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_OES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_OES 0x8DE1
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS_OES 0x8E5A
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_OES 0x8C29
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_OES 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS_OES 0x92D5
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS_OES 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_OES 0x90D7
#define GL_FIRST_VERTEX_CONVENTION_OES 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_OES 0x8E4E
#define GL_UNDEFINED_VERTEX_OES 0x8260
#define GL_PRIMITIVES_GENERATED_OES 0x8C87
#define GL_FRAMEBUFFER_DEFAULT_LAYERS_OES 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS_OES 0x9317
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_OES 0x8DA8
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_OES 0x8DA7
#define GL_REFERENCED_BY_GEOMETRY_SHADER_OES 0x9309
#define GL_PROGRAM_BINARY_LENGTH_OES 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS_OES 0x87FE
#define GL_PROGRAM_BINARY_FORMATS_OES 0x87FF
#define GL_WRITE_ONLY_OES 0x88B9
#define GL_BUFFER_ACCESS_OES 0x88BB
#define GL_BUFFER_MAPPED_OES 0x88BC
#define GL_BUFFER_MAP_POINTER_OES 0x88BD
#define GL_PRIMITIVE_BOUNDING_BOX_OES 0x92BE
#define GL_ALPHA8_OES 0x803C
#define GL_DEPTH_COMPONENT16_OES 0x81A5
#define GL_LUMINANCE4_ALPHA4_OES 0x8043
#define GL_LUMINANCE8_ALPHA8_OES 0x8045
#define GL_LUMINANCE8_OES 0x8040
#define GL_RGBA4_OES 0x8056
#define GL_RGB5_A1_OES 0x8057
#define GL_RGB565_OES 0x8D62
#define GL_RGB8_OES 0x8051
#define GL_RGBA8_OES 0x8058
#define GL_SAMPLE_SHADING_OES 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_OES 0x8C37
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_OES 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_OES 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS_OES 0x8E5D
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES 0x8B8B
#define GL_STENCIL_INDEX1_OES 0x8D46
#define GL_STENCIL_INDEX4_OES 0x8D47
#define GL_FRAMEBUFFER_UNDEFINED_OES 0x8219
#define GL_PATCHES_OES 0x000E
#define GL_PATCH_VERTICES_OES 0x8E72
#define GL_TESS_CONTROL_OUTPUT_VERTICES_OES 0x8E75
#define GL_TESS_GEN_MODE_OES 0x8E76
#define GL_TESS_GEN_SPACING_OES 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER_OES 0x8E78
#define GL_TESS_GEN_POINT_MODE_OES 0x8E79
#define GL_ISOLINES_OES 0x8E7A
#define GL_QUADS_OES 0x0007
#define GL_FRACTIONAL_ODD_OES 0x8E7B
#define GL_FRACTIONAL_EVEN_OES 0x8E7C
#define GL_MAX_PATCH_VERTICES_OES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL_OES 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS_OES 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS_OES 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS_OES 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS_OES 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS_OES 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS_OES 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS_OES 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS_OES 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS_OES 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS_OES 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS_OES 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS_OES 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS_OES 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS_OES 0x8E1F
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS_OES 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS_OES 0x92CE
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS_OES 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS_OES 0x92D4
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS_OES 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS_OES 0x90CC
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS_OES 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS_OES 0x90D9
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED_OES 0x8221
#define GL_IS_PER_PATCH_OES 0x92E7
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER_OES 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER_OES 0x9308
#define GL_TESS_CONTROL_SHADER_OES 0x8E88
#define GL_TESS_EVALUATION_SHADER_OES 0x8E87
#define GL_TESS_CONTROL_SHADER_BIT_OES 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT_OES 0x00000010
#define GL_TEXTURE_WRAP_R_OES 0x8072
#define GL_TEXTURE_3D_OES 0x806F
#define GL_TEXTURE_BINDING_3D_OES 0x806A
#define GL_MAX_3D_TEXTURE_SIZE_OES 0x8073
#define GL_SAMPLER_3D_OES 0x8B5F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES 0x8CD4
#define GL_TEXTURE_BORDER_COLOR_OES 0x1004
#define GL_CLAMP_TO_BORDER_OES 0x812D
#define GL_TEXTURE_BUFFER_OES 0x8C2A
#define GL_TEXTURE_BUFFER_BINDING_OES 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_OES 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_OES 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_OES 0x8C2D
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT_OES 0x919F
#define GL_SAMPLER_BUFFER_OES 0x8DC2
#define GL_INT_SAMPLER_BUFFER_OES 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_OES 0x8DD8
#define GL_IMAGE_BUFFER_OES 0x9051
#define GL_INT_IMAGE_BUFFER_OES 0x905C
#define GL_UNSIGNED_INT_IMAGE_BUFFER_OES 0x9067
#define GL_TEXTURE_BUFFER_OFFSET_OES 0x919D
#define GL_TEXTURE_BUFFER_SIZE_OES 0x919E
#define GL_COMPRESSED_RGBA_ASTC_3x3x3_OES 0x93C0
#define GL_COMPRESSED_RGBA_ASTC_4x3x3_OES 0x93C1
#define GL_COMPRESSED_RGBA_ASTC_4x4x3_OES 0x93C2
#define GL_COMPRESSED_RGBA_ASTC_4x4x4_OES 0x93C3
#define GL_COMPRESSED_RGBA_ASTC_5x4x4_OES 0x93C4
#define GL_COMPRESSED_RGBA_ASTC_5x5x4_OES 0x93C5
#define GL_COMPRESSED_RGBA_ASTC_5x5x5_OES 0x93C6
#define GL_COMPRESSED_RGBA_ASTC_6x5x5_OES 0x93C7
#define GL_COMPRESSED_RGBA_ASTC_6x6x5_OES 0x93C8
#define GL_COMPRESSED_RGBA_ASTC_6x6x6_OES 0x93C9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_3x3x3_OES 0x93E0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x3x3_OES 0x93E1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x3_OES 0x93E2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x4_OES 0x93E3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4x4_OES 0x93E4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x4_OES 0x93E5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x5_OES 0x93E6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5x5_OES 0x93E7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x5_OES 0x93E8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x6_OES 0x93E9
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_OES 0x900A
#define GL_SAMPLER_CUBE_MAP_ARRAY_OES 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_OES 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_OES 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_OES 0x900F
#define GL_IMAGE_CUBE_MAP_ARRAY_OES 0x9054
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_OES 0x905F
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_OES 0x906A
#define GL_HALF_FLOAT_OES 0x8D61
#define GL_STENCIL_INDEX_OES 0x1901
#define GL_STENCIL_INDEX8_OES 0x8D48
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY_OES 0x9102
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY_OES 0x9105
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY_OES 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY_OES 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY_OES 0x910D
#define GL_TEXTURE_VIEW_MIN_LEVEL_OES 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS_OES 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER_OES 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS_OES 0x82DE
#define GL_VERTEX_ARRAY_BINDING_OES 0x85B5
#define GL_UNSIGNED_INT_10_10_10_2_OES 0x8DF6
#define GL_INT_10_10_10_2_OES 0x8DF7
#define GL_MAX_VIEWPORTS_OES 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS_OES 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE_OES 0x825D
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX_OES 0x825F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR 0x9630
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR 0x9632
#define GL_MAX_VIEWS_OVR 0x9631
#define GL_FRAMEBUFFER_INCOMPLETE_VIEW_TARGETS_OVR 0x9633
#define GL_ALPHA_TEST_QCOM 0x0BC0
#define GL_ALPHA_TEST_FUNC_QCOM 0x0BC1
#define GL_ALPHA_TEST_REF_QCOM 0x0BC2
#define GL_BINNING_CONTROL_HINT_QCOM 0x8FB0
#define GL_CPU_OPTIMIZED_QCOM 0x8FB1
#define GL_GPU_OPTIMIZED_QCOM 0x8FB2
#define GL_RENDER_DIRECT_TO_FRAMEBUFFER_QCOM 0x8FB3
#define GL_TEXTURE_WIDTH_QCOM 0x8BD2
#define GL_TEXTURE_HEIGHT_QCOM 0x8BD3
#define GL_TEXTURE_DEPTH_QCOM 0x8BD4
#define GL_TEXTURE_INTERNAL_FORMAT_QCOM 0x8BD5
#define GL_TEXTURE_FORMAT_QCOM 0x8BD6
#define GL_TEXTURE_TYPE_QCOM 0x8BD7
#define GL_TEXTURE_IMAGE_VALID_QCOM 0x8BD8
#define GL_TEXTURE_NUM_LEVELS_QCOM 0x8BD9
#define GL_TEXTURE_TARGET_QCOM 0x8BDA
#define GL_TEXTURE_OBJECT_VALID_QCOM 0x8BDB
#define GL_STATE_RESTORE 0x8BDC
#define GL_FOVEATION_ENABLE_BIT_QCOM 0x00000001
#define GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM 0x00000002
#define GL_MOTION_ESTIMATION_SEARCH_BLOCK_X_QCOM 0x8C90
#define GL_MOTION_ESTIMATION_SEARCH_BLOCK_Y_QCOM 0x8C91
#define GL_PERFMON_GLOBAL_MODE_QCOM 0x8FA0
#define GL_FRAMEBUFFER_FETCH_NONCOHERENT_QCOM 0x96A2
#define GL_SHADING_RATE_QCOM 0x96A4
#define GL_SHADING_RATE_PRESERVE_ASPECT_RATIO_QCOM 0x96A5
#define GL_SHADING_RATE_1X1_PIXELS_QCOM 0x96A6
#define GL_SHADING_RATE_1X2_PIXELS_QCOM 0x96A7
#define GL_SHADING_RATE_2X1_PIXELS_QCOM 0x96A8
#define GL_SHADING_RATE_2X2_PIXELS_QCOM 0x96A9
#define GL_SHADING_RATE_4X2_PIXELS_QCOM 0x96AC
#define GL_SHADING_RATE_4X4_PIXELS_QCOM 0x96AE
#define GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM 0x8BFB
#define GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM 0x8BFC
#define GL_TEXTURE_FOVEATED_FEATURE_QUERY_QCOM 0x8BFD
#define GL_TEXTURE_FOVEATED_NUM_FOCAL_POINTS_QUERY_QCOM 0x8BFE
#define GL_FRAMEBUFFER_INCOMPLETE_FOVEATION_QCOM 0x8BFF
#define GL_TEXTURE_FOVEATED_CUTOFF_DENSITY_QCOM 0x96A0
#define GL_FOVEATION_SUBSAMPLED_LAYOUT_METHOD_BIT_QCOM 0x00000004
#define GL_MAX_SHADER_SUBSAMPLED_IMAGE_UNITS_QCOM 0x8FA1
#define GL_COLOR_BUFFER_BIT0_QCOM 0x00000001
#define GL_COLOR_BUFFER_BIT1_QCOM 0x00000002
#define GL_COLOR_BUFFER_BIT2_QCOM 0x00000004
#define GL_COLOR_BUFFER_BIT3_QCOM 0x00000008
#define GL_COLOR_BUFFER_BIT4_QCOM 0x00000010
#define GL_COLOR_BUFFER_BIT5_QCOM 0x00000020
#define GL_COLOR_BUFFER_BIT6_QCOM 0x00000040
#define GL_COLOR_BUFFER_BIT7_QCOM 0x00000080
#define GL_DEPTH_BUFFER_BIT0_QCOM 0x00000100
#define GL_DEPTH_BUFFER_BIT1_QCOM 0x00000200
#define GL_DEPTH_BUFFER_BIT2_QCOM 0x00000400
#define GL_DEPTH_BUFFER_BIT3_QCOM 0x00000800
#define GL_DEPTH_BUFFER_BIT4_QCOM 0x00001000
#define GL_DEPTH_BUFFER_BIT5_QCOM 0x00002000
#define GL_DEPTH_BUFFER_BIT6_QCOM 0x00004000
#define GL_DEPTH_BUFFER_BIT7_QCOM 0x00008000
#define GL_STENCIL_BUFFER_BIT0_QCOM 0x00010000
#define GL_STENCIL_BUFFER_BIT1_QCOM 0x00020000
#define GL_STENCIL_BUFFER_BIT2_QCOM 0x00040000
#define GL_STENCIL_BUFFER_BIT3_QCOM 0x00080000
#define GL_STENCIL_BUFFER_BIT4_QCOM 0x00100000
#define GL_STENCIL_BUFFER_BIT5_QCOM 0x00200000
#define GL_STENCIL_BUFFER_BIT6_QCOM 0x00400000
#define GL_STENCIL_BUFFER_BIT7_QCOM 0x00800000
#define GL_MULTISAMPLE_BUFFER_BIT0_QCOM 0x01000000
#define GL_MULTISAMPLE_BUFFER_BIT1_QCOM 0x02000000
#define GL_MULTISAMPLE_BUFFER_BIT2_QCOM 0x04000000
#define GL_MULTISAMPLE_BUFFER_BIT3_QCOM 0x08000000
#define GL_MULTISAMPLE_BUFFER_BIT4_QCOM 0x10000000
#define GL_MULTISAMPLE_BUFFER_BIT5_QCOM 0x20000000
#define GL_MULTISAMPLE_BUFFER_BIT6_QCOM 0x40000000
#define GL_MULTISAMPLE_BUFFER_BIT7_QCOM 0x80000000
#define GL_WRITEONLY_RENDERING_QCOM 0x8823
#define GL_SHADER_BINARY_VIV 0x8FC4
#ifndef GL_AMD_compressed_3DC_texture
#define GL_AMD_compressed_3DC_texture 1
GLAPI int GLAD_GL_AMD_compressed_3DC_texture;
#endif
#ifndef GL_AMD_compressed_ATC_texture
#define GL_AMD_compressed_ATC_texture 1
GLAPI int GLAD_GL_AMD_compressed_ATC_texture;
#endif
#ifndef GL_AMD_framebuffer_multisample_advanced
#define GL_AMD_framebuffer_multisample_advanced 1
GLAPI int GLAD_GL_AMD_framebuffer_multisample_advanced;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEADVANCEDAMDPROC)(GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEADVANCEDAMDPROC glad_glRenderbufferStorageMultisampleAdvancedAMD;
#define glRenderbufferStorageMultisampleAdvancedAMD glad_glRenderbufferStorageMultisampleAdvancedAMD
typedef void (APIENTRYP PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEADVANCEDAMDPROC)(GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEADVANCEDAMDPROC glad_glNamedRenderbufferStorageMultisampleAdvancedAMD;
#define glNamedRenderbufferStorageMultisampleAdvancedAMD glad_glNamedRenderbufferStorageMultisampleAdvancedAMD
#endif
#ifndef GL_AMD_performance_monitor
#define GL_AMD_performance_monitor 1
GLAPI int GLAD_GL_AMD_performance_monitor;
typedef void (APIENTRYP PFNGLGETPERFMONITORGROUPSAMDPROC)(GLint *numGroups, GLsizei groupsSize, GLuint *groups);
GLAPI PFNGLGETPERFMONITORGROUPSAMDPROC glad_glGetPerfMonitorGroupsAMD;
#define glGetPerfMonitorGroupsAMD glad_glGetPerfMonitorGroupsAMD
typedef void (APIENTRYP PFNGLGETPERFMONITORCOUNTERSAMDPROC)(GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters);
GLAPI PFNGLGETPERFMONITORCOUNTERSAMDPROC glad_glGetPerfMonitorCountersAMD;
#define glGetPerfMonitorCountersAMD glad_glGetPerfMonitorCountersAMD
typedef void (APIENTRYP PFNGLGETPERFMONITORGROUPSTRINGAMDPROC)(GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString);
GLAPI PFNGLGETPERFMONITORGROUPSTRINGAMDPROC glad_glGetPerfMonitorGroupStringAMD;
#define glGetPerfMonitorGroupStringAMD glad_glGetPerfMonitorGroupStringAMD
typedef void (APIENTRYP PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC)(GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString);
GLAPI PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC glad_glGetPerfMonitorCounterStringAMD;
#define glGetPerfMonitorCounterStringAMD glad_glGetPerfMonitorCounterStringAMD
typedef void (APIENTRYP PFNGLGETPERFMONITORCOUNTERINFOAMDPROC)(GLuint group, GLuint counter, GLenum pname, void *data);
GLAPI PFNGLGETPERFMONITORCOUNTERINFOAMDPROC glad_glGetPerfMonitorCounterInfoAMD;
#define glGetPerfMonitorCounterInfoAMD glad_glGetPerfMonitorCounterInfoAMD
typedef void (APIENTRYP PFNGLGENPERFMONITORSAMDPROC)(GLsizei n, GLuint *monitors);
GLAPI PFNGLGENPERFMONITORSAMDPROC glad_glGenPerfMonitorsAMD;
#define glGenPerfMonitorsAMD glad_glGenPerfMonitorsAMD
typedef void (APIENTRYP PFNGLDELETEPERFMONITORSAMDPROC)(GLsizei n, GLuint *monitors);
GLAPI PFNGLDELETEPERFMONITORSAMDPROC glad_glDeletePerfMonitorsAMD;
#define glDeletePerfMonitorsAMD glad_glDeletePerfMonitorsAMD
typedef void (APIENTRYP PFNGLSELECTPERFMONITORCOUNTERSAMDPROC)(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList);
GLAPI PFNGLSELECTPERFMONITORCOUNTERSAMDPROC glad_glSelectPerfMonitorCountersAMD;
#define glSelectPerfMonitorCountersAMD glad_glSelectPerfMonitorCountersAMD
typedef void (APIENTRYP PFNGLBEGINPERFMONITORAMDPROC)(GLuint monitor);
GLAPI PFNGLBEGINPERFMONITORAMDPROC glad_glBeginPerfMonitorAMD;
#define glBeginPerfMonitorAMD glad_glBeginPerfMonitorAMD
typedef void (APIENTRYP PFNGLENDPERFMONITORAMDPROC)(GLuint monitor);
GLAPI PFNGLENDPERFMONITORAMDPROC glad_glEndPerfMonitorAMD;
#define glEndPerfMonitorAMD glad_glEndPerfMonitorAMD
typedef void (APIENTRYP PFNGLGETPERFMONITORCOUNTERDATAAMDPROC)(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten);
GLAPI PFNGLGETPERFMONITORCOUNTERDATAAMDPROC glad_glGetPerfMonitorCounterDataAMD;
#define glGetPerfMonitorCounterDataAMD glad_glGetPerfMonitorCounterDataAMD
#endif
#ifndef GL_AMD_program_binary_Z400
#define GL_AMD_program_binary_Z400 1
GLAPI int GLAD_GL_AMD_program_binary_Z400;
#endif
#ifndef GL_ANDROID_extension_pack_es31a
#define GL_ANDROID_extension_pack_es31a 1
GLAPI int GLAD_GL_ANDROID_extension_pack_es31a;
#endif
#ifndef GL_ANGLE_depth_texture
#define GL_ANGLE_depth_texture 1
GLAPI int GLAD_GL_ANGLE_depth_texture;
#endif
#ifndef GL_ANGLE_framebuffer_blit
#define GL_ANGLE_framebuffer_blit 1
GLAPI int GLAD_GL_ANGLE_framebuffer_blit;
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERANGLEPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI PFNGLBLITFRAMEBUFFERANGLEPROC glad_glBlitFramebufferANGLE;
#define glBlitFramebufferANGLE glad_glBlitFramebufferANGLE
#endif
#ifndef GL_ANGLE_framebuffer_multisample
#define GL_ANGLE_framebuffer_multisample 1
GLAPI int GLAD_GL_ANGLE_framebuffer_multisample;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEANGLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEANGLEPROC glad_glRenderbufferStorageMultisampleANGLE;
#define glRenderbufferStorageMultisampleANGLE glad_glRenderbufferStorageMultisampleANGLE
#endif
#ifndef GL_ANGLE_instanced_arrays
#define GL_ANGLE_instanced_arrays 1
GLAPI int GLAD_GL_ANGLE_instanced_arrays;
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDANGLEPROC)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI PFNGLDRAWARRAYSINSTANCEDANGLEPROC glad_glDrawArraysInstancedANGLE;
#define glDrawArraysInstancedANGLE glad_glDrawArraysInstancedANGLE
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDANGLEPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);
GLAPI PFNGLDRAWELEMENTSINSTANCEDANGLEPROC glad_glDrawElementsInstancedANGLE;
#define glDrawElementsInstancedANGLE glad_glDrawElementsInstancedANGLE
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORANGLEPROC)(GLuint index, GLuint divisor);
GLAPI PFNGLVERTEXATTRIBDIVISORANGLEPROC glad_glVertexAttribDivisorANGLE;
#define glVertexAttribDivisorANGLE glad_glVertexAttribDivisorANGLE
#endif
#ifndef GL_ANGLE_pack_reverse_row_order
#define GL_ANGLE_pack_reverse_row_order 1
GLAPI int GLAD_GL_ANGLE_pack_reverse_row_order;
#endif
#ifndef GL_ANGLE_program_binary
#define GL_ANGLE_program_binary 1
GLAPI int GLAD_GL_ANGLE_program_binary;
#endif
#ifndef GL_ANGLE_texture_compression_dxt3
#define GL_ANGLE_texture_compression_dxt3 1
GLAPI int GLAD_GL_ANGLE_texture_compression_dxt3;
#endif
#ifndef GL_ANGLE_texture_compression_dxt5
#define GL_ANGLE_texture_compression_dxt5 1
GLAPI int GLAD_GL_ANGLE_texture_compression_dxt5;
#endif
#ifndef GL_ANGLE_texture_usage
#define GL_ANGLE_texture_usage 1
GLAPI int GLAD_GL_ANGLE_texture_usage;
#endif
#ifndef GL_ANGLE_translated_shader_source
#define GL_ANGLE_translated_shader_source 1
GLAPI int GLAD_GL_ANGLE_translated_shader_source;
typedef void (APIENTRYP PFNGLGETTRANSLATEDSHADERSOURCEANGLEPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLAPI PFNGLGETTRANSLATEDSHADERSOURCEANGLEPROC glad_glGetTranslatedShaderSourceANGLE;
#define glGetTranslatedShaderSourceANGLE glad_glGetTranslatedShaderSourceANGLE
#endif
#ifndef GL_APPLE_clip_distance
#define GL_APPLE_clip_distance 1
GLAPI int GLAD_GL_APPLE_clip_distance;
#endif
#ifndef GL_APPLE_color_buffer_packed_float
#define GL_APPLE_color_buffer_packed_float 1
GLAPI int GLAD_GL_APPLE_color_buffer_packed_float;
#endif
#ifndef GL_APPLE_copy_texture_levels
#define GL_APPLE_copy_texture_levels 1
GLAPI int GLAD_GL_APPLE_copy_texture_levels;
typedef void (APIENTRYP PFNGLCOPYTEXTURELEVELSAPPLEPROC)(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);
GLAPI PFNGLCOPYTEXTURELEVELSAPPLEPROC glad_glCopyTextureLevelsAPPLE;
#define glCopyTextureLevelsAPPLE glad_glCopyTextureLevelsAPPLE
#endif
#ifndef GL_APPLE_framebuffer_multisample
#define GL_APPLE_framebuffer_multisample 1
GLAPI int GLAD_GL_APPLE_framebuffer_multisample;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEAPPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEAPPLEPROC glad_glRenderbufferStorageMultisampleAPPLE;
#define glRenderbufferStorageMultisampleAPPLE glad_glRenderbufferStorageMultisampleAPPLE
typedef void (APIENTRYP PFNGLRESOLVEMULTISAMPLEFRAMEBUFFERAPPLEPROC)(void);
GLAPI PFNGLRESOLVEMULTISAMPLEFRAMEBUFFERAPPLEPROC glad_glResolveMultisampleFramebufferAPPLE;
#define glResolveMultisampleFramebufferAPPLE glad_glResolveMultisampleFramebufferAPPLE
#endif
#ifndef GL_APPLE_rgb_422
#define GL_APPLE_rgb_422 1
GLAPI int GLAD_GL_APPLE_rgb_422;
#endif
#ifndef GL_APPLE_sync
#define GL_APPLE_sync 1
GLAPI int GLAD_GL_APPLE_sync;
typedef GLsync (APIENTRYP PFNGLFENCESYNCAPPLEPROC)(GLenum condition, GLbitfield flags);
GLAPI PFNGLFENCESYNCAPPLEPROC glad_glFenceSyncAPPLE;
#define glFenceSyncAPPLE glad_glFenceSyncAPPLE
typedef GLboolean (APIENTRYP PFNGLISSYNCAPPLEPROC)(GLsync sync);
GLAPI PFNGLISSYNCAPPLEPROC glad_glIsSyncAPPLE;
#define glIsSyncAPPLE glad_glIsSyncAPPLE
typedef void (APIENTRYP PFNGLDELETESYNCAPPLEPROC)(GLsync sync);
GLAPI PFNGLDELETESYNCAPPLEPROC glad_glDeleteSyncAPPLE;
#define glDeleteSyncAPPLE glad_glDeleteSyncAPPLE
typedef GLenum (APIENTRYP PFNGLCLIENTWAITSYNCAPPLEPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLCLIENTWAITSYNCAPPLEPROC glad_glClientWaitSyncAPPLE;
#define glClientWaitSyncAPPLE glad_glClientWaitSyncAPPLE
typedef void (APIENTRYP PFNGLWAITSYNCAPPLEPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLWAITSYNCAPPLEPROC glad_glWaitSyncAPPLE;
#define glWaitSyncAPPLE glad_glWaitSyncAPPLE
typedef void (APIENTRYP PFNGLGETINTEGER64VAPPLEPROC)(GLenum pname, GLint64 *params);
GLAPI PFNGLGETINTEGER64VAPPLEPROC glad_glGetInteger64vAPPLE;
#define glGetInteger64vAPPLE glad_glGetInteger64vAPPLE
typedef void (APIENTRYP PFNGLGETSYNCIVAPPLEPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
GLAPI PFNGLGETSYNCIVAPPLEPROC glad_glGetSyncivAPPLE;
#define glGetSyncivAPPLE glad_glGetSyncivAPPLE
#endif
#ifndef GL_APPLE_texture_format_BGRA8888
#define GL_APPLE_texture_format_BGRA8888 1
GLAPI int GLAD_GL_APPLE_texture_format_BGRA8888;
#endif
#ifndef GL_APPLE_texture_max_level
#define GL_APPLE_texture_max_level 1
GLAPI int GLAD_GL_APPLE_texture_max_level;
#endif
#ifndef GL_APPLE_texture_packed_float
#define GL_APPLE_texture_packed_float 1
GLAPI int GLAD_GL_APPLE_texture_packed_float;
#endif
#ifndef GL_ARM_mali_program_binary
#define GL_ARM_mali_program_binary 1
GLAPI int GLAD_GL_ARM_mali_program_binary;
#endif
#ifndef GL_ARM_mali_shader_binary
#define GL_ARM_mali_shader_binary 1
GLAPI int GLAD_GL_ARM_mali_shader_binary;
#endif
#ifndef GL_ARM_rgba8
#define GL_ARM_rgba8 1
GLAPI int GLAD_GL_ARM_rgba8;
#endif
#ifndef GL_ARM_shader_framebuffer_fetch
#define GL_ARM_shader_framebuffer_fetch 1
GLAPI int GLAD_GL_ARM_shader_framebuffer_fetch;
#endif
#ifndef GL_ARM_shader_framebuffer_fetch_depth_stencil
#define GL_ARM_shader_framebuffer_fetch_depth_stencil 1
GLAPI int GLAD_GL_ARM_shader_framebuffer_fetch_depth_stencil;
#endif
#ifndef GL_ARM_texture_unnormalized_coordinates
#define GL_ARM_texture_unnormalized_coordinates 1
GLAPI int GLAD_GL_ARM_texture_unnormalized_coordinates;
#endif
#ifndef GL_DMP_program_binary
#define GL_DMP_program_binary 1
GLAPI int GLAD_GL_DMP_program_binary;
#endif
#ifndef GL_DMP_shader_binary
#define GL_DMP_shader_binary 1
GLAPI int GLAD_GL_DMP_shader_binary;
#endif
#ifndef GL_EXT_EGL_image_array
#define GL_EXT_EGL_image_array 1
GLAPI int GLAD_GL_EXT_EGL_image_array;
#endif
#ifndef GL_EXT_EGL_image_storage
#define GL_EXT_EGL_image_storage 1
GLAPI int GLAD_GL_EXT_EGL_image_storage;
typedef void (APIENTRYP PFNGLEGLIMAGETARGETTEXSTORAGEEXTPROC)(GLenum target, GLeglImageOES image, const GLint* attrib_list);
GLAPI PFNGLEGLIMAGETARGETTEXSTORAGEEXTPROC glad_glEGLImageTargetTexStorageEXT;
#define glEGLImageTargetTexStorageEXT glad_glEGLImageTargetTexStorageEXT
typedef void (APIENTRYP PFNGLEGLIMAGETARGETTEXTURESTORAGEEXTPROC)(GLuint texture, GLeglImageOES image, const GLint* attrib_list);
GLAPI PFNGLEGLIMAGETARGETTEXTURESTORAGEEXTPROC glad_glEGLImageTargetTextureStorageEXT;
#define glEGLImageTargetTextureStorageEXT glad_glEGLImageTargetTextureStorageEXT
#endif
#ifndef GL_EXT_EGL_image_storage_compression
#define GL_EXT_EGL_image_storage_compression 1
GLAPI int GLAD_GL_EXT_EGL_image_storage_compression;
#endif
#ifndef GL_EXT_YUV_target
#define GL_EXT_YUV_target 1
GLAPI int GLAD_GL_EXT_YUV_target;
#endif
#ifndef GL_EXT_base_instance
#define GL_EXT_base_instance 1
GLAPI int GLAD_GL_EXT_base_instance;
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
GLAPI PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC glad_glDrawArraysInstancedBaseInstanceEXT;
#define glDrawArraysInstancedBaseInstanceEXT glad_glDrawArraysInstancedBaseInstanceEXT
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC glad_glDrawElementsInstancedBaseInstanceEXT;
#define glDrawElementsInstancedBaseInstanceEXT glad_glDrawElementsInstancedBaseInstanceEXT
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC glad_glDrawElementsInstancedBaseVertexBaseInstanceEXT;
#define glDrawElementsInstancedBaseVertexBaseInstanceEXT glad_glDrawElementsInstancedBaseVertexBaseInstanceEXT
#endif
#ifndef GL_EXT_blend_func_extended
#define GL_EXT_blend_func_extended 1
GLAPI int GLAD_GL_EXT_blend_func_extended;
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONINDEXEDEXTPROC)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
GLAPI PFNGLBINDFRAGDATALOCATIONINDEXEDEXTPROC glad_glBindFragDataLocationIndexedEXT;
#define glBindFragDataLocationIndexedEXT glad_glBindFragDataLocationIndexedEXT
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONEXTPROC)(GLuint program, GLuint color, const GLchar *name);
GLAPI PFNGLBINDFRAGDATALOCATIONEXTPROC glad_glBindFragDataLocationEXT;
#define glBindFragDataLocationEXT glad_glBindFragDataLocationEXT
typedef GLint (APIENTRYP PFNGLGETPROGRAMRESOURCELOCATIONINDEXEXTPROC)(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI PFNGLGETPROGRAMRESOURCELOCATIONINDEXEXTPROC glad_glGetProgramResourceLocationIndexEXT;
#define glGetProgramResourceLocationIndexEXT glad_glGetProgramResourceLocationIndexEXT
typedef GLint (APIENTRYP PFNGLGETFRAGDATAINDEXEXTPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETFRAGDATAINDEXEXTPROC glad_glGetFragDataIndexEXT;
#define glGetFragDataIndexEXT glad_glGetFragDataIndexEXT
#endif
#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1
GLAPI int GLAD_GL_EXT_blend_minmax;
typedef void (APIENTRYP PFNGLBLENDEQUATIONEXTPROC)(GLenum mode);
GLAPI PFNGLBLENDEQUATIONEXTPROC glad_glBlendEquationEXT;
#define glBlendEquationEXT glad_glBlendEquationEXT
#endif
#ifndef GL_EXT_buffer_storage
#define GL_EXT_buffer_storage 1
GLAPI int GLAD_GL_EXT_buffer_storage;
typedef void (APIENTRYP PFNGLBUFFERSTORAGEEXTPROC)(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
GLAPI PFNGLBUFFERSTORAGEEXTPROC glad_glBufferStorageEXT;
#define glBufferStorageEXT glad_glBufferStorageEXT
#endif
#ifndef GL_EXT_clear_texture
#define GL_EXT_clear_texture 1
GLAPI int GLAD_GL_EXT_clear_texture;
typedef void (APIENTRYP PFNGLCLEARTEXIMAGEEXTPROC)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
GLAPI PFNGLCLEARTEXIMAGEEXTPROC glad_glClearTexImageEXT;
#define glClearTexImageEXT glad_glClearTexImageEXT
typedef void (APIENTRYP PFNGLCLEARTEXSUBIMAGEEXTPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
GLAPI PFNGLCLEARTEXSUBIMAGEEXTPROC glad_glClearTexSubImageEXT;
#define glClearTexSubImageEXT glad_glClearTexSubImageEXT
#endif
#ifndef GL_EXT_clip_control
#define GL_EXT_clip_control 1
GLAPI int GLAD_GL_EXT_clip_control;
typedef void (APIENTRYP PFNGLCLIPCONTROLEXTPROC)(GLenum origin, GLenum depth);
GLAPI PFNGLCLIPCONTROLEXTPROC glad_glClipControlEXT;
#define glClipControlEXT glad_glClipControlEXT
#endif
#ifndef GL_EXT_clip_cull_distance
#define GL_EXT_clip_cull_distance 1
GLAPI int GLAD_GL_EXT_clip_cull_distance;
#endif
#ifndef GL_EXT_color_buffer_float
#define GL_EXT_color_buffer_float 1
GLAPI int GLAD_GL_EXT_color_buffer_float;
#endif
#ifndef GL_EXT_color_buffer_half_float
#define GL_EXT_color_buffer_half_float 1
GLAPI int GLAD_GL_EXT_color_buffer_half_float;
#endif
#ifndef GL_EXT_conservative_depth
#define GL_EXT_conservative_depth 1
GLAPI int GLAD_GL_EXT_conservative_depth;
#endif
#ifndef GL_EXT_copy_image
#define GL_EXT_copy_image 1
GLAPI int GLAD_GL_EXT_copy_image;
typedef void (APIENTRYP PFNGLCOPYIMAGESUBDATAEXTPROC)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GLAPI PFNGLCOPYIMAGESUBDATAEXTPROC glad_glCopyImageSubDataEXT;
#define glCopyImageSubDataEXT glad_glCopyImageSubDataEXT
#endif
#ifndef GL_EXT_debug_label
#define GL_EXT_debug_label 1
GLAPI int GLAD_GL_EXT_debug_label;
typedef void (APIENTRYP PFNGLLABELOBJECTEXTPROC)(GLenum type, GLuint object, GLsizei length, const GLchar *label);
GLAPI PFNGLLABELOBJECTEXTPROC glad_glLabelObjectEXT;
#define glLabelObjectEXT glad_glLabelObjectEXT
typedef void (APIENTRYP PFNGLGETOBJECTLABELEXTPROC)(GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI PFNGLGETOBJECTLABELEXTPROC glad_glGetObjectLabelEXT;
#define glGetObjectLabelEXT glad_glGetObjectLabelEXT
#endif
#ifndef GL_EXT_debug_marker
#define GL_EXT_debug_marker 1
GLAPI int GLAD_GL_EXT_debug_marker;
typedef void (APIENTRYP PFNGLINSERTEVENTMARKEREXTPROC)(GLsizei length, const GLchar *marker);
GLAPI PFNGLINSERTEVENTMARKEREXTPROC glad_glInsertEventMarkerEXT;
#define glInsertEventMarkerEXT glad_glInsertEventMarkerEXT
typedef void (APIENTRYP PFNGLPUSHGROUPMARKEREXTPROC)(GLsizei length, const GLchar *marker);
GLAPI PFNGLPUSHGROUPMARKEREXTPROC glad_glPushGroupMarkerEXT;
#define glPushGroupMarkerEXT glad_glPushGroupMarkerEXT
typedef void (APIENTRYP PFNGLPOPGROUPMARKEREXTPROC)(void);
GLAPI PFNGLPOPGROUPMARKEREXTPROC glad_glPopGroupMarkerEXT;
#define glPopGroupMarkerEXT glad_glPopGroupMarkerEXT
#endif
#ifndef GL_EXT_depth_clamp
#define GL_EXT_depth_clamp 1
GLAPI int GLAD_GL_EXT_depth_clamp;
#endif
#ifndef GL_EXT_discard_framebuffer
#define GL_EXT_discard_framebuffer 1
GLAPI int GLAD_GL_EXT_discard_framebuffer;
typedef void (APIENTRYP PFNGLDISCARDFRAMEBUFFEREXTPROC)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
GLAPI PFNGLDISCARDFRAMEBUFFEREXTPROC glad_glDiscardFramebufferEXT;
#define glDiscardFramebufferEXT glad_glDiscardFramebufferEXT
#endif
#ifndef GL_EXT_disjoint_timer_query
#define GL_EXT_disjoint_timer_query 1
GLAPI int GLAD_GL_EXT_disjoint_timer_query;
typedef void (APIENTRYP PFNGLGENQUERIESEXTPROC)(GLsizei n, GLuint *ids);
GLAPI PFNGLGENQUERIESEXTPROC glad_glGenQueriesEXT;
#define glGenQueriesEXT glad_glGenQueriesEXT
typedef void (APIENTRYP PFNGLDELETEQUERIESEXTPROC)(GLsizei n, const GLuint *ids);
GLAPI PFNGLDELETEQUERIESEXTPROC glad_glDeleteQueriesEXT;
#define glDeleteQueriesEXT glad_glDeleteQueriesEXT
typedef GLboolean (APIENTRYP PFNGLISQUERYEXTPROC)(GLuint id);
GLAPI PFNGLISQUERYEXTPROC glad_glIsQueryEXT;
#define glIsQueryEXT glad_glIsQueryEXT
typedef void (APIENTRYP PFNGLBEGINQUERYEXTPROC)(GLenum target, GLuint id);
GLAPI PFNGLBEGINQUERYEXTPROC glad_glBeginQueryEXT;
#define glBeginQueryEXT glad_glBeginQueryEXT
typedef void (APIENTRYP PFNGLENDQUERYEXTPROC)(GLenum target);
GLAPI PFNGLENDQUERYEXTPROC glad_glEndQueryEXT;
#define glEndQueryEXT glad_glEndQueryEXT
typedef void (APIENTRYP PFNGLQUERYCOUNTEREXTPROC)(GLuint id, GLenum target);
GLAPI PFNGLQUERYCOUNTEREXTPROC glad_glQueryCounterEXT;
#define glQueryCounterEXT glad_glQueryCounterEXT
typedef void (APIENTRYP PFNGLGETQUERYIVEXTPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETQUERYIVEXTPROC glad_glGetQueryivEXT;
#define glGetQueryivEXT glad_glGetQueryivEXT
typedef void (APIENTRYP PFNGLGETQUERYOBJECTIVEXTPROC)(GLuint id, GLenum pname, GLint *params);
GLAPI PFNGLGETQUERYOBJECTIVEXTPROC glad_glGetQueryObjectivEXT;
#define glGetQueryObjectivEXT glad_glGetQueryObjectivEXT
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUIVEXTPROC)(GLuint id, GLenum pname, GLuint *params);
GLAPI PFNGLGETQUERYOBJECTUIVEXTPROC glad_glGetQueryObjectuivEXT;
#define glGetQueryObjectuivEXT glad_glGetQueryObjectuivEXT
typedef void (APIENTRYP PFNGLGETQUERYOBJECTI64VEXTPROC)(GLuint id, GLenum pname, GLint64 *params);
GLAPI PFNGLGETQUERYOBJECTI64VEXTPROC glad_glGetQueryObjecti64vEXT;
#define glGetQueryObjecti64vEXT glad_glGetQueryObjecti64vEXT
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUI64VEXTPROC)(GLuint id, GLenum pname, GLuint64 *params);
GLAPI PFNGLGETQUERYOBJECTUI64VEXTPROC glad_glGetQueryObjectui64vEXT;
#define glGetQueryObjectui64vEXT glad_glGetQueryObjectui64vEXT
typedef void (APIENTRYP PFNGLGETINTEGER64VEXTPROC)(GLenum pname, GLint64 *data);
GLAPI PFNGLGETINTEGER64VEXTPROC glad_glGetInteger64vEXT;
#define glGetInteger64vEXT glad_glGetInteger64vEXT
#endif
#ifndef GL_EXT_draw_buffers
#define GL_EXT_draw_buffers 1
GLAPI int GLAD_GL_EXT_draw_buffers;
typedef void (APIENTRYP PFNGLDRAWBUFFERSEXTPROC)(GLsizei n, const GLenum *bufs);
GLAPI PFNGLDRAWBUFFERSEXTPROC glad_glDrawBuffersEXT;
#define glDrawBuffersEXT glad_glDrawBuffersEXT
#endif
#ifndef GL_EXT_draw_buffers_indexed
#define GL_EXT_draw_buffers_indexed 1
GLAPI int GLAD_GL_EXT_draw_buffers_indexed;
typedef void (APIENTRYP PFNGLENABLEIEXTPROC)(GLenum target, GLuint index);
GLAPI PFNGLENABLEIEXTPROC glad_glEnableiEXT;
#define glEnableiEXT glad_glEnableiEXT
typedef void (APIENTRYP PFNGLDISABLEIEXTPROC)(GLenum target, GLuint index);
GLAPI PFNGLDISABLEIEXTPROC glad_glDisableiEXT;
#define glDisableiEXT glad_glDisableiEXT
typedef void (APIENTRYP PFNGLBLENDEQUATIONIEXTPROC)(GLuint buf, GLenum mode);
GLAPI PFNGLBLENDEQUATIONIEXTPROC glad_glBlendEquationiEXT;
#define glBlendEquationiEXT glad_glBlendEquationiEXT
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIEXTPROC)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLAPI PFNGLBLENDEQUATIONSEPARATEIEXTPROC glad_glBlendEquationSeparateiEXT;
#define glBlendEquationSeparateiEXT glad_glBlendEquationSeparateiEXT
typedef void (APIENTRYP PFNGLBLENDFUNCIEXTPROC)(GLuint buf, GLenum src, GLenum dst);
GLAPI PFNGLBLENDFUNCIEXTPROC glad_glBlendFunciEXT;
#define glBlendFunciEXT glad_glBlendFunciEXT
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIEXTPROC)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLAPI PFNGLBLENDFUNCSEPARATEIEXTPROC glad_glBlendFuncSeparateiEXT;
#define glBlendFuncSeparateiEXT glad_glBlendFuncSeparateiEXT
typedef void (APIENTRYP PFNGLCOLORMASKIEXTPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI PFNGLCOLORMASKIEXTPROC glad_glColorMaskiEXT;
#define glColorMaskiEXT glad_glColorMaskiEXT
typedef GLboolean (APIENTRYP PFNGLISENABLEDIEXTPROC)(GLenum target, GLuint index);
GLAPI PFNGLISENABLEDIEXTPROC glad_glIsEnablediEXT;
#define glIsEnablediEXT glad_glIsEnablediEXT
#endif
#ifndef GL_EXT_draw_elements_base_vertex
#define GL_EXT_draw_elements_base_vertex 1
GLAPI int GLAD_GL_EXT_draw_elements_base_vertex;
typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXEXTPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSBASEVERTEXEXTPROC glad_glDrawElementsBaseVertexEXT;
#define glDrawElementsBaseVertexEXT glad_glDrawElementsBaseVertexEXT
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXEXTPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXEXTPROC glad_glDrawRangeElementsBaseVertexEXT;
#define glDrawRangeElementsBaseVertexEXT glad_glDrawRangeElementsBaseVertexEXT
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXEXTPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXEXTPROC glad_glDrawElementsInstancedBaseVertexEXT;
#define glDrawElementsInstancedBaseVertexEXT glad_glDrawElementsInstancedBaseVertexEXT
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSBASEVERTEXEXTPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
GLAPI PFNGLMULTIDRAWELEMENTSBASEVERTEXEXTPROC glad_glMultiDrawElementsBaseVertexEXT;
#define glMultiDrawElementsBaseVertexEXT glad_glMultiDrawElementsBaseVertexEXT
#endif
#ifndef GL_EXT_draw_instanced
#define GL_EXT_draw_instanced 1
GLAPI int GLAD_GL_EXT_draw_instanced;
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDEXTPROC)(GLenum mode, GLint start, GLsizei count, GLsizei primcount);
GLAPI PFNGLDRAWARRAYSINSTANCEDEXTPROC glad_glDrawArraysInstancedEXT;
#define glDrawArraysInstancedEXT glad_glDrawArraysInstancedEXT
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDEXTPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);
GLAPI PFNGLDRAWELEMENTSINSTANCEDEXTPROC glad_glDrawElementsInstancedEXT;
#define glDrawElementsInstancedEXT glad_glDrawElementsInstancedEXT
#endif
#ifndef GL_EXT_draw_transform_feedback
#define GL_EXT_draw_transform_feedback 1
GLAPI int GLAD_GL_EXT_draw_transform_feedback;
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKEXTPROC)(GLenum mode, GLuint id);
GLAPI PFNGLDRAWTRANSFORMFEEDBACKEXTPROC glad_glDrawTransformFeedbackEXT;
#define glDrawTransformFeedbackEXT glad_glDrawTransformFeedbackEXT
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDEXTPROC)(GLenum mode, GLuint id, GLsizei instancecount);
GLAPI PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDEXTPROC glad_glDrawTransformFeedbackInstancedEXT;
#define glDrawTransformFeedbackInstancedEXT glad_glDrawTransformFeedbackInstancedEXT
#endif
#ifndef GL_EXT_external_buffer
#define GL_EXT_external_buffer 1
GLAPI int GLAD_GL_EXT_external_buffer;
typedef void (APIENTRYP PFNGLBUFFERSTORAGEEXTERNALEXTPROC)(GLenum target, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags);
GLAPI PFNGLBUFFERSTORAGEEXTERNALEXTPROC glad_glBufferStorageExternalEXT;
#define glBufferStorageExternalEXT glad_glBufferStorageExternalEXT
typedef void (APIENTRYP PFNGLNAMEDBUFFERSTORAGEEXTERNALEXTPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags);
GLAPI PFNGLNAMEDBUFFERSTORAGEEXTERNALEXTPROC glad_glNamedBufferStorageExternalEXT;
#define glNamedBufferStorageExternalEXT glad_glNamedBufferStorageExternalEXT
#endif
#ifndef GL_EXT_float_blend
#define GL_EXT_float_blend 1
GLAPI int GLAD_GL_EXT_float_blend;
#endif
#ifndef GL_EXT_geometry_point_size
#define GL_EXT_geometry_point_size 1
GLAPI int GLAD_GL_EXT_geometry_point_size;
#endif
#ifndef GL_EXT_geometry_shader
#define GL_EXT_geometry_shader 1
GLAPI int GLAD_GL_EXT_geometry_shader;
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREEXTPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTUREEXTPROC glad_glFramebufferTextureEXT;
#define glFramebufferTextureEXT glad_glFramebufferTextureEXT
#endif
#ifndef GL_EXT_gpu_shader5
#define GL_EXT_gpu_shader5 1
GLAPI int GLAD_GL_EXT_gpu_shader5;
#endif
#ifndef GL_EXT_instanced_arrays
#define GL_EXT_instanced_arrays 1
GLAPI int GLAD_GL_EXT_instanced_arrays;
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISOREXTPROC)(GLuint index, GLuint divisor);
GLAPI PFNGLVERTEXATTRIBDIVISOREXTPROC glad_glVertexAttribDivisorEXT;
#define glVertexAttribDivisorEXT glad_glVertexAttribDivisorEXT
#endif
#ifndef GL_EXT_map_buffer_range
#define GL_EXT_map_buffer_range 1
GLAPI int GLAD_GL_EXT_map_buffer_range;
typedef void * (APIENTRYP PFNGLMAPBUFFERRANGEEXTPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI PFNGLMAPBUFFERRANGEEXTPROC glad_glMapBufferRangeEXT;
#define glMapBufferRangeEXT glad_glMapBufferRangeEXT
typedef void (APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
GLAPI PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC glad_glFlushMappedBufferRangeEXT;
#define glFlushMappedBufferRangeEXT glad_glFlushMappedBufferRangeEXT
#endif
#ifndef GL_EXT_memory_object
#define GL_EXT_memory_object 1
GLAPI int GLAD_GL_EXT_memory_object;
typedef void (APIENTRYP PFNGLGETUNSIGNEDBYTEVEXTPROC)(GLenum pname, GLubyte *data);
GLAPI PFNGLGETUNSIGNEDBYTEVEXTPROC glad_glGetUnsignedBytevEXT;
#define glGetUnsignedBytevEXT glad_glGetUnsignedBytevEXT
typedef void (APIENTRYP PFNGLGETUNSIGNEDBYTEI_VEXTPROC)(GLenum target, GLuint index, GLubyte *data);
GLAPI PFNGLGETUNSIGNEDBYTEI_VEXTPROC glad_glGetUnsignedBytei_vEXT;
#define glGetUnsignedBytei_vEXT glad_glGetUnsignedBytei_vEXT
typedef void (APIENTRYP PFNGLDELETEMEMORYOBJECTSEXTPROC)(GLsizei n, const GLuint *memoryObjects);
GLAPI PFNGLDELETEMEMORYOBJECTSEXTPROC glad_glDeleteMemoryObjectsEXT;
#define glDeleteMemoryObjectsEXT glad_glDeleteMemoryObjectsEXT
typedef GLboolean (APIENTRYP PFNGLISMEMORYOBJECTEXTPROC)(GLuint memoryObject);
GLAPI PFNGLISMEMORYOBJECTEXTPROC glad_glIsMemoryObjectEXT;
#define glIsMemoryObjectEXT glad_glIsMemoryObjectEXT
typedef void (APIENTRYP PFNGLCREATEMEMORYOBJECTSEXTPROC)(GLsizei n, GLuint *memoryObjects);
GLAPI PFNGLCREATEMEMORYOBJECTSEXTPROC glad_glCreateMemoryObjectsEXT;
#define glCreateMemoryObjectsEXT glad_glCreateMemoryObjectsEXT
typedef void (APIENTRYP PFNGLMEMORYOBJECTPARAMETERIVEXTPROC)(GLuint memoryObject, GLenum pname, const GLint *params);
GLAPI PFNGLMEMORYOBJECTPARAMETERIVEXTPROC glad_glMemoryObjectParameterivEXT;
#define glMemoryObjectParameterivEXT glad_glMemoryObjectParameterivEXT
typedef void (APIENTRYP PFNGLGETMEMORYOBJECTPARAMETERIVEXTPROC)(GLuint memoryObject, GLenum pname, GLint *params);
GLAPI PFNGLGETMEMORYOBJECTPARAMETERIVEXTPROC glad_glGetMemoryObjectParameterivEXT;
#define glGetMemoryObjectParameterivEXT glad_glGetMemoryObjectParameterivEXT
typedef void (APIENTRYP PFNGLTEXSTORAGEMEM2DEXTPROC)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXSTORAGEMEM2DEXTPROC glad_glTexStorageMem2DEXT;
#define glTexStorageMem2DEXT glad_glTexStorageMem2DEXT
typedef void (APIENTRYP PFNGLTEXSTORAGEMEM2DMULTISAMPLEEXTPROC)(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXSTORAGEMEM2DMULTISAMPLEEXTPROC glad_glTexStorageMem2DMultisampleEXT;
#define glTexStorageMem2DMultisampleEXT glad_glTexStorageMem2DMultisampleEXT
typedef void (APIENTRYP PFNGLTEXSTORAGEMEM3DEXTPROC)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXSTORAGEMEM3DEXTPROC glad_glTexStorageMem3DEXT;
#define glTexStorageMem3DEXT glad_glTexStorageMem3DEXT
typedef void (APIENTRYP PFNGLTEXSTORAGEMEM3DMULTISAMPLEEXTPROC)(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXSTORAGEMEM3DMULTISAMPLEEXTPROC glad_glTexStorageMem3DMultisampleEXT;
#define glTexStorageMem3DMultisampleEXT glad_glTexStorageMem3DMultisampleEXT
typedef void (APIENTRYP PFNGLBUFFERSTORAGEMEMEXTPROC)(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset);
GLAPI PFNGLBUFFERSTORAGEMEMEXTPROC glad_glBufferStorageMemEXT;
#define glBufferStorageMemEXT glad_glBufferStorageMemEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGEMEM2DEXTPROC)(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTURESTORAGEMEM2DEXTPROC glad_glTextureStorageMem2DEXT;
#define glTextureStorageMem2DEXT glad_glTextureStorageMem2DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGEMEM2DMULTISAMPLEEXTPROC)(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTURESTORAGEMEM2DMULTISAMPLEEXTPROC glad_glTextureStorageMem2DMultisampleEXT;
#define glTextureStorageMem2DMultisampleEXT glad_glTextureStorageMem2DMultisampleEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGEMEM3DEXTPROC)(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTURESTORAGEMEM3DEXTPROC glad_glTextureStorageMem3DEXT;
#define glTextureStorageMem3DEXT glad_glTextureStorageMem3DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGEMEM3DMULTISAMPLEEXTPROC)(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTURESTORAGEMEM3DMULTISAMPLEEXTPROC glad_glTextureStorageMem3DMultisampleEXT;
#define glTextureStorageMem3DMultisampleEXT glad_glTextureStorageMem3DMultisampleEXT
typedef void (APIENTRYP PFNGLNAMEDBUFFERSTORAGEMEMEXTPROC)(GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset);
GLAPI PFNGLNAMEDBUFFERSTORAGEMEMEXTPROC glad_glNamedBufferStorageMemEXT;
#define glNamedBufferStorageMemEXT glad_glNamedBufferStorageMemEXT
typedef void (APIENTRYP PFNGLTEXSTORAGEMEM1DEXTPROC)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXSTORAGEMEM1DEXTPROC glad_glTexStorageMem1DEXT;
#define glTexStorageMem1DEXT glad_glTexStorageMem1DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGEMEM1DEXTPROC)(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTURESTORAGEMEM1DEXTPROC glad_glTextureStorageMem1DEXT;
#define glTextureStorageMem1DEXT glad_glTextureStorageMem1DEXT
#endif
#ifndef GL_EXT_memory_object_fd
#define GL_EXT_memory_object_fd 1
GLAPI int GLAD_GL_EXT_memory_object_fd;
typedef void (APIENTRYP PFNGLIMPORTMEMORYFDEXTPROC)(GLuint memory, GLuint64 size, GLenum handleType, GLint fd);
GLAPI PFNGLIMPORTMEMORYFDEXTPROC glad_glImportMemoryFdEXT;
#define glImportMemoryFdEXT glad_glImportMemoryFdEXT
#endif
#ifndef GL_EXT_memory_object_win32
#define GL_EXT_memory_object_win32 1
GLAPI int GLAD_GL_EXT_memory_object_win32;
typedef void (APIENTRYP PFNGLIMPORTMEMORYWIN32HANDLEEXTPROC)(GLuint memory, GLuint64 size, GLenum handleType, void *handle);
GLAPI PFNGLIMPORTMEMORYWIN32HANDLEEXTPROC glad_glImportMemoryWin32HandleEXT;
#define glImportMemoryWin32HandleEXT glad_glImportMemoryWin32HandleEXT
typedef void (APIENTRYP PFNGLIMPORTMEMORYWIN32NAMEEXTPROC)(GLuint memory, GLuint64 size, GLenum handleType, const void *name);
GLAPI PFNGLIMPORTMEMORYWIN32NAMEEXTPROC glad_glImportMemoryWin32NameEXT;
#define glImportMemoryWin32NameEXT glad_glImportMemoryWin32NameEXT
#endif
#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays 1
GLAPI int GLAD_GL_EXT_multi_draw_arrays;
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSEXTPROC)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
GLAPI PFNGLMULTIDRAWARRAYSEXTPROC glad_glMultiDrawArraysEXT;
#define glMultiDrawArraysEXT glad_glMultiDrawArraysEXT
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSEXTPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount);
GLAPI PFNGLMULTIDRAWELEMENTSEXTPROC glad_glMultiDrawElementsEXT;
#define glMultiDrawElementsEXT glad_glMultiDrawElementsEXT
#endif
#ifndef GL_EXT_multi_draw_indirect
#define GL_EXT_multi_draw_indirect 1
GLAPI int GLAD_GL_EXT_multi_draw_indirect;
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSINDIRECTEXTPROC)(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
GLAPI PFNGLMULTIDRAWARRAYSINDIRECTEXTPROC glad_glMultiDrawArraysIndirectEXT;
#define glMultiDrawArraysIndirectEXT glad_glMultiDrawArraysIndirectEXT
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSINDIRECTEXTPROC)(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
GLAPI PFNGLMULTIDRAWELEMENTSINDIRECTEXTPROC glad_glMultiDrawElementsIndirectEXT;
#define glMultiDrawElementsIndirectEXT glad_glMultiDrawElementsIndirectEXT
#endif
#ifndef GL_EXT_multisampled_compatibility
#define GL_EXT_multisampled_compatibility 1
GLAPI int GLAD_GL_EXT_multisampled_compatibility;
#endif
#ifndef GL_EXT_multisampled_render_to_texture
#define GL_EXT_multisampled_render_to_texture 1
GLAPI int GLAD_GL_EXT_multisampled_render_to_texture;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glad_glRenderbufferStorageMultisampleEXT;
#define glRenderbufferStorageMultisampleEXT glad_glRenderbufferStorageMultisampleEXT
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
GLAPI PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glad_glFramebufferTexture2DMultisampleEXT;
#define glFramebufferTexture2DMultisampleEXT glad_glFramebufferTexture2DMultisampleEXT
#endif
#ifndef GL_EXT_multisampled_render_to_texture2
#define GL_EXT_multisampled_render_to_texture2 1
GLAPI int GLAD_GL_EXT_multisampled_render_to_texture2;
#endif
#ifndef GL_EXT_multiview_draw_buffers
#define GL_EXT_multiview_draw_buffers 1
GLAPI int GLAD_GL_EXT_multiview_draw_buffers;
typedef void (APIENTRYP PFNGLREADBUFFERINDEXEDEXTPROC)(GLenum src, GLint index);
GLAPI PFNGLREADBUFFERINDEXEDEXTPROC glad_glReadBufferIndexedEXT;
#define glReadBufferIndexedEXT glad_glReadBufferIndexedEXT
typedef void (APIENTRYP PFNGLDRAWBUFFERSINDEXEDEXTPROC)(GLint n, const GLenum *location, const GLint *indices);
GLAPI PFNGLDRAWBUFFERSINDEXEDEXTPROC glad_glDrawBuffersIndexedEXT;
#define glDrawBuffersIndexedEXT glad_glDrawBuffersIndexedEXT
typedef void (APIENTRYP PFNGLGETINTEGERI_VEXTPROC)(GLenum target, GLuint index, GLint *data);
GLAPI PFNGLGETINTEGERI_VEXTPROC glad_glGetIntegeri_vEXT;
#define glGetIntegeri_vEXT glad_glGetIntegeri_vEXT
#endif
#ifndef GL_EXT_multiview_tessellation_geometry_shader
#define GL_EXT_multiview_tessellation_geometry_shader 1
GLAPI int GLAD_GL_EXT_multiview_tessellation_geometry_shader;
#endif
#ifndef GL_EXT_multiview_texture_multisample
#define GL_EXT_multiview_texture_multisample 1
GLAPI int GLAD_GL_EXT_multiview_texture_multisample;
#endif
#ifndef GL_EXT_multiview_timer_query
#define GL_EXT_multiview_timer_query 1
GLAPI int GLAD_GL_EXT_multiview_timer_query;
#endif
#ifndef GL_EXT_occlusion_query_boolean
#define GL_EXT_occlusion_query_boolean 1
GLAPI int GLAD_GL_EXT_occlusion_query_boolean;
#endif
#ifndef GL_EXT_polygon_offset_clamp
#define GL_EXT_polygon_offset_clamp 1
GLAPI int GLAD_GL_EXT_polygon_offset_clamp;
typedef void (APIENTRYP PFNGLPOLYGONOFFSETCLAMPEXTPROC)(GLfloat factor, GLfloat units, GLfloat clamp);
GLAPI PFNGLPOLYGONOFFSETCLAMPEXTPROC glad_glPolygonOffsetClampEXT;
#define glPolygonOffsetClampEXT glad_glPolygonOffsetClampEXT
#endif
#ifndef GL_EXT_post_depth_coverage
#define GL_EXT_post_depth_coverage 1
GLAPI int GLAD_GL_EXT_post_depth_coverage;
#endif
#ifndef GL_EXT_primitive_bounding_box
#define GL_EXT_primitive_bounding_box 1
GLAPI int GLAD_GL_EXT_primitive_bounding_box;
typedef void (APIENTRYP PFNGLPRIMITIVEBOUNDINGBOXEXTPROC)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
GLAPI PFNGLPRIMITIVEBOUNDINGBOXEXTPROC glad_glPrimitiveBoundingBoxEXT;
#define glPrimitiveBoundingBoxEXT glad_glPrimitiveBoundingBoxEXT
#endif
#ifndef GL_EXT_protected_textures
#define GL_EXT_protected_textures 1
GLAPI int GLAD_GL_EXT_protected_textures;
#endif
#ifndef GL_EXT_pvrtc_sRGB
#define GL_EXT_pvrtc_sRGB 1
GLAPI int GLAD_GL_EXT_pvrtc_sRGB;
#endif
#ifndef GL_EXT_raster_multisample
#define GL_EXT_raster_multisample 1
GLAPI int GLAD_GL_EXT_raster_multisample;
typedef void (APIENTRYP PFNGLRASTERSAMPLESEXTPROC)(GLuint samples, GLboolean fixedsamplelocations);
GLAPI PFNGLRASTERSAMPLESEXTPROC glad_glRasterSamplesEXT;
#define glRasterSamplesEXT glad_glRasterSamplesEXT
#endif
#ifndef GL_EXT_read_format_bgra
#define GL_EXT_read_format_bgra 1
GLAPI int GLAD_GL_EXT_read_format_bgra;
#endif
#ifndef GL_EXT_render_snorm
#define GL_EXT_render_snorm 1
GLAPI int GLAD_GL_EXT_render_snorm;
#endif
#ifndef GL_EXT_robustness
#define GL_EXT_robustness 1
GLAPI int GLAD_GL_EXT_robustness;
typedef GLenum (APIENTRYP PFNGLGETGRAPHICSRESETSTATUSEXTPROC)(void);
GLAPI PFNGLGETGRAPHICSRESETSTATUSEXTPROC glad_glGetGraphicsResetStatusEXT;
#define glGetGraphicsResetStatusEXT glad_glGetGraphicsResetStatusEXT
typedef void (APIENTRYP PFNGLREADNPIXELSEXTPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GLAPI PFNGLREADNPIXELSEXTPROC glad_glReadnPixelsEXT;
#define glReadnPixelsEXT glad_glReadnPixelsEXT
typedef void (APIENTRYP PFNGLGETNUNIFORMFVEXTPROC)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI PFNGLGETNUNIFORMFVEXTPROC glad_glGetnUniformfvEXT;
#define glGetnUniformfvEXT glad_glGetnUniformfvEXT
typedef void (APIENTRYP PFNGLGETNUNIFORMIVEXTPROC)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI PFNGLGETNUNIFORMIVEXTPROC glad_glGetnUniformivEXT;
#define glGetnUniformivEXT glad_glGetnUniformivEXT
#endif
#ifndef GL_EXT_sRGB
#define GL_EXT_sRGB 1
GLAPI int GLAD_GL_EXT_sRGB;
#endif
#ifndef GL_EXT_sRGB_write_control
#define GL_EXT_sRGB_write_control 1
GLAPI int GLAD_GL_EXT_sRGB_write_control;
#endif
#ifndef GL_EXT_semaphore
#define GL_EXT_semaphore 1
GLAPI int GLAD_GL_EXT_semaphore;
typedef void (APIENTRYP PFNGLGENSEMAPHORESEXTPROC)(GLsizei n, GLuint *semaphores);
GLAPI PFNGLGENSEMAPHORESEXTPROC glad_glGenSemaphoresEXT;
#define glGenSemaphoresEXT glad_glGenSemaphoresEXT
typedef void (APIENTRYP PFNGLDELETESEMAPHORESEXTPROC)(GLsizei n, const GLuint *semaphores);
GLAPI PFNGLDELETESEMAPHORESEXTPROC glad_glDeleteSemaphoresEXT;
#define glDeleteSemaphoresEXT glad_glDeleteSemaphoresEXT
typedef GLboolean (APIENTRYP PFNGLISSEMAPHOREEXTPROC)(GLuint semaphore);
GLAPI PFNGLISSEMAPHOREEXTPROC glad_glIsSemaphoreEXT;
#define glIsSemaphoreEXT glad_glIsSemaphoreEXT
typedef void (APIENTRYP PFNGLSEMAPHOREPARAMETERUI64VEXTPROC)(GLuint semaphore, GLenum pname, const GLuint64 *params);
GLAPI PFNGLSEMAPHOREPARAMETERUI64VEXTPROC glad_glSemaphoreParameterui64vEXT;
#define glSemaphoreParameterui64vEXT glad_glSemaphoreParameterui64vEXT
typedef void (APIENTRYP PFNGLGETSEMAPHOREPARAMETERUI64VEXTPROC)(GLuint semaphore, GLenum pname, GLuint64 *params);
GLAPI PFNGLGETSEMAPHOREPARAMETERUI64VEXTPROC glad_glGetSemaphoreParameterui64vEXT;
#define glGetSemaphoreParameterui64vEXT glad_glGetSemaphoreParameterui64vEXT
typedef void (APIENTRYP PFNGLWAITSEMAPHOREEXTPROC)(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *srcLayouts);
GLAPI PFNGLWAITSEMAPHOREEXTPROC glad_glWaitSemaphoreEXT;
#define glWaitSemaphoreEXT glad_glWaitSemaphoreEXT
typedef void (APIENTRYP PFNGLSIGNALSEMAPHOREEXTPROC)(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *dstLayouts);
GLAPI PFNGLSIGNALSEMAPHOREEXTPROC glad_glSignalSemaphoreEXT;
#define glSignalSemaphoreEXT glad_glSignalSemaphoreEXT
#endif
#ifndef GL_EXT_semaphore_fd
#define GL_EXT_semaphore_fd 1
GLAPI int GLAD_GL_EXT_semaphore_fd;
typedef void (APIENTRYP PFNGLIMPORTSEMAPHOREFDEXTPROC)(GLuint semaphore, GLenum handleType, GLint fd);
GLAPI PFNGLIMPORTSEMAPHOREFDEXTPROC glad_glImportSemaphoreFdEXT;
#define glImportSemaphoreFdEXT glad_glImportSemaphoreFdEXT
#endif
#ifndef GL_EXT_semaphore_win32
#define GL_EXT_semaphore_win32 1
GLAPI int GLAD_GL_EXT_semaphore_win32;
typedef void (APIENTRYP PFNGLIMPORTSEMAPHOREWIN32HANDLEEXTPROC)(GLuint semaphore, GLenum handleType, void *handle);
GLAPI PFNGLIMPORTSEMAPHOREWIN32HANDLEEXTPROC glad_glImportSemaphoreWin32HandleEXT;
#define glImportSemaphoreWin32HandleEXT glad_glImportSemaphoreWin32HandleEXT
typedef void (APIENTRYP PFNGLIMPORTSEMAPHOREWIN32NAMEEXTPROC)(GLuint semaphore, GLenum handleType, const void *name);
GLAPI PFNGLIMPORTSEMAPHOREWIN32NAMEEXTPROC glad_glImportSemaphoreWin32NameEXT;
#define glImportSemaphoreWin32NameEXT glad_glImportSemaphoreWin32NameEXT
#endif
#ifndef GL_EXT_separate_shader_objects
#define GL_EXT_separate_shader_objects 1
GLAPI int GLAD_GL_EXT_separate_shader_objects;
typedef void (APIENTRYP PFNGLUSESHADERPROGRAMEXTPROC)(GLenum type, GLuint program);
GLAPI PFNGLUSESHADERPROGRAMEXTPROC glad_glUseShaderProgramEXT;
#define glUseShaderProgramEXT glad_glUseShaderProgramEXT
typedef void (APIENTRYP PFNGLACTIVEPROGRAMEXTPROC)(GLuint program);
GLAPI PFNGLACTIVEPROGRAMEXTPROC glad_glActiveProgramEXT;
#define glActiveProgramEXT glad_glActiveProgramEXT
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROGRAMEXTPROC)(GLenum type, const GLchar *string);
GLAPI PFNGLCREATESHADERPROGRAMEXTPROC glad_glCreateShaderProgramEXT;
#define glCreateShaderProgramEXT glad_glCreateShaderProgramEXT
typedef void (APIENTRYP PFNGLACTIVESHADERPROGRAMEXTPROC)(GLuint pipeline, GLuint program);
GLAPI PFNGLACTIVESHADERPROGRAMEXTPROC glad_glActiveShaderProgramEXT;
#define glActiveShaderProgramEXT glad_glActiveShaderProgramEXT
typedef void (APIENTRYP PFNGLBINDPROGRAMPIPELINEEXTPROC)(GLuint pipeline);
GLAPI PFNGLBINDPROGRAMPIPELINEEXTPROC glad_glBindProgramPipelineEXT;
#define glBindProgramPipelineEXT glad_glBindProgramPipelineEXT
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROGRAMVEXTPROC)(GLenum type, GLsizei count, const GLchar **strings);
GLAPI PFNGLCREATESHADERPROGRAMVEXTPROC glad_glCreateShaderProgramvEXT;
#define glCreateShaderProgramvEXT glad_glCreateShaderProgramvEXT
typedef void (APIENTRYP PFNGLDELETEPROGRAMPIPELINESEXTPROC)(GLsizei n, const GLuint *pipelines);
GLAPI PFNGLDELETEPROGRAMPIPELINESEXTPROC glad_glDeleteProgramPipelinesEXT;
#define glDeleteProgramPipelinesEXT glad_glDeleteProgramPipelinesEXT
typedef void (APIENTRYP PFNGLGENPROGRAMPIPELINESEXTPROC)(GLsizei n, GLuint *pipelines);
GLAPI PFNGLGENPROGRAMPIPELINESEXTPROC glad_glGenProgramPipelinesEXT;
#define glGenProgramPipelinesEXT glad_glGenProgramPipelinesEXT
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC glad_glGetProgramPipelineInfoLogEXT;
#define glGetProgramPipelineInfoLogEXT glad_glGetProgramPipelineInfoLogEXT
typedef void (APIENTRYP PFNGLGETPROGRAMPIPELINEIVEXTPROC)(GLuint pipeline, GLenum pname, GLint *params);
GLAPI PFNGLGETPROGRAMPIPELINEIVEXTPROC glad_glGetProgramPipelineivEXT;
#define glGetProgramPipelineivEXT glad_glGetProgramPipelineivEXT
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPIPELINEEXTPROC)(GLuint pipeline);
GLAPI PFNGLISPROGRAMPIPELINEEXTPROC glad_glIsProgramPipelineEXT;
#define glIsProgramPipelineEXT glad_glIsProgramPipelineEXT
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERIEXTPROC)(GLuint program, GLenum pname, GLint value);
GLAPI PFNGLPROGRAMPARAMETERIEXTPROC glad_glProgramParameteriEXT;
#define glProgramParameteriEXT glad_glProgramParameteriEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FEXTPROC)(GLuint program, GLint location, GLfloat v0);
GLAPI PFNGLPROGRAMUNIFORM1FEXTPROC glad_glProgramUniform1fEXT;
#define glProgramUniform1fEXT glad_glProgramUniform1fEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1FVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM1FVEXTPROC glad_glProgramUniform1fvEXT;
#define glProgramUniform1fvEXT glad_glProgramUniform1fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IEXTPROC)(GLuint program, GLint location, GLint v0);
GLAPI PFNGLPROGRAMUNIFORM1IEXTPROC glad_glProgramUniform1iEXT;
#define glProgramUniform1iEXT glad_glProgramUniform1iEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1IVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM1IVEXTPROC glad_glProgramUniform1ivEXT;
#define glProgramUniform1ivEXT glad_glProgramUniform1ivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FEXTPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
GLAPI PFNGLPROGRAMUNIFORM2FEXTPROC glad_glProgramUniform2fEXT;
#define glProgramUniform2fEXT glad_glProgramUniform2fEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2FVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM2FVEXTPROC glad_glProgramUniform2fvEXT;
#define glProgramUniform2fvEXT glad_glProgramUniform2fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IEXTPROC)(GLuint program, GLint location, GLint v0, GLint v1);
GLAPI PFNGLPROGRAMUNIFORM2IEXTPROC glad_glProgramUniform2iEXT;
#define glProgramUniform2iEXT glad_glProgramUniform2iEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2IVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM2IVEXTPROC glad_glProgramUniform2ivEXT;
#define glProgramUniform2ivEXT glad_glProgramUniform2ivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FEXTPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI PFNGLPROGRAMUNIFORM3FEXTPROC glad_glProgramUniform3fEXT;
#define glProgramUniform3fEXT glad_glProgramUniform3fEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3FVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM3FVEXTPROC glad_glProgramUniform3fvEXT;
#define glProgramUniform3fvEXT glad_glProgramUniform3fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IEXTPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
GLAPI PFNGLPROGRAMUNIFORM3IEXTPROC glad_glProgramUniform3iEXT;
#define glProgramUniform3iEXT glad_glProgramUniform3iEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3IVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM3IVEXTPROC glad_glProgramUniform3ivEXT;
#define glProgramUniform3ivEXT glad_glProgramUniform3ivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FEXTPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI PFNGLPROGRAMUNIFORM4FEXTPROC glad_glProgramUniform4fEXT;
#define glProgramUniform4fEXT glad_glProgramUniform4fEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4FVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORM4FVEXTPROC glad_glProgramUniform4fvEXT;
#define glProgramUniform4fvEXT glad_glProgramUniform4fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IEXTPROC)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI PFNGLPROGRAMUNIFORM4IEXTPROC glad_glProgramUniform4iEXT;
#define glProgramUniform4iEXT glad_glProgramUniform4iEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4IVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLPROGRAMUNIFORM4IVEXTPROC glad_glProgramUniform4ivEXT;
#define glProgramUniform4ivEXT glad_glProgramUniform4ivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC glad_glProgramUniformMatrix2fvEXT;
#define glProgramUniformMatrix2fvEXT glad_glProgramUniformMatrix2fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC glad_glProgramUniformMatrix3fvEXT;
#define glProgramUniformMatrix3fvEXT glad_glProgramUniformMatrix3fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC glad_glProgramUniformMatrix4fvEXT;
#define glProgramUniformMatrix4fvEXT glad_glProgramUniformMatrix4fvEXT
typedef void (APIENTRYP PFNGLUSEPROGRAMSTAGESEXTPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
GLAPI PFNGLUSEPROGRAMSTAGESEXTPROC glad_glUseProgramStagesEXT;
#define glUseProgramStagesEXT glad_glUseProgramStagesEXT
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPIPELINEEXTPROC)(GLuint pipeline);
GLAPI PFNGLVALIDATEPROGRAMPIPELINEEXTPROC glad_glValidateProgramPipelineEXT;
#define glValidateProgramPipelineEXT glad_glValidateProgramPipelineEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIEXTPROC)(GLuint program, GLint location, GLuint v0);
GLAPI PFNGLPROGRAMUNIFORM1UIEXTPROC glad_glProgramUniform1uiEXT;
#define glProgramUniform1uiEXT glad_glProgramUniform1uiEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIEXTPROC)(GLuint program, GLint location, GLuint v0, GLuint v1);
GLAPI PFNGLPROGRAMUNIFORM2UIEXTPROC glad_glProgramUniform2uiEXT;
#define glProgramUniform2uiEXT glad_glProgramUniform2uiEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIEXTPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI PFNGLPROGRAMUNIFORM3UIEXTPROC glad_glProgramUniform3uiEXT;
#define glProgramUniform3uiEXT glad_glProgramUniform3uiEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIEXTPROC)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI PFNGLPROGRAMUNIFORM4UIEXTPROC glad_glProgramUniform4uiEXT;
#define glProgramUniform4uiEXT glad_glProgramUniform4uiEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UIVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM1UIVEXTPROC glad_glProgramUniform1uivEXT;
#define glProgramUniform1uivEXT glad_glProgramUniform1uivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UIVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM2UIVEXTPROC glad_glProgramUniform2uivEXT;
#define glProgramUniform2uivEXT glad_glProgramUniform2uivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UIVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM3UIVEXTPROC glad_glProgramUniform3uivEXT;
#define glProgramUniform3uivEXT glad_glProgramUniform3uivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UIVEXTPROC)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLPROGRAMUNIFORM4UIVEXTPROC glad_glProgramUniform4uivEXT;
#define glProgramUniform4uivEXT glad_glProgramUniform4uivEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC glad_glProgramUniformMatrix2x3fvEXT;
#define glProgramUniformMatrix2x3fvEXT glad_glProgramUniformMatrix2x3fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC glad_glProgramUniformMatrix3x2fvEXT;
#define glProgramUniformMatrix3x2fvEXT glad_glProgramUniformMatrix3x2fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC glad_glProgramUniformMatrix2x4fvEXT;
#define glProgramUniformMatrix2x4fvEXT glad_glProgramUniformMatrix2x4fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC glad_glProgramUniformMatrix4x2fvEXT;
#define glProgramUniformMatrix4x2fvEXT glad_glProgramUniformMatrix4x2fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC glad_glProgramUniformMatrix3x4fvEXT;
#define glProgramUniformMatrix3x4fvEXT glad_glProgramUniformMatrix3x4fvEXT
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC glad_glProgramUniformMatrix4x3fvEXT;
#define glProgramUniformMatrix4x3fvEXT glad_glProgramUniformMatrix4x3fvEXT
#endif
#ifndef GL_EXT_shader_framebuffer_fetch
#define GL_EXT_shader_framebuffer_fetch 1
GLAPI int GLAD_GL_EXT_shader_framebuffer_fetch;
#endif
#ifndef GL_EXT_shader_framebuffer_fetch_non_coherent
#define GL_EXT_shader_framebuffer_fetch_non_coherent 1
GLAPI int GLAD_GL_EXT_shader_framebuffer_fetch_non_coherent;
typedef void (APIENTRYP PFNGLFRAMEBUFFERFETCHBARRIEREXTPROC)(void);
GLAPI PFNGLFRAMEBUFFERFETCHBARRIEREXTPROC glad_glFramebufferFetchBarrierEXT;
#define glFramebufferFetchBarrierEXT glad_glFramebufferFetchBarrierEXT
#endif
#ifndef GL_EXT_shader_group_vote
#define GL_EXT_shader_group_vote 1
GLAPI int GLAD_GL_EXT_shader_group_vote;
#endif
#ifndef GL_EXT_shader_implicit_conversions
#define GL_EXT_shader_implicit_conversions 1
GLAPI int GLAD_GL_EXT_shader_implicit_conversions;
#endif
#ifndef GL_EXT_shader_integer_mix
#define GL_EXT_shader_integer_mix 1
GLAPI int GLAD_GL_EXT_shader_integer_mix;
#endif
#ifndef GL_EXT_shader_io_blocks
#define GL_EXT_shader_io_blocks 1
GLAPI int GLAD_GL_EXT_shader_io_blocks;
#endif
#ifndef GL_EXT_shader_non_constant_global_initializers
#define GL_EXT_shader_non_constant_global_initializers 1
GLAPI int GLAD_GL_EXT_shader_non_constant_global_initializers;
#endif
#ifndef GL_EXT_shader_pixel_local_storage
#define GL_EXT_shader_pixel_local_storage 1
GLAPI int GLAD_GL_EXT_shader_pixel_local_storage;
#endif
#ifndef GL_EXT_shader_pixel_local_storage2
#define GL_EXT_shader_pixel_local_storage2 1
GLAPI int GLAD_GL_EXT_shader_pixel_local_storage2;
typedef void (APIENTRYP PFNGLFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC)(GLuint target, GLsizei size);
GLAPI PFNGLFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC glad_glFramebufferPixelLocalStorageSizeEXT;
#define glFramebufferPixelLocalStorageSizeEXT glad_glFramebufferPixelLocalStorageSizeEXT
typedef GLsizei (APIENTRYP PFNGLGETFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC)(GLuint target);
GLAPI PFNGLGETFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC glad_glGetFramebufferPixelLocalStorageSizeEXT;
#define glGetFramebufferPixelLocalStorageSizeEXT glad_glGetFramebufferPixelLocalStorageSizeEXT
typedef void (APIENTRYP PFNGLCLEARPIXELLOCALSTORAGEUIEXTPROC)(GLsizei offset, GLsizei n, const GLuint *values);
GLAPI PFNGLCLEARPIXELLOCALSTORAGEUIEXTPROC glad_glClearPixelLocalStorageuiEXT;
#define glClearPixelLocalStorageuiEXT glad_glClearPixelLocalStorageuiEXT
#endif
#ifndef GL_EXT_shader_texture_lod
#define GL_EXT_shader_texture_lod 1
GLAPI int GLAD_GL_EXT_shader_texture_lod;
#endif
#ifndef GL_EXT_shadow_samplers
#define GL_EXT_shadow_samplers 1
GLAPI int GLAD_GL_EXT_shadow_samplers;
#endif
#ifndef GL_EXT_sparse_texture
#define GL_EXT_sparse_texture 1
GLAPI int GLAD_GL_EXT_sparse_texture;
typedef void (APIENTRYP PFNGLTEXPAGECOMMITMENTEXTPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
GLAPI PFNGLTEXPAGECOMMITMENTEXTPROC glad_glTexPageCommitmentEXT;
#define glTexPageCommitmentEXT glad_glTexPageCommitmentEXT
#endif
#ifndef GL_EXT_sparse_texture2
#define GL_EXT_sparse_texture2 1
GLAPI int GLAD_GL_EXT_sparse_texture2;
#endif
#ifndef GL_EXT_tessellation_point_size
#define GL_EXT_tessellation_point_size 1
GLAPI int GLAD_GL_EXT_tessellation_point_size;
#endif
#ifndef GL_EXT_tessellation_shader
#define GL_EXT_tessellation_shader 1
GLAPI int GLAD_GL_EXT_tessellation_shader;
typedef void (APIENTRYP PFNGLPATCHPARAMETERIEXTPROC)(GLenum pname, GLint value);
GLAPI PFNGLPATCHPARAMETERIEXTPROC glad_glPatchParameteriEXT;
#define glPatchParameteriEXT glad_glPatchParameteriEXT
#endif
#ifndef GL_EXT_texture_border_clamp
#define GL_EXT_texture_border_clamp 1
GLAPI int GLAD_GL_EXT_texture_border_clamp;
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVEXTPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIIVEXTPROC glad_glTexParameterIivEXT;
#define glTexParameterIivEXT glad_glTexParameterIivEXT
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVEXTPROC)(GLenum target, GLenum pname, const GLuint *params);
GLAPI PFNGLTEXPARAMETERIUIVEXTPROC glad_glTexParameterIuivEXT;
#define glTexParameterIuivEXT glad_glTexParameterIuivEXT
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVEXTPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIIVEXTPROC glad_glGetTexParameterIivEXT;
#define glGetTexParameterIivEXT glad_glGetTexParameterIivEXT
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVEXTPROC)(GLenum target, GLenum pname, GLuint *params);
GLAPI PFNGLGETTEXPARAMETERIUIVEXTPROC glad_glGetTexParameterIuivEXT;
#define glGetTexParameterIuivEXT glad_glGetTexParameterIuivEXT
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVEXTPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIIVEXTPROC glad_glSamplerParameterIivEXT;
#define glSamplerParameterIivEXT glad_glSamplerParameterIivEXT
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVEXTPROC)(GLuint sampler, GLenum pname, const GLuint *param);
GLAPI PFNGLSAMPLERPARAMETERIUIVEXTPROC glad_glSamplerParameterIuivEXT;
#define glSamplerParameterIuivEXT glad_glSamplerParameterIuivEXT
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVEXTPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIIVEXTPROC glad_glGetSamplerParameterIivEXT;
#define glGetSamplerParameterIivEXT glad_glGetSamplerParameterIivEXT
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIUIVEXTPROC)(GLuint sampler, GLenum pname, GLuint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIUIVEXTPROC glad_glGetSamplerParameterIuivEXT;
#define glGetSamplerParameterIuivEXT glad_glGetSamplerParameterIuivEXT
#endif
#ifndef GL_EXT_texture_buffer
#define GL_EXT_texture_buffer 1
GLAPI int GLAD_GL_EXT_texture_buffer;
typedef void (APIENTRYP PFNGLTEXBUFFEREXTPROC)(GLenum target, GLenum internalformat, GLuint buffer);
GLAPI PFNGLTEXBUFFEREXTPROC glad_glTexBufferEXT;
#define glTexBufferEXT glad_glTexBufferEXT
typedef void (APIENTRYP PFNGLTEXBUFFERRANGEEXTPROC)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI PFNGLTEXBUFFERRANGEEXTPROC glad_glTexBufferRangeEXT;
#define glTexBufferRangeEXT glad_glTexBufferRangeEXT
#endif
#ifndef GL_EXT_texture_compression_astc_decode_mode
#define GL_EXT_texture_compression_astc_decode_mode 1
GLAPI int GLAD_GL_EXT_texture_compression_astc_decode_mode;
#endif
#ifndef GL_EXT_texture_compression_bptc
#define GL_EXT_texture_compression_bptc 1
GLAPI int GLAD_GL_EXT_texture_compression_bptc;
#endif
#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1
GLAPI int GLAD_GL_EXT_texture_compression_dxt1;
#endif
#ifndef GL_EXT_texture_compression_rgtc
#define GL_EXT_texture_compression_rgtc 1
GLAPI int GLAD_GL_EXT_texture_compression_rgtc;
#endif
#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1
GLAPI int GLAD_GL_EXT_texture_compression_s3tc;
#endif
#ifndef GL_EXT_texture_compression_s3tc_srgb
#define GL_EXT_texture_compression_s3tc_srgb 1
GLAPI int GLAD_GL_EXT_texture_compression_s3tc_srgb;
#endif
#ifndef GL_EXT_texture_cube_map_array
#define GL_EXT_texture_cube_map_array 1
GLAPI int GLAD_GL_EXT_texture_cube_map_array;
#endif
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1
GLAPI int GLAD_GL_EXT_texture_filter_anisotropic;
#endif
#ifndef GL_EXT_texture_filter_minmax
#define GL_EXT_texture_filter_minmax 1
GLAPI int GLAD_GL_EXT_texture_filter_minmax;
#endif
#ifndef GL_EXT_texture_format_BGRA8888
#define GL_EXT_texture_format_BGRA8888 1
GLAPI int GLAD_GL_EXT_texture_format_BGRA8888;
#endif
#ifndef GL_EXT_texture_format_sRGB_override
#define GL_EXT_texture_format_sRGB_override 1
GLAPI int GLAD_GL_EXT_texture_format_sRGB_override;
#endif
#ifndef GL_EXT_texture_mirror_clamp_to_edge
#define GL_EXT_texture_mirror_clamp_to_edge 1
GLAPI int GLAD_GL_EXT_texture_mirror_clamp_to_edge;
#endif
#ifndef GL_EXT_texture_norm16
#define GL_EXT_texture_norm16 1
GLAPI int GLAD_GL_EXT_texture_norm16;
#endif
#ifndef GL_EXT_texture_query_lod
#define GL_EXT_texture_query_lod 1
GLAPI int GLAD_GL_EXT_texture_query_lod;
#endif
#ifndef GL_EXT_texture_rg
#define GL_EXT_texture_rg 1
GLAPI int GLAD_GL_EXT_texture_rg;
#endif
#ifndef GL_EXT_texture_sRGB_R8
#define GL_EXT_texture_sRGB_R8 1
GLAPI int GLAD_GL_EXT_texture_sRGB_R8;
#endif
#ifndef GL_EXT_texture_sRGB_RG8
#define GL_EXT_texture_sRGB_RG8 1
GLAPI int GLAD_GL_EXT_texture_sRGB_RG8;
#endif
#ifndef GL_EXT_texture_sRGB_decode
#define GL_EXT_texture_sRGB_decode 1
GLAPI int GLAD_GL_EXT_texture_sRGB_decode;
#endif
#ifndef GL_EXT_texture_shadow_lod
#define GL_EXT_texture_shadow_lod 1
GLAPI int GLAD_GL_EXT_texture_shadow_lod;
#endif
#ifndef GL_EXT_texture_storage
#define GL_EXT_texture_storage 1
GLAPI int GLAD_GL_EXT_texture_storage;
typedef void (APIENTRYP PFNGLTEXSTORAGE1DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GLAPI PFNGLTEXSTORAGE1DEXTPROC glad_glTexStorage1DEXT;
#define glTexStorage1DEXT glad_glTexStorage1DEXT
typedef void (APIENTRYP PFNGLTEXSTORAGE2DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLTEXSTORAGE2DEXTPROC glad_glTexStorage2DEXT;
#define glTexStorage2DEXT glad_glTexStorage2DEXT
typedef void (APIENTRYP PFNGLTEXSTORAGE3DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI PFNGLTEXSTORAGE3DEXTPROC glad_glTexStorage3DEXT;
#define glTexStorage3DEXT glad_glTexStorage3DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGE1DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GLAPI PFNGLTEXTURESTORAGE1DEXTPROC glad_glTextureStorage1DEXT;
#define glTextureStorage1DEXT glad_glTextureStorage1DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGE2DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLTEXTURESTORAGE2DEXTPROC glad_glTextureStorage2DEXT;
#define glTextureStorage2DEXT glad_glTextureStorage2DEXT
typedef void (APIENTRYP PFNGLTEXTURESTORAGE3DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI PFNGLTEXTURESTORAGE3DEXTPROC glad_glTextureStorage3DEXT;
#define glTextureStorage3DEXT glad_glTextureStorage3DEXT
#endif
#ifndef GL_EXT_texture_storage_compression
#define GL_EXT_texture_storage_compression 1
GLAPI int GLAD_GL_EXT_texture_storage_compression;
typedef void (APIENTRYP PFNTEXSTORAGEATTRIBS2DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, const int *attrib_list);
GLAPI PFNTEXSTORAGEATTRIBS2DEXTPROC glad_TexStorageAttribs2DEXT;
#define TexStorageAttribs2DEXT glad_TexStorageAttribs2DEXT
typedef void (APIENTRYP PFNTEXSTORAGEATTRIBS3DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, const int *attrib_list);
GLAPI PFNTEXSTORAGEATTRIBS3DEXTPROC glad_TexStorageAttribs3DEXT;
#define TexStorageAttribs3DEXT glad_TexStorageAttribs3DEXT
#endif
#ifndef GL_EXT_texture_type_2_10_10_10_REV
#define GL_EXT_texture_type_2_10_10_10_REV 1
GLAPI int GLAD_GL_EXT_texture_type_2_10_10_10_REV;
#endif
#ifndef GL_EXT_texture_view
#define GL_EXT_texture_view 1
GLAPI int GLAD_GL_EXT_texture_view;
typedef void (APIENTRYP PFNGLTEXTUREVIEWEXTPROC)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
GLAPI PFNGLTEXTUREVIEWEXTPROC glad_glTextureViewEXT;
#define glTextureViewEXT glad_glTextureViewEXT
#endif
#ifndef GL_EXT_unpack_subimage
#define GL_EXT_unpack_subimage 1
GLAPI int GLAD_GL_EXT_unpack_subimage;
#endif
#ifndef GL_EXT_win32_keyed_mutex
#define GL_EXT_win32_keyed_mutex 1
GLAPI int GLAD_GL_EXT_win32_keyed_mutex;
typedef GLboolean (APIENTRYP PFNGLACQUIREKEYEDMUTEXWIN32EXTPROC)(GLuint memory, GLuint64 key, GLuint timeout);
GLAPI PFNGLACQUIREKEYEDMUTEXWIN32EXTPROC glad_glAcquireKeyedMutexWin32EXT;
#define glAcquireKeyedMutexWin32EXT glad_glAcquireKeyedMutexWin32EXT
typedef GLboolean (APIENTRYP PFNGLRELEASEKEYEDMUTEXWIN32EXTPROC)(GLuint memory, GLuint64 key);
GLAPI PFNGLRELEASEKEYEDMUTEXWIN32EXTPROC glad_glReleaseKeyedMutexWin32EXT;
#define glReleaseKeyedMutexWin32EXT glad_glReleaseKeyedMutexWin32EXT
#endif
#ifndef GL_EXT_window_rectangles
#define GL_EXT_window_rectangles 1
GLAPI int GLAD_GL_EXT_window_rectangles;
typedef void (APIENTRYP PFNGLWINDOWRECTANGLESEXTPROC)(GLenum mode, GLsizei count, const GLint *box);
GLAPI PFNGLWINDOWRECTANGLESEXTPROC glad_glWindowRectanglesEXT;
#define glWindowRectanglesEXT glad_glWindowRectanglesEXT
#endif
#ifndef GL_FJ_shader_binary_GCCSO
#define GL_FJ_shader_binary_GCCSO 1
GLAPI int GLAD_GL_FJ_shader_binary_GCCSO;
#endif
#ifndef GL_IMG_bindless_texture
#define GL_IMG_bindless_texture 1
GLAPI int GLAD_GL_IMG_bindless_texture;
typedef GLuint64 (APIENTRYP PFNGLGETTEXTUREHANDLEIMGPROC)(GLuint texture);
GLAPI PFNGLGETTEXTUREHANDLEIMGPROC glad_glGetTextureHandleIMG;
#define glGetTextureHandleIMG glad_glGetTextureHandleIMG
typedef GLuint64 (APIENTRYP PFNGLGETTEXTURESAMPLERHANDLEIMGPROC)(GLuint texture, GLuint sampler);
GLAPI PFNGLGETTEXTURESAMPLERHANDLEIMGPROC glad_glGetTextureSamplerHandleIMG;
#define glGetTextureSamplerHandleIMG glad_glGetTextureSamplerHandleIMG
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64IMGPROC)(GLint location, GLuint64 value);
GLAPI PFNGLUNIFORMHANDLEUI64IMGPROC glad_glUniformHandleui64IMG;
#define glUniformHandleui64IMG glad_glUniformHandleui64IMG
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64VIMGPROC)(GLint location, GLsizei count, const GLuint64 *value);
GLAPI PFNGLUNIFORMHANDLEUI64VIMGPROC glad_glUniformHandleui64vIMG;
#define glUniformHandleui64vIMG glad_glUniformHandleui64vIMG
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64IMGPROC)(GLuint program, GLint location, GLuint64 value);
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64IMGPROC glad_glProgramUniformHandleui64IMG;
#define glProgramUniformHandleui64IMG glad_glProgramUniformHandleui64IMG
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64VIMGPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 *values);
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64VIMGPROC glad_glProgramUniformHandleui64vIMG;
#define glProgramUniformHandleui64vIMG glad_glProgramUniformHandleui64vIMG
#endif
#ifndef GL_IMG_framebuffer_downsample
#define GL_IMG_framebuffer_downsample 1
GLAPI int GLAD_GL_IMG_framebuffer_downsample;
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DDOWNSAMPLEIMGPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint xscale, GLint yscale);
GLAPI PFNGLFRAMEBUFFERTEXTURE2DDOWNSAMPLEIMGPROC glad_glFramebufferTexture2DDownsampleIMG;
#define glFramebufferTexture2DDownsampleIMG glad_glFramebufferTexture2DDownsampleIMG
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERDOWNSAMPLEIMGPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer, GLint xscale, GLint yscale);
GLAPI PFNGLFRAMEBUFFERTEXTURELAYERDOWNSAMPLEIMGPROC glad_glFramebufferTextureLayerDownsampleIMG;
#define glFramebufferTextureLayerDownsampleIMG glad_glFramebufferTextureLayerDownsampleIMG
#endif
#ifndef GL_IMG_multisampled_render_to_texture
#define GL_IMG_multisampled_render_to_texture 1
GLAPI int GLAD_GL_IMG_multisampled_render_to_texture;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEIMGPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEIMGPROC glad_glRenderbufferStorageMultisampleIMG;
#define glRenderbufferStorageMultisampleIMG glad_glRenderbufferStorageMultisampleIMG
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEIMGPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
GLAPI PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEIMGPROC glad_glFramebufferTexture2DMultisampleIMG;
#define glFramebufferTexture2DMultisampleIMG glad_glFramebufferTexture2DMultisampleIMG
#endif
#ifndef GL_IMG_program_binary
#define GL_IMG_program_binary 1
GLAPI int GLAD_GL_IMG_program_binary;
#endif
#ifndef GL_IMG_read_format
#define GL_IMG_read_format 1
GLAPI int GLAD_GL_IMG_read_format;
#endif
#ifndef GL_IMG_shader_binary
#define GL_IMG_shader_binary 1
GLAPI int GLAD_GL_IMG_shader_binary;
#endif
#ifndef GL_IMG_texture_compression_pvrtc
#define GL_IMG_texture_compression_pvrtc 1
GLAPI int GLAD_GL_IMG_texture_compression_pvrtc;
#endif
#ifndef GL_IMG_texture_compression_pvrtc2
#define GL_IMG_texture_compression_pvrtc2 1
GLAPI int GLAD_GL_IMG_texture_compression_pvrtc2;
#endif
#ifndef GL_IMG_texture_filter_cubic
#define GL_IMG_texture_filter_cubic 1
GLAPI int GLAD_GL_IMG_texture_filter_cubic;
#endif
#ifndef GL_INTEL_blackhole_render
#define GL_INTEL_blackhole_render 1
GLAPI int GLAD_GL_INTEL_blackhole_render;
#endif
#ifndef GL_INTEL_conservative_rasterization
#define GL_INTEL_conservative_rasterization 1
GLAPI int GLAD_GL_INTEL_conservative_rasterization;
#endif
#ifndef GL_INTEL_framebuffer_CMAA
#define GL_INTEL_framebuffer_CMAA 1
GLAPI int GLAD_GL_INTEL_framebuffer_CMAA;
typedef void (APIENTRYP PFNGLAPPLYFRAMEBUFFERATTACHMENTCMAAINTELPROC)(void);
GLAPI PFNGLAPPLYFRAMEBUFFERATTACHMENTCMAAINTELPROC glad_glApplyFramebufferAttachmentCMAAINTEL;
#define glApplyFramebufferAttachmentCMAAINTEL glad_glApplyFramebufferAttachmentCMAAINTEL
#endif
#ifndef GL_INTEL_performance_query
#define GL_INTEL_performance_query 1
GLAPI int GLAD_GL_INTEL_performance_query;
typedef void (APIENTRYP PFNGLBEGINPERFQUERYINTELPROC)(GLuint queryHandle);
GLAPI PFNGLBEGINPERFQUERYINTELPROC glad_glBeginPerfQueryINTEL;
#define glBeginPerfQueryINTEL glad_glBeginPerfQueryINTEL
typedef void (APIENTRYP PFNGLCREATEPERFQUERYINTELPROC)(GLuint queryId, GLuint *queryHandle);
GLAPI PFNGLCREATEPERFQUERYINTELPROC glad_glCreatePerfQueryINTEL;
#define glCreatePerfQueryINTEL glad_glCreatePerfQueryINTEL
typedef void (APIENTRYP PFNGLDELETEPERFQUERYINTELPROC)(GLuint queryHandle);
GLAPI PFNGLDELETEPERFQUERYINTELPROC glad_glDeletePerfQueryINTEL;
#define glDeletePerfQueryINTEL glad_glDeletePerfQueryINTEL
typedef void (APIENTRYP PFNGLENDPERFQUERYINTELPROC)(GLuint queryHandle);
GLAPI PFNGLENDPERFQUERYINTELPROC glad_glEndPerfQueryINTEL;
#define glEndPerfQueryINTEL glad_glEndPerfQueryINTEL
typedef void (APIENTRYP PFNGLGETFIRSTPERFQUERYIDINTELPROC)(GLuint *queryId);
GLAPI PFNGLGETFIRSTPERFQUERYIDINTELPROC glad_glGetFirstPerfQueryIdINTEL;
#define glGetFirstPerfQueryIdINTEL glad_glGetFirstPerfQueryIdINTEL
typedef void (APIENTRYP PFNGLGETNEXTPERFQUERYIDINTELPROC)(GLuint queryId, GLuint *nextQueryId);
GLAPI PFNGLGETNEXTPERFQUERYIDINTELPROC glad_glGetNextPerfQueryIdINTEL;
#define glGetNextPerfQueryIdINTEL glad_glGetNextPerfQueryIdINTEL
typedef void (APIENTRYP PFNGLGETPERFCOUNTERINFOINTELPROC)(GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar *counterName, GLuint counterDescLength, GLchar *counterDesc, GLuint *counterOffset, GLuint *counterDataSize, GLuint *counterTypeEnum, GLuint *counterDataTypeEnum, GLuint64 *rawCounterMaxValue);
GLAPI PFNGLGETPERFCOUNTERINFOINTELPROC glad_glGetPerfCounterInfoINTEL;
#define glGetPerfCounterInfoINTEL glad_glGetPerfCounterInfoINTEL
typedef void (APIENTRYP PFNGLGETPERFQUERYDATAINTELPROC)(GLuint queryHandle, GLuint flags, GLsizei dataSize, void *data, GLuint *bytesWritten);
GLAPI PFNGLGETPERFQUERYDATAINTELPROC glad_glGetPerfQueryDataINTEL;
#define glGetPerfQueryDataINTEL glad_glGetPerfQueryDataINTEL
typedef void (APIENTRYP PFNGLGETPERFQUERYIDBYNAMEINTELPROC)(GLchar *queryName, GLuint *queryId);
GLAPI PFNGLGETPERFQUERYIDBYNAMEINTELPROC glad_glGetPerfQueryIdByNameINTEL;
#define glGetPerfQueryIdByNameINTEL glad_glGetPerfQueryIdByNameINTEL
typedef void (APIENTRYP PFNGLGETPERFQUERYINFOINTELPROC)(GLuint queryId, GLuint queryNameLength, GLchar *queryName, GLuint *dataSize, GLuint *noCounters, GLuint *noInstances, GLuint *capsMask);
GLAPI PFNGLGETPERFQUERYINFOINTELPROC glad_glGetPerfQueryInfoINTEL;
#define glGetPerfQueryInfoINTEL glad_glGetPerfQueryInfoINTEL
#endif
#ifndef GL_KHR_blend_equation_advanced
#define GL_KHR_blend_equation_advanced 1
GLAPI int GLAD_GL_KHR_blend_equation_advanced;
typedef void (APIENTRYP PFNGLBLENDBARRIERKHRPROC)(void);
GLAPI PFNGLBLENDBARRIERKHRPROC glad_glBlendBarrierKHR;
#define glBlendBarrierKHR glad_glBlendBarrierKHR
#endif
#ifndef GL_KHR_blend_equation_advanced_coherent
#define GL_KHR_blend_equation_advanced_coherent 1
GLAPI int GLAD_GL_KHR_blend_equation_advanced_coherent;
#endif
#ifndef GL_KHR_context_flush_control
#define GL_KHR_context_flush_control 1
GLAPI int GLAD_GL_KHR_context_flush_control;
#endif
#ifndef GL_KHR_debug
#define GL_KHR_debug 1
GLAPI int GLAD_GL_KHR_debug;
typedef void (APIENTRYP PFNGLDEBUGMESSAGECONTROLKHRPROC)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI PFNGLDEBUGMESSAGECONTROLKHRPROC glad_glDebugMessageControlKHR;
#define glDebugMessageControlKHR glad_glDebugMessageControlKHR
typedef void (APIENTRYP PFNGLDEBUGMESSAGEINSERTKHRPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GLAPI PFNGLDEBUGMESSAGEINSERTKHRPROC glad_glDebugMessageInsertKHR;
#define glDebugMessageInsertKHR glad_glDebugMessageInsertKHR
typedef void (APIENTRYP PFNGLDEBUGMESSAGECALLBACKKHRPROC)(GLDEBUGPROCKHR callback, const void *userParam);
GLAPI PFNGLDEBUGMESSAGECALLBACKKHRPROC glad_glDebugMessageCallbackKHR;
#define glDebugMessageCallbackKHR glad_glDebugMessageCallbackKHR
typedef GLuint (APIENTRYP PFNGLGETDEBUGMESSAGELOGKHRPROC)(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
GLAPI PFNGLGETDEBUGMESSAGELOGKHRPROC glad_glGetDebugMessageLogKHR;
#define glGetDebugMessageLogKHR glad_glGetDebugMessageLogKHR
typedef void (APIENTRYP PFNGLPUSHDEBUGGROUPKHRPROC)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
GLAPI PFNGLPUSHDEBUGGROUPKHRPROC glad_glPushDebugGroupKHR;
#define glPushDebugGroupKHR glad_glPushDebugGroupKHR
typedef void (APIENTRYP PFNGLPOPDEBUGGROUPKHRPROC)(void);
GLAPI PFNGLPOPDEBUGGROUPKHRPROC glad_glPopDebugGroupKHR;
#define glPopDebugGroupKHR glad_glPopDebugGroupKHR
typedef void (APIENTRYP PFNGLOBJECTLABELKHRPROC)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
GLAPI PFNGLOBJECTLABELKHRPROC glad_glObjectLabelKHR;
#define glObjectLabelKHR glad_glObjectLabelKHR
typedef void (APIENTRYP PFNGLGETOBJECTLABELKHRPROC)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI PFNGLGETOBJECTLABELKHRPROC glad_glGetObjectLabelKHR;
#define glGetObjectLabelKHR glad_glGetObjectLabelKHR
typedef void (APIENTRYP PFNGLOBJECTPTRLABELKHRPROC)(const void *ptr, GLsizei length, const GLchar *label);
GLAPI PFNGLOBJECTPTRLABELKHRPROC glad_glObjectPtrLabelKHR;
#define glObjectPtrLabelKHR glad_glObjectPtrLabelKHR
typedef void (APIENTRYP PFNGLGETOBJECTPTRLABELKHRPROC)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI PFNGLGETOBJECTPTRLABELKHRPROC glad_glGetObjectPtrLabelKHR;
#define glGetObjectPtrLabelKHR glad_glGetObjectPtrLabelKHR
typedef void (APIENTRYP PFNGLGETPOINTERVKHRPROC)(GLenum pname, void **params);
GLAPI PFNGLGETPOINTERVKHRPROC glad_glGetPointervKHR;
#define glGetPointervKHR glad_glGetPointervKHR
#endif
#ifndef GL_KHR_no_error
#define GL_KHR_no_error 1
GLAPI int GLAD_GL_KHR_no_error;
#endif
#ifndef GL_KHR_parallel_shader_compile
#define GL_KHR_parallel_shader_compile 1
GLAPI int GLAD_GL_KHR_parallel_shader_compile;
typedef void (APIENTRYP PFNGLMAXSHADERCOMPILERTHREADSKHRPROC)(GLuint count);
GLAPI PFNGLMAXSHADERCOMPILERTHREADSKHRPROC glad_glMaxShaderCompilerThreadsKHR;
#define glMaxShaderCompilerThreadsKHR glad_glMaxShaderCompilerThreadsKHR
#endif
#ifndef GL_KHR_robust_buffer_access_behavior
#define GL_KHR_robust_buffer_access_behavior 1
GLAPI int GLAD_GL_KHR_robust_buffer_access_behavior;
#endif
#ifndef GL_KHR_robustness
#define GL_KHR_robustness 1
GLAPI int GLAD_GL_KHR_robustness;
typedef GLenum (APIENTRYP PFNGLGETGRAPHICSRESETSTATUSKHRPROC)(void);
GLAPI PFNGLGETGRAPHICSRESETSTATUSKHRPROC glad_glGetGraphicsResetStatusKHR;
#define glGetGraphicsResetStatusKHR glad_glGetGraphicsResetStatusKHR
typedef void (APIENTRYP PFNGLREADNPIXELSKHRPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GLAPI PFNGLREADNPIXELSKHRPROC glad_glReadnPixelsKHR;
#define glReadnPixelsKHR glad_glReadnPixelsKHR
typedef void (APIENTRYP PFNGLGETNUNIFORMFVKHRPROC)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI PFNGLGETNUNIFORMFVKHRPROC glad_glGetnUniformfvKHR;
#define glGetnUniformfvKHR glad_glGetnUniformfvKHR
typedef void (APIENTRYP PFNGLGETNUNIFORMIVKHRPROC)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI PFNGLGETNUNIFORMIVKHRPROC glad_glGetnUniformivKHR;
#define glGetnUniformivKHR glad_glGetnUniformivKHR
typedef void (APIENTRYP PFNGLGETNUNIFORMUIVKHRPROC)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI PFNGLGETNUNIFORMUIVKHRPROC glad_glGetnUniformuivKHR;
#define glGetnUniformuivKHR glad_glGetnUniformuivKHR
#endif
#ifndef GL_KHR_shader_subgroup
#define GL_KHR_shader_subgroup 1
GLAPI int GLAD_GL_KHR_shader_subgroup;
#endif
#ifndef GL_KHR_texture_compression_astc_hdr
#define GL_KHR_texture_compression_astc_hdr 1
GLAPI int GLAD_GL_KHR_texture_compression_astc_hdr;
#endif
#ifndef GL_KHR_texture_compression_astc_ldr
#define GL_KHR_texture_compression_astc_ldr 1
GLAPI int GLAD_GL_KHR_texture_compression_astc_ldr;
#endif
#ifndef GL_KHR_texture_compression_astc_sliced_3d
#define GL_KHR_texture_compression_astc_sliced_3d 1
GLAPI int GLAD_GL_KHR_texture_compression_astc_sliced_3d;
#endif
#ifndef GL_MESA_bgra
#define GL_MESA_bgra 1
GLAPI int GLAD_GL_MESA_bgra;
#endif
#ifndef GL_MESA_framebuffer_flip_x
#define GL_MESA_framebuffer_flip_x 1
GLAPI int GLAD_GL_MESA_framebuffer_flip_x;
#endif
#ifndef GL_MESA_framebuffer_flip_y
#define GL_MESA_framebuffer_flip_y 1
GLAPI int GLAD_GL_MESA_framebuffer_flip_y;
typedef void (APIENTRYP PFNGLFRAMEBUFFERPARAMETERIMESAPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLFRAMEBUFFERPARAMETERIMESAPROC glad_glFramebufferParameteriMESA;
#define glFramebufferParameteriMESA glad_glFramebufferParameteriMESA
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERPARAMETERIVMESAPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETFRAMEBUFFERPARAMETERIVMESAPROC glad_glGetFramebufferParameterivMESA;
#define glGetFramebufferParameterivMESA glad_glGetFramebufferParameterivMESA
#endif
#ifndef GL_MESA_framebuffer_swap_xy
#define GL_MESA_framebuffer_swap_xy 1
GLAPI int GLAD_GL_MESA_framebuffer_swap_xy;
#endif
#ifndef GL_MESA_program_binary_formats
#define GL_MESA_program_binary_formats 1
GLAPI int GLAD_GL_MESA_program_binary_formats;
#endif
#ifndef GL_MESA_shader_integer_functions
#define GL_MESA_shader_integer_functions 1
GLAPI int GLAD_GL_MESA_shader_integer_functions;
#endif
#ifndef GL_NVX_blend_equation_advanced_multi_draw_buffers
#define GL_NVX_blend_equation_advanced_multi_draw_buffers 1
GLAPI int GLAD_GL_NVX_blend_equation_advanced_multi_draw_buffers;
#endif
#ifndef GL_NV_bindless_texture
#define GL_NV_bindless_texture 1
GLAPI int GLAD_GL_NV_bindless_texture;
typedef GLuint64 (APIENTRYP PFNGLGETTEXTUREHANDLENVPROC)(GLuint texture);
GLAPI PFNGLGETTEXTUREHANDLENVPROC glad_glGetTextureHandleNV;
#define glGetTextureHandleNV glad_glGetTextureHandleNV
typedef GLuint64 (APIENTRYP PFNGLGETTEXTURESAMPLERHANDLENVPROC)(GLuint texture, GLuint sampler);
GLAPI PFNGLGETTEXTURESAMPLERHANDLENVPROC glad_glGetTextureSamplerHandleNV;
#define glGetTextureSamplerHandleNV glad_glGetTextureSamplerHandleNV
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLERESIDENTNVPROC)(GLuint64 handle);
GLAPI PFNGLMAKETEXTUREHANDLERESIDENTNVPROC glad_glMakeTextureHandleResidentNV;
#define glMakeTextureHandleResidentNV glad_glMakeTextureHandleResidentNV
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC)(GLuint64 handle);
GLAPI PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC glad_glMakeTextureHandleNonResidentNV;
#define glMakeTextureHandleNonResidentNV glad_glMakeTextureHandleNonResidentNV
typedef GLuint64 (APIENTRYP PFNGLGETIMAGEHANDLENVPROC)(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
GLAPI PFNGLGETIMAGEHANDLENVPROC glad_glGetImageHandleNV;
#define glGetImageHandleNV glad_glGetImageHandleNV
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLERESIDENTNVPROC)(GLuint64 handle, GLenum access);
GLAPI PFNGLMAKEIMAGEHANDLERESIDENTNVPROC glad_glMakeImageHandleResidentNV;
#define glMakeImageHandleResidentNV glad_glMakeImageHandleResidentNV
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC)(GLuint64 handle);
GLAPI PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC glad_glMakeImageHandleNonResidentNV;
#define glMakeImageHandleNonResidentNV glad_glMakeImageHandleNonResidentNV
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64NVPROC)(GLint location, GLuint64 value);
GLAPI PFNGLUNIFORMHANDLEUI64NVPROC glad_glUniformHandleui64NV;
#define glUniformHandleui64NV glad_glUniformHandleui64NV
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64VNVPROC)(GLint location, GLsizei count, const GLuint64 *value);
GLAPI PFNGLUNIFORMHANDLEUI64VNVPROC glad_glUniformHandleui64vNV;
#define glUniformHandleui64vNV glad_glUniformHandleui64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC)(GLuint program, GLint location, GLuint64 value);
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC glad_glProgramUniformHandleui64NV;
#define glProgramUniformHandleui64NV glad_glProgramUniformHandleui64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLuint64 *values);
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC glad_glProgramUniformHandleui64vNV;
#define glProgramUniformHandleui64vNV glad_glProgramUniformHandleui64vNV
typedef GLboolean (APIENTRYP PFNGLISTEXTUREHANDLERESIDENTNVPROC)(GLuint64 handle);
GLAPI PFNGLISTEXTUREHANDLERESIDENTNVPROC glad_glIsTextureHandleResidentNV;
#define glIsTextureHandleResidentNV glad_glIsTextureHandleResidentNV
typedef GLboolean (APIENTRYP PFNGLISIMAGEHANDLERESIDENTNVPROC)(GLuint64 handle);
GLAPI PFNGLISIMAGEHANDLERESIDENTNVPROC glad_glIsImageHandleResidentNV;
#define glIsImageHandleResidentNV glad_glIsImageHandleResidentNV
#endif
#ifndef GL_NV_blend_equation_advanced
#define GL_NV_blend_equation_advanced 1
GLAPI int GLAD_GL_NV_blend_equation_advanced;
typedef void (APIENTRYP PFNGLBLENDPARAMETERINVPROC)(GLenum pname, GLint value);
GLAPI PFNGLBLENDPARAMETERINVPROC glad_glBlendParameteriNV;
#define glBlendParameteriNV glad_glBlendParameteriNV
typedef void (APIENTRYP PFNGLBLENDBARRIERNVPROC)(void);
GLAPI PFNGLBLENDBARRIERNVPROC glad_glBlendBarrierNV;
#define glBlendBarrierNV glad_glBlendBarrierNV
#endif
#ifndef GL_NV_blend_equation_advanced_coherent
#define GL_NV_blend_equation_advanced_coherent 1
GLAPI int GLAD_GL_NV_blend_equation_advanced_coherent;
#endif
#ifndef GL_NV_blend_minmax_factor
#define GL_NV_blend_minmax_factor 1
GLAPI int GLAD_GL_NV_blend_minmax_factor;
#endif
#ifndef GL_NV_clip_space_w_scaling
#define GL_NV_clip_space_w_scaling 1
GLAPI int GLAD_GL_NV_clip_space_w_scaling;
typedef void (APIENTRYP PFNGLVIEWPORTPOSITIONWSCALENVPROC)(GLuint index, GLfloat xcoeff, GLfloat ycoeff);
GLAPI PFNGLVIEWPORTPOSITIONWSCALENVPROC glad_glViewportPositionWScaleNV;
#define glViewportPositionWScaleNV glad_glViewportPositionWScaleNV
#endif
#ifndef GL_NV_compute_shader_derivatives
#define GL_NV_compute_shader_derivatives 1
GLAPI int GLAD_GL_NV_compute_shader_derivatives;
#endif
#ifndef GL_NV_conditional_render
#define GL_NV_conditional_render 1
GLAPI int GLAD_GL_NV_conditional_render;
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERNVPROC)(GLuint id, GLenum mode);
GLAPI PFNGLBEGINCONDITIONALRENDERNVPROC glad_glBeginConditionalRenderNV;
#define glBeginConditionalRenderNV glad_glBeginConditionalRenderNV
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERNVPROC)(void);
GLAPI PFNGLENDCONDITIONALRENDERNVPROC glad_glEndConditionalRenderNV;
#define glEndConditionalRenderNV glad_glEndConditionalRenderNV
#endif
#ifndef GL_NV_conservative_raster
#define GL_NV_conservative_raster 1
GLAPI int GLAD_GL_NV_conservative_raster;
typedef void (APIENTRYP PFNGLSUBPIXELPRECISIONBIASNVPROC)(GLuint xbits, GLuint ybits);
GLAPI PFNGLSUBPIXELPRECISIONBIASNVPROC glad_glSubpixelPrecisionBiasNV;
#define glSubpixelPrecisionBiasNV glad_glSubpixelPrecisionBiasNV
#endif
#ifndef GL_NV_conservative_raster_pre_snap
#define GL_NV_conservative_raster_pre_snap 1
GLAPI int GLAD_GL_NV_conservative_raster_pre_snap;
#endif
#ifndef GL_NV_conservative_raster_pre_snap_triangles
#define GL_NV_conservative_raster_pre_snap_triangles 1
GLAPI int GLAD_GL_NV_conservative_raster_pre_snap_triangles;
typedef void (APIENTRYP PFNGLCONSERVATIVERASTERPARAMETERINVPROC)(GLenum pname, GLint param);
GLAPI PFNGLCONSERVATIVERASTERPARAMETERINVPROC glad_glConservativeRasterParameteriNV;
#define glConservativeRasterParameteriNV glad_glConservativeRasterParameteriNV
#endif
#ifndef GL_NV_copy_buffer
#define GL_NV_copy_buffer 1
GLAPI int GLAD_GL_NV_copy_buffer;
typedef void (APIENTRYP PFNGLCOPYBUFFERSUBDATANVPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLAPI PFNGLCOPYBUFFERSUBDATANVPROC glad_glCopyBufferSubDataNV;
#define glCopyBufferSubDataNV glad_glCopyBufferSubDataNV
#endif
#ifndef GL_NV_coverage_sample
#define GL_NV_coverage_sample 1
GLAPI int GLAD_GL_NV_coverage_sample;
typedef void (APIENTRYP PFNGLCOVERAGEMASKNVPROC)(GLboolean mask);
GLAPI PFNGLCOVERAGEMASKNVPROC glad_glCoverageMaskNV;
#define glCoverageMaskNV glad_glCoverageMaskNV
typedef void (APIENTRYP PFNGLCOVERAGEOPERATIONNVPROC)(GLenum operation);
GLAPI PFNGLCOVERAGEOPERATIONNVPROC glad_glCoverageOperationNV;
#define glCoverageOperationNV glad_glCoverageOperationNV
#endif
#ifndef GL_NV_depth_nonlinear
#define GL_NV_depth_nonlinear 1
GLAPI int GLAD_GL_NV_depth_nonlinear;
#endif
#ifndef GL_NV_draw_buffers
#define GL_NV_draw_buffers 1
GLAPI int GLAD_GL_NV_draw_buffers;
typedef void (APIENTRYP PFNGLDRAWBUFFERSNVPROC)(GLsizei n, const GLenum *bufs);
GLAPI PFNGLDRAWBUFFERSNVPROC glad_glDrawBuffersNV;
#define glDrawBuffersNV glad_glDrawBuffersNV
#endif
#ifndef GL_NV_draw_instanced
#define GL_NV_draw_instanced 1
GLAPI int GLAD_GL_NV_draw_instanced;
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDNVPROC)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI PFNGLDRAWARRAYSINSTANCEDNVPROC glad_glDrawArraysInstancedNV;
#define glDrawArraysInstancedNV glad_glDrawArraysInstancedNV
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDNVPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);
GLAPI PFNGLDRAWELEMENTSINSTANCEDNVPROC glad_glDrawElementsInstancedNV;
#define glDrawElementsInstancedNV glad_glDrawElementsInstancedNV
#endif
#ifndef GL_NV_draw_vulkan_image
#define GL_NV_draw_vulkan_image 1
GLAPI int GLAD_GL_NV_draw_vulkan_image;
typedef void (APIENTRYP PFNGLDRAWVKIMAGENVPROC)(GLuint64 vkImage, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1);
GLAPI PFNGLDRAWVKIMAGENVPROC glad_glDrawVkImageNV;
#define glDrawVkImageNV glad_glDrawVkImageNV
typedef GLVULKANPROCNV (APIENTRYP PFNGLGETVKPROCADDRNVPROC)(const GLchar *name);
GLAPI PFNGLGETVKPROCADDRNVPROC glad_glGetVkProcAddrNV;
#define glGetVkProcAddrNV glad_glGetVkProcAddrNV
typedef void (APIENTRYP PFNGLWAITVKSEMAPHORENVPROC)(GLuint64 vkSemaphore);
GLAPI PFNGLWAITVKSEMAPHORENVPROC glad_glWaitVkSemaphoreNV;
#define glWaitVkSemaphoreNV glad_glWaitVkSemaphoreNV
typedef void (APIENTRYP PFNGLSIGNALVKSEMAPHORENVPROC)(GLuint64 vkSemaphore);
GLAPI PFNGLSIGNALVKSEMAPHORENVPROC glad_glSignalVkSemaphoreNV;
#define glSignalVkSemaphoreNV glad_glSignalVkSemaphoreNV
typedef void (APIENTRYP PFNGLSIGNALVKFENCENVPROC)(GLuint64 vkFence);
GLAPI PFNGLSIGNALVKFENCENVPROC glad_glSignalVkFenceNV;
#define glSignalVkFenceNV glad_glSignalVkFenceNV
#endif
#ifndef GL_NV_explicit_attrib_location
#define GL_NV_explicit_attrib_location 1
GLAPI int GLAD_GL_NV_explicit_attrib_location;
#endif
#ifndef GL_NV_fbo_color_attachments
#define GL_NV_fbo_color_attachments 1
GLAPI int GLAD_GL_NV_fbo_color_attachments;
#endif
#ifndef GL_NV_fence
#define GL_NV_fence 1
GLAPI int GLAD_GL_NV_fence;
typedef void (APIENTRYP PFNGLDELETEFENCESNVPROC)(GLsizei n, const GLuint *fences);
GLAPI PFNGLDELETEFENCESNVPROC glad_glDeleteFencesNV;
#define glDeleteFencesNV glad_glDeleteFencesNV
typedef void (APIENTRYP PFNGLGENFENCESNVPROC)(GLsizei n, GLuint *fences);
GLAPI PFNGLGENFENCESNVPROC glad_glGenFencesNV;
#define glGenFencesNV glad_glGenFencesNV
typedef GLboolean (APIENTRYP PFNGLISFENCENVPROC)(GLuint fence);
GLAPI PFNGLISFENCENVPROC glad_glIsFenceNV;
#define glIsFenceNV glad_glIsFenceNV
typedef GLboolean (APIENTRYP PFNGLTESTFENCENVPROC)(GLuint fence);
GLAPI PFNGLTESTFENCENVPROC glad_glTestFenceNV;
#define glTestFenceNV glad_glTestFenceNV
typedef void (APIENTRYP PFNGLGETFENCEIVNVPROC)(GLuint fence, GLenum pname, GLint *params);
GLAPI PFNGLGETFENCEIVNVPROC glad_glGetFenceivNV;
#define glGetFenceivNV glad_glGetFenceivNV
typedef void (APIENTRYP PFNGLFINISHFENCENVPROC)(GLuint fence);
GLAPI PFNGLFINISHFENCENVPROC glad_glFinishFenceNV;
#define glFinishFenceNV glad_glFinishFenceNV
typedef void (APIENTRYP PFNGLSETFENCENVPROC)(GLuint fence, GLenum condition);
GLAPI PFNGLSETFENCENVPROC glad_glSetFenceNV;
#define glSetFenceNV glad_glSetFenceNV
#endif
#ifndef GL_NV_fill_rectangle
#define GL_NV_fill_rectangle 1
GLAPI int GLAD_GL_NV_fill_rectangle;
#endif
#ifndef GL_NV_fragment_coverage_to_color
#define GL_NV_fragment_coverage_to_color 1
GLAPI int GLAD_GL_NV_fragment_coverage_to_color;
typedef void (APIENTRYP PFNGLFRAGMENTCOVERAGECOLORNVPROC)(GLuint color);
GLAPI PFNGLFRAGMENTCOVERAGECOLORNVPROC glad_glFragmentCoverageColorNV;
#define glFragmentCoverageColorNV glad_glFragmentCoverageColorNV
#endif
#ifndef GL_NV_fragment_shader_barycentric
#define GL_NV_fragment_shader_barycentric 1
GLAPI int GLAD_GL_NV_fragment_shader_barycentric;
#endif
#ifndef GL_NV_fragment_shader_interlock
#define GL_NV_fragment_shader_interlock 1
GLAPI int GLAD_GL_NV_fragment_shader_interlock;
#endif
#ifndef GL_NV_framebuffer_blit
#define GL_NV_framebuffer_blit 1
GLAPI int GLAD_GL_NV_framebuffer_blit;
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERNVPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI PFNGLBLITFRAMEBUFFERNVPROC glad_glBlitFramebufferNV;
#define glBlitFramebufferNV glad_glBlitFramebufferNV
#endif
#ifndef GL_NV_framebuffer_mixed_samples
#define GL_NV_framebuffer_mixed_samples 1
GLAPI int GLAD_GL_NV_framebuffer_mixed_samples;
typedef void (APIENTRYP PFNGLCOVERAGEMODULATIONTABLENVPROC)(GLsizei n, const GLfloat *v);
GLAPI PFNGLCOVERAGEMODULATIONTABLENVPROC glad_glCoverageModulationTableNV;
#define glCoverageModulationTableNV glad_glCoverageModulationTableNV
typedef void (APIENTRYP PFNGLGETCOVERAGEMODULATIONTABLENVPROC)(GLsizei bufSize, GLfloat *v);
GLAPI PFNGLGETCOVERAGEMODULATIONTABLENVPROC glad_glGetCoverageModulationTableNV;
#define glGetCoverageModulationTableNV glad_glGetCoverageModulationTableNV
typedef void (APIENTRYP PFNGLCOVERAGEMODULATIONNVPROC)(GLenum components);
GLAPI PFNGLCOVERAGEMODULATIONNVPROC glad_glCoverageModulationNV;
#define glCoverageModulationNV glad_glCoverageModulationNV
#endif
#ifndef GL_NV_framebuffer_multisample
#define GL_NV_framebuffer_multisample 1
GLAPI int GLAD_GL_NV_framebuffer_multisample;
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLENVPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLENVPROC glad_glRenderbufferStorageMultisampleNV;
#define glRenderbufferStorageMultisampleNV glad_glRenderbufferStorageMultisampleNV
#endif
#ifndef GL_NV_generate_mipmap_sRGB
#define GL_NV_generate_mipmap_sRGB 1
GLAPI int GLAD_GL_NV_generate_mipmap_sRGB;
#endif
#ifndef GL_NV_geometry_shader_passthrough
#define GL_NV_geometry_shader_passthrough 1
GLAPI int GLAD_GL_NV_geometry_shader_passthrough;
#endif
#ifndef GL_NV_gpu_shader5
#define GL_NV_gpu_shader5 1
GLAPI int GLAD_GL_NV_gpu_shader5;
typedef void (APIENTRYP PFNGLUNIFORM1I64NVPROC)(GLint location, GLint64EXT x);
GLAPI PFNGLUNIFORM1I64NVPROC glad_glUniform1i64NV;
#define glUniform1i64NV glad_glUniform1i64NV
typedef void (APIENTRYP PFNGLUNIFORM2I64NVPROC)(GLint location, GLint64EXT x, GLint64EXT y);
GLAPI PFNGLUNIFORM2I64NVPROC glad_glUniform2i64NV;
#define glUniform2i64NV glad_glUniform2i64NV
typedef void (APIENTRYP PFNGLUNIFORM3I64NVPROC)(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLAPI PFNGLUNIFORM3I64NVPROC glad_glUniform3i64NV;
#define glUniform3i64NV glad_glUniform3i64NV
typedef void (APIENTRYP PFNGLUNIFORM4I64NVPROC)(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLAPI PFNGLUNIFORM4I64NVPROC glad_glUniform4i64NV;
#define glUniform4i64NV glad_glUniform4i64NV
typedef void (APIENTRYP PFNGLUNIFORM1I64VNVPROC)(GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLUNIFORM1I64VNVPROC glad_glUniform1i64vNV;
#define glUniform1i64vNV glad_glUniform1i64vNV
typedef void (APIENTRYP PFNGLUNIFORM2I64VNVPROC)(GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLUNIFORM2I64VNVPROC glad_glUniform2i64vNV;
#define glUniform2i64vNV glad_glUniform2i64vNV
typedef void (APIENTRYP PFNGLUNIFORM3I64VNVPROC)(GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLUNIFORM3I64VNVPROC glad_glUniform3i64vNV;
#define glUniform3i64vNV glad_glUniform3i64vNV
typedef void (APIENTRYP PFNGLUNIFORM4I64VNVPROC)(GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLUNIFORM4I64VNVPROC glad_glUniform4i64vNV;
#define glUniform4i64vNV glad_glUniform4i64vNV
typedef void (APIENTRYP PFNGLUNIFORM1UI64NVPROC)(GLint location, GLuint64EXT x);
GLAPI PFNGLUNIFORM1UI64NVPROC glad_glUniform1ui64NV;
#define glUniform1ui64NV glad_glUniform1ui64NV
typedef void (APIENTRYP PFNGLUNIFORM2UI64NVPROC)(GLint location, GLuint64EXT x, GLuint64EXT y);
GLAPI PFNGLUNIFORM2UI64NVPROC glad_glUniform2ui64NV;
#define glUniform2ui64NV glad_glUniform2ui64NV
typedef void (APIENTRYP PFNGLUNIFORM3UI64NVPROC)(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLAPI PFNGLUNIFORM3UI64NVPROC glad_glUniform3ui64NV;
#define glUniform3ui64NV glad_glUniform3ui64NV
typedef void (APIENTRYP PFNGLUNIFORM4UI64NVPROC)(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLAPI PFNGLUNIFORM4UI64NVPROC glad_glUniform4ui64NV;
#define glUniform4ui64NV glad_glUniform4ui64NV
typedef void (APIENTRYP PFNGLUNIFORM1UI64VNVPROC)(GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLUNIFORM1UI64VNVPROC glad_glUniform1ui64vNV;
#define glUniform1ui64vNV glad_glUniform1ui64vNV
typedef void (APIENTRYP PFNGLUNIFORM2UI64VNVPROC)(GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLUNIFORM2UI64VNVPROC glad_glUniform2ui64vNV;
#define glUniform2ui64vNV glad_glUniform2ui64vNV
typedef void (APIENTRYP PFNGLUNIFORM3UI64VNVPROC)(GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLUNIFORM3UI64VNVPROC glad_glUniform3ui64vNV;
#define glUniform3ui64vNV glad_glUniform3ui64vNV
typedef void (APIENTRYP PFNGLUNIFORM4UI64VNVPROC)(GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLUNIFORM4UI64VNVPROC glad_glUniform4ui64vNV;
#define glUniform4ui64vNV glad_glUniform4ui64vNV
typedef void (APIENTRYP PFNGLGETUNIFORMI64VNVPROC)(GLuint program, GLint location, GLint64EXT *params);
GLAPI PFNGLGETUNIFORMI64VNVPROC glad_glGetUniformi64vNV;
#define glGetUniformi64vNV glad_glGetUniformi64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1I64NVPROC)(GLuint program, GLint location, GLint64EXT x);
GLAPI PFNGLPROGRAMUNIFORM1I64NVPROC glad_glProgramUniform1i64NV;
#define glProgramUniform1i64NV glad_glProgramUniform1i64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2I64NVPROC)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
GLAPI PFNGLPROGRAMUNIFORM2I64NVPROC glad_glProgramUniform2i64NV;
#define glProgramUniform2i64NV glad_glProgramUniform2i64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3I64NVPROC)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLAPI PFNGLPROGRAMUNIFORM3I64NVPROC glad_glProgramUniform3i64NV;
#define glProgramUniform3i64NV glad_glProgramUniform3i64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4I64NVPROC)(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLAPI PFNGLPROGRAMUNIFORM4I64NVPROC glad_glProgramUniform4i64NV;
#define glProgramUniform4i64NV glad_glProgramUniform4i64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1I64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM1I64VNVPROC glad_glProgramUniform1i64vNV;
#define glProgramUniform1i64vNV glad_glProgramUniform1i64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2I64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM2I64VNVPROC glad_glProgramUniform2i64vNV;
#define glProgramUniform2i64vNV glad_glProgramUniform2i64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3I64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM3I64VNVPROC glad_glProgramUniform3i64vNV;
#define glProgramUniform3i64vNV glad_glProgramUniform3i64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4I64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM4I64VNVPROC glad_glProgramUniform4i64vNV;
#define glProgramUniform4i64vNV glad_glProgramUniform4i64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UI64NVPROC)(GLuint program, GLint location, GLuint64EXT x);
GLAPI PFNGLPROGRAMUNIFORM1UI64NVPROC glad_glProgramUniform1ui64NV;
#define glProgramUniform1ui64NV glad_glProgramUniform1ui64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UI64NVPROC)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
GLAPI PFNGLPROGRAMUNIFORM2UI64NVPROC glad_glProgramUniform2ui64NV;
#define glProgramUniform2ui64NV glad_glProgramUniform2ui64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UI64NVPROC)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLAPI PFNGLPROGRAMUNIFORM3UI64NVPROC glad_glProgramUniform3ui64NV;
#define glProgramUniform3ui64NV glad_glProgramUniform3ui64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UI64NVPROC)(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLAPI PFNGLPROGRAMUNIFORM4UI64NVPROC glad_glProgramUniform4ui64NV;
#define glProgramUniform4ui64NV glad_glProgramUniform4ui64NV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UI64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM1UI64VNVPROC glad_glProgramUniform1ui64vNV;
#define glProgramUniform1ui64vNV glad_glProgramUniform1ui64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UI64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM2UI64VNVPROC glad_glProgramUniform2ui64vNV;
#define glProgramUniform2ui64vNV glad_glProgramUniform2ui64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UI64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM3UI64VNVPROC glad_glProgramUniform3ui64vNV;
#define glProgramUniform3ui64vNV glad_glProgramUniform3ui64vNV
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UI64VNVPROC)(GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI PFNGLPROGRAMUNIFORM4UI64VNVPROC glad_glProgramUniform4ui64vNV;
#define glProgramUniform4ui64vNV glad_glProgramUniform4ui64vNV
#endif
#ifndef GL_NV_image_formats
#define GL_NV_image_formats 1
GLAPI int GLAD_GL_NV_image_formats;
#endif
#ifndef GL_NV_instanced_arrays
#define GL_NV_instanced_arrays 1
GLAPI int GLAD_GL_NV_instanced_arrays;
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORNVPROC)(GLuint index, GLuint divisor);
GLAPI PFNGLVERTEXATTRIBDIVISORNVPROC glad_glVertexAttribDivisorNV;
#define glVertexAttribDivisorNV glad_glVertexAttribDivisorNV
#endif
#ifndef GL_NV_internalformat_sample_query
#define GL_NV_internalformat_sample_query 1
GLAPI int GLAD_GL_NV_internalformat_sample_query;
typedef void (APIENTRYP PFNGLGETINTERNALFORMATSAMPLEIVNVPROC)(GLenum target, GLenum internalformat, GLsizei samples, GLenum pname, GLsizei count, GLint *params);
GLAPI PFNGLGETINTERNALFORMATSAMPLEIVNVPROC glad_glGetInternalformatSampleivNV;
#define glGetInternalformatSampleivNV glad_glGetInternalformatSampleivNV
#endif
#ifndef GL_NV_memory_attachment
#define GL_NV_memory_attachment 1
GLAPI int GLAD_GL_NV_memory_attachment;
typedef void (APIENTRYP PFNGLGETMEMORYOBJECTDETACHEDRESOURCESUIVNVPROC)(GLuint memory, GLenum pname, GLint first, GLsizei count, GLuint *params);
GLAPI PFNGLGETMEMORYOBJECTDETACHEDRESOURCESUIVNVPROC glad_glGetMemoryObjectDetachedResourcesuivNV;
#define glGetMemoryObjectDetachedResourcesuivNV glad_glGetMemoryObjectDetachedResourcesuivNV
typedef void (APIENTRYP PFNGLRESETMEMORYOBJECTPARAMETERNVPROC)(GLuint memory, GLenum pname);
GLAPI PFNGLRESETMEMORYOBJECTPARAMETERNVPROC glad_glResetMemoryObjectParameterNV;
#define glResetMemoryObjectParameterNV glad_glResetMemoryObjectParameterNV
typedef void (APIENTRYP PFNGLTEXATTACHMEMORYNVPROC)(GLenum target, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXATTACHMEMORYNVPROC glad_glTexAttachMemoryNV;
#define glTexAttachMemoryNV glad_glTexAttachMemoryNV
typedef void (APIENTRYP PFNGLBUFFERATTACHMEMORYNVPROC)(GLenum target, GLuint memory, GLuint64 offset);
GLAPI PFNGLBUFFERATTACHMEMORYNVPROC glad_glBufferAttachMemoryNV;
#define glBufferAttachMemoryNV glad_glBufferAttachMemoryNV
typedef void (APIENTRYP PFNGLTEXTUREATTACHMEMORYNVPROC)(GLuint texture, GLuint memory, GLuint64 offset);
GLAPI PFNGLTEXTUREATTACHMEMORYNVPROC glad_glTextureAttachMemoryNV;
#define glTextureAttachMemoryNV glad_glTextureAttachMemoryNV
typedef void (APIENTRYP PFNGLNAMEDBUFFERATTACHMEMORYNVPROC)(GLuint buffer, GLuint memory, GLuint64 offset);
GLAPI PFNGLNAMEDBUFFERATTACHMEMORYNVPROC glad_glNamedBufferAttachMemoryNV;
#define glNamedBufferAttachMemoryNV glad_glNamedBufferAttachMemoryNV
#endif
#ifndef GL_NV_memory_object_sparse
#define GL_NV_memory_object_sparse 1
GLAPI int GLAD_GL_NV_memory_object_sparse;
typedef void (APIENTRYP PFNGLBUFFERPAGECOMMITMENTMEMNVPROC)(GLenum target, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit);
GLAPI PFNGLBUFFERPAGECOMMITMENTMEMNVPROC glad_glBufferPageCommitmentMemNV;
#define glBufferPageCommitmentMemNV glad_glBufferPageCommitmentMemNV
typedef void (APIENTRYP PFNGLTEXPAGECOMMITMENTMEMNVPROC)(GLenum target, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit);
GLAPI PFNGLTEXPAGECOMMITMENTMEMNVPROC glad_glTexPageCommitmentMemNV;
#define glTexPageCommitmentMemNV glad_glTexPageCommitmentMemNV
typedef void (APIENTRYP PFNGLNAMEDBUFFERPAGECOMMITMENTMEMNVPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit);
GLAPI PFNGLNAMEDBUFFERPAGECOMMITMENTMEMNVPROC glad_glNamedBufferPageCommitmentMemNV;
#define glNamedBufferPageCommitmentMemNV glad_glNamedBufferPageCommitmentMemNV
typedef void (APIENTRYP PFNGLTEXTUREPAGECOMMITMENTMEMNVPROC)(GLuint texture, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit);
GLAPI PFNGLTEXTUREPAGECOMMITMENTMEMNVPROC glad_glTexturePageCommitmentMemNV;
#define glTexturePageCommitmentMemNV glad_glTexturePageCommitmentMemNV
#endif
#ifndef GL_NV_mesh_shader
#define GL_NV_mesh_shader 1
GLAPI int GLAD_GL_NV_mesh_shader;
typedef void (APIENTRYP PFNGLDRAWMESHTASKSNVPROC)(GLuint first, GLuint count);
GLAPI PFNGLDRAWMESHTASKSNVPROC glad_glDrawMeshTasksNV;
#define glDrawMeshTasksNV glad_glDrawMeshTasksNV
typedef void (APIENTRYP PFNGLDRAWMESHTASKSINDIRECTNVPROC)(GLintptr indirect);
GLAPI PFNGLDRAWMESHTASKSINDIRECTNVPROC glad_glDrawMeshTasksIndirectNV;
#define glDrawMeshTasksIndirectNV glad_glDrawMeshTasksIndirectNV
typedef void (APIENTRYP PFNGLMULTIDRAWMESHTASKSINDIRECTNVPROC)(GLintptr indirect, GLsizei drawcount, GLsizei stride);
GLAPI PFNGLMULTIDRAWMESHTASKSINDIRECTNVPROC glad_glMultiDrawMeshTasksIndirectNV;
#define glMultiDrawMeshTasksIndirectNV glad_glMultiDrawMeshTasksIndirectNV
typedef void (APIENTRYP PFNGLMULTIDRAWMESHTASKSINDIRECTCOUNTNVPROC)(GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
GLAPI PFNGLMULTIDRAWMESHTASKSINDIRECTCOUNTNVPROC glad_glMultiDrawMeshTasksIndirectCountNV;
#define glMultiDrawMeshTasksIndirectCountNV glad_glMultiDrawMeshTasksIndirectCountNV
#endif
#ifndef GL_NV_non_square_matrices
#define GL_NV_non_square_matrices 1
GLAPI int GLAD_GL_NV_non_square_matrices;
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X3FVNVPROC glad_glUniformMatrix2x3fvNV;
#define glUniformMatrix2x3fvNV glad_glUniformMatrix2x3fvNV
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X2FVNVPROC glad_glUniformMatrix3x2fvNV;
#define glUniformMatrix3x2fvNV glad_glUniformMatrix3x2fvNV
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X4FVNVPROC glad_glUniformMatrix2x4fvNV;
#define glUniformMatrix2x4fvNV glad_glUniformMatrix2x4fvNV
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X2FVNVPROC glad_glUniformMatrix4x2fvNV;
#define glUniformMatrix4x2fvNV glad_glUniformMatrix4x2fvNV
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X4FVNVPROC glad_glUniformMatrix3x4fvNV;
#define glUniformMatrix3x4fvNV glad_glUniformMatrix3x4fvNV
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3FVNVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X3FVNVPROC glad_glUniformMatrix4x3fvNV;
#define glUniformMatrix4x3fvNV glad_glUniformMatrix4x3fvNV
#endif
#ifndef GL_NV_path_rendering
#define GL_NV_path_rendering 1
GLAPI int GLAD_GL_NV_path_rendering;
typedef GLuint (APIENTRYP PFNGLGENPATHSNVPROC)(GLsizei range);
GLAPI PFNGLGENPATHSNVPROC glad_glGenPathsNV;
#define glGenPathsNV glad_glGenPathsNV
typedef void (APIENTRYP PFNGLDELETEPATHSNVPROC)(GLuint path, GLsizei range);
GLAPI PFNGLDELETEPATHSNVPROC glad_glDeletePathsNV;
#define glDeletePathsNV glad_glDeletePathsNV
typedef GLboolean (APIENTRYP PFNGLISPATHNVPROC)(GLuint path);
GLAPI PFNGLISPATHNVPROC glad_glIsPathNV;
#define glIsPathNV glad_glIsPathNV
typedef void (APIENTRYP PFNGLPATHCOMMANDSNVPROC)(GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords);
GLAPI PFNGLPATHCOMMANDSNVPROC glad_glPathCommandsNV;
#define glPathCommandsNV glad_glPathCommandsNV
typedef void (APIENTRYP PFNGLPATHCOORDSNVPROC)(GLuint path, GLsizei numCoords, GLenum coordType, const void *coords);
GLAPI PFNGLPATHCOORDSNVPROC glad_glPathCoordsNV;
#define glPathCoordsNV glad_glPathCoordsNV
typedef void (APIENTRYP PFNGLPATHSUBCOMMANDSNVPROC)(GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords);
GLAPI PFNGLPATHSUBCOMMANDSNVPROC glad_glPathSubCommandsNV;
#define glPathSubCommandsNV glad_glPathSubCommandsNV
typedef void (APIENTRYP PFNGLPATHSUBCOORDSNVPROC)(GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void *coords);
GLAPI PFNGLPATHSUBCOORDSNVPROC glad_glPathSubCoordsNV;
#define glPathSubCoordsNV glad_glPathSubCoordsNV
typedef void (APIENTRYP PFNGLPATHSTRINGNVPROC)(GLuint path, GLenum format, GLsizei length, const void *pathString);
GLAPI PFNGLPATHSTRINGNVPROC glad_glPathStringNV;
#define glPathStringNV glad_glPathStringNV
typedef void (APIENTRYP PFNGLPATHGLYPHSNVPROC)(GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const void *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI PFNGLPATHGLYPHSNVPROC glad_glPathGlyphsNV;
#define glPathGlyphsNV glad_glPathGlyphsNV
typedef void (APIENTRYP PFNGLPATHGLYPHRANGENVPROC)(GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI PFNGLPATHGLYPHRANGENVPROC glad_glPathGlyphRangeNV;
#define glPathGlyphRangeNV glad_glPathGlyphRangeNV
typedef void (APIENTRYP PFNGLWEIGHTPATHSNVPROC)(GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights);
GLAPI PFNGLWEIGHTPATHSNVPROC glad_glWeightPathsNV;
#define glWeightPathsNV glad_glWeightPathsNV
typedef void (APIENTRYP PFNGLCOPYPATHNVPROC)(GLuint resultPath, GLuint srcPath);
GLAPI PFNGLCOPYPATHNVPROC glad_glCopyPathNV;
#define glCopyPathNV glad_glCopyPathNV
typedef void (APIENTRYP PFNGLINTERPOLATEPATHSNVPROC)(GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
GLAPI PFNGLINTERPOLATEPATHSNVPROC glad_glInterpolatePathsNV;
#define glInterpolatePathsNV glad_glInterpolatePathsNV
typedef void (APIENTRYP PFNGLTRANSFORMPATHNVPROC)(GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLTRANSFORMPATHNVPROC glad_glTransformPathNV;
#define glTransformPathNV glad_glTransformPathNV
typedef void (APIENTRYP PFNGLPATHPARAMETERIVNVPROC)(GLuint path, GLenum pname, const GLint *value);
GLAPI PFNGLPATHPARAMETERIVNVPROC glad_glPathParameterivNV;
#define glPathParameterivNV glad_glPathParameterivNV
typedef void (APIENTRYP PFNGLPATHPARAMETERINVPROC)(GLuint path, GLenum pname, GLint value);
GLAPI PFNGLPATHPARAMETERINVPROC glad_glPathParameteriNV;
#define glPathParameteriNV glad_glPathParameteriNV
typedef void (APIENTRYP PFNGLPATHPARAMETERFVNVPROC)(GLuint path, GLenum pname, const GLfloat *value);
GLAPI PFNGLPATHPARAMETERFVNVPROC glad_glPathParameterfvNV;
#define glPathParameterfvNV glad_glPathParameterfvNV
typedef void (APIENTRYP PFNGLPATHPARAMETERFNVPROC)(GLuint path, GLenum pname, GLfloat value);
GLAPI PFNGLPATHPARAMETERFNVPROC glad_glPathParameterfNV;
#define glPathParameterfNV glad_glPathParameterfNV
typedef void (APIENTRYP PFNGLPATHDASHARRAYNVPROC)(GLuint path, GLsizei dashCount, const GLfloat *dashArray);
GLAPI PFNGLPATHDASHARRAYNVPROC glad_glPathDashArrayNV;
#define glPathDashArrayNV glad_glPathDashArrayNV
typedef void (APIENTRYP PFNGLPATHSTENCILFUNCNVPROC)(GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLPATHSTENCILFUNCNVPROC glad_glPathStencilFuncNV;
#define glPathStencilFuncNV glad_glPathStencilFuncNV
typedef void (APIENTRYP PFNGLPATHSTENCILDEPTHOFFSETNVPROC)(GLfloat factor, GLfloat units);
GLAPI PFNGLPATHSTENCILDEPTHOFFSETNVPROC glad_glPathStencilDepthOffsetNV;
#define glPathStencilDepthOffsetNV glad_glPathStencilDepthOffsetNV
typedef void (APIENTRYP PFNGLSTENCILFILLPATHNVPROC)(GLuint path, GLenum fillMode, GLuint mask);
GLAPI PFNGLSTENCILFILLPATHNVPROC glad_glStencilFillPathNV;
#define glStencilFillPathNV glad_glStencilFillPathNV
typedef void (APIENTRYP PFNGLSTENCILSTROKEPATHNVPROC)(GLuint path, GLint reference, GLuint mask);
GLAPI PFNGLSTENCILSTROKEPATHNVPROC glad_glStencilStrokePathNV;
#define glStencilStrokePathNV glad_glStencilStrokePathNV
typedef void (APIENTRYP PFNGLSTENCILFILLPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLSTENCILFILLPATHINSTANCEDNVPROC glad_glStencilFillPathInstancedNV;
#define glStencilFillPathInstancedNV glad_glStencilFillPathInstancedNV
typedef void (APIENTRYP PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC glad_glStencilStrokePathInstancedNV;
#define glStencilStrokePathInstancedNV glad_glStencilStrokePathInstancedNV
typedef void (APIENTRYP PFNGLPATHCOVERDEPTHFUNCNVPROC)(GLenum func);
GLAPI PFNGLPATHCOVERDEPTHFUNCNVPROC glad_glPathCoverDepthFuncNV;
#define glPathCoverDepthFuncNV glad_glPathCoverDepthFuncNV
typedef void (APIENTRYP PFNGLCOVERFILLPATHNVPROC)(GLuint path, GLenum coverMode);
GLAPI PFNGLCOVERFILLPATHNVPROC glad_glCoverFillPathNV;
#define glCoverFillPathNV glad_glCoverFillPathNV
typedef void (APIENTRYP PFNGLCOVERSTROKEPATHNVPROC)(GLuint path, GLenum coverMode);
GLAPI PFNGLCOVERSTROKEPATHNVPROC glad_glCoverStrokePathNV;
#define glCoverStrokePathNV glad_glCoverStrokePathNV
typedef void (APIENTRYP PFNGLCOVERFILLPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLCOVERFILLPATHINSTANCEDNVPROC glad_glCoverFillPathInstancedNV;
#define glCoverFillPathInstancedNV glad_glCoverFillPathInstancedNV
typedef void (APIENTRYP PFNGLCOVERSTROKEPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLCOVERSTROKEPATHINSTANCEDNVPROC glad_glCoverStrokePathInstancedNV;
#define glCoverStrokePathInstancedNV glad_glCoverStrokePathInstancedNV
typedef void (APIENTRYP PFNGLGETPATHPARAMETERIVNVPROC)(GLuint path, GLenum pname, GLint *value);
GLAPI PFNGLGETPATHPARAMETERIVNVPROC glad_glGetPathParameterivNV;
#define glGetPathParameterivNV glad_glGetPathParameterivNV
typedef void (APIENTRYP PFNGLGETPATHPARAMETERFVNVPROC)(GLuint path, GLenum pname, GLfloat *value);
GLAPI PFNGLGETPATHPARAMETERFVNVPROC glad_glGetPathParameterfvNV;
#define glGetPathParameterfvNV glad_glGetPathParameterfvNV
typedef void (APIENTRYP PFNGLGETPATHCOMMANDSNVPROC)(GLuint path, GLubyte *commands);
GLAPI PFNGLGETPATHCOMMANDSNVPROC glad_glGetPathCommandsNV;
#define glGetPathCommandsNV glad_glGetPathCommandsNV
typedef void (APIENTRYP PFNGLGETPATHCOORDSNVPROC)(GLuint path, GLfloat *coords);
GLAPI PFNGLGETPATHCOORDSNVPROC glad_glGetPathCoordsNV;
#define glGetPathCoordsNV glad_glGetPathCoordsNV
typedef void (APIENTRYP PFNGLGETPATHDASHARRAYNVPROC)(GLuint path, GLfloat *dashArray);
GLAPI PFNGLGETPATHDASHARRAYNVPROC glad_glGetPathDashArrayNV;
#define glGetPathDashArrayNV glad_glGetPathDashArrayNV
typedef void (APIENTRYP PFNGLGETPATHMETRICSNVPROC)(GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
GLAPI PFNGLGETPATHMETRICSNVPROC glad_glGetPathMetricsNV;
#define glGetPathMetricsNV glad_glGetPathMetricsNV
typedef void (APIENTRYP PFNGLGETPATHMETRICRANGENVPROC)(GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics);
GLAPI PFNGLGETPATHMETRICRANGENVPROC glad_glGetPathMetricRangeNV;
#define glGetPathMetricRangeNV glad_glGetPathMetricRangeNV
typedef void (APIENTRYP PFNGLGETPATHSPACINGNVPROC)(GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
GLAPI PFNGLGETPATHSPACINGNVPROC glad_glGetPathSpacingNV;
#define glGetPathSpacingNV glad_glGetPathSpacingNV
typedef GLboolean (APIENTRYP PFNGLISPOINTINFILLPATHNVPROC)(GLuint path, GLuint mask, GLfloat x, GLfloat y);
GLAPI PFNGLISPOINTINFILLPATHNVPROC glad_glIsPointInFillPathNV;
#define glIsPointInFillPathNV glad_glIsPointInFillPathNV
typedef GLboolean (APIENTRYP PFNGLISPOINTINSTROKEPATHNVPROC)(GLuint path, GLfloat x, GLfloat y);
GLAPI PFNGLISPOINTINSTROKEPATHNVPROC glad_glIsPointInStrokePathNV;
#define glIsPointInStrokePathNV glad_glIsPointInStrokePathNV
typedef GLfloat (APIENTRYP PFNGLGETPATHLENGTHNVPROC)(GLuint path, GLsizei startSegment, GLsizei numSegments);
GLAPI PFNGLGETPATHLENGTHNVPROC glad_glGetPathLengthNV;
#define glGetPathLengthNV glad_glGetPathLengthNV
typedef GLboolean (APIENTRYP PFNGLPOINTALONGPATHNVPROC)(GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
GLAPI PFNGLPOINTALONGPATHNVPROC glad_glPointAlongPathNV;
#define glPointAlongPathNV glad_glPointAlongPathNV
typedef void (APIENTRYP PFNGLMATRIXLOAD3X2FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXLOAD3X2FNVPROC glad_glMatrixLoad3x2fNV;
#define glMatrixLoad3x2fNV glad_glMatrixLoad3x2fNV
typedef void (APIENTRYP PFNGLMATRIXLOAD3X3FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXLOAD3X3FNVPROC glad_glMatrixLoad3x3fNV;
#define glMatrixLoad3x3fNV glad_glMatrixLoad3x3fNV
typedef void (APIENTRYP PFNGLMATRIXLOADTRANSPOSE3X3FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXLOADTRANSPOSE3X3FNVPROC glad_glMatrixLoadTranspose3x3fNV;
#define glMatrixLoadTranspose3x3fNV glad_glMatrixLoadTranspose3x3fNV
typedef void (APIENTRYP PFNGLMATRIXMULT3X2FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXMULT3X2FNVPROC glad_glMatrixMult3x2fNV;
#define glMatrixMult3x2fNV glad_glMatrixMult3x2fNV
typedef void (APIENTRYP PFNGLMATRIXMULT3X3FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXMULT3X3FNVPROC glad_glMatrixMult3x3fNV;
#define glMatrixMult3x3fNV glad_glMatrixMult3x3fNV
typedef void (APIENTRYP PFNGLMATRIXMULTTRANSPOSE3X3FNVPROC)(GLenum matrixMode, const GLfloat *m);
GLAPI PFNGLMATRIXMULTTRANSPOSE3X3FNVPROC glad_glMatrixMultTranspose3x3fNV;
#define glMatrixMultTranspose3x3fNV glad_glMatrixMultTranspose3x3fNV
typedef void (APIENTRYP PFNGLSTENCILTHENCOVERFILLPATHNVPROC)(GLuint path, GLenum fillMode, GLuint mask, GLenum coverMode);
GLAPI PFNGLSTENCILTHENCOVERFILLPATHNVPROC glad_glStencilThenCoverFillPathNV;
#define glStencilThenCoverFillPathNV glad_glStencilThenCoverFillPathNV
typedef void (APIENTRYP PFNGLSTENCILTHENCOVERSTROKEPATHNVPROC)(GLuint path, GLint reference, GLuint mask, GLenum coverMode);
GLAPI PFNGLSTENCILTHENCOVERSTROKEPATHNVPROC glad_glStencilThenCoverStrokePathNV;
#define glStencilThenCoverStrokePathNV glad_glStencilThenCoverStrokePathNV
typedef void (APIENTRYP PFNGLSTENCILTHENCOVERFILLPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLSTENCILTHENCOVERFILLPATHINSTANCEDNVPROC glad_glStencilThenCoverFillPathInstancedNV;
#define glStencilThenCoverFillPathInstancedNV glad_glStencilThenCoverFillPathInstancedNV
typedef void (APIENTRYP PFNGLSTENCILTHENCOVERSTROKEPATHINSTANCEDNVPROC)(GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI PFNGLSTENCILTHENCOVERSTROKEPATHINSTANCEDNVPROC glad_glStencilThenCoverStrokePathInstancedNV;
#define glStencilThenCoverStrokePathInstancedNV glad_glStencilThenCoverStrokePathInstancedNV
typedef GLenum (APIENTRYP PFNGLPATHGLYPHINDEXRANGENVPROC)(GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint pathParameterTemplate, GLfloat emScale, GLuint *baseAndCount);
GLAPI PFNGLPATHGLYPHINDEXRANGENVPROC glad_glPathGlyphIndexRangeNV;
#define glPathGlyphIndexRangeNV glad_glPathGlyphIndexRangeNV
typedef GLenum (APIENTRYP PFNGLPATHGLYPHINDEXARRAYNVPROC)(GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI PFNGLPATHGLYPHINDEXARRAYNVPROC glad_glPathGlyphIndexArrayNV;
#define glPathGlyphIndexArrayNV glad_glPathGlyphIndexArrayNV
typedef GLenum (APIENTRYP PFNGLPATHMEMORYGLYPHINDEXARRAYNVPROC)(GLuint firstPathName, GLenum fontTarget, GLsizeiptr fontSize, const void *fontData, GLsizei faceIndex, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI PFNGLPATHMEMORYGLYPHINDEXARRAYNVPROC glad_glPathMemoryGlyphIndexArrayNV;
#define glPathMemoryGlyphIndexArrayNV glad_glPathMemoryGlyphIndexArrayNV
typedef void (APIENTRYP PFNGLPROGRAMPATHFRAGMENTINPUTGENNVPROC)(GLuint program, GLint location, GLenum genMode, GLint components, const GLfloat *coeffs);
GLAPI PFNGLPROGRAMPATHFRAGMENTINPUTGENNVPROC glad_glProgramPathFragmentInputGenNV;
#define glProgramPathFragmentInputGenNV glad_glProgramPathFragmentInputGenNV
typedef void (APIENTRYP PFNGLGETPROGRAMRESOURCEFVNVPROC)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLfloat *params);
GLAPI PFNGLGETPROGRAMRESOURCEFVNVPROC glad_glGetProgramResourcefvNV;
#define glGetProgramResourcefvNV glad_glGetProgramResourcefvNV
typedef void (APIENTRYP PFNGLPATHCOLORGENNVPROC)(GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs);
GLAPI PFNGLPATHCOLORGENNVPROC glad_glPathColorGenNV;
#define glPathColorGenNV glad_glPathColorGenNV
typedef void (APIENTRYP PFNGLPATHTEXGENNVPROC)(GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs);
GLAPI PFNGLPATHTEXGENNVPROC glad_glPathTexGenNV;
#define glPathTexGenNV glad_glPathTexGenNV
typedef void (APIENTRYP PFNGLPATHFOGGENNVPROC)(GLenum genMode);
GLAPI PFNGLPATHFOGGENNVPROC glad_glPathFogGenNV;
#define glPathFogGenNV glad_glPathFogGenNV
typedef void (APIENTRYP PFNGLGETPATHCOLORGENIVNVPROC)(GLenum color, GLenum pname, GLint *value);
GLAPI PFNGLGETPATHCOLORGENIVNVPROC glad_glGetPathColorGenivNV;
#define glGetPathColorGenivNV glad_glGetPathColorGenivNV
typedef void (APIENTRYP PFNGLGETPATHCOLORGENFVNVPROC)(GLenum color, GLenum pname, GLfloat *value);
GLAPI PFNGLGETPATHCOLORGENFVNVPROC glad_glGetPathColorGenfvNV;
#define glGetPathColorGenfvNV glad_glGetPathColorGenfvNV
typedef void (APIENTRYP PFNGLGETPATHTEXGENIVNVPROC)(GLenum texCoordSet, GLenum pname, GLint *value);
GLAPI PFNGLGETPATHTEXGENIVNVPROC glad_glGetPathTexGenivNV;
#define glGetPathTexGenivNV glad_glGetPathTexGenivNV
typedef void (APIENTRYP PFNGLGETPATHTEXGENFVNVPROC)(GLenum texCoordSet, GLenum pname, GLfloat *value);
GLAPI PFNGLGETPATHTEXGENFVNVPROC glad_glGetPathTexGenfvNV;
#define glGetPathTexGenfvNV glad_glGetPathTexGenfvNV
typedef void (APIENTRYP PFNGLMATRIXFRUSTUMEXTPROC)(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI PFNGLMATRIXFRUSTUMEXTPROC glad_glMatrixFrustumEXT;
#define glMatrixFrustumEXT glad_glMatrixFrustumEXT
typedef void (APIENTRYP PFNGLMATRIXLOADIDENTITYEXTPROC)(GLenum mode);
GLAPI PFNGLMATRIXLOADIDENTITYEXTPROC glad_glMatrixLoadIdentityEXT;
#define glMatrixLoadIdentityEXT glad_glMatrixLoadIdentityEXT
typedef void (APIENTRYP PFNGLMATRIXLOADTRANSPOSEFEXTPROC)(GLenum mode, const GLfloat *m);
GLAPI PFNGLMATRIXLOADTRANSPOSEFEXTPROC glad_glMatrixLoadTransposefEXT;
#define glMatrixLoadTransposefEXT glad_glMatrixLoadTransposefEXT
typedef void (APIENTRYP PFNGLMATRIXLOADTRANSPOSEDEXTPROC)(GLenum mode, const GLdouble *m);
GLAPI PFNGLMATRIXLOADTRANSPOSEDEXTPROC glad_glMatrixLoadTransposedEXT;
#define glMatrixLoadTransposedEXT glad_glMatrixLoadTransposedEXT
typedef void (APIENTRYP PFNGLMATRIXLOADFEXTPROC)(GLenum mode, const GLfloat *m);
GLAPI PFNGLMATRIXLOADFEXTPROC glad_glMatrixLoadfEXT;
#define glMatrixLoadfEXT glad_glMatrixLoadfEXT
typedef void (APIENTRYP PFNGLMATRIXLOADDEXTPROC)(GLenum mode, const GLdouble *m);
GLAPI PFNGLMATRIXLOADDEXTPROC glad_glMatrixLoaddEXT;
#define glMatrixLoaddEXT glad_glMatrixLoaddEXT
typedef void (APIENTRYP PFNGLMATRIXMULTTRANSPOSEFEXTPROC)(GLenum mode, const GLfloat *m);
GLAPI PFNGLMATRIXMULTTRANSPOSEFEXTPROC glad_glMatrixMultTransposefEXT;
#define glMatrixMultTransposefEXT glad_glMatrixMultTransposefEXT
typedef void (APIENTRYP PFNGLMATRIXMULTTRANSPOSEDEXTPROC)(GLenum mode, const GLdouble *m);
GLAPI PFNGLMATRIXMULTTRANSPOSEDEXTPROC glad_glMatrixMultTransposedEXT;
#define glMatrixMultTransposedEXT glad_glMatrixMultTransposedEXT
typedef void (APIENTRYP PFNGLMATRIXMULTFEXTPROC)(GLenum mode, const GLfloat *m);
GLAPI PFNGLMATRIXMULTFEXTPROC glad_glMatrixMultfEXT;
#define glMatrixMultfEXT glad_glMatrixMultfEXT
typedef void (APIENTRYP PFNGLMATRIXMULTDEXTPROC)(GLenum mode, const GLdouble *m);
GLAPI PFNGLMATRIXMULTDEXTPROC glad_glMatrixMultdEXT;
#define glMatrixMultdEXT glad_glMatrixMultdEXT
typedef void (APIENTRYP PFNGLMATRIXORTHOEXTPROC)(GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI PFNGLMATRIXORTHOEXTPROC glad_glMatrixOrthoEXT;
#define glMatrixOrthoEXT glad_glMatrixOrthoEXT
typedef void (APIENTRYP PFNGLMATRIXPOPEXTPROC)(GLenum mode);
GLAPI PFNGLMATRIXPOPEXTPROC glad_glMatrixPopEXT;
#define glMatrixPopEXT glad_glMatrixPopEXT
typedef void (APIENTRYP PFNGLMATRIXPUSHEXTPROC)(GLenum mode);
GLAPI PFNGLMATRIXPUSHEXTPROC glad_glMatrixPushEXT;
#define glMatrixPushEXT glad_glMatrixPushEXT
typedef void (APIENTRYP PFNGLMATRIXROTATEFEXTPROC)(GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLMATRIXROTATEFEXTPROC glad_glMatrixRotatefEXT;
#define glMatrixRotatefEXT glad_glMatrixRotatefEXT
typedef void (APIENTRYP PFNGLMATRIXROTATEDEXTPROC)(GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLAPI PFNGLMATRIXROTATEDEXTPROC glad_glMatrixRotatedEXT;
#define glMatrixRotatedEXT glad_glMatrixRotatedEXT
typedef void (APIENTRYP PFNGLMATRIXSCALEFEXTPROC)(GLenum mode, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLMATRIXSCALEFEXTPROC glad_glMatrixScalefEXT;
#define glMatrixScalefEXT glad_glMatrixScalefEXT
typedef void (APIENTRYP PFNGLMATRIXSCALEDEXTPROC)(GLenum mode, GLdouble x, GLdouble y, GLdouble z);
GLAPI PFNGLMATRIXSCALEDEXTPROC glad_glMatrixScaledEXT;
#define glMatrixScaledEXT glad_glMatrixScaledEXT
typedef void (APIENTRYP PFNGLMATRIXTRANSLATEFEXTPROC)(GLenum mode, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLMATRIXTRANSLATEFEXTPROC glad_glMatrixTranslatefEXT;
#define glMatrixTranslatefEXT glad_glMatrixTranslatefEXT
typedef void (APIENTRYP PFNGLMATRIXTRANSLATEDEXTPROC)(GLenum mode, GLdouble x, GLdouble y, GLdouble z);
GLAPI PFNGLMATRIXTRANSLATEDEXTPROC glad_glMatrixTranslatedEXT;
#define glMatrixTranslatedEXT glad_glMatrixTranslatedEXT
#endif
#ifndef GL_NV_path_rendering_shared_edge
#define GL_NV_path_rendering_shared_edge 1
GLAPI int GLAD_GL_NV_path_rendering_shared_edge;
#endif
#ifndef GL_NV_pixel_buffer_object
#define GL_NV_pixel_buffer_object 1
GLAPI int GLAD_GL_NV_pixel_buffer_object;
#endif
#ifndef GL_NV_polygon_mode
#define GL_NV_polygon_mode 1
GLAPI int GLAD_GL_NV_polygon_mode;
typedef void (APIENTRYP PFNGLPOLYGONMODENVPROC)(GLenum face, GLenum mode);
GLAPI PFNGLPOLYGONMODENVPROC glad_glPolygonModeNV;
#define glPolygonModeNV glad_glPolygonModeNV
#endif
#ifndef GL_NV_primitive_shading_rate
#define GL_NV_primitive_shading_rate 1
GLAPI int GLAD_GL_NV_primitive_shading_rate;
#endif
#ifndef GL_NV_read_buffer
#define GL_NV_read_buffer 1
GLAPI int GLAD_GL_NV_read_buffer;
typedef void (APIENTRYP PFNGLREADBUFFERNVPROC)(GLenum mode);
GLAPI PFNGLREADBUFFERNVPROC glad_glReadBufferNV;
#define glReadBufferNV glad_glReadBufferNV
#endif
#ifndef GL_NV_read_buffer_front
#define GL_NV_read_buffer_front 1
GLAPI int GLAD_GL_NV_read_buffer_front;
#endif
#ifndef GL_NV_read_depth
#define GL_NV_read_depth 1
GLAPI int GLAD_GL_NV_read_depth;
#endif
#ifndef GL_NV_read_depth_stencil
#define GL_NV_read_depth_stencil 1
GLAPI int GLAD_GL_NV_read_depth_stencil;
#endif
#ifndef GL_NV_read_stencil
#define GL_NV_read_stencil 1
GLAPI int GLAD_GL_NV_read_stencil;
#endif
#ifndef GL_NV_representative_fragment_test
#define GL_NV_representative_fragment_test 1
GLAPI int GLAD_GL_NV_representative_fragment_test;
#endif
#ifndef GL_NV_sRGB_formats
#define GL_NV_sRGB_formats 1
GLAPI int GLAD_GL_NV_sRGB_formats;
#endif
#ifndef GL_NV_sample_locations
#define GL_NV_sample_locations 1
GLAPI int GLAD_GL_NV_sample_locations;
typedef void (APIENTRYP PFNGLFRAMEBUFFERSAMPLELOCATIONSFVNVPROC)(GLenum target, GLuint start, GLsizei count, const GLfloat *v);
GLAPI PFNGLFRAMEBUFFERSAMPLELOCATIONSFVNVPROC glad_glFramebufferSampleLocationsfvNV;
#define glFramebufferSampleLocationsfvNV glad_glFramebufferSampleLocationsfvNV
typedef void (APIENTRYP PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVNVPROC)(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v);
GLAPI PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVNVPROC glad_glNamedFramebufferSampleLocationsfvNV;
#define glNamedFramebufferSampleLocationsfvNV glad_glNamedFramebufferSampleLocationsfvNV
typedef void (APIENTRYP PFNGLRESOLVEDEPTHVALUESNVPROC)(void);
GLAPI PFNGLRESOLVEDEPTHVALUESNVPROC glad_glResolveDepthValuesNV;
#define glResolveDepthValuesNV glad_glResolveDepthValuesNV
#endif
#ifndef GL_NV_sample_mask_override_coverage
#define GL_NV_sample_mask_override_coverage 1
GLAPI int GLAD_GL_NV_sample_mask_override_coverage;
#endif
#ifndef GL_NV_scissor_exclusive
#define GL_NV_scissor_exclusive 1
GLAPI int GLAD_GL_NV_scissor_exclusive;
typedef void (APIENTRYP PFNGLSCISSOREXCLUSIVENVPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSOREXCLUSIVENVPROC glad_glScissorExclusiveNV;
#define glScissorExclusiveNV glad_glScissorExclusiveNV
typedef void (APIENTRYP PFNGLSCISSOREXCLUSIVEARRAYVNVPROC)(GLuint first, GLsizei count, const GLint *v);
GLAPI PFNGLSCISSOREXCLUSIVEARRAYVNVPROC glad_glScissorExclusiveArrayvNV;
#define glScissorExclusiveArrayvNV glad_glScissorExclusiveArrayvNV
#endif
#ifndef GL_NV_shader_atomic_fp16_vector
#define GL_NV_shader_atomic_fp16_vector 1
GLAPI int GLAD_GL_NV_shader_atomic_fp16_vector;
#endif
#ifndef GL_NV_shader_noperspective_interpolation
#define GL_NV_shader_noperspective_interpolation 1
GLAPI int GLAD_GL_NV_shader_noperspective_interpolation;
#endif
#ifndef GL_NV_shader_subgroup_partitioned
#define GL_NV_shader_subgroup_partitioned 1
GLAPI int GLAD_GL_NV_shader_subgroup_partitioned;
#endif
#ifndef GL_NV_shader_texture_footprint
#define GL_NV_shader_texture_footprint 1
GLAPI int GLAD_GL_NV_shader_texture_footprint;
#endif
#ifndef GL_NV_shading_rate_image
#define GL_NV_shading_rate_image 1
GLAPI int GLAD_GL_NV_shading_rate_image;
typedef void (APIENTRYP PFNGLBINDSHADINGRATEIMAGENVPROC)(GLuint texture);
GLAPI PFNGLBINDSHADINGRATEIMAGENVPROC glad_glBindShadingRateImageNV;
#define glBindShadingRateImageNV glad_glBindShadingRateImageNV
typedef void (APIENTRYP PFNGLGETSHADINGRATEIMAGEPALETTENVPROC)(GLuint viewport, GLuint entry, GLenum *rate);
GLAPI PFNGLGETSHADINGRATEIMAGEPALETTENVPROC glad_glGetShadingRateImagePaletteNV;
#define glGetShadingRateImagePaletteNV glad_glGetShadingRateImagePaletteNV
typedef void (APIENTRYP PFNGLGETSHADINGRATESAMPLELOCATIONIVNVPROC)(GLenum rate, GLuint samples, GLuint index, GLint *location);
GLAPI PFNGLGETSHADINGRATESAMPLELOCATIONIVNVPROC glad_glGetShadingRateSampleLocationivNV;
#define glGetShadingRateSampleLocationivNV glad_glGetShadingRateSampleLocationivNV
typedef void (APIENTRYP PFNGLSHADINGRATEIMAGEBARRIERNVPROC)(GLboolean synchronize);
GLAPI PFNGLSHADINGRATEIMAGEBARRIERNVPROC glad_glShadingRateImageBarrierNV;
#define glShadingRateImageBarrierNV glad_glShadingRateImageBarrierNV
typedef void (APIENTRYP PFNGLSHADINGRATEIMAGEPALETTENVPROC)(GLuint viewport, GLuint first, GLsizei count, const GLenum *rates);
GLAPI PFNGLSHADINGRATEIMAGEPALETTENVPROC glad_glShadingRateImagePaletteNV;
#define glShadingRateImagePaletteNV glad_glShadingRateImagePaletteNV
typedef void (APIENTRYP PFNGLSHADINGRATESAMPLEORDERNVPROC)(GLenum order);
GLAPI PFNGLSHADINGRATESAMPLEORDERNVPROC glad_glShadingRateSampleOrderNV;
#define glShadingRateSampleOrderNV glad_glShadingRateSampleOrderNV
typedef void (APIENTRYP PFNGLSHADINGRATESAMPLEORDERCUSTOMNVPROC)(GLenum rate, GLuint samples, const GLint *locations);
GLAPI PFNGLSHADINGRATESAMPLEORDERCUSTOMNVPROC glad_glShadingRateSampleOrderCustomNV;
#define glShadingRateSampleOrderCustomNV glad_glShadingRateSampleOrderCustomNV
#endif
#ifndef GL_NV_shadow_samplers_array
#define GL_NV_shadow_samplers_array 1
GLAPI int GLAD_GL_NV_shadow_samplers_array;
#endif
#ifndef GL_NV_shadow_samplers_cube
#define GL_NV_shadow_samplers_cube 1
GLAPI int GLAD_GL_NV_shadow_samplers_cube;
#endif
#ifndef GL_NV_stereo_view_rendering
#define GL_NV_stereo_view_rendering 1
GLAPI int GLAD_GL_NV_stereo_view_rendering;
#endif
#ifndef GL_NV_texture_border_clamp
#define GL_NV_texture_border_clamp 1
GLAPI int GLAD_GL_NV_texture_border_clamp;
#endif
#ifndef GL_NV_texture_compression_s3tc_update
#define GL_NV_texture_compression_s3tc_update 1
GLAPI int GLAD_GL_NV_texture_compression_s3tc_update;
#endif
#ifndef GL_NV_texture_npot_2D_mipmap
#define GL_NV_texture_npot_2D_mipmap 1
GLAPI int GLAD_GL_NV_texture_npot_2D_mipmap;
#endif
#ifndef GL_NV_timeline_semaphore
#define GL_NV_timeline_semaphore 1
GLAPI int GLAD_GL_NV_timeline_semaphore;
typedef void (APIENTRYP PFNGLCREATESEMAPHORESNVPROC)(GLsizei n, GLuint *semaphores);
GLAPI PFNGLCREATESEMAPHORESNVPROC glad_glCreateSemaphoresNV;
#define glCreateSemaphoresNV glad_glCreateSemaphoresNV
typedef void (APIENTRYP PFNGLSEMAPHOREPARAMETERIVNVPROC)(GLuint semaphore, GLenum pname, const GLint *params);
GLAPI PFNGLSEMAPHOREPARAMETERIVNVPROC glad_glSemaphoreParameterivNV;
#define glSemaphoreParameterivNV glad_glSemaphoreParameterivNV
typedef void (APIENTRYP PFNGLGETSEMAPHOREPARAMETERIVNVPROC)(GLuint semaphore, GLenum pname, GLint *params);
GLAPI PFNGLGETSEMAPHOREPARAMETERIVNVPROC glad_glGetSemaphoreParameterivNV;
#define glGetSemaphoreParameterivNV glad_glGetSemaphoreParameterivNV
#endif
#ifndef GL_NV_viewport_array
#define GL_NV_viewport_array 1
GLAPI int GLAD_GL_NV_viewport_array;
typedef void (APIENTRYP PFNGLVIEWPORTARRAYVNVPROC)(GLuint first, GLsizei count, const GLfloat *v);
GLAPI PFNGLVIEWPORTARRAYVNVPROC glad_glViewportArrayvNV;
#define glViewportArrayvNV glad_glViewportArrayvNV
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFNVPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
GLAPI PFNGLVIEWPORTINDEXEDFNVPROC glad_glViewportIndexedfNV;
#define glViewportIndexedfNV glad_glViewportIndexedfNV
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFVNVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVIEWPORTINDEXEDFVNVPROC glad_glViewportIndexedfvNV;
#define glViewportIndexedfvNV glad_glViewportIndexedfvNV
typedef void (APIENTRYP PFNGLSCISSORARRAYVNVPROC)(GLuint first, GLsizei count, const GLint *v);
GLAPI PFNGLSCISSORARRAYVNVPROC glad_glScissorArrayvNV;
#define glScissorArrayvNV glad_glScissorArrayvNV
typedef void (APIENTRYP PFNGLSCISSORINDEXEDNVPROC)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSORINDEXEDNVPROC glad_glScissorIndexedNV;
#define glScissorIndexedNV glad_glScissorIndexedNV
typedef void (APIENTRYP PFNGLSCISSORINDEXEDVNVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLSCISSORINDEXEDVNVPROC glad_glScissorIndexedvNV;
#define glScissorIndexedvNV glad_glScissorIndexedvNV
typedef void (APIENTRYP PFNGLDEPTHRANGEARRAYFVNVPROC)(GLuint first, GLsizei count, const GLfloat *v);
GLAPI PFNGLDEPTHRANGEARRAYFVNVPROC glad_glDepthRangeArrayfvNV;
#define glDepthRangeArrayfvNV glad_glDepthRangeArrayfvNV
typedef void (APIENTRYP PFNGLDEPTHRANGEINDEXEDFNVPROC)(GLuint index, GLfloat n, GLfloat f);
GLAPI PFNGLDEPTHRANGEINDEXEDFNVPROC glad_glDepthRangeIndexedfNV;
#define glDepthRangeIndexedfNV glad_glDepthRangeIndexedfNV
typedef void (APIENTRYP PFNGLGETFLOATI_VNVPROC)(GLenum target, GLuint index, GLfloat *data);
GLAPI PFNGLGETFLOATI_VNVPROC glad_glGetFloati_vNV;
#define glGetFloati_vNV glad_glGetFloati_vNV
typedef void (APIENTRYP PFNGLENABLEINVPROC)(GLenum target, GLuint index);
GLAPI PFNGLENABLEINVPROC glad_glEnableiNV;
#define glEnableiNV glad_glEnableiNV
typedef void (APIENTRYP PFNGLDISABLEINVPROC)(GLenum target, GLuint index);
GLAPI PFNGLDISABLEINVPROC glad_glDisableiNV;
#define glDisableiNV glad_glDisableiNV
typedef GLboolean (APIENTRYP PFNGLISENABLEDINVPROC)(GLenum target, GLuint index);
GLAPI PFNGLISENABLEDINVPROC glad_glIsEnablediNV;
#define glIsEnablediNV glad_glIsEnablediNV
#endif
#ifndef GL_NV_viewport_array2
#define GL_NV_viewport_array2 1
GLAPI int GLAD_GL_NV_viewport_array2;
#endif
#ifndef GL_NV_viewport_swizzle
#define GL_NV_viewport_swizzle 1
GLAPI int GLAD_GL_NV_viewport_swizzle;
typedef void (APIENTRYP PFNGLVIEWPORTSWIZZLENVPROC)(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew);
GLAPI PFNGLVIEWPORTSWIZZLENVPROC glad_glViewportSwizzleNV;
#define glViewportSwizzleNV glad_glViewportSwizzleNV
#endif
#ifndef GL_OES_EGL_image
#define GL_OES_EGL_image 1
GLAPI int GLAD_GL_OES_EGL_image;
typedef void (APIENTRYP PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)(GLenum target, GLeglImageOES image);
GLAPI PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glad_glEGLImageTargetTexture2DOES;
#define glEGLImageTargetTexture2DOES glad_glEGLImageTargetTexture2DOES
typedef void (APIENTRYP PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC)(GLenum target, GLeglImageOES image);
GLAPI PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC glad_glEGLImageTargetRenderbufferStorageOES;
#define glEGLImageTargetRenderbufferStorageOES glad_glEGLImageTargetRenderbufferStorageOES
#endif
#ifndef GL_OES_EGL_image_external
#define GL_OES_EGL_image_external 1
GLAPI int GLAD_GL_OES_EGL_image_external;
#endif
#ifndef GL_OES_EGL_image_external_essl3
#define GL_OES_EGL_image_external_essl3 1
GLAPI int GLAD_GL_OES_EGL_image_external_essl3;
#endif
#ifndef GL_OES_compressed_ETC1_RGB8_sub_texture
#define GL_OES_compressed_ETC1_RGB8_sub_texture 1
GLAPI int GLAD_GL_OES_compressed_ETC1_RGB8_sub_texture;
#endif
#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_OES_compressed_ETC1_RGB8_texture 1
GLAPI int GLAD_GL_OES_compressed_ETC1_RGB8_texture;
#endif
#ifndef GL_OES_compressed_paletted_texture
#define GL_OES_compressed_paletted_texture 1
GLAPI int GLAD_GL_OES_compressed_paletted_texture;
#endif
#ifndef GL_OES_copy_image
#define GL_OES_copy_image 1
GLAPI int GLAD_GL_OES_copy_image;
typedef void (APIENTRYP PFNGLCOPYIMAGESUBDATAOESPROC)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GLAPI PFNGLCOPYIMAGESUBDATAOESPROC glad_glCopyImageSubDataOES;
#define glCopyImageSubDataOES glad_glCopyImageSubDataOES
#endif
#ifndef GL_OES_depth24
#define GL_OES_depth24 1
GLAPI int GLAD_GL_OES_depth24;
#endif
#ifndef GL_OES_depth32
#define GL_OES_depth32 1
GLAPI int GLAD_GL_OES_depth32;
#endif
#ifndef GL_OES_depth_texture
#define GL_OES_depth_texture 1
GLAPI int GLAD_GL_OES_depth_texture;
#endif
#ifndef GL_OES_draw_buffers_indexed
#define GL_OES_draw_buffers_indexed 1
GLAPI int GLAD_GL_OES_draw_buffers_indexed;
typedef void (APIENTRYP PFNGLENABLEIOESPROC)(GLenum target, GLuint index);
GLAPI PFNGLENABLEIOESPROC glad_glEnableiOES;
#define glEnableiOES glad_glEnableiOES
typedef void (APIENTRYP PFNGLDISABLEIOESPROC)(GLenum target, GLuint index);
GLAPI PFNGLDISABLEIOESPROC glad_glDisableiOES;
#define glDisableiOES glad_glDisableiOES
typedef void (APIENTRYP PFNGLBLENDEQUATIONIOESPROC)(GLuint buf, GLenum mode);
GLAPI PFNGLBLENDEQUATIONIOESPROC glad_glBlendEquationiOES;
#define glBlendEquationiOES glad_glBlendEquationiOES
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIOESPROC)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLAPI PFNGLBLENDEQUATIONSEPARATEIOESPROC glad_glBlendEquationSeparateiOES;
#define glBlendEquationSeparateiOES glad_glBlendEquationSeparateiOES
typedef void (APIENTRYP PFNGLBLENDFUNCIOESPROC)(GLuint buf, GLenum src, GLenum dst);
GLAPI PFNGLBLENDFUNCIOESPROC glad_glBlendFunciOES;
#define glBlendFunciOES glad_glBlendFunciOES
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIOESPROC)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLAPI PFNGLBLENDFUNCSEPARATEIOESPROC glad_glBlendFuncSeparateiOES;
#define glBlendFuncSeparateiOES glad_glBlendFuncSeparateiOES
typedef void (APIENTRYP PFNGLCOLORMASKIOESPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI PFNGLCOLORMASKIOESPROC glad_glColorMaskiOES;
#define glColorMaskiOES glad_glColorMaskiOES
typedef GLboolean (APIENTRYP PFNGLISENABLEDIOESPROC)(GLenum target, GLuint index);
GLAPI PFNGLISENABLEDIOESPROC glad_glIsEnablediOES;
#define glIsEnablediOES glad_glIsEnablediOES
#endif
#ifndef GL_OES_draw_elements_base_vertex
#define GL_OES_draw_elements_base_vertex 1
GLAPI int GLAD_GL_OES_draw_elements_base_vertex;
typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXOESPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSBASEVERTEXOESPROC glad_glDrawElementsBaseVertexOES;
#define glDrawElementsBaseVertexOES glad_glDrawElementsBaseVertexOES
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXOESPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXOESPROC glad_glDrawRangeElementsBaseVertexOES;
#define glDrawRangeElementsBaseVertexOES glad_glDrawRangeElementsBaseVertexOES
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXOESPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXOESPROC glad_glDrawElementsInstancedBaseVertexOES;
#define glDrawElementsInstancedBaseVertexOES glad_glDrawElementsInstancedBaseVertexOES
#endif
#ifndef GL_OES_element_index_uint
#define GL_OES_element_index_uint 1
GLAPI int GLAD_GL_OES_element_index_uint;
#endif
#ifndef GL_OES_fbo_render_mipmap
#define GL_OES_fbo_render_mipmap 1
GLAPI int GLAD_GL_OES_fbo_render_mipmap;
#endif
#ifndef GL_OES_fragment_precision_high
#define GL_OES_fragment_precision_high 1
GLAPI int GLAD_GL_OES_fragment_precision_high;
#endif
#ifndef GL_OES_geometry_point_size
#define GL_OES_geometry_point_size 1
GLAPI int GLAD_GL_OES_geometry_point_size;
#endif
#ifndef GL_OES_geometry_shader
#define GL_OES_geometry_shader 1
GLAPI int GLAD_GL_OES_geometry_shader;
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREOESPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTUREOESPROC glad_glFramebufferTextureOES;
#define glFramebufferTextureOES glad_glFramebufferTextureOES
#endif
#ifndef GL_OES_get_program_binary
#define GL_OES_get_program_binary 1
GLAPI int GLAD_GL_OES_get_program_binary;
typedef void (APIENTRYP PFNGLGETPROGRAMBINARYOESPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
GLAPI PFNGLGETPROGRAMBINARYOESPROC glad_glGetProgramBinaryOES;
#define glGetProgramBinaryOES glad_glGetProgramBinaryOES
typedef void (APIENTRYP PFNGLPROGRAMBINARYOESPROC)(GLuint program, GLenum binaryFormat, const void *binary, GLint length);
GLAPI PFNGLPROGRAMBINARYOESPROC glad_glProgramBinaryOES;
#define glProgramBinaryOES glad_glProgramBinaryOES
#endif
#ifndef GL_OES_gpu_shader5
#define GL_OES_gpu_shader5 1
GLAPI int GLAD_GL_OES_gpu_shader5;
#endif
#ifndef GL_OES_mapbuffer
#define GL_OES_mapbuffer 1
GLAPI int GLAD_GL_OES_mapbuffer;
typedef void * (APIENTRYP PFNGLMAPBUFFEROESPROC)(GLenum target, GLenum access);
GLAPI PFNGLMAPBUFFEROESPROC glad_glMapBufferOES;
#define glMapBufferOES glad_glMapBufferOES
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFEROESPROC)(GLenum target);
GLAPI PFNGLUNMAPBUFFEROESPROC glad_glUnmapBufferOES;
#define glUnmapBufferOES glad_glUnmapBufferOES
typedef void (APIENTRYP PFNGLGETBUFFERPOINTERVOESPROC)(GLenum target, GLenum pname, void **params);
GLAPI PFNGLGETBUFFERPOINTERVOESPROC glad_glGetBufferPointervOES;
#define glGetBufferPointervOES glad_glGetBufferPointervOES
#endif
#ifndef GL_OES_packed_depth_stencil
#define GL_OES_packed_depth_stencil 1
GLAPI int GLAD_GL_OES_packed_depth_stencil;
#endif
#ifndef GL_OES_primitive_bounding_box
#define GL_OES_primitive_bounding_box 1
GLAPI int GLAD_GL_OES_primitive_bounding_box;
typedef void (APIENTRYP PFNGLPRIMITIVEBOUNDINGBOXOESPROC)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
GLAPI PFNGLPRIMITIVEBOUNDINGBOXOESPROC glad_glPrimitiveBoundingBoxOES;
#define glPrimitiveBoundingBoxOES glad_glPrimitiveBoundingBoxOES
#endif
#ifndef GL_OES_required_internalformat
#define GL_OES_required_internalformat 1
GLAPI int GLAD_GL_OES_required_internalformat;
#endif
#ifndef GL_OES_rgb8_rgba8
#define GL_OES_rgb8_rgba8 1
GLAPI int GLAD_GL_OES_rgb8_rgba8;
#endif
#ifndef GL_OES_sample_shading
#define GL_OES_sample_shading 1
GLAPI int GLAD_GL_OES_sample_shading;
typedef void (APIENTRYP PFNGLMINSAMPLESHADINGOESPROC)(GLfloat value);
GLAPI PFNGLMINSAMPLESHADINGOESPROC glad_glMinSampleShadingOES;
#define glMinSampleShadingOES glad_glMinSampleShadingOES
#endif
#ifndef GL_OES_sample_variables
#define GL_OES_sample_variables 1
GLAPI int GLAD_GL_OES_sample_variables;
#endif
#ifndef GL_OES_shader_image_atomic
#define GL_OES_shader_image_atomic 1
GLAPI int GLAD_GL_OES_shader_image_atomic;
#endif
#ifndef GL_OES_shader_io_blocks
#define GL_OES_shader_io_blocks 1
GLAPI int GLAD_GL_OES_shader_io_blocks;
#endif
#ifndef GL_OES_shader_multisample_interpolation
#define GL_OES_shader_multisample_interpolation 1
GLAPI int GLAD_GL_OES_shader_multisample_interpolation;
#endif
#ifndef GL_OES_standard_derivatives
#define GL_OES_standard_derivatives 1
GLAPI int GLAD_GL_OES_standard_derivatives;
#endif
#ifndef GL_OES_stencil1
#define GL_OES_stencil1 1
GLAPI int GLAD_GL_OES_stencil1;
#endif
#ifndef GL_OES_stencil4
#define GL_OES_stencil4 1
GLAPI int GLAD_GL_OES_stencil4;
#endif
#ifndef GL_OES_surfaceless_context
#define GL_OES_surfaceless_context 1
GLAPI int GLAD_GL_OES_surfaceless_context;
#endif
#ifndef GL_OES_tessellation_point_size
#define GL_OES_tessellation_point_size 1
GLAPI int GLAD_GL_OES_tessellation_point_size;
#endif
#ifndef GL_OES_tessellation_shader
#define GL_OES_tessellation_shader 1
GLAPI int GLAD_GL_OES_tessellation_shader;
typedef void (APIENTRYP PFNGLPATCHPARAMETERIOESPROC)(GLenum pname, GLint value);
GLAPI PFNGLPATCHPARAMETERIOESPROC glad_glPatchParameteriOES;
#define glPatchParameteriOES glad_glPatchParameteriOES
#endif
#ifndef GL_OES_texture_3D
#define GL_OES_texture_3D 1
GLAPI int GLAD_GL_OES_texture_3D;
typedef void (APIENTRYP PFNGLTEXIMAGE3DOESPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE3DOESPROC glad_glTexImage3DOES;
#define glTexImage3DOES glad_glTexImage3DOES
typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE3DOESPROC glad_glTexSubImage3DOES;
#define glTexSubImage3DOES glad_glTexSubImage3DOES
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE3DOESPROC glad_glCopyTexSubImage3DOES;
#define glCopyTexSubImage3DOES glad_glCopyTexSubImage3DOES
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DOESPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE3DOESPROC glad_glCompressedTexImage3DOES;
#define glCompressedTexImage3DOES glad_glCompressedTexImage3DOES
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC glad_glCompressedTexSubImage3DOES;
#define glCompressedTexSubImage3DOES glad_glCompressedTexSubImage3DOES
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DOESPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLAPI PFNGLFRAMEBUFFERTEXTURE3DOESPROC glad_glFramebufferTexture3DOES;
#define glFramebufferTexture3DOES glad_glFramebufferTexture3DOES
#endif
#ifndef GL_OES_texture_border_clamp
#define GL_OES_texture_border_clamp 1
GLAPI int GLAD_GL_OES_texture_border_clamp;
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVOESPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIIVOESPROC glad_glTexParameterIivOES;
#define glTexParameterIivOES glad_glTexParameterIivOES
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVOESPROC)(GLenum target, GLenum pname, const GLuint *params);
GLAPI PFNGLTEXPARAMETERIUIVOESPROC glad_glTexParameterIuivOES;
#define glTexParameterIuivOES glad_glTexParameterIuivOES
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVOESPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIIVOESPROC glad_glGetTexParameterIivOES;
#define glGetTexParameterIivOES glad_glGetTexParameterIivOES
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVOESPROC)(GLenum target, GLenum pname, GLuint *params);
GLAPI PFNGLGETTEXPARAMETERIUIVOESPROC glad_glGetTexParameterIuivOES;
#define glGetTexParameterIuivOES glad_glGetTexParameterIuivOES
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVOESPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIIVOESPROC glad_glSamplerParameterIivOES;
#define glSamplerParameterIivOES glad_glSamplerParameterIivOES
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVOESPROC)(GLuint sampler, GLenum pname, const GLuint *param);
GLAPI PFNGLSAMPLERPARAMETERIUIVOESPROC glad_glSamplerParameterIuivOES;
#define glSamplerParameterIuivOES glad_glSamplerParameterIuivOES
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVOESPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIIVOESPROC glad_glGetSamplerParameterIivOES;
#define glGetSamplerParameterIivOES glad_glGetSamplerParameterIivOES
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIUIVOESPROC)(GLuint sampler, GLenum pname, GLuint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIUIVOESPROC glad_glGetSamplerParameterIuivOES;
#define glGetSamplerParameterIuivOES glad_glGetSamplerParameterIuivOES
#endif
#ifndef GL_OES_texture_buffer
#define GL_OES_texture_buffer 1
GLAPI int GLAD_GL_OES_texture_buffer;
typedef void (APIENTRYP PFNGLTEXBUFFEROESPROC)(GLenum target, GLenum internalformat, GLuint buffer);
GLAPI PFNGLTEXBUFFEROESPROC glad_glTexBufferOES;
#define glTexBufferOES glad_glTexBufferOES
typedef void (APIENTRYP PFNGLTEXBUFFERRANGEOESPROC)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI PFNGLTEXBUFFERRANGEOESPROC glad_glTexBufferRangeOES;
#define glTexBufferRangeOES glad_glTexBufferRangeOES
#endif
#ifndef GL_OES_texture_compression_astc
#define GL_OES_texture_compression_astc 1
GLAPI int GLAD_GL_OES_texture_compression_astc;
#endif
#ifndef GL_OES_texture_cube_map_array
#define GL_OES_texture_cube_map_array 1
GLAPI int GLAD_GL_OES_texture_cube_map_array;
#endif
#ifndef GL_OES_texture_float
#define GL_OES_texture_float 1
GLAPI int GLAD_GL_OES_texture_float;
#endif
#ifndef GL_OES_texture_float_linear
#define GL_OES_texture_float_linear 1
GLAPI int GLAD_GL_OES_texture_float_linear;
#endif
#ifndef GL_OES_texture_half_float
#define GL_OES_texture_half_float 1
GLAPI int GLAD_GL_OES_texture_half_float;
#endif
#ifndef GL_OES_texture_half_float_linear
#define GL_OES_texture_half_float_linear 1
GLAPI int GLAD_GL_OES_texture_half_float_linear;
#endif
#ifndef GL_OES_texture_npot
#define GL_OES_texture_npot 1
GLAPI int GLAD_GL_OES_texture_npot;
#endif
#ifndef GL_OES_texture_stencil8
#define GL_OES_texture_stencil8 1
GLAPI int GLAD_GL_OES_texture_stencil8;
#endif
#ifndef GL_OES_texture_storage_multisample_2d_array
#define GL_OES_texture_storage_multisample_2d_array 1
GLAPI int GLAD_GL_OES_texture_storage_multisample_2d_array;
typedef void (APIENTRYP PFNGLTEXSTORAGE3DMULTISAMPLEOESPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI PFNGLTEXSTORAGE3DMULTISAMPLEOESPROC glad_glTexStorage3DMultisampleOES;
#define glTexStorage3DMultisampleOES glad_glTexStorage3DMultisampleOES
#endif
#ifndef GL_OES_texture_view
#define GL_OES_texture_view 1
GLAPI int GLAD_GL_OES_texture_view;
typedef void (APIENTRYP PFNGLTEXTUREVIEWOESPROC)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
GLAPI PFNGLTEXTUREVIEWOESPROC glad_glTextureViewOES;
#define glTextureViewOES glad_glTextureViewOES
#endif
#ifndef GL_OES_vertex_array_object
#define GL_OES_vertex_array_object 1
GLAPI int GLAD_GL_OES_vertex_array_object;
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYOESPROC)(GLuint array);
GLAPI PFNGLBINDVERTEXARRAYOESPROC glad_glBindVertexArrayOES;
#define glBindVertexArrayOES glad_glBindVertexArrayOES
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSOESPROC)(GLsizei n, const GLuint *arrays);
GLAPI PFNGLDELETEVERTEXARRAYSOESPROC glad_glDeleteVertexArraysOES;
#define glDeleteVertexArraysOES glad_glDeleteVertexArraysOES
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSOESPROC)(GLsizei n, GLuint *arrays);
GLAPI PFNGLGENVERTEXARRAYSOESPROC glad_glGenVertexArraysOES;
#define glGenVertexArraysOES glad_glGenVertexArraysOES
typedef GLboolean (APIENTRYP PFNGLISVERTEXARRAYOESPROC)(GLuint array);
GLAPI PFNGLISVERTEXARRAYOESPROC glad_glIsVertexArrayOES;
#define glIsVertexArrayOES glad_glIsVertexArrayOES
#endif
#ifndef GL_OES_vertex_half_float
#define GL_OES_vertex_half_float 1
GLAPI int GLAD_GL_OES_vertex_half_float;
#endif
#ifndef GL_OES_vertex_type_10_10_10_2
#define GL_OES_vertex_type_10_10_10_2 1
GLAPI int GLAD_GL_OES_vertex_type_10_10_10_2;
#endif
#ifndef GL_OES_viewport_array
#define GL_OES_viewport_array 1
GLAPI int GLAD_GL_OES_viewport_array;
typedef void (APIENTRYP PFNGLVIEWPORTARRAYVOESPROC)(GLuint first, GLsizei count, const GLfloat *v);
GLAPI PFNGLVIEWPORTARRAYVOESPROC glad_glViewportArrayvOES;
#define glViewportArrayvOES glad_glViewportArrayvOES
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFOESPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
GLAPI PFNGLVIEWPORTINDEXEDFOESPROC glad_glViewportIndexedfOES;
#define glViewportIndexedfOES glad_glViewportIndexedfOES
typedef void (APIENTRYP PFNGLVIEWPORTINDEXEDFVOESPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVIEWPORTINDEXEDFVOESPROC glad_glViewportIndexedfvOES;
#define glViewportIndexedfvOES glad_glViewportIndexedfvOES
typedef void (APIENTRYP PFNGLSCISSORARRAYVOESPROC)(GLuint first, GLsizei count, const GLint *v);
GLAPI PFNGLSCISSORARRAYVOESPROC glad_glScissorArrayvOES;
#define glScissorArrayvOES glad_glScissorArrayvOES
typedef void (APIENTRYP PFNGLSCISSORINDEXEDOESPROC)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSORINDEXEDOESPROC glad_glScissorIndexedOES;
#define glScissorIndexedOES glad_glScissorIndexedOES
typedef void (APIENTRYP PFNGLSCISSORINDEXEDVOESPROC)(GLuint index, const GLint *v);
GLAPI PFNGLSCISSORINDEXEDVOESPROC glad_glScissorIndexedvOES;
#define glScissorIndexedvOES glad_glScissorIndexedvOES
typedef void (APIENTRYP PFNGLDEPTHRANGEARRAYFVOESPROC)(GLuint first, GLsizei count, const GLfloat *v);
GLAPI PFNGLDEPTHRANGEARRAYFVOESPROC glad_glDepthRangeArrayfvOES;
#define glDepthRangeArrayfvOES glad_glDepthRangeArrayfvOES
typedef void (APIENTRYP PFNGLDEPTHRANGEINDEXEDFOESPROC)(GLuint index, GLfloat n, GLfloat f);
GLAPI PFNGLDEPTHRANGEINDEXEDFOESPROC glad_glDepthRangeIndexedfOES;
#define glDepthRangeIndexedfOES glad_glDepthRangeIndexedfOES
typedef void (APIENTRYP PFNGLGETFLOATI_VOESPROC)(GLenum target, GLuint index, GLfloat *data);
GLAPI PFNGLGETFLOATI_VOESPROC glad_glGetFloati_vOES;
#define glGetFloati_vOES glad_glGetFloati_vOES
#endif
#ifndef GL_OVR_multiview
#define GL_OVR_multiview 1
GLAPI int GLAD_GL_OVR_multiview;
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews);
GLAPI PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC glad_glFramebufferTextureMultiviewOVR;
#define glFramebufferTextureMultiviewOVR glad_glFramebufferTextureMultiviewOVR
#endif
#ifndef GL_OVR_multiview2
#define GL_OVR_multiview2 1
GLAPI int GLAD_GL_OVR_multiview2;
#endif
#ifndef GL_OVR_multiview_multisampled_render_to_texture
#define GL_OVR_multiview_multisampled_render_to_texture 1
GLAPI int GLAD_GL_OVR_multiview_multisampled_render_to_texture;
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLsizei samples, GLint baseViewIndex, GLsizei numViews);
GLAPI PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC glad_glFramebufferTextureMultisampleMultiviewOVR;
#define glFramebufferTextureMultisampleMultiviewOVR glad_glFramebufferTextureMultisampleMultiviewOVR
#endif
#ifndef GL_QCOM_YUV_texture_gather
#define GL_QCOM_YUV_texture_gather 1
GLAPI int GLAD_GL_QCOM_YUV_texture_gather;
#endif
#ifndef GL_QCOM_alpha_test
#define GL_QCOM_alpha_test 1
GLAPI int GLAD_GL_QCOM_alpha_test;
typedef void (APIENTRYP PFNGLALPHAFUNCQCOMPROC)(GLenum func, GLclampf ref);
GLAPI PFNGLALPHAFUNCQCOMPROC glad_glAlphaFuncQCOM;
#define glAlphaFuncQCOM glad_glAlphaFuncQCOM
#endif
#ifndef GL_QCOM_binning_control
#define GL_QCOM_binning_control 1
GLAPI int GLAD_GL_QCOM_binning_control;
#endif
#ifndef GL_QCOM_driver_control
#define GL_QCOM_driver_control 1
GLAPI int GLAD_GL_QCOM_driver_control;
typedef void (APIENTRYP PFNGLGETDRIVERCONTROLSQCOMPROC)(GLint *num, GLsizei size, GLuint *driverControls);
GLAPI PFNGLGETDRIVERCONTROLSQCOMPROC glad_glGetDriverControlsQCOM;
#define glGetDriverControlsQCOM glad_glGetDriverControlsQCOM
typedef void (APIENTRYP PFNGLGETDRIVERCONTROLSTRINGQCOMPROC)(GLuint driverControl, GLsizei bufSize, GLsizei *length, GLchar *driverControlString);
GLAPI PFNGLGETDRIVERCONTROLSTRINGQCOMPROC glad_glGetDriverControlStringQCOM;
#define glGetDriverControlStringQCOM glad_glGetDriverControlStringQCOM
typedef void (APIENTRYP PFNGLENABLEDRIVERCONTROLQCOMPROC)(GLuint driverControl);
GLAPI PFNGLENABLEDRIVERCONTROLQCOMPROC glad_glEnableDriverControlQCOM;
#define glEnableDriverControlQCOM glad_glEnableDriverControlQCOM
typedef void (APIENTRYP PFNGLDISABLEDRIVERCONTROLQCOMPROC)(GLuint driverControl);
GLAPI PFNGLDISABLEDRIVERCONTROLQCOMPROC glad_glDisableDriverControlQCOM;
#define glDisableDriverControlQCOM glad_glDisableDriverControlQCOM
#endif
#ifndef GL_QCOM_extended_get
#define GL_QCOM_extended_get 1
GLAPI int GLAD_GL_QCOM_extended_get;
typedef void (APIENTRYP PFNGLEXTGETTEXTURESQCOMPROC)(GLuint *textures, GLint maxTextures, GLint *numTextures);
GLAPI PFNGLEXTGETTEXTURESQCOMPROC glad_glExtGetTexturesQCOM;
#define glExtGetTexturesQCOM glad_glExtGetTexturesQCOM
typedef void (APIENTRYP PFNGLEXTGETBUFFERSQCOMPROC)(GLuint *buffers, GLint maxBuffers, GLint *numBuffers);
GLAPI PFNGLEXTGETBUFFERSQCOMPROC glad_glExtGetBuffersQCOM;
#define glExtGetBuffersQCOM glad_glExtGetBuffersQCOM
typedef void (APIENTRYP PFNGLEXTGETRENDERBUFFERSQCOMPROC)(GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers);
GLAPI PFNGLEXTGETRENDERBUFFERSQCOMPROC glad_glExtGetRenderbuffersQCOM;
#define glExtGetRenderbuffersQCOM glad_glExtGetRenderbuffersQCOM
typedef void (APIENTRYP PFNGLEXTGETFRAMEBUFFERSQCOMPROC)(GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers);
GLAPI PFNGLEXTGETFRAMEBUFFERSQCOMPROC glad_glExtGetFramebuffersQCOM;
#define glExtGetFramebuffersQCOM glad_glExtGetFramebuffersQCOM
typedef void (APIENTRYP PFNGLEXTGETTEXLEVELPARAMETERIVQCOMPROC)(GLuint texture, GLenum face, GLint level, GLenum pname, GLint *params);
GLAPI PFNGLEXTGETTEXLEVELPARAMETERIVQCOMPROC glad_glExtGetTexLevelParameterivQCOM;
#define glExtGetTexLevelParameterivQCOM glad_glExtGetTexLevelParameterivQCOM
typedef void (APIENTRYP PFNGLEXTTEXOBJECTSTATEOVERRIDEIQCOMPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLEXTTEXOBJECTSTATEOVERRIDEIQCOMPROC glad_glExtTexObjectStateOverrideiQCOM;
#define glExtTexObjectStateOverrideiQCOM glad_glExtTexObjectStateOverrideiQCOM
typedef void (APIENTRYP PFNGLEXTGETTEXSUBIMAGEQCOMPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void *texels);
GLAPI PFNGLEXTGETTEXSUBIMAGEQCOMPROC glad_glExtGetTexSubImageQCOM;
#define glExtGetTexSubImageQCOM glad_glExtGetTexSubImageQCOM
typedef void (APIENTRYP PFNGLEXTGETBUFFERPOINTERVQCOMPROC)(GLenum target, void **params);
GLAPI PFNGLEXTGETBUFFERPOINTERVQCOMPROC glad_glExtGetBufferPointervQCOM;
#define glExtGetBufferPointervQCOM glad_glExtGetBufferPointervQCOM
#endif
#ifndef GL_QCOM_extended_get2
#define GL_QCOM_extended_get2 1
GLAPI int GLAD_GL_QCOM_extended_get2;
typedef void (APIENTRYP PFNGLEXTGETSHADERSQCOMPROC)(GLuint *shaders, GLint maxShaders, GLint *numShaders);
GLAPI PFNGLEXTGETSHADERSQCOMPROC glad_glExtGetShadersQCOM;
#define glExtGetShadersQCOM glad_glExtGetShadersQCOM
typedef void (APIENTRYP PFNGLEXTGETPROGRAMSQCOMPROC)(GLuint *programs, GLint maxPrograms, GLint *numPrograms);
GLAPI PFNGLEXTGETPROGRAMSQCOMPROC glad_glExtGetProgramsQCOM;
#define glExtGetProgramsQCOM glad_glExtGetProgramsQCOM
typedef GLboolean (APIENTRYP PFNGLEXTISPROGRAMBINARYQCOMPROC)(GLuint program);
GLAPI PFNGLEXTISPROGRAMBINARYQCOMPROC glad_glExtIsProgramBinaryQCOM;
#define glExtIsProgramBinaryQCOM glad_glExtIsProgramBinaryQCOM
typedef void (APIENTRYP PFNGLEXTGETPROGRAMBINARYSOURCEQCOMPROC)(GLuint program, GLenum shadertype, GLchar *source, GLint *length);
GLAPI PFNGLEXTGETPROGRAMBINARYSOURCEQCOMPROC glad_glExtGetProgramBinarySourceQCOM;
#define glExtGetProgramBinarySourceQCOM glad_glExtGetProgramBinarySourceQCOM
#endif
#ifndef GL_QCOM_frame_extrapolation
#define GL_QCOM_frame_extrapolation 1
GLAPI int GLAD_GL_QCOM_frame_extrapolation;
typedef void (APIENTRYP PFNGLEXTRAPOLATETEX2DQCOMPROC)(GLuint src1, GLuint src2, GLuint output, GLfloat scaleFactor);
GLAPI PFNGLEXTRAPOLATETEX2DQCOMPROC glad_glExtrapolateTex2DQCOM;
#define glExtrapolateTex2DQCOM glad_glExtrapolateTex2DQCOM
#endif
#ifndef GL_QCOM_framebuffer_foveated
#define GL_QCOM_framebuffer_foveated 1
GLAPI int GLAD_GL_QCOM_framebuffer_foveated;
typedef void (APIENTRYP PFNGLFRAMEBUFFERFOVEATIONCONFIGQCOMPROC)(GLuint framebuffer, GLuint numLayers, GLuint focalPointsPerLayer, GLuint requestedFeatures, GLuint *providedFeatures);
GLAPI PFNGLFRAMEBUFFERFOVEATIONCONFIGQCOMPROC glad_glFramebufferFoveationConfigQCOM;
#define glFramebufferFoveationConfigQCOM glad_glFramebufferFoveationConfigQCOM
typedef void (APIENTRYP PFNGLFRAMEBUFFERFOVEATIONPARAMETERSQCOMPROC)(GLuint framebuffer, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea);
GLAPI PFNGLFRAMEBUFFERFOVEATIONPARAMETERSQCOMPROC glad_glFramebufferFoveationParametersQCOM;
#define glFramebufferFoveationParametersQCOM glad_glFramebufferFoveationParametersQCOM
#endif
#ifndef GL_QCOM_motion_estimation
#define GL_QCOM_motion_estimation 1
GLAPI int GLAD_GL_QCOM_motion_estimation;
typedef void (APIENTRYP PFNGLTEXESTIMATEMOTIONQCOMPROC)(GLuint ref, GLuint target, GLuint output);
GLAPI PFNGLTEXESTIMATEMOTIONQCOMPROC glad_glTexEstimateMotionQCOM;
#define glTexEstimateMotionQCOM glad_glTexEstimateMotionQCOM
typedef void (APIENTRYP PFNGLTEXESTIMATEMOTIONREGIONSQCOMPROC)(GLuint ref, GLuint target, GLuint output, GLuint mask);
GLAPI PFNGLTEXESTIMATEMOTIONREGIONSQCOMPROC glad_glTexEstimateMotionRegionsQCOM;
#define glTexEstimateMotionRegionsQCOM glad_glTexEstimateMotionRegionsQCOM
#endif
#ifndef GL_QCOM_perfmon_global_mode
#define GL_QCOM_perfmon_global_mode 1
GLAPI int GLAD_GL_QCOM_perfmon_global_mode;
#endif
#ifndef GL_QCOM_render_shared_exponent
#define GL_QCOM_render_shared_exponent 1
GLAPI int GLAD_GL_QCOM_render_shared_exponent;
#endif
#ifndef GL_QCOM_shader_framebuffer_fetch_noncoherent
#define GL_QCOM_shader_framebuffer_fetch_noncoherent 1
GLAPI int GLAD_GL_QCOM_shader_framebuffer_fetch_noncoherent;
typedef void (APIENTRYP PFNGLFRAMEBUFFERFETCHBARRIERQCOMPROC)(void);
GLAPI PFNGLFRAMEBUFFERFETCHBARRIERQCOMPROC glad_glFramebufferFetchBarrierQCOM;
#define glFramebufferFetchBarrierQCOM glad_glFramebufferFetchBarrierQCOM
#endif
#ifndef GL_QCOM_shader_framebuffer_fetch_rate
#define GL_QCOM_shader_framebuffer_fetch_rate 1
GLAPI int GLAD_GL_QCOM_shader_framebuffer_fetch_rate;
#endif
#ifndef GL_QCOM_shading_rate
#define GL_QCOM_shading_rate 1
GLAPI int GLAD_GL_QCOM_shading_rate;
typedef void (APIENTRYP PFNGLSHADINGRATEQCOMPROC)(GLenum rate);
GLAPI PFNGLSHADINGRATEQCOMPROC glad_glShadingRateQCOM;
#define glShadingRateQCOM glad_glShadingRateQCOM
#endif
#ifndef GL_QCOM_texture_foveated
#define GL_QCOM_texture_foveated 1
GLAPI int GLAD_GL_QCOM_texture_foveated;
typedef void (APIENTRYP PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC)(GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea);
GLAPI PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC glad_glTextureFoveationParametersQCOM;
#define glTextureFoveationParametersQCOM glad_glTextureFoveationParametersQCOM
#endif
#ifndef GL_QCOM_texture_foveated2
#define GL_QCOM_texture_foveated2 1
GLAPI int GLAD_GL_QCOM_texture_foveated2;
#endif
#ifndef GL_QCOM_texture_foveated_subsampled_layout
#define GL_QCOM_texture_foveated_subsampled_layout 1
GLAPI int GLAD_GL_QCOM_texture_foveated_subsampled_layout;
#endif
#ifndef GL_QCOM_tiled_rendering
#define GL_QCOM_tiled_rendering 1
GLAPI int GLAD_GL_QCOM_tiled_rendering;
typedef void (APIENTRYP PFNGLSTARTTILINGQCOMPROC)(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask);
GLAPI PFNGLSTARTTILINGQCOMPROC glad_glStartTilingQCOM;
#define glStartTilingQCOM glad_glStartTilingQCOM
typedef void (APIENTRYP PFNGLENDTILINGQCOMPROC)(GLbitfield preserveMask);
GLAPI PFNGLENDTILINGQCOMPROC glad_glEndTilingQCOM;
#define glEndTilingQCOM glad_glEndTilingQCOM
#endif
#ifndef GL_QCOM_writeonly_rendering
#define GL_QCOM_writeonly_rendering 1
GLAPI int GLAD_GL_QCOM_writeonly_rendering;
#endif
#ifndef GL_VIV_shader_binary
#define GL_VIV_shader_binary 1
GLAPI int GLAD_GL_VIV_shader_binary;
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif


#ifdef HEXO_GRAPHICS_RHI_D3D12
	/// comes with Windows SDK
	#include <d3d12.h>
	#include <dxgi1_6.h>


/** \mainpage D3D12 Memory Allocator

<b>Version 2.0.1</b> (2022-04-05)

Copyright (c) 2019-2022 Advanced Micro Devices, Inc. All rights reserved. \n
License: MIT

Documentation of all members: D3D12MemAlloc.h

\section main_table_of_contents Table of contents

- \subpage quick_start
    - [Project setup](@ref quick_start_project_setup)
    - [Creating resources](@ref quick_start_creating_resources)
    - [Resource reference counting](@ref quick_start_resource_reference_counting)
    - [Mapping memory](@ref quick_start_mapping_memory)
- \subpage custom_pools
- \subpage defragmentation
- \subpage statistics
- \subpage resource_aliasing
- \subpage linear_algorithm
- \subpage virtual_allocator
- \subpage configuration
  - [Custom CPU memory allocator](@ref custom_memory_allocator)
  - [Debug margins](@ref debug_margins)
- \subpage general_considerations
  - [Thread safety](@ref general_considerations_thread_safety)
  - [Versioning and compatibility](@ref general_considerations_versioning_and_compatibility)
  - [Features not supported](@ref general_considerations_features_not_supported)
		
\section main_see_also See also

- [Product page on GPUOpen](https://gpuopen.com/gaming-product/d3d12-memory-allocator/)
- [Source repository on GitHub](https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator)
*/

#ifndef D3D12MA_D3D12_HEADERS_ALREADY_INCLUDED
    #include <d3d12.h>
    #include <dxgi1_4.h>
#endif

#ifndef D3D12MA_DXGI_1_4
    #ifdef __IDXGIAdapter3_INTERFACE_DEFINED__
        #define D3D12MA_DXGI_1_4 1
    #else
        #define D3D12MA_DXGI_1_4 0
    #endif
#endif

/*
When defined to value other than 0, the library will try to use
D3D12_SMALL_RESOURCE_PLACEMENT_ALIGNMENT or D3D12_SMALL_MSAA_RESOURCE_PLACEMENT_ALIGNMENT
for created textures when possible, which can save memory because some small textures
may get their alignment 4K and their size a multiply of 4K instead of 64K.

#define D3D12MA_USE_SMALL_RESOURCE_PLACEMENT_ALIGNMENT 0
    Disables small texture alignment.
#define D3D12MA_USE_SMALL_RESOURCE_PLACEMENT_ALIGNMENT 1
    Enables conservative algorithm that will use small alignment only for some textures
    that are surely known to support it.
#define D3D12MA_USE_SMALL_RESOURCE_PLACEMENT_ALIGNMENT 2
    Enables query for small alignment to D3D12 (based on Microsoft sample) which will
    enable small alignment for more textures, but will also generate D3D Debug Layer
    error #721 on call to ID3D12Device::GetResourceAllocationInfo, which you should just
    ignore.
*/
#ifndef D3D12MA_USE_SMALL_RESOURCE_PLACEMENT_ALIGNMENT
    #define D3D12MA_USE_SMALL_RESOURCE_PLACEMENT_ALIGNMENT 1
#endif


#define D3D12MA_CLASS_NO_COPY(className) \
    private: \
        className(const className&) = delete; \
        className(className&&) = delete; \
        className& operator=(const className&) = delete; \
        className& operator=(className&&) = delete;

#define FACILITY_D3D12MA 3542

/*
If providing your own implementation, you need to implement a subset of std::atomic.
*/
#if !defined(D3D12MA_ATOMIC_UINT32) || !defined(D3D12MA_ATOMIC_UINT64)
    #include <atomic>
#endif

#ifndef D3D12MA_ATOMIC_UINT32
    #define D3D12MA_ATOMIC_UINT32 std::atomic<UINT>
#endif

#ifndef D3D12MA_ATOMIC_UINT64
    #define D3D12MA_ATOMIC_UINT64 std::atomic<UINT64>
#endif

#ifdef D3D12MA_EXPORTS
    #define D3D12MA_API __declspec(dllexport)
#elif defined(D3D12MA_IMPORTS)
    #define D3D12MA_API __declspec(dllimport)
#else
    #define D3D12MA_API
#endif

struct ID3D12ProtectedResourceSession;

namespace D3D12MA
{
class D3D12MA_API IUnknownImpl : public IUnknown
{
public:
    virtual ~IUnknownImpl() = default;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
protected:
    virtual void ReleaseThis() { delete this; }
private:
    D3D12MA_ATOMIC_UINT32 m_RefCount = 1;
};
} // namespace D3D12MA


namespace D3D12MA
{

class DefragmentationContextPimpl;
class AllocatorPimpl;
class PoolPimpl;
class NormalBlock;
class BlockVector;
class CommittedAllocationList;
class JsonWriter;
class VirtualBlockPimpl;

class Pool;
class Allocator;
struct Statistics;
struct DetailedStatistics;
struct TotalStatistics;

typedef UINT64 AllocHandle;

using ALLOCATE_FUNC_PTR = void* (*)(size_t Size, size_t Alignment, void* pPrivateData);
/**
\brief Pointer to custom callback function that deallocates CPU memory.

`pMemory = null` should be accepted and ignored.
*/
using FREE_FUNC_PTR = void (*)(void* pMemory, void* pPrivateData);

struct ALLOCATION_CALLBACKS
{
    /// %Allocation function.
    ALLOCATE_FUNC_PTR pAllocate;
    /// Dellocation function.
    FREE_FUNC_PTR pFree;
    /// Custom data that will be passed to allocation and deallocation functions as `pUserData` parameter.
    void* pPrivateData;
};


enum ALLOCATION_FLAGS
{
    /// Zero
    ALLOCATION_FLAG_NONE = 0,

    /**
    Set this flag if the allocation should have its own dedicated memory allocation (committed resource with implicit heap).
    
    Use it for special, big resources, like fullscreen textures used as render targets.

    - When used with functions like D3D12MA::Allocator::CreateResource, it will use `ID3D12Device::CreateCommittedResource`,
      so the created allocation will contain a resource (D3D12MA::Allocation::GetResource() `!= NULL`) but will not have
      a heap (D3D12MA::Allocation::GetHeap() `== NULL`), as the heap is implicit.
    - When used with raw memory allocation like D3D12MA::Allocator::AllocateMemory, it will use `ID3D12Device::CreateHeap`,
      so the created allocation will contain a heap (D3D12MA::Allocation::GetHeap() `!= NULL`) and its offset will always be 0.
    */
    ALLOCATION_FLAG_COMMITTED = 0x1,

    /**
    Set this flag to only try to allocate from existing memory heaps and never create new such heap.

    If new allocation cannot be placed in any of the existing heaps, allocation
    fails with `E_OUTOFMEMORY` error.

    You should not use D3D12MA::ALLOCATION_FLAG_COMMITTED and
    D3D12MA::ALLOCATION_FLAG_NEVER_ALLOCATE at the same time. It makes no sense.
    */
    ALLOCATION_FLAG_NEVER_ALLOCATE = 0x2,

    /** Create allocation only if additional memory required for it, if any, won't exceed
    memory budget. Otherwise return `E_OUTOFMEMORY`.
    */
    ALLOCATION_FLAG_WITHIN_BUDGET = 0x4,

    /** Allocation will be created from upper stack in a double stack pool.

    This flag is only allowed for custom pools created with #POOL_FLAG_ALGORITHM_LINEAR flag.
    */
    ALLOCATION_FLAG_UPPER_ADDRESS = 0x8,

    /** Set this flag if the allocated memory will have aliasing resources.
    
    Use this when calling D3D12MA::Allocator::CreateResource() and similar to
    guarantee creation of explicit heap for desired allocation and prevent it from using `CreateCommittedResource`,
    so that new allocation object will always have `allocation->GetHeap() != NULL`.
    */
    ALLOCATION_FLAG_CAN_ALIAS = 0x10,

    /** Allocation strategy that chooses smallest possible free range for the allocation
    to minimize memory usage and fragmentation, possibly at the expense of allocation time.
    */
    ALLOCATION_FLAG_STRATEGY_MIN_MEMORY = 0x00010000,

    /** Allocation strategy that chooses first suitable free range for the allocation -
    not necessarily in terms of the smallest offset but the one that is easiest and fastest to find
    to minimize allocation time, possibly at the expense of allocation quality.
    */
    ALLOCATION_FLAG_STRATEGY_MIN_TIME = 0x00020000,

    /** Allocation strategy that chooses always the lowest offset in available space.
    This is not the most efficient strategy but achieves highly packed data.
    Used internally by defragmentation, not recomended in typical usage.
    */
    ALLOCATION_FLAG_STRATEGY_MIN_OFFSET = 0x0004000,

    /// Alias to #ALLOCATION_FLAG_STRATEGY_MIN_MEMORY.
    ALLOCATION_FLAG_STRATEGY_BEST_FIT = ALLOCATION_FLAG_STRATEGY_MIN_MEMORY,
    /// Alias to #ALLOCATION_FLAG_STRATEGY_MIN_TIME.
    ALLOCATION_FLAG_STRATEGY_FIRST_FIT = ALLOCATION_FLAG_STRATEGY_MIN_TIME,

    /// A bit mask to extract only `STRATEGY` bits from entire set of flags.
    ALLOCATION_FLAG_STRATEGY_MASK =
        ALLOCATION_FLAG_STRATEGY_MIN_MEMORY |
        ALLOCATION_FLAG_STRATEGY_MIN_TIME |
        ALLOCATION_FLAG_STRATEGY_MIN_OFFSET,
};

struct ALLOCATION_DESC
{
    /// Flags.
    ALLOCATION_FLAGS Flags;
    /** \brief The type of memory heap where the new allocation should be placed.

    It must be one of: `D3D12_HEAP_TYPE_DEFAULT`, `D3D12_HEAP_TYPE_UPLOAD`, `D3D12_HEAP_TYPE_READBACK`.

    When D3D12MA::ALLOCATION_DESC::CustomPool != NULL this member is ignored.
    */
    D3D12_HEAP_TYPE HeapType;
    /** \brief Additional heap flags to be used when allocating memory.

    In most cases it can be 0.
    
    - If you use D3D12MA::Allocator::CreateResource(), you don't need to care.
      Necessary flag `D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS`, `D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES`,
      or `D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES` is added automatically.
    - If you use D3D12MA::Allocator::AllocateMemory(), you should specify one of those `ALLOW_ONLY` flags.
      Except when you validate that D3D12MA::Allocator::GetD3D12Options()`.ResourceHeapTier == D3D12_RESOURCE_HEAP_TIER_1` -
      then you can leave it 0.
    - You can specify additional flags if needed. Then the memory will always be allocated as
      separate block using `D3D12Device::CreateCommittedResource` or `CreateHeap`, not as part of an existing larget block.

    When D3D12MA::ALLOCATION_DESC::CustomPool != NULL this member is ignored.
    */
    D3D12_HEAP_FLAGS ExtraHeapFlags;
    /** \brief Custom pool to place the new resource in. Optional.

    When not NULL, the resource will be created inside specified custom pool.
    It will then never be created as committed.
    */
    Pool* CustomPool;
    /// Custom general-purpose pointer that will be stored in D3D12MA::Allocation.
    void* pPrivateData;
};

/** \brief Calculated statistics of memory usage e.g. in a specific memory heap type,
memory segment group, custom pool, or total.

These are fast to calculate.
See functions: D3D12MA::Allocator::GetBudget(), D3D12MA::Pool::GetStatistics().
*/
struct Statistics
{
    /** \brief Number of D3D12 memory blocks allocated - `ID3D12Heap` objects and committed resources.
    */
    UINT BlockCount;
    /** \brief Number of D3D12MA::Allocation objects allocated.

    Committed allocations have their own blocks, so each one adds 1 to `AllocationCount` as well as `BlockCount`.
    */
    UINT AllocationCount;
    /** \brief Number of bytes allocated in memory blocks.
    */
    UINT64 BlockBytes;
    /** \brief Total number of bytes occupied by all D3D12MA::Allocation objects.

    Always less or equal than `BlockBytes`.
    Difference `(BlockBytes - AllocationBytes)` is the amount of memory allocated from D3D12
    but unused by any D3D12MA::Allocation.
    */
    UINT64 AllocationBytes;
};

/** \brief More detailed statistics than D3D12MA::Statistics.

These are slower to calculate. Use for debugging purposes.
See functions: D3D12MA::Allocator::CalculateStatistics(), D3D12MA::Pool::CalculateStatistics().

Averages are not provided because they can be easily calculated as:

\code
UINT64 AllocationSizeAvg = DetailedStats.Statistics.AllocationBytes / detailedStats.Statistics.AllocationCount;
UINT64 UnusedBytes = DetailedStats.Statistics.BlockBytes - DetailedStats.Statistics.AllocationBytes;
UINT64 UnusedRangeSizeAvg = UnusedBytes / DetailedStats.UnusedRangeCount;
\endcode
*/
struct DetailedStatistics
{
    /// Basic statistics.
    Statistics Stats;
    /// Number of free ranges of memory between allocations.
    UINT UnusedRangeCount;
    /// Smallest allocation size. `UINT64_MAX` if there are 0 allocations.
    UINT64 AllocationSizeMin;
    /// Largest allocation size. 0 if there are 0 allocations.
    UINT64 AllocationSizeMax;
    /// Smallest empty range size. `UINT64_MAX` if there are 0 empty ranges.
    UINT64 UnusedRangeSizeMin;
    /// Largest empty range size. 0 if there are 0 empty ranges.
    UINT64 UnusedRangeSizeMax;
};

/** \brief  General statistics from current state of the allocator -
total memory usage across all memory heaps and segments.

These are slower to calculate. Use for debugging purposes.
See function D3D12MA::Allocator::CalculateStatistics().
*/
struct TotalStatistics
{
    /** \brief One element for each type of heap located at the following indices:

    - 0 = `D3D12_HEAP_TYPE_DEFAULT`
    - 1 = `D3D12_HEAP_TYPE_UPLOAD`
    - 2 = `D3D12_HEAP_TYPE_READBACK`
    - 3 = `D3D12_HEAP_TYPE_CUSTOM`
    */
    DetailedStatistics HeapType[4];
    /** \brief One element for each memory segment group located at the following indices:

    - 0 = `DXGI_MEMORY_SEGMENT_GROUP_LOCAL`
    - 1 = `DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL`

    Meaning of these segment groups is:

    - When `IsUMA() == FALSE` (discrete graphics card):
      - `DXGI_MEMORY_SEGMENT_GROUP_LOCAL` (index 0) represents GPU memory
      (resources allocated in `D3D12_HEAP_TYPE_DEFAULT` or `D3D12_MEMORY_POOL_L1`).
      - `DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL` (index 1) represents system memory
      (resources allocated in `D3D12_HEAP_TYPE_UPLOAD`, `D3D12_HEAP_TYPE_READBACK`, or `D3D12_MEMORY_POOL_L0`).
    - When `IsUMA() == TRUE` (integrated graphics chip):
      - `DXGI_MEMORY_SEGMENT_GROUP_LOCAL` = (index 0) represents memory shared for all the resources.
      - `DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL` = (index 1) is unused and always 0.
    */
    DetailedStatistics MemorySegmentGroup[2];
    /// Total statistics from all memory allocated from D3D12.
    DetailedStatistics Total;
};

/** \brief %Statistics of current memory usage and available budget for a specific memory segment group.

These are fast to calculate. See function D3D12MA::Allocator::GetBudget().
*/
struct Budget
{
    /** \brief %Statistics fetched from the library.
    */
    Statistics Stats;
    /** \brief Estimated current memory usage of the program.

    Fetched from system using `IDXGIAdapter3::QueryVideoMemoryInfo` if possible.

    It might be different than `BlockBytes` (usually higher) due to additional implicit objects
    also occupying the memory, like swapchain, pipeline state objects, descriptor heaps, command lists, or
    heaps and resources allocated outside of this library, if any.
    */
    UINT64 UsageBytes;
    /** \brief Estimated amount of memory available to the program.

    Fetched from system using `IDXGIAdapter3::QueryVideoMemoryInfo` if possible.

    It might be different (most probably smaller) than memory capacity returned
    by D3D12MA::Allocator::GetMemoryCapacity() due to factors
    external to the program, decided by the operating system.
    Difference `BudgetBytes - UsageBytes` is the amount of additional memory that can probably
    be allocated without problems. Exceeding the budget may result in various problems.
    */
    UINT64 BudgetBytes;
};


struct D3D12MA_API VirtualAllocation
{
    /// \brief Unique idenitfier of current allocation. 0 means null/invalid.
    AllocHandle AllocHandle;
};

/** \brief Represents single memory allocation.

It may be either implicit memory heap dedicated to a single resource or a
specific region of a bigger heap plus unique offset.

To create such object, fill structure D3D12MA::ALLOCATION_DESC and call function
Allocator::CreateResource.

The object remembers size and some other information.
To retrieve this information, use methods of this class.

The object also remembers `ID3D12Resource` and "owns" a reference to it,
so it calls `%Release()` on the resource when destroyed.
*/
class D3D12MA_API Allocation : public IUnknownImpl
{
public:
    /** \brief Returns offset in bytes from the start of memory heap.

    You usually don't need to use this offset. If you create a buffer or a texture together with the allocation using function
    D3D12MA::Allocator::CreateResource, functions that operate on that resource refer to the beginning of the resource,
    not entire memory heap.

    If the Allocation represents committed resource with implicit heap, returns 0.
    */
    UINT64 GetOffset() const;

    /// Returns alignment that resource was created with.
    UINT64 GetAlignment() const { return m_Alignment; }

    /** \brief Returns size in bytes of the allocation.

    - If you created a buffer or a texture together with the allocation using function D3D12MA::Allocator::CreateResource,
      this is the size of the resource returned by `ID3D12Device::GetResourceAllocationInfo`.
    - For allocations made out of bigger memory blocks, this also is the size of the memory region assigned exclusively to this allocation.
    - For resources created as committed, this value may not be accurate. DirectX implementation may optimize memory usage internally
      so that you may even observe regions of `ID3D12Resource::GetGPUVirtualAddress()` + Allocation::GetSize() to overlap in memory and still work correctly.
    */
    UINT64 GetSize() const { return m_Size; }

    /** \brief Returns D3D12 resource associated with this object.

    Calling this method doesn't increment resource's reference counter.
    */
    ID3D12Resource* GetResource() const { return m_Resource; }

    /// Releases the resource currently pointed by the allocation (if any), sets it to new one, incrementing its reference counter (if not null).
    void SetResource(ID3D12Resource* pResource);

    /** \brief Returns memory heap that the resource is created in.

    If the Allocation represents committed resource with implicit heap, returns NULL.
    */
    ID3D12Heap* GetHeap() const;

    /// Changes custom pointer for an allocation to a new value.
    void SetPrivateData(void* pPrivateData) { m_pPrivateData = pPrivateData; }

    /// Get custom pointer associated with the allocation.
    void* GetPrivateData() const { return m_pPrivateData; }

    /** \brief Associates a name with the allocation object. This name is for use in debug diagnostics and tools.

    Internal copy of the string is made, so the memory pointed by the argument can be
    changed of freed immediately after this call.

    `Name` can be null.
    */
    void SetName(LPCWSTR Name);

    /** \brief Returns the name associated with the allocation object.

    Returned string points to an internal copy.

    If no name was associated with the allocation, returns null.
    */
    LPCWSTR GetName() const { return m_Name; }

    /** \brief Returns `TRUE` if the memory of the allocation was filled with zeros when the allocation was created.

    Returns `TRUE` only if the allocator is sure that the entire memory where the
    allocation was created was filled with zeros at the moment the allocation was made.
    
    Returns `FALSE` if the memory could potentially contain garbage data.
    If it's a render-target or depth-stencil texture, it then needs proper
    initialization with `ClearRenderTargetView`, `ClearDepthStencilView`, `DiscardResource`,
    or a copy operation, as described on page
    "ID3D12Device::CreatePlacedResource method - Notes on the required resource initialization" in Microsoft documentation.
    Please note that rendering a fullscreen triangle or quad to the texture as
    a render target is not a proper way of initialization!

    See also articles:

    - "Coming to DirectX 12: More control over memory allocation" on DirectX Developer Blog
    - ["Initializing DX12 Textures After Allocation and Aliasing"](https://asawicki.info/news_1724_initializing_dx12_textures_after_allocation_and_aliasing).
    */
    BOOL WasZeroInitialized() const { return m_PackedData.WasZeroInitialized(); }

protected:
    void ReleaseThis() override;

private:
    friend class AllocatorPimpl;
    friend class BlockVector;
    friend class CommittedAllocationList;
    friend class JsonWriter;
    friend class BlockMetadata_Linear;
    friend class DefragmentationContextPimpl;
    friend struct CommittedAllocationListItemTraits;
    template<typename T> friend void D3D12MA_DELETE(const ALLOCATION_CALLBACKS&, T*);
    template<typename T> friend class PoolAllocator;

    enum Type
    {
        TYPE_COMMITTED,
        TYPE_PLACED,
        TYPE_HEAP,
        TYPE_COUNT
    };

    AllocatorPimpl* m_Allocator;
    UINT64 m_Size;
    UINT64 m_Alignment;
    ID3D12Resource* m_Resource;
    void* m_pPrivateData;
    wchar_t* m_Name;

    union
    {
        struct
        {
            CommittedAllocationList* list;
            Allocation* prev;
            Allocation* next;
        } m_Committed;

        struct
        {
            AllocHandle allocHandle;
            NormalBlock* block;
        } m_Placed;

        struct
        {
            // Beginning must be compatible with m_Committed.
            CommittedAllocationList* list;
            Allocation* prev;
            Allocation* next;
            ID3D12Heap* heap;
        } m_Heap;
    };

    struct PackedData
    {
    public:
        PackedData() :
            m_Type(0), m_ResourceDimension(0), m_ResourceFlags(0), m_TextureLayout(0), m_WasZeroInitialized(0) { }

        Type GetType() const { return (Type)m_Type; }
        D3D12_RESOURCE_DIMENSION GetResourceDimension() const { return (D3D12_RESOURCE_DIMENSION)m_ResourceDimension; }
        D3D12_RESOURCE_FLAGS GetResourceFlags() const { return (D3D12_RESOURCE_FLAGS)m_ResourceFlags; }
        D3D12_TEXTURE_LAYOUT GetTextureLayout() const { return (D3D12_TEXTURE_LAYOUT)m_TextureLayout; }
        BOOL WasZeroInitialized() const { return (BOOL)m_WasZeroInitialized; }

        void SetType(Type type);
        void SetResourceDimension(D3D12_RESOURCE_DIMENSION resourceDimension);
        void SetResourceFlags(D3D12_RESOURCE_FLAGS resourceFlags);
        void SetTextureLayout(D3D12_TEXTURE_LAYOUT textureLayout);
        void SetWasZeroInitialized(BOOL wasZeroInitialized) { m_WasZeroInitialized = wasZeroInitialized ? 1 : 0; }

    private:
        UINT m_Type : 2;               // enum Type
        UINT m_ResourceDimension : 3;  // enum D3D12_RESOURCE_DIMENSION
        UINT m_ResourceFlags : 24;     // flags D3D12_RESOURCE_FLAGS
        UINT m_TextureLayout : 9;      // enum D3D12_TEXTURE_LAYOUT
        UINT m_WasZeroInitialized : 1; // BOOL
    } m_PackedData;

    Allocation(AllocatorPimpl* allocator, UINT64 size, UINT64 alignment, BOOL wasZeroInitialized);
    //  Nothing here, everything already done in Release.
    virtual ~Allocation() = default;

    void InitCommitted(CommittedAllocationList* list);
    void InitPlaced(AllocHandle allocHandle, NormalBlock* block);
    void InitHeap(CommittedAllocationList* list, ID3D12Heap* heap);
    void SwapBlockAllocation(Allocation* allocation);
    // If the Allocation represents committed resource with implicit heap, returns UINT64_MAX.
    AllocHandle GetAllocHandle() const;
    NormalBlock* GetBlock();
    template<typename D3D12_RESOURCE_DESC_T>
    void SetResourcePointer(ID3D12Resource* resource, const D3D12_RESOURCE_DESC_T* pResourceDesc);
    void FreeName();

    D3D12MA_CLASS_NO_COPY(Allocation)
};


enum DEFRAGMENTATION_FLAGS
{
    /** Use simple but fast algorithm for defragmentation.
    May not achieve best results but will require least time to compute and least allocations to copy.
    */
    DEFRAGMENTATION_FLAG_ALGORITHM_FAST = 0x1,
    /** Default defragmentation algorithm, applied also when no `ALGORITHM` flag is specified.
    Offers a balance between defragmentation quality and the amount of allocations and bytes that need to be moved.
    */
    DEFRAGMENTATION_FLAG_ALGORITHM_BALANCED = 0x2,
    /** Perform full defragmentation of memory.
    Can result in notably more time to compute and allocations to copy, but will achieve best memory packing.
    */
    DEFRAGMENTATION_FLAG_ALGORITHM_FULL = 0x4,

    /// A bit mask to extract only `ALGORITHM` bits from entire set of flags.
    DEFRAGMENTATION_FLAG_ALGORITHM_MASK =
        DEFRAGMENTATION_FLAG_ALGORITHM_FAST |
        DEFRAGMENTATION_FLAG_ALGORITHM_BALANCED |
        DEFRAGMENTATION_FLAG_ALGORITHM_FULL
};

/** \brief Parameters for defragmentation.

To be used with functions Allocator::BeginDefragmentation() and Pool::BeginDefragmentation().
*/
struct DEFRAGMENTATION_DESC
{
    /// Flags.
    DEFRAGMENTATION_FLAGS Flags;
    /** \brief Maximum numbers of bytes that can be copied during single pass, while moving allocations to different places.

    0 means no limit.
    */
    UINT64 MaxBytesPerPass;
    /** \brief Maximum number of allocations that can be moved during single pass to a different place.

    0 means no limit.
    */
    UINT32 MaxAllocationsPerPass;
};

enum DEFRAGMENTATION_MOVE_OPERATION
{
    /** Resource has been recreated at `pDstTmpAllocation`, data has been copied, old resource has been destroyed.
    `pSrcAllocation` will be changed to point to the new place. This is the default value set by DefragmentationContext::BeginPass().
    */
    DEFRAGMENTATION_MOVE_OPERATION_COPY = 0,
    /// Set this value if you cannot move the allocation. New place reserved at `pDstTmpAllocation` will be freed. `pSrcAllocation` will remain unchanged.
    DEFRAGMENTATION_MOVE_OPERATION_IGNORE = 1,
    /// Set this value if you decide to abandon the allocation and you destroyed the resource. New place reserved `pDstTmpAllocation` will be freed, along with `pSrcAllocation`.
    DEFRAGMENTATION_MOVE_OPERATION_DESTROY = 2,
};

struct DEFRAGMENTATION_MOVE
{
    /** \brief Operation to be performed on the allocation by DefragmentationContext::EndPass().
    Default value is #DEFRAGMENTATION_MOVE_OPERATION_COPY. You can modify it.
    */
    DEFRAGMENTATION_MOVE_OPERATION Operation;
    /// %Allocation that should be moved.
    Allocation* pSrcAllocation;
    /** \brief Temporary allocation pointing to destination memory that will replace `pSrcAllocation`.

    Use it to retrieve new `ID3D12Heap` and offset to create new `ID3D12Resource` and then store it here via Allocation::SetResource().

    \warning Do not store this allocation in your data structures! It exists only temporarily, for the duration of the defragmentation pass,
    to be used for storing newly created resource. DefragmentationContext::EndPass() will destroy it and make `pSrcAllocation` point to this memory.
    */
    Allocation* pDstTmpAllocation;
};

/** \brief Parameters for incremental defragmentation steps.

To be used with function DefragmentationContext::BeginPass().
*/
struct DEFRAGMENTATION_PASS_MOVE_INFO
{
    /// Number of elements in the `pMoves` array.
    UINT32 MoveCount;
    /** \brief Array of moves to be performed by the user in the current defragmentation pass.

    Pointer to an array of `MoveCount` elements, owned by %D3D12MA, created in DefragmentationContext::BeginPass(), destroyed in DefragmentationContext::EndPass().

    For each element, you should:

    1. Create a new resource in the place pointed by `pMoves[i].pDstTmpAllocation->GetHeap()` + `pMoves[i].pDstTmpAllocation->GetOffset()`.
    2. Store new resource in `pMoves[i].pDstTmpAllocation` by using Allocation::SetResource(). It will later replace old resource from `pMoves[i].pSrcAllocation`.
    3. Copy data from the `pMoves[i].pSrcAllocation` e.g. using `D3D12GraphicsCommandList::CopyResource`.
    4. Make sure these commands finished executing on the GPU.

    Only then you can finish defragmentation pass by calling DefragmentationContext::EndPass().
    After this call, the allocation will point to the new place in memory.

    Alternatively, if you cannot move specific allocation,
    you can set DEFRAGMENTATION_MOVE::Operation to D3D12MA::DEFRAGMENTATION_MOVE_OPERATION_IGNORE.

    Alternatively, if you decide you want to completely remove the allocation,
    set DEFRAGMENTATION_MOVE::Operation to D3D12MA::DEFRAGMENTATION_MOVE_OPERATION_DESTROY.
    Then, after DefragmentationContext::EndPass() the allocation will be released.
    */
    DEFRAGMENTATION_MOVE* pMoves;
};

struct DEFRAGMENTATION_STATS
{
    /// Total number of bytes that have been copied while moving allocations to different places.
    UINT64 BytesMoved;
    /// Total number of bytes that have been released to the system by freeing empty heaps.
    UINT64 BytesFreed;
    /// Number of allocations that have been moved to different places.
    UINT32 AllocationsMoved;
    /// Number of empty `ID3D12Heap` objects that have been released to the system.
    UINT32 HeapsFreed;
};

/** \brief Represents defragmentation process in progress.

You can create this object using Allocator::BeginDefragmentation (for default pools) or
Pool::BeginDefragmentation (for a custom pool).
*/
class D3D12MA_API DefragmentationContext : public IUnknownImpl
{
public:
    /** \brief Starts single defragmentation pass.

    \param[out] pPassInfo Computed informations for current pass.
    \returns
    - `S_OK` if no more moves are possible. Then you can omit call to DefragmentationContext::EndPass() and simply end whole defragmentation.
    - `S_FALSE` if there are pending moves returned in `pPassInfo`. You need to perform them, call DefragmentationContext::EndPass(),
      and then preferably try another pass with DefragmentationContext::BeginPass().
    */
    HRESULT BeginPass(DEFRAGMENTATION_PASS_MOVE_INFO* pPassInfo);
    /** \brief Ends single defragmentation pass.

    \param pPassInfo Computed informations for current pass filled by DefragmentationContext::BeginPass() and possibly modified by you.
    \return Returns `S_OK` if no more moves are possible or `S_FALSE` if more defragmentations are possible.

    Ends incremental defragmentation pass and commits all defragmentation moves from `pPassInfo`.
    After this call:

    - %Allocation at `pPassInfo[i].pSrcAllocation` that had `pPassInfo[i].Operation ==` #DEFRAGMENTATION_MOVE_OPERATION_COPY
      (which is the default) will be pointing to the new destination place.
    - %Allocation at `pPassInfo[i].pSrcAllocation` that had `pPassInfo[i].operation ==` #DEFRAGMENTATION_MOVE_OPERATION_DESTROY
      will be released.

    If no more moves are possible you can end whole defragmentation.
    */
    HRESULT EndPass(DEFRAGMENTATION_PASS_MOVE_INFO* pPassInfo);
    /** \brief Returns statistics of the defragmentation performed so far.
    */
    void GetStats(DEFRAGMENTATION_STATS* pStats);

protected:
    void ReleaseThis() override;

private:
    friend class Pool;
    friend class Allocator;
    template<typename T> friend void D3D12MA_DELETE(const ALLOCATION_CALLBACKS&, T*);

    DefragmentationContextPimpl* m_Pimpl;

    DefragmentationContext(AllocatorPimpl* allocator,
        const DEFRAGMENTATION_DESC& desc,
        BlockVector* poolVector);
    ~DefragmentationContext();

    D3D12MA_CLASS_NO_COPY(DefragmentationContext)
};

enum POOL_FLAGS
{
    /// Zero
    POOL_FLAG_NONE = 0,

    /** \brief Enables alternative, linear allocation algorithm in this pool.

    Specify this flag to enable linear allocation algorithm, which always creates
    new allocations after last one and doesn't reuse space from allocations freed in
    between. It trades memory consumption for simplified algorithm and data
    structure, which has better performance and uses less memory for metadata.

    By using this flag, you can achieve behavior of free-at-once, stack,
    ring buffer, and double stack.
    For details, see documentation chapter \ref linear_algorithm.
    */
    POOL_FLAG_ALGORITHM_LINEAR = 0x1,

    /** \brief Optimization, allocate MSAA textures as committed resources always.
    
    Specify this flag to create MSAA textures with implicit heaps, as if they were created
    with flag ALLOCATION_FLAG_COMMITTED. Usage of this flags enables pool to create its heaps
    on smaller alignment not suitable for MSAA textures.
    */
    POOL_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED = 0x2,

    // Bit mask to extract only `ALGORITHM` bits from entire set of flags.
    POOL_FLAG_ALGORITHM_MASK = POOL_FLAG_ALGORITHM_LINEAR
};

struct POOL_DESC
{
    /// Flags.
    POOL_FLAGS Flags;
    /** \brief The parameters of memory heap where allocations of this pool should be placed.

    In the simplest case, just fill it with zeros and set `Type` to one of: `D3D12_HEAP_TYPE_DEFAULT`,
    `D3D12_HEAP_TYPE_UPLOAD`, `D3D12_HEAP_TYPE_READBACK`. Additional parameters can be used e.g. to utilize UMA.
    */
    D3D12_HEAP_PROPERTIES HeapProperties;
    /** \brief Heap flags to be used when allocating heaps of this pool.

    It should contain one of these values, depending on type of resources you are going to create in this heap:
    `D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS`,
    `D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES`,
    `D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES`.
    Except if ResourceHeapTier = 2, then it may be `D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES` = 0.
    
    You can specify additional flags if needed.
    */
    D3D12_HEAP_FLAGS HeapFlags;
    /** \brief Size of a single heap (memory block) to be allocated as part of this pool, in bytes. Optional.

    Specify nonzero to set explicit, constant size of memory blocks used by this pool.
    Leave 0 to use default and let the library manage block sizes automatically.
    Then sizes of particular blocks may vary.
    */
    UINT64 BlockSize;
    /** \brief Minimum number of heaps (memory blocks) to be always allocated in this pool, even if they stay empty. Optional.

    Set to 0 to have no preallocated blocks and allow the pool be completely empty.
    */
    UINT MinBlockCount;
    /** \brief Maximum number of heaps (memory blocks) that can be allocated in this pool. Optional.

    Set to 0 to use default, which is `UINT64_MAX`, which means no limit.

    Set to same value as D3D12MA::POOL_DESC::MinBlockCount to have fixed amount of memory allocated
    throughout whole lifetime of this pool.
    */
    UINT MaxBlockCount;
    /** \brief Additional minimum alignment to be used for all allocations created from this pool. Can be 0.
    
    Leave 0 (default) not to impose any additional alignment. If not 0, it must be a power of two.
    */
    UINT64 MinAllocationAlignment;
    /** \brief Additional parameter allowing pool to create resources with passed protected session.
    
    If not null then all the heaps and committed resources will be created with this parameter.
    Valid only if ID3D12Device4 interface is present in current Windows SDK!
    */
    ID3D12ProtectedResourceSession* pProtectedSession;
};

/** \brief Custom memory pool

Represents a separate set of heaps (memory blocks) that can be used to create
D3D12MA::Allocation-s and resources in it. Usually there is no need to create custom
pools - creating resources in default pool is sufficient.

To create custom pool, fill D3D12MA::POOL_DESC and call D3D12MA::Allocator::CreatePool.
*/
class D3D12MA_API Pool : public IUnknownImpl
{
public:
    /** \brief Returns copy of parameters of the pool.

    These are the same parameters as passed to D3D12MA::Allocator::CreatePool.
    */
    POOL_DESC GetDesc() const;

    /** \brief Retrieves basic statistics of the custom pool that are fast to calculate.

    \param[out] pStats %Statistics of the current pool.
    */
    void GetStatistics(Statistics* pStats);

    /** \brief Retrieves detailed statistics of the custom pool that are slower to calculate.

    \param[out] pStats %Statistics of the current pool.
    */
    void CalculateStatistics(DetailedStatistics* pStats);

    /** \brief Associates a name with the pool. This name is for use in debug diagnostics and tools.

    Internal copy of the string is made, so the memory pointed by the argument can be
    changed of freed immediately after this call.

    `Name` can be NULL.
    */
    void SetName(LPCWSTR Name);

    /** \brief Returns the name associated with the pool object.

    Returned string points to an internal copy.

    If no name was associated with the allocation, returns NULL.
    */
    LPCWSTR GetName() const;

    /** \brief Begins defragmentation process of the current pool.

    \param pDesc Structure filled with parameters of defragmentation.
    \param[out] ppContext Context object that will manage defragmentation.
    \returns
    - `S_OK` if defragmentation can begin.
    - `E_NOINTERFACE` if defragmentation is not supported.

    For more information about defragmentation, see documentation chapter:
    [Defragmentation](@ref defragmentation).
    */
    HRESULT BeginDefragmentation(const DEFRAGMENTATION_DESC* pDesc, DefragmentationContext** ppContext);

protected:
    void ReleaseThis() override;

private:
    friend class Allocator;
    friend class AllocatorPimpl;
    template<typename T> friend void D3D12MA_DELETE(const ALLOCATION_CALLBACKS&, T*);

    PoolPimpl* m_Pimpl;

    Pool(Allocator* allocator, const POOL_DESC &desc);
    ~Pool();

    D3D12MA_CLASS_NO_COPY(Pool)
};


enum ALLOCATOR_FLAGS
{
    /// Zero
    ALLOCATOR_FLAG_NONE = 0,

    /**
    Allocator and all objects created from it will not be synchronized internally,
    so you must guarantee they are used from only one thread at a time or
    synchronized by you.

    Using this flag may increase performance because internal mutexes are not used.
    */
    ALLOCATOR_FLAG_SINGLETHREADED = 0x1,

    /**
    Every allocation will have its own memory block.
    To be used for debugging purposes.
    */
    ALLOCATOR_FLAG_ALWAYS_COMMITTED = 0x2,

    /**
    Heaps created for the default pools will be created with flag `D3D12_HEAP_FLAG_CREATE_NOT_ZEROED`,
    allowing for their memory to be not zeroed by the system if possible,
    which can speed up allocation.

    Only affects default pools.
    To use the flag with @ref custom_pools, you need to add it manually:

    \code
    poolDesc.heapFlags |= D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
    \endcode

    Only avaiable if `ID3D12Device8` is present. Otherwise, the flag is ignored.
    */
    ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED = 0x4,

    /** \brief Optimization, allocate MSAA textures as committed resources always.

    Specify this flag to create MSAA textures with implicit heaps, as if they were created
    with flag ALLOCATION_FLAG_COMMITTED. Usage of this flags enables all default pools
    to create its heaps on smaller alignment not suitable for MSAA textures.
    */
    ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED = 0x8,
};

struct ALLOCATOR_DESC
{
    /// Flags.
    ALLOCATOR_FLAGS Flags;
    
    /** Direct3D device object that the allocator should be attached to.

    Allocator is doing `AddRef`/`Release` on this object.
    */
    ID3D12Device* pDevice;
    
    /** \brief Preferred size of a single `ID3D12Heap` block to be allocated.
    
    Set to 0 to use default, which is currently 64 MiB.
    */
    UINT64 PreferredBlockSize;
    
    /** \brief Custom CPU memory allocation callbacks. Optional.

    Optional, can be null. When specified, will be used for all CPU-side memory allocations.
    */
    const ALLOCATION_CALLBACKS* pAllocationCallbacks;

    /** DXGI Adapter object that you use for D3D12 and this allocator.

    Allocator is doing `AddRef`/`Release` on this object.
    */
    IDXGIAdapter* pAdapter;
};

/**
\brief Represents main object of this library initialized for particular `ID3D12Device`.

Fill structure D3D12MA::ALLOCATOR_DESC and call function CreateAllocator() to create it.
Call method `Release()` to destroy it.

It is recommended to create just one object of this type per `ID3D12Device` object,
right after Direct3D 12 is initialized and keep it alive until before Direct3D device is destroyed.
*/
class D3D12MA_API Allocator : public IUnknownImpl
{
public:
    /// Returns cached options retrieved from D3D12 device.
    const D3D12_FEATURE_DATA_D3D12_OPTIONS& GetD3D12Options() const;
    /** \brief Returns true if `D3D12_FEATURE_DATA_ARCHITECTURE1::UMA` was found to be true.
    
    For more information about how to use it, see articles in Microsoft Docs articles:

    - "UMA Optimizations: CPU Accessible Textures and Standard Swizzle"
    - "D3D12_FEATURE_DATA_ARCHITECTURE structure (d3d12.h)"
    - "ID3D12Device::GetCustomHeapProperties method (d3d12.h)"
    */
    BOOL IsUMA() const;
    /** \brief Returns true if `D3D12_FEATURE_DATA_ARCHITECTURE1::CacheCoherentUMA` was found to be true.

    For more information about how to use it, see articles in Microsoft Docs articles:

    - "UMA Optimizations: CPU Accessible Textures and Standard Swizzle"
    - "D3D12_FEATURE_DATA_ARCHITECTURE structure (d3d12.h)"
    - "ID3D12Device::GetCustomHeapProperties method (d3d12.h)"
    */
    BOOL IsCacheCoherentUMA() const;
    /** \brief Returns total amount of memory of specific segment group, in bytes.
    
    \param memorySegmentGroup use `DXGI_MEMORY_SEGMENT_GROUP_LOCAL` or DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL`.

    This information is taken from `DXGI_ADAPTER_DESC`.
    It is not recommended to use this number.
    You should preferably call GetBudget() and limit memory usage to D3D12MA::Budget::BudgetBytes instead.

    - When IsUMA() `== FALSE` (discrete graphics card):
      - `GetMemoryCapacity(DXGI_MEMORY_SEGMENT_GROUP_LOCAL)` returns the size of the video memory.
      - `GetMemoryCapacity(DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL)` returns the size of the system memory available for D3D12 resources.
    - When IsUMA() `== TRUE` (integrated graphics chip):
      - `GetMemoryCapacity(DXGI_MEMORY_SEGMENT_GROUP_LOCAL)` returns the size of the shared memory available for all D3D12 resources.
        All memory is considered "local".
      - `GetMemoryCapacity(DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL)` is not applicable and returns 0.
    */
    UINT64 GetMemoryCapacity(UINT memorySegmentGroup) const;

    /** \brief Allocates memory and creates a D3D12 resource (buffer or texture). This is the main allocation function.

    The function is similar to `ID3D12Device::CreateCommittedResource`, but it may
    really call `ID3D12Device::CreatePlacedResource` to assign part of a larger,
    existing memory heap to the new resource, which is the main purpose of this
    whole library.

    If `ppvResource` is null, you receive only `ppAllocation` object from this function.
    It holds pointer to `ID3D12Resource` that can be queried using function D3D12MA::Allocation::GetResource().
    Reference count of the resource object is 1.
    It is automatically destroyed when you destroy the allocation object.

    If `ppvResource` is not null, you receive pointer to the resource next to allocation object.
    Reference count of the resource object is then increased by calling `QueryInterface`, so you need to manually `Release` it
    along with the allocation.

    \param pAllocDesc   Parameters of the allocation.
    \param pResourceDesc   Description of created resource.
    \param InitialResourceState   Initial resource state.
    \param pOptimizedClearValue   Optional. Either null or optimized clear value.
    \param[out] ppAllocation   Filled with pointer to new allocation object created.
    \param riidResource   IID of a resource to be returned via `ppvResource`.
    \param[out] ppvResource   Optional. If not null, filled with pointer to new resouce created.

    \note This function creates a new resource. Sub-allocation of parts of one large buffer,
    although recommended as a good practice, is out of scope of this library and could be implemented
    by the user as a higher-level logic on top of it, e.g. using the \ref virtual_allocator feature.
    */
    HRESULT CreateResource(
        const ALLOCATION_DESC* pAllocDesc,
        const D3D12_RESOURCE_DESC* pResourceDesc,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
        Allocation** ppAllocation,
        REFIID riidResource,
        void** ppvResource);

#ifdef __ID3D12Device8_INTERFACE_DEFINED__
    /** \brief Similar to Allocator::CreateResource, but supports new structure `D3D12_RESOURCE_DESC1`.
    
    It internally uses `ID3D12Device8::CreateCommittedResource2` or `ID3D12Device8::CreatePlacedResource1`.

    To work correctly, `ID3D12Device8` interface must be available in the current system. Otherwise, `E_NOINTERFACE` is returned.
    */
    HRESULT CreateResource2(
        const ALLOCATION_DESC* pAllocDesc,
        const D3D12_RESOURCE_DESC1* pResourceDesc,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
        Allocation** ppAllocation,
        REFIID riidResource,
        void** ppvResource);
#endif // #ifdef __ID3D12Device4_INTERFACE_DEFINED__

    /** \brief Allocates memory without creating any resource placed in it.

    This function is similar to `ID3D12Device::CreateHeap`, but it may really assign
    part of a larger, existing heap to the allocation.

    `pAllocDesc->heapFlags` should contain one of these values, depending on type of resources you are going to create in this memory:
    `D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS`,
    `D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES`,
    `D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES`.
    Except if you validate that ResourceHeapTier = 2 - then `heapFlags`
    may be `D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES` = 0.
    Additional flags in `heapFlags` are allowed as well.

    `pAllocInfo->SizeInBytes` must be multiply of 64KB.
    `pAllocInfo->Alignment` must be one of the legal values as described in documentation of `D3D12_HEAP_DESC`.

    If you use D3D12MA::ALLOCATION_FLAG_COMMITTED you will get a separate memory block -
    a heap that always has offset 0.
    */
    HRESULT AllocateMemory(
        const ALLOCATION_DESC* pAllocDesc,
        const D3D12_RESOURCE_ALLOCATION_INFO* pAllocInfo,
        Allocation** ppAllocation);

    /** \brief Creates a new resource in place of an existing allocation. This is useful for memory aliasing.

    \param pAllocation Existing allocation indicating the memory where the new resource should be created.
        It can be created using D3D12MA::Allocator::CreateResource and already have a resource bound to it,
        or can be a raw memory allocated with D3D12MA::Allocator::AllocateMemory.
        It must not be created as committed so that `ID3D12Heap` is available and not implicit.
    \param AllocationLocalOffset Additional offset in bytes to be applied when allocating the resource.
        Local from the start of `pAllocation`, not the beginning of the whole `ID3D12Heap`!
        If the new resource should start from the beginning of the `pAllocation` it should be 0.
    \param pResourceDesc Description of the new resource to be created.
    \param InitialResourceState
    \param pOptimizedClearValue
    \param riidResource
    \param[out] ppvResource Returns pointer to the new resource.
        The resource is not bound with `pAllocation`.
        This pointer must not be null - you must get the resource pointer and `Release` it when no longer needed.

    Memory requirements of the new resource are checked for validation.
    If its size exceeds the end of `pAllocation` or required alignment is not fulfilled
    considering `pAllocation->GetOffset() + AllocationLocalOffset`, the function
    returns `E_INVALIDARG`.
    */
    HRESULT CreateAliasingResource(
        Allocation* pAllocation,
        UINT64 AllocationLocalOffset,
        const D3D12_RESOURCE_DESC* pResourceDesc,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
        REFIID riidResource,
        void** ppvResource);

    /** \brief Creates custom pool.
    */
    HRESULT CreatePool(
        const POOL_DESC* pPoolDesc,
        Pool** ppPool);

    /** \brief Sets the index of the current frame.

    This function is used to set the frame index in the allocator when a new game frame begins.
    */
    void SetCurrentFrameIndex(UINT frameIndex);

    /** \brief Retrieves information about current memory usage and budget.

    \param[out] pLocalBudget Optional, can be null.
    \param[out] pNonLocalBudget Optional, can be null.

    - When IsUMA() `== FALSE` (discrete graphics card):
      - `pLocalBudget` returns the budget of the video memory.
      - `pNonLocalBudget` returns the budget of the system memory available for D3D12 resources.
    - When IsUMA() `== TRUE` (integrated graphics chip):
      - `pLocalBudget` returns the budget of the shared memory available for all D3D12 resources.
        All memory is considered "local".
      - `pNonLocalBudget` is not applicable and returns zeros.

    This function is called "get" not "calculate" because it is very fast, suitable to be called
    every frame or every allocation. For more detailed statistics use CalculateStatistics().

    Note that when using allocator from multiple threads, returned information may immediately
    become outdated.
    */
    void GetBudget(Budget* pLocalBudget, Budget* pNonLocalBudget);

    /** \brief Retrieves statistics from current state of the allocator.

    This function is called "calculate" not "get" because it has to traverse all
    internal data structures, so it may be quite slow. Use it for debugging purposes.
    For faster but more brief statistics suitable to be called every frame or every allocation,
    use GetBudget().

    Note that when using allocator from multiple threads, returned information may immediately
    become outdated.
    */
    void CalculateStatistics(TotalStatistics* pStats);

    /** \brief Builds and returns statistics as a string in JSON format.
    * 
    @param[out] ppStatsString Must be freed using Allocator::FreeStatsString.
    @param DetailedMap `TRUE` to include full list of allocations (can make the string quite long), `FALSE` to only return statistics.
    */
    void BuildStatsString(WCHAR** ppStatsString, BOOL DetailedMap) const;

    /// Frees memory of a string returned from Allocator::BuildStatsString.
    void FreeStatsString(WCHAR* pStatsString) const;

    /** \brief Begins defragmentation process of the default pools.

    \param pDesc Structure filled with parameters of defragmentation.
    \param[out] ppContext Context object that will manage defragmentation.

    For more information about defragmentation, see documentation chapter:
    [Defragmentation](@ref defragmentation).
    */
    void BeginDefragmentation(const DEFRAGMENTATION_DESC* pDesc, DefragmentationContext** ppContext);

protected:
    void ReleaseThis() override;

private:
    friend D3D12MA_API HRESULT CreateAllocator(const ALLOCATOR_DESC*, Allocator**);
    template<typename T> friend void D3D12MA_DELETE(const ALLOCATION_CALLBACKS&, T*);
    friend class DefragmentationContext;
    friend class Pool;

    Allocator(const ALLOCATION_CALLBACKS& allocationCallbacks, const ALLOCATOR_DESC& desc);
    ~Allocator();
    
    AllocatorPimpl* m_Pimpl;
    
    D3D12MA_CLASS_NO_COPY(Allocator)
};


enum VIRTUAL_BLOCK_FLAGS
{
    /// Zero
    VIRTUAL_BLOCK_FLAG_NONE = 0,

    /** \brief Enables alternative, linear allocation algorithm in this virtual block.

    Specify this flag to enable linear allocation algorithm, which always creates
    new allocations after last one and doesn't reuse space from allocations freed in
    between. It trades memory consumption for simplified algorithm and data
    structure, which has better performance and uses less memory for metadata.

    By using this flag, you can achieve behavior of free-at-once, stack,
    ring buffer, and double stack.
    For details, see documentation chapter \ref linear_algorithm.
    */
    VIRTUAL_BLOCK_FLAG_ALGORITHM_LINEAR = POOL_FLAG_ALGORITHM_LINEAR,

    // Bit mask to extract only `ALGORITHM` bits from entire set of flags.
    VIRTUAL_BLOCK_FLAG_ALGORITHM_MASK = POOL_FLAG_ALGORITHM_MASK
};

struct VIRTUAL_BLOCK_DESC
{
    /// Flags.
    VIRTUAL_BLOCK_FLAGS Flags;
    /** \brief Total size of the block.

    Sizes can be expressed in bytes or any units you want as long as you are consistent in using them.
    For example, if you allocate from some array of structures, 1 can mean single instance of entire structure.
    */
    UINT64 Size;
    /** \brief Custom CPU memory allocation callbacks. Optional.

    Optional, can be null. When specified, will be used for all CPU-side memory allocations.
    */
    const ALLOCATION_CALLBACKS* pAllocationCallbacks;
};

enum VIRTUAL_ALLOCATION_FLAGS
{
    /// Zero
    VIRTUAL_ALLOCATION_FLAG_NONE = 0,

    /** \brief Allocation will be created from upper stack in a double stack pool.

    This flag is only allowed for virtual blocks created with #VIRTUAL_BLOCK_FLAG_ALGORITHM_LINEAR flag.
    */
    VIRTUAL_ALLOCATION_FLAG_UPPER_ADDRESS = ALLOCATION_FLAG_UPPER_ADDRESS,

    /// Allocation strategy that tries to minimize memory usage.
    VIRTUAL_ALLOCATION_FLAG_STRATEGY_MIN_MEMORY = ALLOCATION_FLAG_STRATEGY_MIN_MEMORY,
    /// Allocation strategy that tries to minimize allocation time.
    VIRTUAL_ALLOCATION_FLAG_STRATEGY_MIN_TIME = ALLOCATION_FLAG_STRATEGY_MIN_TIME,
    /** \brief Allocation strategy that chooses always the lowest offset in available space.
    This is not the most efficient strategy but achieves highly packed data.
    */
    VIRTUAL_ALLOCATION_FLAG_STRATEGY_MIN_OFFSET = ALLOCATION_FLAG_STRATEGY_MIN_OFFSET,
    /** \brief A bit mask to extract only `STRATEGY` bits from entire set of flags.

    These strategy flags are binary compatible with equivalent flags in #ALLOCATION_FLAGS.
    */
    VIRTUAL_ALLOCATION_FLAG_STRATEGY_MASK = ALLOCATION_FLAG_STRATEGY_MASK,
};

struct VIRTUAL_ALLOCATION_DESC
{
    /// Flags.
    VIRTUAL_ALLOCATION_FLAGS Flags;
    /** \brief Size of the allocation.
    
    Cannot be zero.
    */
    UINT64 Size;
    /** \brief Required alignment of the allocation.
    
    Must be power of two. Special value 0 has the same meaning as 1 - means no special alignment is required, so allocation can start at any offset.
    */
    UINT64 Alignment;
    /** \brief Custom pointer to be associated with the allocation.

    It can be fetched or changed later.
    */
    void* pPrivateData;
};

struct VIRTUAL_ALLOCATION_INFO
{
    /// \brief Offset of the allocation.
    UINT64 Offset;
    /** \brief Size of the allocation.

    Same value as passed in VIRTUAL_ALLOCATION_DESC::Size.
    */
    UINT64 Size;
    /** \brief Custom pointer associated with the allocation.

    Same value as passed in VIRTUAL_ALLOCATION_DESC::pPrivateData or VirtualBlock::SetAllocationPrivateData().
    */
    void* pPrivateData;
};

/** \brief Represents pure allocation algorithm and a data structure with allocations in some memory block, without actually allocating any GPU memory.

This class allows to use the core algorithm of the library custom allocations e.g. CPU memory or
sub-allocation regions inside a single GPU buffer.

To create this object, fill in D3D12MA::VIRTUAL_BLOCK_DESC and call CreateVirtualBlock().
To destroy it, call its method `VirtualBlock::Release()`.
You need to free all the allocations within this block or call Clear() before destroying it.

This object is not thread-safe - should not be used from multiple threads simultaneously, must be synchronized externally.
*/
class D3D12MA_API VirtualBlock : public IUnknownImpl
{
public:
    /** \brief Returns true if the block is empty - contains 0 allocations.
    */
    BOOL IsEmpty() const;
    /** \brief Returns information about an allocation - its offset, size and custom pointer.
    */
    void GetAllocationInfo(VirtualAllocation allocation, VIRTUAL_ALLOCATION_INFO* pInfo) const;

    /** \brief Creates new allocation.
    \param pDesc
    \param[out] pAllocation Unique indentifier of the new allocation within single block.
    \param[out] pOffset Returned offset of the new allocation. Optional, can be null.
    \return `S_OK` if allocation succeeded, `E_OUTOFMEMORY` if it failed.

    If the allocation failed, `pAllocation->AllocHandle` is set to 0 and `pOffset`, if not null, is set to `UINT64_MAX`.
    */
    HRESULT Allocate(const VIRTUAL_ALLOCATION_DESC* pDesc, VirtualAllocation* pAllocation, UINT64* pOffset);
    /** \brief Frees the allocation.
    
    Calling this function with `allocation.AllocHandle == 0` is correct and does nothing.
    */
    void FreeAllocation(VirtualAllocation allocation);
    /** \brief Frees all the allocations.
    */
    void Clear();
    /** \brief Changes custom pointer for an allocation to a new value.
    */
    void SetAllocationPrivateData(VirtualAllocation allocation, void* pPrivateData);
    /** \brief Retrieves basic statistics of the virtual block that are fast to calculate.

    \param[out] pStats %Statistics of the virtual block.
    */
    void GetStatistics(Statistics* pStats) const;
    /** \brief Retrieves detailed statistics of the virtual block that are slower to calculate.

    \param[out] pStats %Statistics of the virtual block.
    */
    void CalculateStatistics(DetailedStatistics* pStats) const;

    /** \brief Builds and returns statistics as a string in JSON format, including the list of allocations with their parameters.
    @param[out] ppStatsString Must be freed using VirtualBlock::FreeStatsString.
    */
    void BuildStatsString(WCHAR** ppStatsString) const;

    /** \brief Frees memory of a string returned from VirtualBlock::BuildStatsString.
    */
    void FreeStatsString(WCHAR* pStatsString) const;
   
protected:
    void ReleaseThis() override;

private:
    friend D3D12MA_API HRESULT CreateVirtualBlock(const VIRTUAL_BLOCK_DESC*, VirtualBlock**);
    template<typename T> friend void D3D12MA_DELETE(const ALLOCATION_CALLBACKS&, T*);

    VirtualBlockPimpl* m_Pimpl;

    VirtualBlock(const ALLOCATION_CALLBACKS& allocationCallbacks, const VIRTUAL_BLOCK_DESC& desc);
    ~VirtualBlock();

    D3D12MA_CLASS_NO_COPY(VirtualBlock)
};


/** \brief Creates new main D3D12MA::Allocator object and returns it through `ppAllocator`.

You normally only need to call it once and keep a single Allocator object for your `ID3D12Device`.
*/
D3D12MA_API HRESULT CreateAllocator(const ALLOCATOR_DESC* pDesc, Allocator** ppAllocator);

/** \brief Creates new D3D12MA::VirtualBlock object and returns it through `ppVirtualBlock`.

Note you don't need to create D3D12MA::Allocator to use virtual blocks.
*/
D3D12MA_API HRESULT CreateVirtualBlock(const VIRTUAL_BLOCK_DESC* pDesc, VirtualBlock** ppVirtualBlock);

} // namespace D3D12MA

DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::ALLOCATION_FLAGS);
DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::DEFRAGMENTATION_FLAGS);
DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::ALLOCATOR_FLAGS);
DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::POOL_FLAGS);
DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::VIRTUAL_BLOCK_FLAGS);
DEFINE_ENUM_FLAG_OPERATORS(D3D12MA::VIRTUAL_ALLOCATION_FLAGS);

/**
\page quick_start Quick start

\section quick_start_project_setup Project setup and initialization

This is a small, standalone C++ library. It consists of a pair of 2 files:
"D3D12MemAlloc.h" header file with public interface and "D3D12MemAlloc.cpp" with
internal implementation. The only external dependencies are WinAPI, Direct3D 12,
and parts of C/C++ standard library (but STL containers, exceptions, or RTTI are
not used).

The library is developed and tested using Microsoft Visual Studio 2019, but it
should work with other compilers as well. It is designed for 64-bit code.

To use the library in your project:

(1.) Copy files `D3D12MemAlloc.cpp`, `%D3D12MemAlloc.h` to your project.

(2.) Make `D3D12MemAlloc.cpp` compiling as part of the project, as C++ code.

(3.) Include library header in each CPP file that needs to use the library.

\code
\endcode

(4.) Right after you created `ID3D12Device`, fill D3D12MA::ALLOCATOR_DESC
structure and call function D3D12MA::CreateAllocator to create the main
D3D12MA::Allocator object.

Please note that all symbols of the library are declared inside #D3D12MA namespace.

\code
IDXGIAdapter* adapter = (...)
ID3D12Device* device = (...)

D3D12MA::ALLOCATOR_DESC allocatorDesc = {};
allocatorDesc.pDevice = device;
allocatorDesc.pAdapter = adapter;

D3D12MA::Allocator* allocator;
HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &allocator);
\endcode

(5.) Right before destroying the D3D12 device, destroy the allocator object.

Objects of this library must be destroyed by calling `Release` method.
They are somewhat compatible with COM: they implement `IUnknown` interface with its virtual methods: `AddRef`, `Release`, `QueryInterface`,
and they are reference-counted internally.
You can use smart pointers designed for COM with objects of this library - e.g. `CComPtr` or `Microsoft::WRL::ComPtr`.
The reference counter is thread-safe.
`QueryInterface` method supports only `IUnknown`, as classes of this library don't define their own GUIDs.

\code
allocator->Release();
\endcode


\section quick_start_creating_resources Creating resources

To use the library for creating resources (textures and buffers), call method
D3D12MA::Allocator::CreateResource in the place where you would previously call
`ID3D12Device::CreateCommittedResource`.

The function has similar syntax, but it expects structure D3D12MA::ALLOCATION_DESC
to be passed along with `D3D12_RESOURCE_DESC` and other parameters for created
resource. This structure describes parameters of the desired memory allocation,
including choice of `D3D12_HEAP_TYPE`.

The function returns a new object of type D3D12MA::Allocation.
It represents allocated memory and can be queried for size, offset, `ID3D12Heap`.
It also holds a reference to the `ID3D12Resource`, which can be accessed by calling D3D12MA::Allocation::GetResource().

\code
D3D12_RESOURCE_DESC resourceDesc = {};
resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
resourceDesc.Alignment = 0;
resourceDesc.Width = 1024;
resourceDesc.Height = 1024;
resourceDesc.DepthOrArraySize = 1;
resourceDesc.MipLevels = 1;
resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
resourceDesc.SampleDesc.Count = 1;
resourceDesc.SampleDesc.Quality = 0;
resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

D3D12MA::ALLOCATION_DESC allocationDesc = {};
allocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

D3D12MA::Allocation* allocation;
HRESULT hr = allocator->CreateResource(
    &allocationDesc,
    &resourceDesc,
    D3D12_RESOURCE_STATE_COPY_DEST,
    NULL,
    &allocation,
    IID_NULL, NULL);

\endcode

You need to release the allocation object when no longer needed.
This will also release the D3D12 resource.

\code
allocation->Release();
\endcode

The advantage of using the allocator instead of creating committed resource, and
the main purpose of this library, is that it can decide to allocate bigger memory
heap internally using `ID3D12Device::CreateHeap` and place multiple resources in
it, at different offsets, using `ID3D12Device::CreatePlacedResource`. The library
manages its own collection of allocated memory blocks (heaps) and remembers which
parts of them are occupied and which parts are free to be used for new resources.

It is important to remember that resources created as placed don't have their memory
initialized to zeros, but may contain garbage data, so they need to be fully initialized
before usage, e.g. using Clear (`ClearRenderTargetView`), Discard (`DiscardResource`),
or copy (`CopyResource`).

The library also automatically handles resource heap tier.
When `D3D12_FEATURE_DATA_D3D12_OPTIONS::ResourceHeapTier` equals `D3D12_RESOURCE_HEAP_TIER_1`,
resources of 3 types: buffers, textures that are render targets or depth-stencil,
and other textures must be kept in separate heaps. When `D3D12_RESOURCE_HEAP_TIER_2`,
they can be kept together. By using this library, you don't need to handle this
manually.


\section quick_start_resource_reference_counting Resource reference counting

`ID3D12Resource` and other interfaces of Direct3D 12 use COM, so they are reference-counted.
Objects of this library are reference-counted as well.
An object of type D3D12MA::Allocation remembers the resource (buffer or texture)
that was created together with this memory allocation
and holds a reference to the `ID3D12Resource` object.
(Note this is a difference to Vulkan Memory Allocator, where a `VmaAllocation` object has no connection
with the buffer or image that was created with it.)
Thus, it is important to manage the resource reference counter properly.

<b>The simplest use case</b> is shown in the code snippet above.
When only D3D12MA::Allocation object is obtained from a function call like D3D12MA::Allocator::CreateResource,
it remembers the `ID3D12Resource` that was created with it and holds a reference to it.
The resource can be obtained by calling `allocation->GetResource()`, which doesn't increment the resource
reference counter.
Calling `allocation->Release()` will decrease the resource reference counter, which is = 1 in this case,
so the resource will be released.

<b>Second option</b> is to retrieve a pointer to the resource along with D3D12MA::Allocation.
Last parameters of the resource creation function can be used for this purpose.

\code
D3D12MA::Allocation* allocation;
ID3D12Resource* resource;
HRESULT hr = allocator->CreateResource(
    &allocationDesc,
    &resourceDesc,
    D3D12_RESOURCE_STATE_COPY_DEST,
    NULL,
    &allocation,
    IID_PPV_ARGS(&resource));

\endcode

In this case, returned pointer `resource` is equal to `allocation->GetResource()`,
but the creation function additionally increases resource reference counter for the purpose of returning it from this call
(it actually calls `QueryInterface` internally), so the resource will have the counter = 2.
The resource then need to be released along with the allocation, in this particular order,
to make sure the resource is destroyed before its memory heap can potentially be freed.

\code
resource->Release();
allocation->Release();
\endcode

<b>More advanced use cases</b> are possible when we consider that an D3D12MA::Allocation object can just hold
a reference to any resource.
It can be changed by calling D3D12MA::Allocation::SetResource. This function
releases the old resource and calls `AddRef` on the new one.

Special care must be taken when performing <b>defragmentation</b>.
The new resource created at the destination place should be set as `pass.pMoves[i].pDstTmpAllocation->SetResource(newRes)`,
but it is moved to the source allocation at end of the defragmentation pass,
while the old resource accessible through `pass.pMoves[i].pSrcAllocation->GetResource()` is then released.
For more information, see documentation chapter \ref defragmentation.


\section quick_start_mapping_memory Mapping memory

The process of getting regular CPU-side pointer to the memory of a resource in
Direct3D is called "mapping". There are rules and restrictions to this process,
as described in D3D12 documentation of `ID3D12Resource::Map` method.

Mapping happens on the level of particular resources, not entire memory heaps,
and so it is out of scope of this library. Just as the documentation of the `Map` function says:

- Returned pointer refers to data of particular subresource, not entire memory heap.
- You can map same resource multiple times. It is ref-counted internally.
- Mapping is thread-safe.
- Unmapping is not required before resource destruction.
- Unmapping may not be required before using written data - some heap types on
  some platforms support resources persistently mapped.

When using this library, you can map and use your resources normally without
considering whether they are created as committed resources or placed resources in one large heap.

Example for buffer created and filled in `UPLOAD` heap type:

\code
const UINT64 bufSize = 65536;
const float* bufData = (...);

D3D12_RESOURCE_DESC resourceDesc = {};
resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
resourceDesc.Alignment = 0;
resourceDesc.Width = bufSize;
resourceDesc.Height = 1;
resourceDesc.DepthOrArraySize = 1;
resourceDesc.MipLevels = 1;
resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
resourceDesc.SampleDesc.Count = 1;
resourceDesc.SampleDesc.Quality = 0;
resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

D3D12MA::ALLOCATION_DESC allocationDesc = {};
allocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

D3D12Resource* resource;
D3D12MA::Allocation* allocation;
HRESULT hr = allocator->CreateResource(
    &allocationDesc,
    &resourceDesc,
    D3D12_RESOURCE_STATE_GENERIC_READ,
    NULL,
    &allocation,
    IID_PPV_ARGS(&resource));

void* mappedPtr;
hr = resource->Map(0, NULL, &mappedPtr);

memcpy(mappedPtr, bufData, bufSize);

resource->Unmap(0, NULL);
\endcode


\page custom_pools Custom memory pools

A "pool" is a collection of memory blocks that share certain properties.
Allocator creates 3 default pools: for `D3D12_HEAP_TYPE_DEFAULT`, `UPLOAD`, `READBACK`.
A default pool automatically grows in size. Size of allocated blocks is also variable and managed automatically.
Typical allocations are created in these pools. You can also create custom pools.

\section custom_pools_usage Usage

To create a custom pool, fill in structure D3D12MA::POOL_DESC and call function D3D12MA::Allocator::CreatePool
to obtain object D3D12MA::Pool. Example:

\code
POOL_DESC poolDesc = {};
poolDesc.HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

Pool* pool;
HRESULT hr = allocator->CreatePool(&poolDesc, &pool);
\endcode

To allocate resources out of a custom pool, only set member D3D12MA::ALLOCATION_DESC::CustomPool.
Example:

\code
ALLOCATION_DESC allocDesc = {};
allocDesc.CustomPool = pool;

D3D12_RESOURCE_DESC resDesc = ...
Allocation* alloc;
hr = allocator->CreateResource(&allocDesc, &resDesc,
    D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &alloc, IID_NULL, NULL);
\endcode

All allocations must be released before releasing the pool.
The pool must be released before relasing the allocator.

\code
alloc->Release();
pool->Release();
\endcode

\section custom_pools_features_and_benefits Features and benefits

While it is recommended to use default pools whenever possible for simplicity and to give the allocator
more opportunities for internal optimizations, custom pools may be useful in following cases:

- To keep some resources separate from others in memory.
- To keep track of memory usage of just a specific group of resources. %Statistics can be queried using
  D3D12MA::Pool::CalculateStatistics.
- To use specific size of a memory block (`ID3D12Heap`). To set it, use member D3D12MA::POOL_DESC::BlockSize.
  When set to 0, the library uses automatically determined, variable block sizes.
- To reserve some minimum amount of memory allocated. To use it, set member D3D12MA::POOL_DESC::MinBlockCount.
- To limit maximum amount of memory allocated. To use it, set member D3D12MA::POOL_DESC::MaxBlockCount.
- To use extended parameters of the D3D12 memory allocation. While resources created from default pools
  can only specify `D3D12_HEAP_TYPE_DEFAULT`, `UPLOAD`, `READBACK`, a custom pool may use non-standard
  `D3D12_HEAP_PROPERTIES` (member D3D12MA::POOL_DESC::HeapProperties) and `D3D12_HEAP_FLAGS`
  (D3D12MA::POOL_DESC::HeapFlags), which is useful e.g. for cross-adapter sharing or UMA
  (see also D3D12MA::Allocator::IsUMA).

New versions of this library support creating **committed allocations in custom pools**.
It is supported only when D3D12MA::POOL_DESC::BlockSize = 0.
To use this feature, set D3D12MA::ALLOCATION_DESC::CustomPool to the pointer to your custom pool and
D3D12MA::ALLOCATION_DESC::Flags to D3D12MA::ALLOCATION_FLAG_COMMITTED. Example:

\code
ALLOCATION_DESC allocDesc = {};
allocDesc.CustomPool = pool;
allocDesc.Flags = ALLOCATION_FLAG_COMMITTED;

D3D12_RESOURCE_DESC resDesc = ...
Allocation* alloc;
ID3D12Resource* res;
hr = allocator->CreateResource(&allocDesc, &resDesc,
    D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &alloc, IID_PPV_ARGS(&res));
\endcode

This feature may seem unnecessary, but creating committed allocations from custom pools may be useful
in some cases, e.g. to have separate memory usage statistics for some group of resources or to use
extended allocation parameters, like custom `D3D12_HEAP_PROPERTIES`, which are available only in custom pools.


\page defragmentation Defragmentation

Interleaved allocations and deallocations of many objects of varying size can
cause fragmentation over time, which can lead to a situation where the library is unable
to find a continuous range of free memory for a new allocation despite there is
enough free space, just scattered across many small free ranges between existing
allocations.

To mitigate this problem, you can use defragmentation feature.
It doesn't happen automatically though and needs your cooperation,
because %D3D12MA is a low level library that only allocates memory.
It cannot recreate buffers and textures in a new place as it doesn't remember the contents of `D3D12_RESOURCE_DESC` structure.
It cannot copy their contents as it doesn't record any commands to a command list.

Example:

\code
D3D12MA::DEFRAGMENTATION_DESC defragDesc = {};
defragDesc.Flags = D3D12MA::DEFRAGMENTATION_FLAG_ALGORITHM_FAST;

D3D12MA::DefragmentationContext* defragCtx;
allocator->BeginDefragmentation(&defragDesc, &defragCtx);

for(;;)
{
    D3D12MA::DEFRAGMENTATION_PASS_MOVE_INFO pass;
    HRESULT hr = defragCtx->BeginPass(&pass);
    if(hr == S_OK)
        break;
    else if(hr != S_FALSE)
        // Handle error...

    for(UINT i = 0; i < pass.MoveCount; ++i)
    {
        // Inspect pass.pMoves[i].pSrcAllocation, identify what buffer/texture it represents.
        MyEngineResourceData* resData = (MyEngineResourceData*)pMoves[i].pSrcAllocation->GetPrivateData();
            
        // Recreate this buffer/texture as placed at pass.pMoves[i].pDstTmpAllocation.
        D3D12_RESOURCE_DESC resDesc = ...
        ID3D12Resource* newRes;
        hr = device->CreatePlacedResource(
            pass.pMoves[i].pDstTmpAllocation->GetHeap(),
            pass.pMoves[i].pDstTmpAllocation->GetOffset(), &resDesc,
            D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&newRes));
        // Check hr...

        // Store new resource in the pDstTmpAllocation.
        pass.pMoves[i].pDstTmpAllocation->SetResource(newRes);

        // Copy its content to the new place.
        cmdList->CopyResource(
            pass.pMoves[i].pDstTmpAllocation->GetResource(),
            pass.pMoves[i].pSrcAllocation->GetResource());
    }
        
    // Make sure the copy commands finished executing.
    cmdQueue->ExecuteCommandLists(...);
    // ...
    WaitForSingleObject(fenceEvent, INFINITE);

    // Update appropriate descriptors to point to the new places...
        
    hr = defragCtx->EndPass(&pass);
    if(hr == S_OK)
        break;
    else if(hr != S_FALSE)
        // Handle error...
}

defragCtx->Release();
\endcode

Although functions like D3D12MA::Allocator::CreateResource()
create an allocation and a buffer/texture at once, these are just a shortcut for
allocating memory and creating a placed resource.
Defragmentation works on memory allocations only. You must handle the rest manually.
Defragmentation is an iterative process that should repreat "passes" as long as related functions
return `S_FALSE` not `S_OK`.
In each pass:

1. D3D12MA::DefragmentationContext::BeginPass() function call:
   - Calculates and returns the list of allocations to be moved in this pass.
     Note this can be a time-consuming process.
   - Reserves destination memory for them by creating temporary destination allocations
     that you can query for their `ID3D12Heap` + offset using methods like D3D12MA::Allocation::GetHeap().
2. Inside the pass, **you should**:
   - Inspect the returned list of allocations to be moved.
   - Create new buffers/textures as placed at the returned destination temporary allocations.
   - Copy data from source to destination resources if necessary.
   - Store the pointer to the new resource in the temporary destination allocation.
3. D3D12MA::DefragmentationContext::EndPass() function call:
   - Frees the source memory reserved for the allocations that are moved.
   - Modifies source D3D12MA::Allocation objects that are moved to point to the destination reserved memory
     and destination resource, while source resource is released.
   - Frees `ID3D12Heap` blocks that became empty.

Defragmentation algorithm tries to move all suitable allocations.
You can, however, refuse to move some of them inside a defragmentation pass, by setting
`pass.pMoves[i].Operation` to D3D12MA::DEFRAGMENTATION_MOVE_OPERATION_IGNORE.
This is not recommended and may result in suboptimal packing of the allocations after defragmentation.
If you cannot ensure any allocation can be moved, it is better to keep movable allocations separate in a custom pool.

Inside a pass, for each allocation that should be moved:

- You should copy its data from the source to the destination place by calling e.g. `CopyResource()`.
  - You need to make sure these commands finished executing before the source buffers/textures are released by D3D12MA::DefragmentationContext::EndPass().
- If a resource doesn't contain any meaningful data, e.g. it is a transient render-target texture to be cleared,
  filled, and used temporarily in each rendering frame, you can just recreate this texture
  without copying its data.
- If the resource is in `D3D12_HEAP_TYPE_READBACK` memory, you can copy its data on the CPU
  using `memcpy()`.
- If you cannot move the allocation, you can set `pass.pMoves[i].Operation` to D3D12MA::DEFRAGMENTATION_MOVE_OPERATION_IGNORE.
  This will cancel the move.
  - D3D12MA::DefragmentationContext::EndPass() will then free the destination memory
    not the source memory of the allocation, leaving it unchanged.
- If you decide the allocation is unimportant and can be destroyed instead of moved (e.g. it wasn't used for long time),
  you can set `pass.pMoves[i].Operation` to D3D12MA::DEFRAGMENTATION_MOVE_OPERATION_DESTROY.
  - D3D12MA::DefragmentationContext::EndPass() will then free both source and destination memory, and will destroy the source D3D12MA::Allocation object.

You can defragment a specific custom pool by calling D3D12MA::Pool::BeginDefragmentation
or all the default pools by calling D3D12MA::Allocator::BeginDefragmentation (like in the example above).

Defragmentation is always performed in each pool separately.
Allocations are never moved between different heap types.
The size of the destination memory reserved for a moved allocation is the same as the original one.
Alignment of an allocation as it was determined using `GetResourceAllocationInfo()` is also respected after defragmentation.
Buffers/textures should be recreated with the same `D3D12_RESOURCE_DESC` parameters as the original ones.

You can perform the defragmentation incrementally to limit the number of allocations and bytes to be moved
in each pass, e.g. to call it in sync with render frames and not to experience too big hitches.
See members: D3D12MA::DEFRAGMENTATION_DESC::MaxBytesPerPass, D3D12MA::DEFRAGMENTATION_DESC::MaxAllocationsPerPass.

It is also safe to perform the defragmentation asynchronously to render frames and other Direct3D 12 and %D3D12MA
usage, possibly from multiple threads, with the exception that allocations
returned in D3D12MA::DEFRAGMENTATION_PASS_MOVE_INFO::pMoves shouldn't be released until the defragmentation pass is ended.

<b>Mapping</b> is out of scope of this library and so it is not preserved after an allocation is moved during defragmentation.
You need to map the new resource yourself if needed.

\note Defragmentation is not supported in custom pools created with D3D12MA::POOL_FLAG_ALGORITHM_LINEAR.


\page statistics Statistics

This library contains several functions that return information about its internal state,
especially the amount of memory allocated from D3D12.

\section statistics_numeric_statistics Numeric statistics

If you need to obtain basic statistics about memory usage per memory segment group, together with current budget,
you can call function D3D12MA::Allocator::GetBudget() and inspect structure D3D12MA::Budget.
This is useful to keep track of memory usage and stay withing budget.
Example:

\code
D3D12MA::Budget localBudget;
allocator->GetBudget(&localBudget, NULL);

printf("My GPU memory currently has %u allocations taking %llu B,\n",
    localBudget.Statistics.AllocationCount,
    localBudget.Statistics.AllocationBytes);
printf("allocated out of %u D3D12 memory heaps taking %llu B,\n",
    localBudget.Statistics.BlockCount,
    localBudget.Statistics.BlockBytes);
printf("D3D12 reports total usage %llu B with budget %llu B.\n",
    localBudget.UsageBytes,
    localBudget.BudgetBytes);
\endcode

You can query for more detailed statistics per heap type, memory segment group, and totals,
including minimum and maximum allocation size and unused range size,
by calling function D3D12MA::Allocator::CalculateStatistics() and inspecting structure D3D12MA::TotalStatistics.
This function is slower though, as it has to traverse all the internal data structures,
so it should be used only for debugging purposes.

You can query for statistics of a custom pool using function D3D12MA::Pool::GetStatistics()
or D3D12MA::Pool::CalculateStatistics().

You can query for information about a specific allocation using functions of the D3D12MA::Allocation class,
e.g. `GetSize()`, `GetOffset()`, `GetHeap()`.

\section statistics_json_dump JSON dump

You can dump internal state of the allocator to a string in JSON format using function D3D12MA::Allocator::BuildStatsString().
The result is guaranteed to be correct JSON.
It uses Windows Unicode (UTF-16) encoding.
Any strings provided by user (see D3D12MA::Allocation::SetName())
are copied as-is and properly escaped for JSON.
It must be freed using function D3D12MA::Allocator::FreeStatsString().

The format of this JSON string is not part of official documentation of the library,
but it will not change in backward-incompatible way without increasing library major version number
and appropriate mention in changelog.

The JSON string contains all the data that can be obtained using D3D12MA::Allocator::CalculateStatistics().
It can also contain detailed map of allocated memory blocks and their regions -
free and occupied by allocations.
This allows e.g. to visualize the memory or assess fragmentation.


\page resource_aliasing Resource aliasing (overlap)

New explicit graphics APIs (Vulkan and Direct3D 12), thanks to manual memory
management, give an opportunity to alias (overlap) multiple resources in the
same region of memory - a feature not available in the old APIs (Direct3D 11, OpenGL).
It can be useful to save video memory, but it must be used with caution.

For example, if you know the flow of your whole render frame in advance, you
are going to use some intermediate textures or buffers only during a small range of render passes,
and you know these ranges don't overlap in time, you can create these resources in
the same place in memory, even if they have completely different parameters (width, height, format etc.).

![Resource aliasing (overlap)](../gfx/Aliasing.png)

Such scenario is possible using D3D12MA, but you need to create your resources
using special function D3D12MA::Allocator::CreateAliasingResource.
Before that, you need to allocate memory with parameters calculated using formula:

- allocation size = max(size of each resource)
- allocation alignment = max(alignment of each resource)

Following example shows two different textures created in the same place in memory,
allocated to fit largest of them.

\code
D3D12_RESOURCE_DESC resDesc1 = {};
resDesc1.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
resDesc1.Alignment = 0;
resDesc1.Width = 1920;
resDesc1.Height = 1080;
resDesc1.DepthOrArraySize = 1;
resDesc1.MipLevels = 1;
resDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
resDesc1.SampleDesc.Count = 1;
resDesc1.SampleDesc.Quality = 0;
resDesc1.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
resDesc1.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

D3D12_RESOURCE_DESC resDesc2 = {};
resDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
resDesc2.Alignment = 0;
resDesc2.Width = 1024;
resDesc2.Height = 1024;
resDesc2.DepthOrArraySize = 1;
resDesc2.MipLevels = 0;
resDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
resDesc2.SampleDesc.Count = 1;
resDesc2.SampleDesc.Quality = 0;
resDesc2.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
resDesc2.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

const D3D12_RESOURCE_ALLOCATION_INFO allocInfo1 =
    device->GetResourceAllocationInfo(0, 1, &resDesc1);
const D3D12_RESOURCE_ALLOCATION_INFO allocInfo2 =
    device->GetResourceAllocationInfo(0, 1, &resDesc2);

D3D12_RESOURCE_ALLOCATION_INFO finalAllocInfo = {};
finalAllocInfo.Alignment = std::max(allocInfo1.Alignment, allocInfo2.Alignment);
finalAllocInfo.SizeInBytes = std::max(allocInfo1.SizeInBytes, allocInfo2.SizeInBytes);

D3D12MA::ALLOCATION_DESC allocDesc = {};
allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
allocDesc.ExtraHeapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;

D3D12MA::Allocation* alloc;
hr = allocator->AllocateMemory(&allocDesc, &finalAllocInfo, &alloc);
assert(alloc != NULL && alloc->GetHeap() != NULL);

ID3D12Resource* res1;
hr = allocator->CreateAliasingResource(
    alloc,
    0, // AllocationLocalOffset
    &resDesc1,
    D3D12_RESOURCE_STATE_COMMON,
    NULL, // pOptimizedClearValue
    IID_PPV_ARGS(&res1));

ID3D12Resource* res2;
hr = allocator->CreateAliasingResource(
    alloc,
    0, // AllocationLocalOffset
    &resDesc2,
    D3D12_RESOURCE_STATE_COMMON,
    NULL, // pOptimizedClearValue
    IID_PPV_ARGS(&res2));


res2->Release();
res1->Release();
alloc->Release();
\endcode

Remember that using resouces that alias in memory requires proper synchronization.
You need to issue a special barrier of type `D3D12_RESOURCE_BARRIER_TYPE_ALIASING`.
You also need to treat a resource after aliasing as uninitialized - containing garbage data.
For example, if you use `res1` and then want to use `res2`, you need to first initialize `res2`
using either Clear, Discard, or Copy to the entire resource.

Additional considerations:

- D3D12 also allows to interpret contents of memory between aliasing resources consistently in some cases,
  which is called "data inheritance". For details, see
  Microsoft documentation chapter "Memory Aliasing and Data Inheritance".
- You can create more complex layout where different textures and buffers are bound
  at different offsets inside one large allocation. For example, one can imagine
  a big texture used in some render passes, aliasing with a set of many small buffers
  used in some further passes. To bind a resource at non-zero offset of an allocation,
  call D3D12MA::Allocator::CreateAliasingResource with appropriate value of `AllocationLocalOffset` parameter.
- Resources of the three categories: buffers, textures with `RENDER_TARGET` or `DEPTH_STENCIL` flags, and all other textures,
  can be placed in the same memory only when `allocator->GetD3D12Options().ResourceHeapTier >= D3D12_RESOURCE_HEAP_TIER_2`.
  Otherwise they must be placed in different memory heap types, and thus aliasing them is not possible.


\page linear_algorithm Linear allocation algorithm

Each D3D12 memory block managed by this library has accompanying metadata that
keeps track of used and unused regions. By default, the metadata structure and
algorithm tries to find best place for new allocations among free regions to
optimize memory usage. This way you can allocate and free objects in any order.

![Default allocation algorithm](../gfx/Linear_allocator_1_algo_default.png)

Sometimes there is a need to use simpler, linear allocation algorithm. You can
create custom pool that uses such algorithm by adding flag
D3D12MA::POOL_FLAG_ALGORITHM_LINEAR to D3D12MA::POOL_DESC::Flags while creating
D3D12MA::Pool object. Then an alternative metadata management is used. It always
creates new allocations after last one and doesn't reuse free regions after
allocations freed in the middle. It results in better allocation performance and
less memory consumed by metadata.

![Linear allocation algorithm](../gfx/Linear_allocator_2_algo_linear.png)

With this one flag, you can create a custom pool that can be used in many ways:
free-at-once, stack, double stack, and ring buffer. See below for details.
You don't need to specify explicitly which of these options you are going to use - it is detected automatically.

\section linear_algorithm_free_at_once Free-at-once

In a pool that uses linear algorithm, you still need to free all the allocations
individually by calling `allocation->Release()`. You can free
them in any order. New allocations are always made after last one - free space
in the middle is not reused. However, when you release all the allocation and
the pool becomes empty, allocation starts from the beginning again. This way you
can use linear algorithm to speed up creation of allocations that you are going
to release all at once.

![Free-at-once](../gfx/Linear_allocator_3_free_at_once.png)

This mode is also available for pools created with D3D12MA::POOL_DESC::MaxBlockCount
value that allows multiple memory blocks.

\section linear_algorithm_stack Stack

When you free an allocation that was created last, its space can be reused.
Thanks to this, if you always release allocations in the order opposite to their
creation (LIFO - Last In First Out), you can achieve behavior of a stack.

![Stack](../gfx/Linear_allocator_4_stack.png)

This mode is also available for pools created with D3D12MA::POOL_DESC::MaxBlockCount
value that allows multiple memory blocks.

\section linear_algorithm_double_stack Double stack

The space reserved by a custom pool with linear algorithm may be used by two
stacks:

- First, default one, growing up from offset 0.
- Second, "upper" one, growing down from the end towards lower offsets.

To make allocation from the upper stack, add flag D3D12MA::ALLOCATION_FLAG_UPPER_ADDRESS
to D3D12MA::ALLOCATION_DESC::Flags.

![Double stack](../gfx/Linear_allocator_7_double_stack.png)

Double stack is available only in pools with one memory block -
D3D12MA::POOL_DESC::MaxBlockCount must be 1. Otherwise behavior is undefined.

When the two stacks' ends meet so there is not enough space between them for a
new allocation, such allocation fails with usual `E_OUTOFMEMORY` error.

\section linear_algorithm_ring_buffer Ring buffer

When you free some allocations from the beginning and there is not enough free space
for a new one at the end of a pool, allocator's "cursor" wraps around to the
beginning and starts allocation there. Thanks to this, if you always release
allocations in the same order as you created them (FIFO - First In First Out),
you can achieve behavior of a ring buffer / queue.

![Ring buffer](../gfx/Linear_allocator_5_ring_buffer.png)

Ring buffer is available only in pools with one memory block -
D3D12MA::POOL_DESC::MaxBlockCount must be 1. Otherwise behavior is undefined.

\section linear_algorithm_additional_considerations Additional considerations

Linear algorithm can also be used with \ref virtual_allocator.
See flag D3D12MA::VIRTUAL_BLOCK_FLAG_ALGORITHM_LINEAR.


\page virtual_allocator Virtual allocator

As an extra feature, the core allocation algorithm of the library is exposed through a simple and convenient API of "virtual allocator".
It doesn't allocate any real GPU memory. It just keeps track of used and free regions of a "virtual block".
You can use it to allocate your own memory or other objects, even completely unrelated to D3D12.
A common use case is sub-allocation of pieces of one large GPU buffer.

\section virtual_allocator_creating_virtual_block Creating virtual block

To use this functionality, there is no main "allocator" object.
You don't need to have D3D12MA::Allocator object created.
All you need to do is to create a separate D3D12MA::VirtualBlock object for each block of memory you want to be managed by the allocator:

-# Fill in D3D12MA::ALLOCATOR_DESC structure.
-# Call D3D12MA::CreateVirtualBlock. Get new D3D12MA::VirtualBlock object.

Example:

\code
D3D12MA::VIRTUAL_BLOCK_DESC blockDesc = {};
blockDesc.Size = 1048576; // 1 MB

D3D12MA::VirtualBlock *block;
HRESULT hr = CreateVirtualBlock(&blockDesc, &block);
\endcode

\section virtual_allocator_making_virtual_allocations Making virtual allocations

D3D12MA::VirtualBlock object contains internal data structure that keeps track of free and occupied regions
using the same code as the main D3D12 memory allocator.
A single allocation is identified by a lightweight structure D3D12MA::VirtualAllocation.
You will also likely want to know the offset at which the allocation was made in the block.

In order to make an allocation:

-# Fill in D3D12MA::VIRTUAL_ALLOCATION_DESC structure.
-# Call D3D12MA::VirtualBlock::Allocate. Get new D3D12MA::VirtualAllocation value that identifies the allocation.

Example:

\code
D3D12MA::VIRTUAL_ALLOCATION_DESC allocDesc = {};
allocDesc.Size = 4096; // 4 KB

D3D12MA::VirtualAllocation alloc;
UINT64 allocOffset;
hr = block->Allocate(&allocDesc, &alloc, &allocOffset);
if(SUCCEEDED(hr))
{
    // Use the 4 KB of your memory starting at allocOffset.
}
else
{
    // Allocation failed - no space for it could be found. Handle this error!
}
\endcode

\section virtual_allocator_deallocation Deallocation

When no longer needed, an allocation can be freed by calling D3D12MA::VirtualBlock::FreeAllocation.

When whole block is no longer needed, the block object can be released by calling `block->Release()`.
All allocations must be freed before the block is destroyed, which is checked internally by an assert.
However, if you don't want to call `block->FreeAllocation` for each allocation, you can use D3D12MA::VirtualBlock::Clear to free them all at once -
a feature not available in normal D3D12 memory allocator.

Example:

\code
block->FreeAllocation(alloc);
block->Release();
\endcode

\section virtual_allocator_allocation_parameters Allocation parameters

You can attach a custom pointer to each allocation by using D3D12MA::VirtualBlock::SetAllocationPrivateData.
Its default value is `NULL`.
It can be used to store any data that needs to be associated with that allocation - e.g. an index, a handle, or a pointer to some
larger data structure containing more information. Example:

\code
struct CustomAllocData
{
    std::string m_AllocName;
};
CustomAllocData* allocData = new CustomAllocData();
allocData->m_AllocName = "My allocation 1";
block->SetAllocationPrivateData(alloc, allocData);
\endcode

The pointer can later be fetched, along with allocation offset and size, by passing the allocation handle to function
D3D12MA::VirtualBlock::GetAllocationInfo and inspecting returned structure D3D12MA::VIRTUAL_ALLOCATION_INFO.
If you allocated a new object to be used as the custom pointer, don't forget to delete that object before freeing the allocation!
Example:

\code
VIRTUAL_ALLOCATION_INFO allocInfo;
block->GetAllocationInfo(alloc, &allocInfo);
delete (CustomAllocData*)allocInfo.pPrivateData;

block->FreeAllocation(alloc);
\endcode

\section virtual_allocator_alignment_and_units Alignment and units

It feels natural to express sizes and offsets in bytes.
If an offset of an allocation needs to be aligned to a multiply of some number (e.g. 4 bytes), you can fill optional member
D3D12MA::VIRTUAL_ALLOCATION_DESC::Alignment to request it. Example:

\code
D3D12MA::VIRTUAL_ALLOCATION_DESC allocDesc = {};
allocDesc.Size = 4096; // 4 KB
allocDesc.Alignment = 4; // Returned offset must be a multiply of 4 B

D3D12MA::VirtualAllocation alloc;
UINT64 allocOffset;
hr = block->Allocate(&allocDesc, &alloc, &allocOffset);
\endcode

Alignments of different allocations made from one block may vary.
However, if all alignments and sizes are always multiply of some size e.g. 4 B or `sizeof(MyDataStruct)`,
you can express all sizes, alignments, and offsets in multiples of that size instead of individual bytes.
It might be more convenient, but you need to make sure to use this new unit consistently in all the places:

- D3D12MA::VIRTUAL_BLOCK_DESC::Size
- D3D12MA::VIRTUAL_ALLOCATION_DESC::Size and D3D12MA::VIRTUAL_ALLOCATION_DESC::Alignment
- Using offset returned by D3D12MA::VirtualBlock::Allocate and D3D12MA::VIRTUAL_ALLOCATION_INFO::Offset

\section virtual_allocator_statistics Statistics

You can obtain brief statistics of a virtual block using D3D12MA::VirtualBlock::GetStatistics().
The function fills structure D3D12MA::Statistics - same as used by the normal D3D12 memory allocator.
Example:

\code
D3D12MA::Statistics stats;
block->GetStatistics(&stats);
printf("My virtual block has %llu bytes used by %u virtual allocations\n",
    stats.AllocationBytes, stats.AllocationCount);
\endcode

More detailed statistics can be obtained using function D3D12MA::VirtualBlock::CalculateStatistics(),
but they are slower to calculate.

You can also request a full list of allocations and free regions as a string in JSON format by calling
D3D12MA::VirtualBlock::BuildStatsString.
Returned string must be later freed using D3D12MA::VirtualBlock::FreeStatsString.
The format of this string may differ from the one returned by the main D3D12 allocator, but it is similar.

\section virtual_allocator_additional_considerations Additional considerations

Alternative, linear algorithm can be used with virtual allocator - see flag
D3D12MA::VIRTUAL_BLOCK_FLAG_ALGORITHM_LINEAR and documentation: \ref linear_algorithm.

Note that the "virtual allocator" functionality is implemented on a level of individual memory blocks.
Keeping track of a whole collection of blocks, allocating new ones when out of free space,
deleting empty ones, and deciding which one to try first for a new allocation must be implemented by the user.


\page configuration Configuration

Please check file `D3D12MemAlloc.cpp` lines between "Configuration Begin" and
"Configuration End" to find macros that you can define to change the behavior of
the library, primarily for debugging purposes.

\section custom_memory_allocator Custom CPU memory allocator

If you use custom allocator for CPU memory rather than default C++ operator `new`
and `delete` or `malloc` and `free` functions, you can make this library using
your allocator as well by filling structure D3D12MA::ALLOCATION_CALLBACKS and
passing it as optional member D3D12MA::ALLOCATOR_DESC::pAllocationCallbacks.
Functions pointed there will be used by the library to make any CPU-side
allocations. Example:

\code
#include <malloc.h>

void* CustomAllocate(size_t Size, size_t Alignment, void* pPrivateData)
{
    void* memory = _aligned_malloc(Size, Alignment);
    // Your extra bookkeeping here...
    return memory;
}

void CustomFree(void* pMemory, void* pPrivateData)
{
    // Your extra bookkeeping here...
    _aligned_free(pMemory);
}

(...)

D3D12MA::ALLOCATION_CALLBACKS allocationCallbacks = {};
allocationCallbacks.pAllocate = &CustomAllocate;
allocationCallbacks.pFree = &CustomFree;

D3D12MA::ALLOCATOR_DESC allocatorDesc = {};
allocatorDesc.pDevice = device;
allocatorDesc.pAdapter = adapter;
allocatorDesc.pAllocationCallbacks = &allocationCallbacks;

D3D12MA::Allocator* allocator;
HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &allocator);
\endcode


\section debug_margins Debug margins

By default, allocations are laid out in memory blocks next to each other if possible
(considering required alignment returned by `ID3D12Device::GetResourceAllocationInfo`).

![Allocations without margin](../gfx/Margins_1.png)

Define macro `D3D12MA_DEBUG_MARGIN` to some non-zero value (e.g. 16) inside "D3D12MemAlloc.cpp"
to enforce specified number of bytes as a margin after every allocation.

![Allocations with margin](../gfx/Margins_2.png)

If your bug goes away after enabling margins, it means it may be caused by memory
being overwritten outside of allocation boundaries. It is not 100% certain though.
Change in application behavior may also be caused by different order and distribution
of allocations across memory blocks after margins are applied.

Margins work with all memory heap types.

Margin is applied only to placed allocations made out of memory heaps and not to committed
allocations, which have their own, implicit memory heap of specific size.
It is thus not applied to allocations made using D3D12MA::ALLOCATION_FLAG_COMMITTED flag
or those automatically decided to put into committed allocations, e.g. due to its large size.

Margins appear in [JSON dump](@ref statistics_json_dump) as part of free space.

Note that enabling margins increases memory usage and fragmentation.

Margins do not apply to \ref virtual_allocator.


\page general_considerations General considerations

\section general_considerations_thread_safety Thread safety

- The library has no global state, so separate D3D12MA::Allocator objects can be used independently.
  In typical applications there should be no need to create multiple such objects though - one per `ID3D12Device` is enough.
- All calls to methods of D3D12MA::Allocator class are safe to be made from multiple
  threads simultaneously because they are synchronized internally when needed.
- When the allocator is created with D3D12MA::ALLOCATOR_FLAG_SINGLETHREADED,
  calls to methods of D3D12MA::Allocator class must be made from a single thread or synchronized by the user.
  Using this flag may improve performance.
- D3D12MA::VirtualBlock is not safe to be used from multiple threads simultaneously.

\section general_considerations_versioning_and_compatibility Versioning and compatibility

The library uses [**Semantic Versioning**](https://semver.org/),
which means version numbers follow convention: Major.Minor.Patch (e.g. 2.3.0), where:

- Incremented Patch version means a release is backward- and forward-compatible,
  introducing only some internal improvements, bug fixes, optimizations etc.
  or changes that are out of scope of the official API described in this documentation.
- Incremented Minor version means a release is backward-compatible,
  so existing code that uses the library should continue to work, while some new
  symbols could have been added: new structures, functions, new values in existing
  enums and bit flags, new structure members, but not new function parameters.
- Incrementing Major version means a release could break some backward compatibility.

All changes between official releases are documented in file "CHANGELOG.md".

\warning Backward compatiblity is considered on the level of C++ source code, not binary linkage.
Adding new members to existing structures is treated as backward compatible if initializing
the new members to binary zero results in the old behavior.
You should always fully initialize all library structures to zeros and not rely on their
exact binary size.

\section general_considerations_features_not_supported Features not supported

Features deliberately excluded from the scope of this library:

- **Descriptor allocation.** Although also called "heaps", objects that represent
  descriptors are separate part of the D3D12 API from buffers and textures.
  You can still use \ref virtual_allocator to manage descriptors and their ranges inside a descriptor heap.
- **Support for reserved (tiled) resources.** We don't recommend using them.
- Support for `ID3D12Device::Evict` and `MakeResident`. We don't recommend using them.
  You can call them on the D3D12 objects manually.
  Plese keep in mind, however, that eviction happens on the level of entire `ID3D12Heap` memory blocks
  and not individual buffers or textures which may be placed inside them.
- **Handling CPU memory allocation failures.** When dynamically creating small C++
  objects in CPU memory (not the GPU memory), allocation failures are not
  handled gracefully, because that would complicate code significantly and
  is usually not needed in desktop PC applications anyway.
  Success of an allocation is just checked with an assert.
- **Code free of any compiler warnings.**
  There are many preprocessor macros that make some variables unused, function parameters unreferenced,
  or conditional expressions constant in some configurations.
  The code of this library should not be bigger or more complicated just to silence these warnings.
  It is recommended to disable such warnings instead.
- This is a C++ library. **Bindings or ports to any other programming languages** are welcome as external projects but
  are not going to be included into this repository.
*/

#endif


#ifdef HEXO_GRAPHICS_RHI_VULKAN
    #ifdef HEXO_GRAPHICS_RHI_WIN32_VULKAN_GLFW
        #define VK_USE_PLATFORM_WIN32_KHR
    #endif
    #include <vulkan/vulkan.h>
#endif


#ifdef HEXO_GRAPHICS_WINDOW_GLFW
	#if defined(HEXO_PLATFORM_OS_WINDOWS_10) || defined(HEXO_PLATFORM_OS_WINDOWS_11)
		#define GLFW_EXPOSE_NATIVE_WIN32
	#endif
	#define GLFW_INCLUDE_NONE
/*************************************************************************
 * GLFW 3.3 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef _glfw3_h_
#define _glfw3_h_

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************
 * Doxygen documentation
 *************************************************************************/

/*! @file glfw3.h
 *  @brief The header of the GLFW 3 API.
 *
 *  This is the header file of the GLFW 3 API.  It defines all its types and
 *  declares all its functions.
 *
 *  For more information about how to use this file, see @ref build_include.
 */
/*! @defgroup context Context reference
 *  @brief Functions and types related to OpenGL and OpenGL ES contexts.
 *
 *  This is the reference documentation for OpenGL and OpenGL ES context related
 *  functions.  For more task-oriented information, see the @ref context_guide.
 */
/*! @defgroup vulkan Vulkan support reference
 *  @brief Functions and types related to Vulkan.
 *
 *  This is the reference documentation for Vulkan related functions and types.
 *  For more task-oriented information, see the @ref vulkan_guide.
 */
/*! @defgroup init Initialization, version and error reference
 *  @brief Functions and types related to initialization and error handling.
 *
 *  This is the reference documentation for initialization and termination of
 *  the library, version management and error handling.  For more task-oriented
 *  information, see the @ref intro_guide.
 */
/*! @defgroup input Input reference
 *  @brief Functions and types related to input handling.
 *
 *  This is the reference documentation for input related functions and types.
 *  For more task-oriented information, see the @ref input_guide.
 */
/*! @defgroup monitor Monitor reference
 *  @brief Functions and types related to monitors.
 *
 *  This is the reference documentation for monitor related functions and types.
 *  For more task-oriented information, see the @ref monitor_guide.
 */
/*! @defgroup window Window reference
 *  @brief Functions and types related to windows.
 *
 *  This is the reference documentation for window related functions and types,
 *  including creation, deletion and event polling.  For more task-oriented
 *  information, see the @ref window_guide.
 */


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* If we are we on Windows, we want a single define for it.
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif /* _WIN32 */

/* Include because most Windows GLU headers need wchar_t and
 * the macOS OpenGL header blocks the definition of ptrdiff_t by glext.h.
 * Include it unconditionally to avoid surprising side-effects.
 */
#include <stddef.h>

/* Include because it is needed by Vulkan and related functions.
 * Include it unconditionally to avoid surprising side-effects.
 */
#include <stdint.h>

#if defined(GLFW_INCLUDE_VULKAN)
  #include <vulkan/vulkan.h>
#endif /* Vulkan header */

/* The Vulkan header may have indirectly included windows.h (because of
 * VK_USE_PLATFORM_WIN32_KHR) so we offer our replacement symbols after it.
 */

/* It is customary to use APIENTRY for OpenGL function pointer declarations on
 * all platforms.  Additionally, the Windows OpenGL header needs APIENTRY.
 */
#if !defined(APIENTRY)
 #if defined(_WIN32)
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
 #define GLFW_APIENTRY_DEFINED
#endif /* APIENTRY */

/* Some Windows OpenGL headers need this.
 */
#if !defined(WINGDIAPI) && defined(_WIN32)
 #define WINGDIAPI __declspec(dllimport)
 #define GLFW_WINGDIAPI_DEFINED
#endif /* WINGDIAPI */

/* Some Windows GLU headers need this.
 */
#if !defined(CALLBACK) && defined(_WIN32)
 #define CALLBACK __stdcall
 #define GLFW_CALLBACK_DEFINED
#endif /* CALLBACK */

/* Include the chosen OpenGL or OpenGL ES headers.
 */
#if defined(GLFW_INCLUDE_ES1)

 #include <GLES/gl.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES/glext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES2)

 #include <GLES2/gl2.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES3)

 #include <GLES3/gl3.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES31)

 #include <GLES3/gl31.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES32)

 #include <GLES3/gl32.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_GLCOREARB)

 #if defined(__APPLE__)

  #include <OpenGL/gl3.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <OpenGL/gl3ext.h>
  #endif /*GLFW_INCLUDE_GLEXT*/

 #else /*__APPLE__*/

  #include <GL/glcorearb.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <GL/glext.h>
  #endif

 #endif /*__APPLE__*/

#elif defined(GLFW_INCLUDE_GLU)

 #if defined(__APPLE__)

  #if defined(GLFW_INCLUDE_GLU)
   #include <OpenGL/glu.h>
  #endif

 #else /*__APPLE__*/

  #if defined(GLFW_INCLUDE_GLU)
   #include <GL/glu.h>
  #endif

 #endif /*__APPLE__*/

#elif !defined(GLFW_INCLUDE_NONE) && \
      !defined(__gl_h_) && \
      !defined(__gles1_gl_h_) && \
      !defined(__gles2_gl2_h_) && \
      !defined(__gles2_gl3_h_) && \
      !defined(__gles2_gl31_h_) && \
      !defined(__gles2_gl32_h_) && \
      !defined(__gl_glcorearb_h_) && \
      !defined(__gl2_h_) /*legacy*/ && \
      !defined(__gl3_h_) /*legacy*/ && \
      !defined(__gl31_h_) /*legacy*/ && \
      !defined(__gl32_h_) /*legacy*/ && \
      !defined(__glcorearb_h_) /*legacy*/ && \
      !defined(__GL_H__) /*non-standard*/ && \
      !defined(__gltypes_h_) /*non-standard*/ && \
      !defined(__glee_h_) /*non-standard*/

 #if defined(__APPLE__)

  #if !defined(GLFW_INCLUDE_GLEXT)
   #define GL_GLEXT_LEGACY
  #endif
  #include <OpenGL/gl.h>

 #else /*__APPLE__*/

  #include <GL/gl.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <GL/glext.h>
  #endif

 #endif /*__APPLE__*/

#endif /* OpenGL and OpenGL ES headers */

#if defined(GLFW_DLL) && defined(_GLFW_BUILD_DLL)
 /* GLFW_DLL must be defined by applications that are linking against the DLL
  * version of the GLFW library.  _GLFW_BUILD_DLL is defined by the GLFW
  * configuration header when compiling the DLL version of the library.
  */
 #error "You must not have both GLFW_DLL and _GLFW_BUILD_DLL defined"
#endif

/* GLFWAPI is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(_GLFW_BUILD_DLL)
 /* We are building GLFW as a Win32 DLL */
 #define GLFWAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(GLFW_DLL)
 /* We are calling GLFW as a Win32 DLL */
 #define GLFWAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(_GLFW_BUILD_DLL)
 /* We are building GLFW as a shared / dynamic library */
 #define GLFWAPI __attribute__((visibility("default")))
#else
 /* We are building or calling GLFW as a static library */
 #define GLFWAPI
#endif


/*************************************************************************
 * GLFW API tokens
 *************************************************************************/

/*! @name GLFW version macros
 *  @{ */
/*! @brief The major version number of the GLFW header.
 *
 *  The major version number of the GLFW header.  This is incremented when the
 *  API is changed in non-compatible ways.
 *  @ingroup init
 */
#define GLFW_VERSION_MAJOR          3
/*! @brief The minor version number of the GLFW header.
 *
 *  The minor version number of the GLFW header.  This is incremented when
 *  features are added to the API but it remains backward-compatible.
 *  @ingroup init
 */
#define GLFW_VERSION_MINOR          3
/*! @brief The revision number of the GLFW header.
 *
 *  The revision number of the GLFW header.  This is incremented when a bug fix
 *  release is made that does not contain any API changes.
 *  @ingroup init
 */
#define GLFW_VERSION_REVISION       5
/*! @} */

/*! @brief One.
 *
 *  This is only semantic sugar for the number 1.  You can instead use `1` or
 *  `true` or `_True` or `GL_TRUE` or `VK_TRUE` or anything else that is equal
 *  to one.
 *
 *  @ingroup init
 */
#define GLFW_TRUE                   1
/*! @brief Zero.
 *
 *  This is only semantic sugar for the number 0.  You can instead use `0` or
 *  `false` or `_False` or `GL_FALSE` or `VK_FALSE` or anything else that is
 *  equal to zero.
 *
 *  @ingroup init
 */
#define GLFW_FALSE                  0

/*! @name Key and button actions
 *  @{ */
/*! @brief The key or mouse button was released.
 *
 *  The key or mouse button was released.
 *
 *  @ingroup input
 */
#define GLFW_RELEASE                0
/*! @brief The key or mouse button was pressed.
 *
 *  The key or mouse button was pressed.
 *
 *  @ingroup input
 */
#define GLFW_PRESS                  1
/*! @brief The key was held down until it repeated.
 *
 *  The key was held down until it repeated.
 *
 *  @ingroup input
 */
#define GLFW_REPEAT                 2
/*! @} */

/*! @defgroup hat_state Joystick hat states
 *  @brief Joystick hat states.
 *
 *  See [joystick hat input](@ref joystick_hat) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_HAT_CENTERED           0
#define GLFW_HAT_UP                 1
#define GLFW_HAT_RIGHT              2
#define GLFW_HAT_DOWN               4
#define GLFW_HAT_LEFT               8
#define GLFW_HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define GLFW_HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define GLFW_HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define GLFW_HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)
/*! @} */

/*! @defgroup keys Keyboard keys
 *  @brief Keyboard key IDs.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
 *  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 *  put in the 256+ range).
 *
 *  The naming of the key codes follow these rules:
 *   - The US keyboard layout is used
 *   - Names of printable alpha-numeric characters are used (e.g. "A", "R",
 *     "3", etc.)
 *   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *     correspond to the Unicode standard (usually for brevity)
 *   - Keys that lack a clear US mapping are named "WORLD_x"
 *   - For non-printable keys, custom names are used (e.g. "F4",
 *     "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define GLFW_MOD_SHIFT           0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 *
 *  If this bit is set one or more Control keys were held down.
 */
#define GLFW_MOD_CONTROL         0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 *
 *  If this bit is set one or more Alt keys were held down.
 */
#define GLFW_MOD_ALT             0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 *
 *  If this bit is set one or more Super keys were held down.
 */
#define GLFW_MOD_SUPER           0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
 *
 *  If this bit is set the Caps Lock key is enabled and the @ref
 *  GLFW_LOCK_KEY_MODS input mode is set.
 */
#define GLFW_MOD_CAPS_LOCK       0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
 *
 *  If this bit is set the Num Lock key is enabled and the @ref
 *  GLFW_LOCK_KEY_MODS input mode is set.
 */
#define GLFW_MOD_NUM_LOCK        0x0020

/*! @} */

/*! @defgroup buttons Mouse buttons
 *  @brief Mouse button IDs.
 *
 *  See [mouse button input](@ref input_mouse_button) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
 *  @brief Joystick IDs.
 *
 *  See [joystick input](@ref joystick) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16
/*! @} */

/*! @defgroup gamepad_buttons Gamepad buttons
 *  @brief Gamepad buttons.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y
/*! @} */

/*! @defgroup gamepad_axes Gamepad axes
 *  @brief Gamepad axes.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
/*! @} */

/*! @defgroup errors Error codes
 *  @brief Error codes.
 *
 *  See [error handling](@ref error_handling) for how these are used.
 *
 *  @ingroup init
 *  @{ */
/*! @brief No error has occurred.
 *
 *  No error has occurred.
 *
 *  @analysis Yay.
 */
#define GLFW_NO_ERROR               0
/*! @brief GLFW has not been initialized.
 *
 *  This occurs if a GLFW function was called that must not be called unless the
 *  library is [initialized](@ref intro_init).
 *
 *  @analysis Application programmer error.  Initialize GLFW before calling any
 *  function that requires initialization.
 */
#define GLFW_NOT_INITIALIZED        0x00010001
/*! @brief No context is current for this thread.
 *
 *  This occurs if a GLFW function was called that needs and operates on the
 *  current OpenGL or OpenGL ES context but no context is current on the calling
 *  thread.  One such function is @ref glfwSwapInterval.
 *
 *  @analysis Application programmer error.  Ensure a context is current before
 *  calling functions that require a current context.
 */
#define GLFW_NO_CURRENT_CONTEXT     0x00010002
/*! @brief One of the arguments to the function was an invalid enum value.
 *
 *  One of the arguments to the function was an invalid enum value, for example
 *  requesting @ref GLFW_RED_BITS with @ref glfwGetWindowAttrib.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_INVALID_ENUM           0x00010003
/*! @brief One of the arguments to the function was an invalid value.
 *
 *  One of the arguments to the function was an invalid value, for example
 *  requesting a non-existent OpenGL or OpenGL ES version like 2.7.
 *
 *  Requesting a valid but unavailable OpenGL or OpenGL ES version will instead
 *  result in a @ref GLFW_VERSION_UNAVAILABLE error.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_INVALID_VALUE          0x00010004
/*! @brief A memory allocation failed.
 *
 *  A memory allocation failed.
 *
 *  @analysis A bug in GLFW or the underlying operating system.  Report the bug
 *  to our [issue tracker](https://github.com/glfw/glfw/issues).
 */
#define GLFW_OUT_OF_MEMORY          0x00010005
/*! @brief GLFW could not find support for the requested API on the system.
 *
 *  GLFW could not find support for the requested API on the system.
 *
 *  @analysis The installed graphics driver does not support the requested
 *  API, or does not support it via the chosen context creation backend.
 *  Below are a few examples.
 *
 *  @par
 *  Some pre-installed Windows graphics drivers do not support OpenGL.  AMD only
 *  supports OpenGL ES via EGL, while Nvidia and Intel only support it via
 *  a WGL or GLX extension.  macOS does not provide OpenGL ES at all.  The Mesa
 *  EGL, OpenGL and OpenGL ES libraries do not interface with the Nvidia binary
 *  driver.  Older graphics drivers do not support Vulkan.
 */
#define GLFW_API_UNAVAILABLE        0x00010006
/*! @brief The requested OpenGL or OpenGL ES version is not available.
 *
 *  The requested OpenGL or OpenGL ES version (including any requested context
 *  or framebuffer hints) is not available on this machine.
 *
 *  @analysis The machine does not support your requirements.  If your
 *  application is sufficiently flexible, downgrade your requirements and try
 *  again.  Otherwise, inform the user that their machine does not match your
 *  requirements.
 *
 *  @par
 *  Future invalid OpenGL and OpenGL ES versions, for example OpenGL 4.8 if 5.0
 *  comes out before the 4.x series gets that far, also fail with this error and
 *  not @ref GLFW_INVALID_VALUE, because GLFW cannot know what future versions
 *  will exist.
 */
#define GLFW_VERSION_UNAVAILABLE    0x00010007
/*! @brief A platform-specific error occurred that does not match any of the
 *  more specific categories.
 *
 *  A platform-specific error occurred that does not match any of the more
 *  specific categories.
 *
 *  @analysis A bug or configuration error in GLFW, the underlying operating
 *  system or its drivers, or a lack of required resources.  Report the issue to
 *  our [issue tracker](https://github.com/glfw/glfw/issues).
 */
#define GLFW_PLATFORM_ERROR         0x00010008
/*! @brief The requested format is not supported or available.
 *
 *  If emitted during window creation, the requested pixel format is not
 *  supported.
 *
 *  If emitted when querying the clipboard, the contents of the clipboard could
 *  not be converted to the requested format.
 *
 *  @analysis If emitted during window creation, one or more
 *  [hard constraints](@ref window_hints_hard) did not match any of the
 *  available pixel formats.  If your application is sufficiently flexible,
 *  downgrade your requirements and try again.  Otherwise, inform the user that
 *  their machine does not match your requirements.
 *
 *  @par
 *  If emitted when querying the clipboard, ignore the error or report it to
 *  the user, as appropriate.
 */
#define GLFW_FORMAT_UNAVAILABLE     0x00010009
/*! @brief The specified window does not have an OpenGL or OpenGL ES context.
 *
 *  A window that does not have an OpenGL or OpenGL ES context was passed to
 *  a function that requires it to have one.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_NO_WINDOW_CONTEXT      0x0001000A
/*! @} */

/*! @addtogroup window
 *  @{ */
/*! @brief Input focus window hint and attribute
 *
 *  Input focus [window hint](@ref GLFW_FOCUSED_hint) or
 *  [window attribute](@ref GLFW_FOCUSED_attrib).
 */
#define GLFW_FOCUSED                0x00020001
/*! @brief Window iconification window attribute
 *
 *  Window iconification [window attribute](@ref GLFW_ICONIFIED_attrib).
 */
#define GLFW_ICONIFIED              0x00020002
/*! @brief Window resize-ability window hint and attribute
 *
 *  Window resize-ability [window hint](@ref GLFW_RESIZABLE_hint) and
 *  [window attribute](@ref GLFW_RESIZABLE_attrib).
 */
#define GLFW_RESIZABLE              0x00020003
/*! @brief Window visibility window hint and attribute
 *
 *  Window visibility [window hint](@ref GLFW_VISIBLE_hint) and
 *  [window attribute](@ref GLFW_VISIBLE_attrib).
 */
#define GLFW_VISIBLE                0x00020004
/*! @brief Window decoration window hint and attribute
 *
 *  Window decoration [window hint](@ref GLFW_DECORATED_hint) and
 *  [window attribute](@ref GLFW_DECORATED_attrib).
 */
#define GLFW_DECORATED              0x00020005
/*! @brief Window auto-iconification window hint and attribute
 *
 *  Window auto-iconification [window hint](@ref GLFW_AUTO_ICONIFY_hint) and
 *  [window attribute](@ref GLFW_AUTO_ICONIFY_attrib).
 */
#define GLFW_AUTO_ICONIFY           0x00020006
/*! @brief Window decoration window hint and attribute
 *
 *  Window decoration [window hint](@ref GLFW_FLOATING_hint) and
 *  [window attribute](@ref GLFW_FLOATING_attrib).
 */
#define GLFW_FLOATING               0x00020007
/*! @brief Window maximization window hint and attribute
 *
 *  Window maximization [window hint](@ref GLFW_MAXIMIZED_hint) and
 *  [window attribute](@ref GLFW_MAXIMIZED_attrib).
 */
#define GLFW_MAXIMIZED              0x00020008
/*! @brief Cursor centering window hint
 *
 *  Cursor centering [window hint](@ref GLFW_CENTER_CURSOR_hint).
 */
#define GLFW_CENTER_CURSOR          0x00020009
/*! @brief Window framebuffer transparency hint and attribute
 *
 *  Window framebuffer transparency
 *  [window hint](@ref GLFW_TRANSPARENT_FRAMEBUFFER_hint) and
 *  [window attribute](@ref GLFW_TRANSPARENT_FRAMEBUFFER_attrib).
 */
#define GLFW_TRANSPARENT_FRAMEBUFFER 0x0002000A
/*! @brief Mouse cursor hover window attribute.
 *
 *  Mouse cursor hover [window attribute](@ref GLFW_HOVERED_attrib).
 */
#define GLFW_HOVERED                0x0002000B
/*! @brief Input focus on calling show window hint and attribute
 *
 *  Input focus [window hint](@ref GLFW_FOCUS_ON_SHOW_hint) or
 *  [window attribute](@ref GLFW_FOCUS_ON_SHOW_attrib).
 */
#define GLFW_FOCUS_ON_SHOW          0x0002000C

/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_RED_BITS).
 */
#define GLFW_RED_BITS               0x00021001
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_GREEN_BITS).
 */
#define GLFW_GREEN_BITS             0x00021002
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_BLUE_BITS).
 */
#define GLFW_BLUE_BITS              0x00021003
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ALPHA_BITS).
 */
#define GLFW_ALPHA_BITS             0x00021004
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_DEPTH_BITS).
 */
#define GLFW_DEPTH_BITS             0x00021005
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_STENCIL_BITS).
 */
#define GLFW_STENCIL_BITS           0x00021006
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_RED_BITS).
 */
#define GLFW_ACCUM_RED_BITS         0x00021007
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_GREEN_BITS).
 */
#define GLFW_ACCUM_GREEN_BITS       0x00021008
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_BLUE_BITS).
 */
#define GLFW_ACCUM_BLUE_BITS        0x00021009
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_ALPHA_BITS).
 */
#define GLFW_ACCUM_ALPHA_BITS       0x0002100A
/*! @brief Framebuffer auxiliary buffer hint.
 *
 *  Framebuffer auxiliary buffer [hint](@ref GLFW_AUX_BUFFERS).
 */
#define GLFW_AUX_BUFFERS            0x0002100B
/*! @brief OpenGL stereoscopic rendering hint.
 *
 *  OpenGL stereoscopic rendering [hint](@ref GLFW_STEREO).
 */
#define GLFW_STEREO                 0x0002100C
/*! @brief Framebuffer MSAA samples hint.
 *
 *  Framebuffer MSAA samples [hint](@ref GLFW_SAMPLES).
 */
#define GLFW_SAMPLES                0x0002100D
/*! @brief Framebuffer sRGB hint.
 *
 *  Framebuffer sRGB [hint](@ref GLFW_SRGB_CAPABLE).
 */
#define GLFW_SRGB_CAPABLE           0x0002100E
/*! @brief Monitor refresh rate hint.
 *
 *  Monitor refresh rate [hint](@ref GLFW_REFRESH_RATE).
 */
#define GLFW_REFRESH_RATE           0x0002100F
/*! @brief Framebuffer double buffering hint.
 *
 *  Framebuffer double buffering [hint](@ref GLFW_DOUBLEBUFFER).
 */
#define GLFW_DOUBLEBUFFER           0x00021010

/*! @brief Context client API hint and attribute.
 *
 *  Context client API [hint](@ref GLFW_CLIENT_API_hint) and
 *  [attribute](@ref GLFW_CLIENT_API_attrib).
 */
#define GLFW_CLIENT_API             0x00022001
/*! @brief Context client API major version hint and attribute.
 *
 *  Context client API major version [hint](@ref GLFW_CONTEXT_VERSION_MAJOR_hint)
 *  and [attribute](@ref GLFW_CONTEXT_VERSION_MAJOR_attrib).
 */
#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
/*! @brief Context client API minor version hint and attribute.
 *
 *  Context client API minor version [hint](@ref GLFW_CONTEXT_VERSION_MINOR_hint)
 *  and [attribute](@ref GLFW_CONTEXT_VERSION_MINOR_attrib).
 */
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
/*! @brief Context client API revision number hint and attribute.
 *
 *  Context client API revision number
 *  [attribute](@ref GLFW_CONTEXT_REVISION_attrib).
 */
#define GLFW_CONTEXT_REVISION       0x00022004
/*! @brief Context robustness hint and attribute.
 *
 *  Context client API revision number [hint](@ref GLFW_CONTEXT_ROBUSTNESS_hint)
 *  and [attribute](@ref GLFW_CONTEXT_ROBUSTNESS_attrib).
 */
#define GLFW_CONTEXT_ROBUSTNESS     0x00022005
/*! @brief OpenGL forward-compatibility hint and attribute.
 *
 *  OpenGL forward-compatibility [hint](@ref GLFW_OPENGL_FORWARD_COMPAT_hint)
 *  and [attribute](@ref GLFW_OPENGL_FORWARD_COMPAT_attrib).
 */
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
/*! @brief Debug mode context hint and attribute.
 *
 *  Debug mode context [hint](@ref GLFW_OPENGL_DEBUG_CONTEXT_hint) and
 *  [attribute](@ref GLFW_OPENGL_DEBUG_CONTEXT_attrib).
 */
#define GLFW_OPENGL_DEBUG_CONTEXT   0x00022007
/*! @brief OpenGL profile hint and attribute.
 *
 *  OpenGL profile [hint](@ref GLFW_OPENGL_PROFILE_hint) and
 *  [attribute](@ref GLFW_OPENGL_PROFILE_attrib).
 */
#define GLFW_OPENGL_PROFILE         0x00022008
/*! @brief Context flush-on-release hint and attribute.
 *
 *  Context flush-on-release [hint](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_hint) and
 *  [attribute](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_attrib).
 */
#define GLFW_CONTEXT_RELEASE_BEHAVIOR 0x00022009
/*! @brief Context error suppression hint and attribute.
 *
 *  Context error suppression [hint](@ref GLFW_CONTEXT_NO_ERROR_hint) and
 *  [attribute](@ref GLFW_CONTEXT_NO_ERROR_attrib).
 */
#define GLFW_CONTEXT_NO_ERROR       0x0002200A
/*! @brief Context creation API hint and attribute.
 *
 *  Context creation API [hint](@ref GLFW_CONTEXT_CREATION_API_hint) and
 *  [attribute](@ref GLFW_CONTEXT_CREATION_API_attrib).
 */
#define GLFW_CONTEXT_CREATION_API   0x0002200B
/*! @brief Window content area scaling window
 *  [window hint](@ref GLFW_SCALE_TO_MONITOR).
 */
#define GLFW_SCALE_TO_MONITOR       0x0002200C
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_RETINA_FRAMEBUFFER_hint).
 */
#define GLFW_COCOA_RETINA_FRAMEBUFFER 0x00023001
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_FRAME_NAME_hint).
 */
#define GLFW_COCOA_FRAME_NAME         0x00023002
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_GRAPHICS_SWITCHING_hint).
 */
#define GLFW_COCOA_GRAPHICS_SWITCHING 0x00023003
/*! @brief X11 specific
 *  [window hint](@ref GLFW_X11_CLASS_NAME_hint).
 */
#define GLFW_X11_CLASS_NAME         0x00024001
/*! @brief X11 specific
 *  [window hint](@ref GLFW_X11_CLASS_NAME_hint).
 */
#define GLFW_X11_INSTANCE_NAME      0x00024002
/*! @} */

#define GLFW_NO_API                          0
#define GLFW_OPENGL_API             0x00030001
#define GLFW_OPENGL_ES_API          0x00030002

#define GLFW_NO_ROBUSTNESS                   0
#define GLFW_NO_RESET_NOTIFICATION  0x00031001
#define GLFW_LOSE_CONTEXT_ON_RESET  0x00031002

#define GLFW_OPENGL_ANY_PROFILE              0
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_OPENGL_COMPAT_PROFILE  0x00032002

#define GLFW_CURSOR                 0x00033001
#define GLFW_STICKY_KEYS            0x00033002
#define GLFW_STICKY_MOUSE_BUTTONS   0x00033003
#define GLFW_LOCK_KEY_MODS          0x00033004
#define GLFW_RAW_MOUSE_MOTION       0x00033005

#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
#define GLFW_CURSOR_DISABLED        0x00034003

#define GLFW_ANY_RELEASE_BEHAVIOR            0
#define GLFW_RELEASE_BEHAVIOR_FLUSH 0x00035001
#define GLFW_RELEASE_BEHAVIOR_NONE  0x00035002

#define GLFW_NATIVE_CONTEXT_API     0x00036001
#define GLFW_EGL_CONTEXT_API        0x00036002
#define GLFW_OSMESA_CONTEXT_API     0x00036003

/*! @defgroup shapes Standard cursor shapes
 *  @brief Standard system cursor shapes.
 *
 *  See [standard cursor creation](@ref cursor_standard) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief The regular arrow cursor shape.
 *
 *  The regular arrow cursor.
 */
#define GLFW_ARROW_CURSOR           0x00036001
/*! @brief The text input I-beam cursor shape.
 *
 *  The text input I-beam cursor shape.
 */
#define GLFW_IBEAM_CURSOR           0x00036002
/*! @brief The crosshair shape.
 *
 *  The crosshair shape.
 */
#define GLFW_CROSSHAIR_CURSOR       0x00036003
/*! @brief The hand shape.
 *
 *  The hand shape.
 */
#define GLFW_HAND_CURSOR            0x00036004
/*! @brief The horizontal resize arrow shape.
 *
 *  The horizontal resize arrow shape.
 */
#define GLFW_HRESIZE_CURSOR         0x00036005
/*! @brief The vertical resize arrow shape.
 *
 *  The vertical resize arrow shape.
 */
#define GLFW_VRESIZE_CURSOR         0x00036006
/*! @} */

#define GLFW_CONNECTED              0x00040001
#define GLFW_DISCONNECTED           0x00040002

/*! @addtogroup init
 *  @{ */
/*! @brief Joystick hat buttons init hint.
 *
 *  Joystick hat buttons [init hint](@ref GLFW_JOYSTICK_HAT_BUTTONS).
 */
#define GLFW_JOYSTICK_HAT_BUTTONS   0x00050001
/*! @brief macOS specific init hint.
 *
 *  macOS specific [init hint](@ref GLFW_COCOA_CHDIR_RESOURCES_hint).
 */
#define GLFW_COCOA_CHDIR_RESOURCES  0x00051001
/*! @brief macOS specific init hint.
 *
 *  macOS specific [init hint](@ref GLFW_COCOA_MENUBAR_hint).
 */
#define GLFW_COCOA_MENUBAR          0x00051002
/*! @} */

#define GLFW_DONT_CARE              -1


/*************************************************************************
 * GLFW API types
 *************************************************************************/

/*! @brief Client API function pointer type.
 *
 *  Generic function pointer used for returning client API function pointers
 *  without forcing a cast from a regular pointer.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwGetProcAddress
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
typedef void (*GLFWglproc)(void);

/*! @brief Vulkan API function pointer type.
 *
 *  Generic function pointer used for returning Vulkan API function pointers
 *  without forcing a cast from a regular pointer.
 *
 *  @sa @ref vulkan_proc
 *  @sa @ref glfwGetInstanceProcAddress
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
typedef void (*GLFWvkproc)(void);

/*! @brief Opaque monitor object.
 *
 *  Opaque monitor object.
 *
 *  @see @ref monitor_object
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef struct GLFWmonitor GLFWmonitor;

/*! @brief Opaque window object.
 *
 *  Opaque window object.
 *
 *  @see @ref window_object
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef struct GLFWwindow GLFWwindow;

/*! @brief Opaque cursor object.
 *
 *  Opaque cursor object.
 *
 *  @see @ref cursor_object
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef struct GLFWcursor GLFWcursor;

/*! @brief The function pointer type for error callbacks.
 *
 *  This is the function pointer type for error callbacks.  An error callback
 *  function has the following signature:
 *  @code
 *  void callback_name(int error_code, const char* description)
 *  @endcode
 *
 *  @param[in] error_code An [error code](@ref errors).  Future releases may add
 *  more error codes.
 *  @param[in] description A UTF-8 encoded string describing the error.
 *
 *  @pointer_lifetime The error description string is valid until the callback
 *  function returns.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwSetErrorCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
typedef void (* GLFWerrorfun)(int error_code, const char* description);

/*! @brief The function pointer type for window position callbacks.
 *
 *  This is the function pointer type for window position callbacks.  A window
 *  position callback function has the following signature:
 *  @code
 *  void callback_name(GLFWwindow* window, int xpos, int ypos)
 *  @endcode
 *
 *  @param[in] window The window that was moved.
 *  @param[in] xpos The new x-coordinate, in screen coordinates, of the
 *  upper-left corner of the content area of the window.
 *  @param[in] ypos The new y-coordinate, in screen coordinates, of the
 *  upper-left corner of the content area of the window.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwSetWindowPosCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowposfun)(GLFWwindow* window, int xpos, int ypos);

/*! @brief The function pointer type for window size callbacks.
 *
 *  This is the function pointer type for window size callbacks.  A window size
 *  callback function has the following signature:
 *  @code
 *  void callback_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *
 *  @param[in] window The window that was resized.
 *  @param[in] width The new width, in screen coordinates, of the window.
 *  @param[in] height The new height, in screen coordinates, of the window.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwSetWindowSizeCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);

/*! @brief The function pointer type for window close callbacks.
 *
 *  This is the function pointer type for window close callbacks.  A window
 *  close callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window)
 *  @endcode
 *
 *  @param[in] window The window that the user attempted to close.
 *
 *  @sa @ref window_close
 *  @sa @ref glfwSetWindowCloseCallback
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowclosefun)(GLFWwindow* window);

/*! @brief The function pointer type for window content refresh callbacks.
 *
 *  This is the function pointer type for window content refresh callbacks.
 *  A window content refresh callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window);
 *  @endcode
 *
 *  @param[in] window The window whose content needs to be refreshed.
 *
 *  @sa @ref window_refresh
 *  @sa @ref glfwSetWindowRefreshCallback
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowrefreshfun)(GLFWwindow* window);

/*! @brief The function pointer type for window focus callbacks.
 *
 *  This is the function pointer type for window focus callbacks.  A window
 *  focus callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int focused)
 *  @endcode
 *
 *  @param[in] window The window that gained or lost input focus.
 *  @param[in] focused `GLFW_TRUE` if the window was given input focus, or
 *  `GLFW_FALSE` if it lost it.
 *
 *  @sa @ref window_focus
 *  @sa @ref glfwSetWindowFocusCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowfocusfun)(GLFWwindow* window, int focused);

/*! @brief The function pointer type for window iconify callbacks.
 *
 *  This is the function pointer type for window iconify callbacks.  A window
 *  iconify callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int iconified)
 *  @endcode
 *
 *  @param[in] window The window that was iconified or restored.
 *  @param[in] iconified `GLFW_TRUE` if the window was iconified, or
 *  `GLFW_FALSE` if it was restored.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwSetWindowIconifyCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowiconifyfun)(GLFWwindow* window, int iconified);

/*! @brief The function pointer type for window maximize callbacks.
 *
 *  This is the function pointer type for window maximize callbacks.  A window
 *  maximize callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int maximized)
 *  @endcode
 *
 *  @param[in] window The window that was maximized or restored.
 *  @param[in] maximized `GLFW_TRUE` if the window was maximized, or
 *  `GLFW_FALSE` if it was restored.
 *
 *  @sa @ref window_maximize
 *  @sa glfwSetWindowMaximizeCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowmaximizefun)(GLFWwindow* window, int maximized);

/*! @brief The function pointer type for framebuffer size callbacks.
 *
 *  This is the function pointer type for framebuffer size callbacks.
 *  A framebuffer size callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *
 *  @param[in] window The window whose framebuffer was resized.
 *  @param[in] width The new width, in pixels, of the framebuffer.
 *  @param[in] height The new height, in pixels, of the framebuffer.
 *
 *  @sa @ref window_fbsize
 *  @sa @ref glfwSetFramebufferSizeCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);

/*! @brief The function pointer type for window content scale callbacks.
 *
 *  This is the function pointer type for window content scale callbacks.
 *  A window content scale callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, float xscale, float yscale)
 *  @endcode
 *
 *  @param[in] window The window whose content scale changed.
 *  @param[in] xscale The new x-axis content scale of the window.
 *  @param[in] yscale The new y-axis content scale of the window.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwSetWindowContentScaleCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowcontentscalefun)(GLFWwindow* window, float xscale, float yscale);

/*! @brief The function pointer type for mouse button callbacks.
 *
 *  This is the function pointer type for mouse button callback functions.
 *  A mouse button callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int button, int action, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] button The [mouse button](@ref buttons) that was pressed or
 *  released.
 *  @param[in] action One of `GLFW_PRESS` or `GLFW_RELEASE`.  Future releases
 *  may add more actions.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_mouse_button
 *  @sa @ref glfwSetMouseButtonCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle and modifier mask parameters.
 *
 *  @ingroup input
 */
typedef void (* GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);

/*! @brief The function pointer type for cursor position callbacks.
 *
 *  This is the function pointer type for cursor position callbacks.  A cursor
 *  position callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, double xpos, double ypos);
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] xpos The new cursor x-coordinate, relative to the left edge of
 *  the content area.
 *  @param[in] ypos The new cursor y-coordinate, relative to the top edge of the
 *  content area.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwSetCursorPosCallback
 *
 *  @since Added in version 3.0.  Replaces `GLFWmouseposfun`.
 *
 *  @ingroup input
 */
typedef void (* GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);

/*! @brief The function pointer type for cursor enter/leave callbacks.
 *
 *  This is the function pointer type for cursor enter/leave callbacks.
 *  A cursor enter/leave callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int entered)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] entered `GLFW_TRUE` if the cursor entered the window's content
 *  area, or `GLFW_FALSE` if it left it.
 *
 *  @sa @ref cursor_enter
 *  @sa @ref glfwSetCursorEnterCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
typedef void (* GLFWcursorenterfun)(GLFWwindow* window, int entered);

/*! @brief The function pointer type for scroll callbacks.
 *
 *  This is the function pointer type for scroll callbacks.  A scroll callback
 *  function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, double xoffset, double yoffset)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] xoffset The scroll offset along the x-axis.
 *  @param[in] yoffset The scroll offset along the y-axis.
 *
 *  @sa @ref scrolling
 *  @sa @ref glfwSetScrollCallback
 *
 *  @since Added in version 3.0.  Replaces `GLFWmousewheelfun`.
 *
 *  @ingroup input
 */
typedef void (* GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);

/*! @brief The function pointer type for keyboard key callbacks.
 *
 *  This is the function pointer type for keyboard key callbacks.  A keyboard
 *  key callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
 *  @param[in] scancode The system-specific scancode of the key.
 *  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.  Future
 *  releases may add more actions.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_key
 *  @sa @ref glfwSetKeyCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle, scancode and modifier mask parameters.
 *
 *  @ingroup input
 */
typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);

/*! @brief The function pointer type for Unicode character callbacks.
 *
 *  This is the function pointer type for Unicode character callbacks.
 *  A Unicode character callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] codepoint The Unicode code point of the character.
 *
 *  @sa @ref input_char
 *  @sa @ref glfwSetCharCallback
 *
 *  @since Added in version 2.4.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
typedef void (* GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);

/*! @brief The function pointer type for Unicode character with modifiers
 *  callbacks.
 *
 *  This is the function pointer type for Unicode character with modifiers
 *  callbacks.  It is called for each input character, regardless of what
 *  modifier keys are held down.  A Unicode character with modifiers callback
 *  function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] codepoint The Unicode code point of the character.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_char
 *  @sa @ref glfwSetCharModsCallback
 *
 *  @deprecated Scheduled for removal in version 4.0.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef void (* GLFWcharmodsfun)(GLFWwindow* window, unsigned int codepoint, int mods);

/*! @brief The function pointer type for path drop callbacks.
 *
 *  This is the function pointer type for path drop callbacks.  A path drop
 *  callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int path_count, const char* paths[])
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] path_count The number of dropped paths.
 *  @param[in] paths The UTF-8 encoded file and/or directory path names.
 *
 *  @pointer_lifetime The path array and its strings are valid until the
 *  callback function returns.
 *
 *  @sa @ref path_drop
 *  @sa @ref glfwSetDropCallback
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef void (* GLFWdropfun)(GLFWwindow* window, int path_count, const char* paths[]);

/*! @brief The function pointer type for monitor configuration callbacks.
 *
 *  This is the function pointer type for monitor configuration callbacks.
 *  A monitor callback function has the following signature:
 *  @code
 *  void function_name(GLFWmonitor* monitor, int event)
 *  @endcode
 *
 *  @param[in] monitor The monitor that was connected or disconnected.
 *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.  Future
 *  releases may add more events.
 *
 *  @sa @ref monitor_event
 *  @sa @ref glfwSetMonitorCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef void (* GLFWmonitorfun)(GLFWmonitor* monitor, int event);

/*! @brief The function pointer type for joystick configuration callbacks.
 *
 *  This is the function pointer type for joystick configuration callbacks.
 *  A joystick configuration callback function has the following signature:
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *
 *  @param[in] jid The joystick that was connected or disconnected.
 *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.  Future
 *  releases may add more events.
 *
 *  @sa @ref joystick_event
 *  @sa @ref glfwSetJoystickCallback
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
typedef void (* GLFWjoystickfun)(int jid, int event);

/*! @brief Video mode type.
 *
 *  This describes a single video mode.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoMode
 *  @sa @ref glfwGetVideoModes
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added refresh rate member.
 *
 *  @ingroup monitor
 */
typedef struct GLFWvidmode
{
    /*! The width, in screen coordinates, of the video mode.
     */
    int width;
    /*! The height, in screen coordinates, of the video mode.
     */
    int height;
    /*! The bit depth of the red channel of the video mode.
     */
    int redBits;
    /*! The bit depth of the green channel of the video mode.
     */
    int greenBits;
    /*! The bit depth of the blue channel of the video mode.
     */
    int blueBits;
    /*! The refresh rate, in Hz, of the video mode.
     */
    int refreshRate;
} GLFWvidmode;

/*! @brief Gamma ramp.
 *
 *  This describes the gamma ramp for a monitor.
 *
 *  @sa @ref monitor_gamma
 *  @sa @ref glfwGetGammaRamp
 *  @sa @ref glfwSetGammaRamp
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef struct GLFWgammaramp
{
    /*! An array of value describing the response of the red channel.
     */
    unsigned short* red;
    /*! An array of value describing the response of the green channel.
     */
    unsigned short* green;
    /*! An array of value describing the response of the blue channel.
     */
    unsigned short* blue;
    /*! The number of elements in each array.
     */
    unsigned int size;
} GLFWgammaramp;

/*! @brief Image data.
 *
 *  This describes a single 2D image.  See the documentation for each related
 *  function what the expected pixel format is.
 *
 *  @sa @ref cursor_custom
 *  @sa @ref window_icon
 *
 *  @since Added in version 2.1.
 *  @glfw3 Removed format and bytes-per-pixel members.
 *
 *  @ingroup window
 */
typedef struct GLFWimage
{
    /*! The width, in pixels, of this image.
     */
    int width;
    /*! The height, in pixels, of this image.
     */
    int height;
    /*! The pixel data of this image, arranged left-to-right, top-to-bottom.
     */
    unsigned char* pixels;
} GLFWimage;

/*! @brief Gamepad input state
 *
 *  This describes the input state of a gamepad.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
typedef struct GLFWgamepadstate
{
    /*! The states of each [gamepad button](@ref gamepad_buttons), `GLFW_PRESS`
     *  or `GLFW_RELEASE`.
     */
    unsigned char buttons[15];
    /*! The states of each [gamepad axis](@ref gamepad_axes), in the range -1.0
     *  to 1.0 inclusive.
     */
    float axes[6];
} GLFWgamepadstate;


/*************************************************************************
 * GLFW API functions
 *************************************************************************/

/*! @brief Initializes the GLFW library.
 *
 *  This function initializes the GLFW library.  Before most GLFW functions can
 *  be used, GLFW must be initialized, and before an application terminates GLFW
 *  should be terminated in order to free any resources allocated during or
 *  after initialization.
 *
 *  If this function fails, it calls @ref glfwTerminate before returning.  If it
 *  succeeds, you should call @ref glfwTerminate before the application exits.
 *
 *  Additional calls to this function after successful initialization but before
 *  termination will return `GLFW_TRUE` immediately.
 *
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos This function will change the current directory of the
 *  application to the `Contents/Resources` subdirectory of the application's
 *  bundle, if present.  This can be disabled with the @ref
 *  GLFW_COCOA_CHDIR_RESOURCES init hint.
 *
 *  @remark @x11 This function will set the `LC_CTYPE` category of the
 *  application locale according to the current environment if that category is
 *  still "C".  This is because the "C" locale breaks Unicode text input.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref intro_init
 *  @sa @ref glfwTerminate
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI int glfwInit(void);

/*! @brief Terminates the GLFW library.
 *
 *  This function destroys all remaining windows and cursors, restores any
 *  modified gamma ramps and frees any other allocated resources.  Once this
 *  function is called, you must again call @ref glfwInit successfully before
 *  you will be able to use most GLFW functions.
 *
 *  If GLFW has been successfully initialized, this function should be called
 *  before the application exits.  If initialization fails, there is no need to
 *  call this function, as it is called by @ref glfwInit before it returns
 *  failure.
 *
 *  This function has no effect if GLFW is not initialized.
 *
 *  @errors Possible errors include @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @warning The contexts of any remaining windows must not be current on any
 *  other thread when this function is called.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref intro_init
 *  @sa @ref glfwInit
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI void glfwTerminate(void);

/*! @brief Sets the specified init hint to the desired value.
 *
 *  This function sets hints for the next initialization of GLFW.
 *
 *  The values you set hints to are never reset by GLFW, but they only take
 *  effect during initialization.  Once GLFW has been initialized, any values
 *  you set will be ignored until the library is terminated and initialized
 *  again.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [init hint](@ref init_hints) to set.
 *  @param[in] value The new value of the init hint.
 *
 *  @errors Possible errors include @ref GLFW_INVALID_ENUM and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @remarks This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa init_hints
 *  @sa glfwInit
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup init
 */
GLFWAPI void glfwInitHint(int hint, int value);

/*! @brief Retrieves the version of the GLFW library.
 *
 *  This function retrieves the major, minor and revision numbers of the GLFW
 *  library.  It is intended for when you are using GLFW as a shared library and
 *  want to ensure that you are using the minimum required version.
 *
 *  Any or all of the version arguments may be `NULL`.
 *
 *  @param[out] major Where to store the major version number, or `NULL`.
 *  @param[out] minor Where to store the minor version number, or `NULL`.
 *  @param[out] rev Where to store the revision number, or `NULL`.
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref intro_version
 *  @sa @ref glfwGetVersionString
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev);

/*! @brief Returns a string describing the compile-time configuration.
 *
 *  This function returns the compile-time generated
 *  [version string](@ref intro_version_string) of the GLFW library binary.  It
 *  describes the version, platform, compiler and any platform-specific
 *  compile-time options.  It should not be confused with the OpenGL or OpenGL
 *  ES version string, queried with `glGetString`.
 *
 *  __Do not use the version string__ to parse the GLFW library version.  The
 *  @ref glfwGetVersion function provides the version of the running library
 *  binary in numerical format.
 *
 *  @return The ASCII encoded GLFW version string.
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @pointer_lifetime The returned string is static and compile-time generated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref intro_version
 *  @sa @ref glfwGetVersion
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
GLFWAPI const char* glfwGetVersionString(void);

/*! @brief Returns and clears the last error for the calling thread.
 *
 *  This function returns and clears the [error code](@ref errors) of the last
 *  error that occurred on the calling thread, and optionally a UTF-8 encoded
 *  human-readable description of it.  If no error has occurred since the last
 *  call, it returns @ref GLFW_NO_ERROR (zero) and the description pointer is
 *  set to `NULL`.
 *
 *  @param[in] description Where to store the error description pointer, or `NULL`.
 *  @return The last error code for the calling thread, or @ref GLFW_NO_ERROR
 *  (zero).
 *
 *  @errors None.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  next error occurs or the library is terminated.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwSetErrorCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup init
 */
GLFWAPI int glfwGetError(const char** description);

/*! @brief Sets the error callback.
 *
 *  This function sets the error callback, which is called with an error code
 *  and a human-readable description each time a GLFW error occurs.
 *
 *  The error code is set before the callback is called.  Calling @ref
 *  glfwGetError from the error callback will return the same value as the error
 *  code argument.
 *
 *  The error callback is called on the thread where the error occurred.  If you
 *  are using GLFW from multiple threads, your error callback needs to be
 *  written accordingly.
 *
 *  Because the description string may have been generated specifically for that
 *  error, it is not guaranteed to be valid after the callback has returned.  If
 *  you wish to use it after the callback returns, you need to make a copy.
 *
 *  Once set, the error callback remains set even after the library has been
 *  terminated.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set.
 *
 *  @callback_signature
 *  @code
 *  void callback_name(int error_code, const char* description)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [callback pointer type](@ref GLFWerrorfun).
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwGetError
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun callback);

/*! @brief Returns the currently connected monitors.
 *
 *  This function returns an array of handles for all currently connected
 *  monitors.  The primary monitor is always first in the returned array.  If no
 *  monitors were found, this function returns `NULL`.
 *
 *  @param[out] count Where to store the number of monitors in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of monitor handles, or `NULL` if no monitors were found or
 *  if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  monitor configuration changes or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_monitors
 *  @sa @ref monitor_event
 *  @sa @ref glfwGetPrimaryMonitor
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitor** glfwGetMonitors(int* count);

/*! @brief Returns the primary monitor.
 *
 *  This function returns the primary monitor.  This is usually the monitor
 *  where elements like the task bar or global menu bar are located.
 *
 *  @return The primary monitor, or `NULL` if no monitors were found or if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @remark The primary monitor is always first in the array returned by @ref
 *  glfwGetMonitors.
 *
 *  @sa @ref monitor_monitors
 *  @sa @ref glfwGetMonitors
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitor* glfwGetPrimaryMonitor(void);

/*! @brief Returns the position of the monitor's viewport on the virtual screen.
 *
 *  This function returns the position, in screen coordinates, of the upper-left
 *  corner of the specified monitor.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xpos Where to store the monitor x-coordinate, or `NULL`.
 *  @param[out] ypos Where to store the monitor y-coordinate, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);

/*! @brief Retrieves the work area of the monitor.
 *
 *  This function returns the position, in screen coordinates, of the upper-left
 *  corner of the work area of the specified monitor along with the work area
 *  size in screen coordinates. The work area is defined as the area of the
 *  monitor not occluded by the operating system task bar where present. If no
 *  task bar exists then the work area is the monitor resolution in screen
 *  coordinates.
 *
 *  Any or all of the position and size arguments may be `NULL`.  If an error
 *  occurs, all non-`NULL` position and size arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xpos Where to store the monitor x-coordinate, or `NULL`.
 *  @param[out] ypos Where to store the monitor y-coordinate, or `NULL`.
 *  @param[out] width Where to store the monitor width, or `NULL`.
 *  @param[out] height Where to store the monitor height, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_workarea
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorWorkarea(GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height);

/*! @brief Returns the physical size of the monitor.
 *
 *  This function returns the size, in millimetres, of the display area of the
 *  specified monitor.
 *
 *  Some systems do not provide accurate monitor size information, either
 *  because the monitor
 *  [EDID](https://en.wikipedia.org/wiki/Extended_display_identification_data)
 *  data is incorrect or because the driver does not report it accurately.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] widthMM Where to store the width, in millimetres, of the
 *  monitor's display area, or `NULL`.
 *  @param[out] heightMM Where to store the height, in millimetres, of the
 *  monitor's display area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @win32 On Windows 8 and earlier the physical size is calculated from
 *  the current resolution and system DPI instead of querying the monitor EDID data.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* widthMM, int* heightMM);

/*! @brief Retrieves the content scale for the specified monitor.
 *
 *  This function retrieves the content scale for the specified monitor.  The
 *  content scale is the ratio between the current DPI and the platform's
 *  default DPI.  This is especially important for text and any UI elements.  If
 *  the pixel dimensions of your UI scaled by this look appropriate on your
 *  machine then it should appear at a reasonable size on other machines
 *  regardless of their DPI and scaling settings.  This relies on the system DPI
 *  and scaling settings being somewhat correct.
 *
 *  The content scale may depend on both the monitor resolution and pixel
 *  density and on user settings.  It may be very different from the raw DPI
 *  calculated from the physical size and current resolution.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xscale Where to store the x-axis content scale, or `NULL`.
 *  @param[out] yscale Where to store the y-axis content scale, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_scale
 *  @sa @ref glfwGetWindowContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorContentScale(GLFWmonitor* monitor, float* xscale, float* yscale);

/*! @brief Returns the name of the specified monitor.
 *
 *  This function returns a human-readable name, encoded as UTF-8, of the
 *  specified monitor.  The name typically reflects the make and model of the
 *  monitor and is not guaranteed to be unique among the connected monitors.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The UTF-8 encoded name of the monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI const char* glfwGetMonitorName(GLFWmonitor* monitor);

/*! @brief Sets the user pointer of the specified monitor.
 *
 *  This function sets the user-defined pointer of the specified monitor.  The
 *  current value is retained until the monitor is disconnected.  The initial
 *  value is `NULL`.
 *
 *  This function may be called from the monitor callback, even for a monitor
 *  that is being disconnected.
 *
 *  @param[in] monitor The monitor whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref monitor_userptr
 *  @sa @ref glfwGetMonitorUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetMonitorUserPointer(GLFWmonitor* monitor, void* pointer);

/*! @brief Returns the user pointer of the specified monitor.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified monitor.  The initial value is `NULL`.
 *
 *  This function may be called from the monitor callback, even for a monitor
 *  that is being disconnected.
 *
 *  @param[in] monitor The monitor whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref monitor_userptr
 *  @sa @ref glfwSetMonitorUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void* glfwGetMonitorUserPointer(GLFWmonitor* monitor);

/*! @brief Sets the monitor configuration callback.
 *
 *  This function sets the monitor configuration callback, or removes the
 *  currently set callback.  This is called when a monitor is connected to or
 *  disconnected from the system.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWmonitor* monitor, int event)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWmonitorfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_event
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback);

/*! @brief Returns the available video modes for the specified monitor.
 *
 *  This function returns an array of all video modes supported by the specified
 *  monitor.  The returned array is sorted in ascending order, first by color
 *  bit depth (the sum of all channel depths), then by resolution area (the
 *  product of width and height), then resolution width and finally by refresh
 *  rate.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] count Where to store the number of video modes in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of video modes, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected, this function is called again for that monitor or the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoMode
 *
 *  @since Added in version 1.0.
 *  @glfw3 Changed to return an array of modes for a specific monitor.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);

/*! @brief Returns the current mode of the specified monitor.
 *
 *  This function returns the current video mode of the specified monitor.  If
 *  you have created a full screen window for that monitor, the return value
 *  will depend on whether that window is iconified.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The current mode of the monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoModes
 *
 *  @since Added in version 3.0.  Replaces `glfwGetDesktopMode`.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);

/*! @brief Generates a gamma ramp and sets it for the specified monitor.
 *
 *  This function generates an appropriately sized gamma ramp from the specified
 *  exponent and then calls @ref glfwSetGammaRamp with it.  The value must be
 *  a finite number greater than zero.
 *
 *  The software controlled gamma ramp is applied _in addition_ to the hardware
 *  gamma correction, which today is usually an approximation of sRGB gamma.
 *  This means that setting a perfectly linear ramp, or gamma 1.0, will produce
 *  the default (usually sRGB-like) behavior.
 *
 *  For gamma correct rendering with OpenGL or OpenGL ES, see the @ref
 *  GLFW_SRGB_CAPABLE hint.
 *
 *  @param[in] monitor The monitor whose gamma ramp to set.
 *  @param[in] gamma The desired exponent.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetGamma(GLFWmonitor* monitor, float gamma);

/*! @brief Returns the current gamma ramp for the specified monitor.
 *
 *  This function returns the current gamma ramp of the specified monitor.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The current gamma ramp, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR while
 *  returning `NULL`.
 *
 *  @pointer_lifetime The returned structure and its arrays are allocated and
 *  freed by GLFW.  You should not free them yourself.  They are valid until the
 *  specified monitor is disconnected, this function is called again for that
 *  monitor or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);

/*! @brief Sets the current gamma ramp for the specified monitor.
 *
 *  This function sets the current gamma ramp for the specified monitor.  The
 *  original gamma ramp for that monitor is saved by GLFW the first time this
 *  function is called and is restored by @ref glfwTerminate.
 *
 *  The software controlled gamma ramp is applied _in addition_ to the hardware
 *  gamma correction, which today is usually an approximation of sRGB gamma.
 *  This means that setting a perfectly linear ramp, or gamma 1.0, will produce
 *  the default (usually sRGB-like) behavior.
 *
 *  For gamma correct rendering with OpenGL or OpenGL ES, see the @ref
 *  GLFW_SRGB_CAPABLE hint.
 *
 *  @param[in] monitor The monitor whose gamma ramp to set.
 *  @param[in] ramp The gamma ramp to use.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The size of the specified gamma ramp should match the size of the
 *  current ramp for that monitor.
 *
 *  @remark @win32 The gamma ramp size must be 256.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified gamma ramp is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);

/*! @brief Resets all window hints to their default values.
 *
 *  This function resets all window hints to their
 *  [default values](@ref window_hints_values).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHint
 *  @sa @ref glfwWindowHintString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwDefaultWindowHints(void);

/*! @brief Sets the specified window hint to the desired value.
 *
 *  This function sets hints for the next call to @ref glfwCreateWindow.  The
 *  hints, once set, retain their values until changed by a call to this
 *  function or @ref glfwDefaultWindowHints, or until the library is terminated.
 *
 *  Only integer value hints can be set with this function.  String value hints
 *  are set with @ref glfwWindowHintString.
 *
 *  This function does not check whether the specified hint values are valid.
 *  If you set hints to invalid values this will instead be reported by the next
 *  call to @ref glfwCreateWindow.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [window hint](@ref window_hints) to set.
 *  @param[in] value The new value of the window hint.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHintString
 *  @sa @ref glfwDefaultWindowHints
 *
 *  @since Added in version 3.0.  Replaces `glfwOpenWindowHint`.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWindowHint(int hint, int value);

/*! @brief Sets the specified window hint to the desired value.
 *
 *  This function sets hints for the next call to @ref glfwCreateWindow.  The
 *  hints, once set, retain their values until changed by a call to this
 *  function or @ref glfwDefaultWindowHints, or until the library is terminated.
 *
 *  Only string type hints can be set with this function.  Integer value hints
 *  are set with @ref glfwWindowHint.
 *
 *  This function does not check whether the specified hint values are valid.
 *  If you set hints to invalid values this will instead be reported by the next
 *  call to @ref glfwCreateWindow.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [window hint](@ref window_hints) to set.
 *  @param[in] value The new value of the window hint.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @pointer_lifetime The specified string is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHint
 *  @sa @ref glfwDefaultWindowHints
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWindowHintString(int hint, const char* value);

/*! @brief Creates a window and its associated context.
 *
 *  This function creates a window and its associated OpenGL or OpenGL ES
 *  context.  Most of the options controlling how the window and its context
 *  should be created are specified with [window hints](@ref window_hints).
 *
 *  Successful creation does not change which context is current.  Before you
 *  can use the newly created context, you need to
 *  [make it current](@ref context_current).  For information about the `share`
 *  parameter, see @ref context_sharing.
 *
 *  The created window, framebuffer and context may differ from what you
 *  requested, as not all parameters and hints are
 *  [hard constraints](@ref window_hints_hard).  This includes the size of the
 *  window, especially for full screen windows.  To query the actual attributes
 *  of the created window, framebuffer and context, see @ref
 *  glfwGetWindowAttrib, @ref glfwGetWindowSize and @ref glfwGetFramebufferSize.
 *
 *  To create a full screen window, you need to specify the monitor the window
 *  will cover.  If no monitor is specified, the window will be windowed mode.
 *  Unless you have a way for the user to choose a specific monitor, it is
 *  recommended that you pick the primary monitor.  For more information on how
 *  to query connected monitors, see @ref monitor_monitors.
 *
 *  For full screen windows, the specified size becomes the resolution of the
 *  window's _desired video mode_.  As long as a full screen window is not
 *  iconified, the supported video mode most closely matching the desired video
 *  mode is set for the specified monitor.  For more information about full
 *  screen windows, including the creation of so called _windowed full screen_
 *  or _borderless full screen_ windows, see @ref window_windowed_full_screen.
 *
 *  Once you have created the window, you can switch it between windowed and
 *  full screen mode with @ref glfwSetWindowMonitor.  This will not affect its
 *  OpenGL or OpenGL ES context.
 *
 *  By default, newly created windows use the placement recommended by the
 *  window system.  To create the window at a specific position, make it
 *  initially invisible using the [GLFW_VISIBLE](@ref GLFW_VISIBLE_hint) window
 *  hint, set its [position](@ref window_pos) and then [show](@ref window_hide)
 *  it.
 *
 *  As long as at least one full screen window is not iconified, the screensaver
 *  is prohibited from starting.
 *
 *  Window systems put limits on window sizes.  Very large or very small window
 *  dimensions may be overridden by the window system on creation.  Check the
 *  actual [size](@ref window_size) after creation.
 *
 *  The [swap interval](@ref buffer_swap) is not set during window creation and
 *  the initial value may vary depending on driver settings and defaults.
 *
 *  @param[in] width The desired width, in screen coordinates, of the window.
 *  This must be greater than zero.
 *  @param[in] height The desired height, in screen coordinates, of the window.
 *  This must be greater than zero.
 *  @param[in] title The initial, UTF-8 encoded window title.
 *  @param[in] monitor The monitor to use for full screen mode, or `NULL` for
 *  windowed mode.
 *  @param[in] share The window whose context to share resources with, or `NULL`
 *  to not share resources.
 *  @return The handle of the created window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM, @ref GLFW_INVALID_VALUE, @ref GLFW_API_UNAVAILABLE, @ref
 *  GLFW_VERSION_UNAVAILABLE, @ref GLFW_FORMAT_UNAVAILABLE and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @win32 Window creation will fail if the Microsoft GDI software
 *  OpenGL implementation is the only one available.
 *
 *  @remark @win32 If the executable has an icon resource named `GLFW_ICON,` it
 *  will be set as the initial icon for the window.  If no such icon is present,
 *  the `IDI_APPLICATION` icon will be used instead.  To set a different icon,
 *  see @ref glfwSetWindowIcon.
 *
 *  @remark @win32 The context to share resources with must not be current on
 *  any other thread.
 *
 *  @remark @macos The OS only supports forward-compatible core profile contexts
 *  for OpenGL versions 3.2 and later.  Before creating an OpenGL context of
 *  version 3.2 or later you must set the
 *  [GLFW_OPENGL_FORWARD_COMPAT](@ref GLFW_OPENGL_FORWARD_COMPAT_hint) and
 *  [GLFW_OPENGL_PROFILE](@ref GLFW_OPENGL_PROFILE_hint) hints accordingly.
 *  OpenGL 3.0 and 3.1 contexts are not supported at all on macOS.
 *
 *  @remark @macos The GLFW window has no icon, as it is not a document
 *  window, but the dock icon will be the same as the application bundle's icon.
 *  For more information on bundles, see the
 *  [Bundle Programming Guide](https://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFBundles/)
 *  in the Mac Developer Library.
 *
 *  @remark @macos The first time a window is created the menu bar is created.
 *  If GLFW finds a `MainMenu.nib` it is loaded and assumed to contain a menu
 *  bar.  Otherwise a minimal menu bar is created manually with common commands
 *  like Hide, Quit and About.  The About entry opens a minimal about dialog
 *  with information from the application's bundle.  Menu bar creation can be
 *  disabled entirely with the @ref GLFW_COCOA_MENUBAR init hint.
 *
 *  @remark @macos On OS X 10.10 and later the window frame will not be rendered
 *  at full resolution on Retina displays unless the
 *  [GLFW_COCOA_RETINA_FRAMEBUFFER](@ref GLFW_COCOA_RETINA_FRAMEBUFFER_hint)
 *  hint is `GLFW_TRUE` and the `NSHighResolutionCapable` key is enabled in the
 *  application bundle's `Info.plist`.  For more information, see
 *  [High Resolution Guidelines for OS X](https://developer.apple.com/library/mac/documentation/GraphicsAnimation/Conceptual/HighResolutionOSX/Explained/Explained.html)
 *  in the Mac Developer Library.  The GLFW test and example programs use
 *  a custom `Info.plist` template for this, which can be found as
 *  `CMake/MacOSXBundleInfo.plist.in` in the source tree.
 *
 *  @remark @macos When activating frame autosaving with
 *  [GLFW_COCOA_FRAME_NAME](@ref GLFW_COCOA_FRAME_NAME_hint), the specified
 *  window size and position may be overridden by previously saved values.
 *
 *  @remark @x11 Some window managers will not respect the placement of
 *  initially hidden windows.
 *
 *  @remark @x11 Due to the asynchronous nature of X11, it may take a moment for
 *  a window to reach its requested state.  This means you may not be able to
 *  query the final size, position or other attributes directly after window
 *  creation.
 *
 *  @remark @x11 The class part of the `WM_CLASS` window property will by
 *  default be set to the window title passed to this function.  The instance
 *  part will use the contents of the `RESOURCE_NAME` environment variable, if
 *  present and not empty, or fall back to the window title.  Set the
 *  [GLFW_X11_CLASS_NAME](@ref GLFW_X11_CLASS_NAME_hint) and
 *  [GLFW_X11_INSTANCE_NAME](@ref GLFW_X11_INSTANCE_NAME_hint) window hints to
 *  override this.
 *
 *  @remark @wayland Compositors should implement the xdg-decoration protocol
 *  for GLFW to decorate the window properly.  If this protocol isn't
 *  supported, or if the compositor prefers client-side decorations, a very
 *  simple fallback frame will be drawn using the wp_viewporter protocol.  A
 *  compositor can still emit close, maximize or fullscreen events, using for
 *  instance a keybind mechanism.  If neither of these protocols is supported,
 *  the window won't be decorated.
 *
 *  @remark @wayland A full screen window will not attempt to change the mode,
 *  no matter what the requested size or refresh rate.
 *
 *  @remark @wayland Screensaver inhibition requires the idle-inhibit protocol
 *  to be implemented in the user's compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_creation
 *  @sa @ref glfwDestroyWindow
 *
 *  @since Added in version 3.0.  Replaces `glfwOpenWindow`.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

/*! @brief Destroys the specified window and its context.
 *
 *  This function destroys the specified window and its context.  On calling
 *  this function, no further callbacks will be called for that window.
 *
 *  If the context of the specified window is current on the main thread, it is
 *  detached before being destroyed.
 *
 *  @param[in] window The window to destroy.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @note The context of the specified window must not be current on any other
 *  thread when this function is called.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_creation
 *  @sa @ref glfwCreateWindow
 *
 *  @since Added in version 3.0.  Replaces `glfwCloseWindow`.
 *
 *  @ingroup window
 */
GLFWAPI void glfwDestroyWindow(GLFWwindow* window);

/*! @brief Checks the close flag of the specified window.
 *
 *  This function returns the value of the close flag of the specified window.
 *
 *  @param[in] window The window to query.
 *  @return The value of the close flag.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI int glfwWindowShouldClose(GLFWwindow* window);

/*! @brief Sets the close flag of the specified window.
 *
 *  This function sets the value of the close flag of the specified window.
 *  This can be used to override the user's attempt to close the window, or
 *  to signal that it should be closed.
 *
 *  @param[in] window The window whose flag to change.
 *  @param[in] value The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowShouldClose(GLFWwindow* window, int value);

/*! @brief Sets the title of the specified window.
 *
 *  This function sets the window title, encoded as UTF-8, of the specified
 *  window.
 *
 *  @param[in] window The window whose title to change.
 *  @param[in] title The UTF-8 encoded window title.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos The window title will not be updated until the next time you
 *  process events.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_title
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowTitle(GLFWwindow* window, const char* title);

/*! @brief Sets the icon for the specified window.
 *
 *  This function sets the icon of the specified window.  If passed an array of
 *  candidate images, those of or closest to the sizes desired by the system are
 *  selected.  If no images are specified, the window reverts to its default
 *  icon.
 *
 *  The pixels are 32-bit, little-endian, non-premultiplied RGBA, i.e. eight
 *  bits per channel with the red channel first.  They are arranged canonically
 *  as packed sequential rows, starting from the top-left corner.
 *
 *  The desired image sizes varies depending on platform and system settings.
 *  The selected images will be rescaled as needed.  Good sizes include 16x16,
 *  32x32 and 48x48.
 *
 *  @param[in] window The window whose icon to set.
 *  @param[in] count The number of images in the specified array, or zero to
 *  revert to the default window icon.
 *  @param[in] images The images to create the icon from.  This is ignored if
 *  count is zero.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified image data is copied before this function
 *  returns.
 *
 *  @remark @macos The GLFW window has no icon, as it is not a document
 *  window, so this function does nothing.  The dock icon will be the same as
 *  the application bundle's icon.  For more information on bundles, see the
 *  [Bundle Programming Guide](https://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFBundles/)
 *  in the Mac Developer Library.
 *
 *  @remark @wayland There is no existing protocol to change an icon, the
 *  window will thus inherit the one defined in the application's desktop file.
 *  This function always emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_icon
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowIcon(GLFWwindow* window, int count, const GLFWimage* images);

/*! @brief Retrieves the position of the content area of the specified window.
 *
 *  This function retrieves the position, in screen coordinates, of the
 *  upper-left corner of the content area of the specified window.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] window The window to query.
 *  @param[out] xpos Where to store the x-coordinate of the upper-left corner of
 *  the content area, or `NULL`.
 *  @param[out] ypos Where to store the y-coordinate of the upper-left corner of
 *  the content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland There is no way for an application to retrieve the global
 *  position of its windows, this function will always emit @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwSetWindowPos
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);

/*! @brief Sets the position of the content area of the specified window.
 *
 *  This function sets the position, in screen coordinates, of the upper-left
 *  corner of the content area of the specified windowed mode window.  If the
 *  window is a full screen window, this function does nothing.
 *
 *  __Do not use this function__ to move an already visible window unless you
 *  have very good reasons for doing so, as it will confuse and annoy the user.
 *
 *  The window manager may put limits on what positions are allowed.  GLFW
 *  cannot and should not override these limits.
 *
 *  @param[in] window The window to query.
 *  @param[in] xpos The x-coordinate of the upper-left corner of the content area.
 *  @param[in] ypos The y-coordinate of the upper-left corner of the content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland There is no way for an application to set the global
 *  position of its windows, this function will always emit @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwGetWindowPos
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);

/*! @brief Retrieves the size of the content area of the specified window.
 *
 *  This function retrieves the size, in screen coordinates, of the content area
 *  of the specified window.  If you wish to retrieve the size of the
 *  framebuffer of the window in pixels, see @ref glfwGetFramebufferSize.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose size to retrieve.
 *  @param[out] width Where to store the width, in screen coordinates, of the
 *  content area, or `NULL`.
 *  @param[out] height Where to store the height, in screen coordinates, of the
 *  content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwSetWindowSize
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);

/*! @brief Sets the size limits of the specified window.
 *
 *  This function sets the size limits of the content area of the specified
 *  window.  If the window is full screen, the size limits only take effect
 *  once it is made windowed.  If the window is not resizable, this function
 *  does nothing.
 *
 *  The size limits are applied immediately to a windowed mode window and may
 *  cause it to be resized.
 *
 *  The maximum dimensions must be greater than or equal to the minimum
 *  dimensions and all must be greater than or equal to zero.
 *
 *  @param[in] window The window to set limits for.
 *  @param[in] minwidth The minimum width, in screen coordinates, of the content
 *  area, or `GLFW_DONT_CARE`.
 *  @param[in] minheight The minimum height, in screen coordinates, of the
 *  content area, or `GLFW_DONT_CARE`.
 *  @param[in] maxwidth The maximum width, in screen coordinates, of the content
 *  area, or `GLFW_DONT_CARE`.
 *  @param[in] maxheight The maximum height, in screen coordinates, of the
 *  content area, or `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark If you set size limits and an aspect ratio that conflict, the
 *  results are undefined.
 *
 *  @remark @wayland The size limits will not be applied until the window is
 *  actually resized, either by the user or by the compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_sizelimits
 *  @sa @ref glfwSetWindowAspectRatio
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowSizeLimits(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);

/*! @brief Sets the aspect ratio of the specified window.
 *
 *  This function sets the required aspect ratio of the content area of the
 *  specified window.  If the window is full screen, the aspect ratio only takes
 *  effect once it is made windowed.  If the window is not resizable, this
 *  function does nothing.
 *
 *  The aspect ratio is specified as a numerator and a denominator and both
 *  values must be greater than zero.  For example, the common 16:9 aspect ratio
 *  is specified as 16 and 9, respectively.
 *
 *  If the numerator and denominator is set to `GLFW_DONT_CARE` then the aspect
 *  ratio limit is disabled.
 *
 *  The aspect ratio is applied immediately to a windowed mode window and may
 *  cause it to be resized.
 *
 *  @param[in] window The window to set limits for.
 *  @param[in] numer The numerator of the desired aspect ratio, or
 *  `GLFW_DONT_CARE`.
 *  @param[in] denom The denominator of the desired aspect ratio, or
 *  `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark If you set size limits and an aspect ratio that conflict, the
 *  results are undefined.
 *
 *  @remark @wayland The aspect ratio will not be applied until the window is
 *  actually resized, either by the user or by the compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_sizelimits
 *  @sa @ref glfwSetWindowSizeLimits
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowAspectRatio(GLFWwindow* window, int numer, int denom);

/*! @brief Sets the size of the content area of the specified window.
 *
 *  This function sets the size, in screen coordinates, of the content area of
 *  the specified window.
 *
 *  For full screen windows, this function updates the resolution of its desired
 *  video mode and switches to the video mode closest to it, without affecting
 *  the window's context.  As the context is unaffected, the bit depths of the
 *  framebuffer remain unchanged.
 *
 *  If you wish to update the refresh rate of the desired video mode in addition
 *  to its resolution, see @ref glfwSetWindowMonitor.
 *
 *  The window manager may put limits on what sizes are allowed.  GLFW cannot
 *  and should not override these limits.
 *
 *  @param[in] window The window to resize.
 *  @param[in] width The desired width, in screen coordinates, of the window
 *  content area.
 *  @param[in] height The desired height, in screen coordinates, of the window
 *  content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland A full screen window will not attempt to change the mode,
 *  no matter what the requested size.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwGetWindowSize
 *  @sa @ref glfwSetWindowMonitor
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowSize(GLFWwindow* window, int width, int height);

/*! @brief Retrieves the size of the framebuffer of the specified window.
 *
 *  This function retrieves the size, in pixels, of the framebuffer of the
 *  specified window.  If you wish to retrieve the size of the window in screen
 *  coordinates, see @ref glfwGetWindowSize.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose framebuffer to query.
 *  @param[out] width Where to store the width, in pixels, of the framebuffer,
 *  or `NULL`.
 *  @param[out] height Where to store the height, in pixels, of the framebuffer,
 *  or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_fbsize
 *  @sa @ref glfwSetFramebufferSizeCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);

/*! @brief Retrieves the size of the frame of the window.
 *
 *  This function retrieves the size, in screen coordinates, of each edge of the
 *  frame of the specified window.  This size includes the title bar, if the
 *  window has one.  The size of the frame may vary depending on the
 *  [window-related hints](@ref window_hints_wnd) used to create it.
 *
 *  Because this function retrieves the size of each window frame edge and not
 *  the offset along a particular coordinate axis, the retrieved values will
 *  always be zero or positive.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose frame size to query.
 *  @param[out] left Where to store the size, in screen coordinates, of the left
 *  edge of the window frame, or `NULL`.
 *  @param[out] top Where to store the size, in screen coordinates, of the top
 *  edge of the window frame, or `NULL`.
 *  @param[out] right Where to store the size, in screen coordinates, of the
 *  right edge of the window frame, or `NULL`.
 *  @param[out] bottom Where to store the size, in screen coordinates, of the
 *  bottom edge of the window frame, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowFrameSize(GLFWwindow* window, int* left, int* top, int* right, int* bottom);

/*! @brief Retrieves the content scale for the specified window.
 *
 *  This function retrieves the content scale for the specified window.  The
 *  content scale is the ratio between the current DPI and the platform's
 *  default DPI.  This is especially important for text and any UI elements.  If
 *  the pixel dimensions of your UI scaled by this look appropriate on your
 *  machine then it should appear at a reasonable size on other machines
 *  regardless of their DPI and scaling settings.  This relies on the system DPI
 *  and scaling settings being somewhat correct.
 *
 *  On systems where each monitors can have its own content scale, the window
 *  content scale will depend on which monitor the system considers the window
 *  to be on.
 *
 *  @param[in] window The window to query.
 *  @param[out] xscale Where to store the x-axis content scale, or `NULL`.
 *  @param[out] yscale Where to store the y-axis content scale, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwSetWindowContentScaleCallback
 *  @sa @ref glfwGetMonitorContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowContentScale(GLFWwindow* window, float* xscale, float* yscale);

/*! @brief Returns the opacity of the whole window.
 *
 *  This function returns the opacity of the window, including any decorations.
 *
 *  The opacity (or alpha) value is a positive finite number between zero and
 *  one, where zero is fully transparent and one is fully opaque.  If the system
 *  does not support whole window transparency, this function always returns one.
 *
 *  The initial opacity value for newly created windows is one.
 *
 *  @param[in] window The window to query.
 *  @return The opacity value of the specified window.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_transparency
 *  @sa @ref glfwSetWindowOpacity
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI float glfwGetWindowOpacity(GLFWwindow* window);

/*! @brief Sets the opacity of the whole window.
 *
 *  This function sets the opacity of the window, including any decorations.
 *
 *  The opacity (or alpha) value is a positive finite number between zero and
 *  one, where zero is fully transparent and one is fully opaque.
 *
 *  The initial opacity value for newly created windows is one.
 *
 *  A window created with framebuffer transparency may not use whole window
 *  transparency.  The results of doing this are undefined.
 *
 *  @param[in] window The window to set the opacity for.
 *  @param[in] opacity The desired opacity of the specified window.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_transparency
 *  @sa @ref glfwGetWindowOpacity
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowOpacity(GLFWwindow* window, float opacity);

/*! @brief Iconifies the specified window.
 *
 *  This function iconifies (minimizes) the specified window if it was
 *  previously restored.  If the window is already iconified, this function does
 *  nothing.
 *
 *  If the specified window is a full screen window, the original monitor
 *  resolution is restored until the window is restored.
 *
 *  @param[in] window The window to iconify.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland There is no concept of iconification in wl_shell, this
 *  function will emit @ref GLFW_PLATFORM_ERROR when using this deprecated
 *  protocol.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwRestoreWindow
 *  @sa @ref glfwMaximizeWindow
 *
 *  @since Added in version 2.1.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwIconifyWindow(GLFWwindow* window);

/*! @brief Restores the specified window.
 *
 *  This function restores the specified window if it was previously iconified
 *  (minimized) or maximized.  If the window is already restored, this function
 *  does nothing.
 *
 *  If the specified window is a full screen window, the resolution chosen for
 *  the window is restored on the selected monitor.
 *
 *  @param[in] window The window to restore.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwIconifyWindow
 *  @sa @ref glfwMaximizeWindow
 *
 *  @since Added in version 2.1.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwRestoreWindow(GLFWwindow* window);

/*! @brief Maximizes the specified window.
 *
 *  This function maximizes the specified window if it was previously not
 *  maximized.  If the window is already maximized, this function does nothing.
 *
 *  If the specified window is a full screen window, this function does nothing.
 *
 *  @param[in] window The window to maximize.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @par Thread Safety
 *  This function may only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwIconifyWindow
 *  @sa @ref glfwRestoreWindow
 *
 *  @since Added in GLFW 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwMaximizeWindow(GLFWwindow* window);

/*! @brief Makes the specified window visible.
 *
 *  This function makes the specified window visible if it was previously
 *  hidden.  If the window is already visible or is in full screen mode, this
 *  function does nothing.
 *
 *  By default, windowed mode windows are focused when shown
 *  Set the [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_hint) window hint
 *  to change this behavior for all newly created windows, or change the
 *  behavior for an existing window with @ref glfwSetWindowAttrib.
 *
 *  @param[in] window The window to make visible.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hide
 *  @sa @ref glfwHideWindow
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwShowWindow(GLFWwindow* window);

/*! @brief Hides the specified window.
 *
 *  This function hides the specified window if it was previously visible.  If
 *  the window is already hidden or is in full screen mode, this function does
 *  nothing.
 *
 *  @param[in] window The window to hide.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hide
 *  @sa @ref glfwShowWindow
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwHideWindow(GLFWwindow* window);

/*! @brief Brings the specified window to front and sets input focus.
 *
 *  This function brings the specified window to front and sets input focus.
 *  The window should already be visible and not iconified.
 *
 *  By default, both windowed and full screen mode windows are focused when
 *  initially created.  Set the [GLFW_FOCUSED](@ref GLFW_FOCUSED_hint) to
 *  disable this behavior.
 *
 *  Also by default, windowed mode windows are focused when shown
 *  with @ref glfwShowWindow. Set the
 *  [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_hint) to disable this behavior.
 *
 *  __Do not use this function__ to steal focus from other applications unless
 *  you are certain that is what the user wants.  Focus stealing can be
 *  extremely disruptive.
 *
 *  For a less disruptive way of getting the user's attention, see
 *  [attention requests](@ref window_attention).
 *
 *  @param[in] window The window to give input focus.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland It is not possible for an application to bring its windows
 *  to front, this function will always emit @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_focus
 *  @sa @ref window_attention
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwFocusWindow(GLFWwindow* window);

/*! @brief Requests user attention to the specified window.
 *
 *  This function requests user attention to the specified window.  On
 *  platforms where this is not supported, attention is requested to the
 *  application as a whole.
 *
 *  Once the user has given attention, usually by focusing the window or
 *  application, the system will end the request automatically.
 *
 *  @param[in] window The window to request attention to.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos Attention is requested to the application as a whole, not the
 *  specific window.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attention
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwRequestWindowAttention(GLFWwindow* window);

/*! @brief Returns the monitor that the window uses for full screen mode.
 *
 *  This function returns the handle of the monitor that the specified window is
 *  in full screen on.
 *
 *  @param[in] window The window to query.
 *  @return The monitor, or `NULL` if the window is in windowed mode or an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_monitor
 *  @sa @ref glfwSetWindowMonitor
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);

/*! @brief Sets the mode, monitor, video mode and placement of a window.
 *
 *  This function sets the monitor that the window uses for full screen mode or,
 *  if the monitor is `NULL`, makes it windowed mode.
 *
 *  When setting a monitor, this function updates the width, height and refresh
 *  rate of the desired video mode and switches to the video mode closest to it.
 *  The window position is ignored when setting a monitor.
 *
 *  When the monitor is `NULL`, the position, width and height are used to
 *  place the window content area.  The refresh rate is ignored when no monitor
 *  is specified.
 *
 *  If you only wish to update the resolution of a full screen window or the
 *  size of a windowed mode window, see @ref glfwSetWindowSize.
 *
 *  When a window transitions from full screen to windowed mode, this function
 *  restores any previous window settings such as whether it is decorated,
 *  floating, resizable, has size or aspect ratio limits, etc.
 *
 *  @param[in] window The window whose monitor, size or video mode to set.
 *  @param[in] monitor The desired monitor, or `NULL` to set windowed mode.
 *  @param[in] xpos The desired x-coordinate of the upper-left corner of the
 *  content area.
 *  @param[in] ypos The desired y-coordinate of the upper-left corner of the
 *  content area.
 *  @param[in] width The desired with, in screen coordinates, of the content
 *  area or video mode.
 *  @param[in] height The desired height, in screen coordinates, of the content
 *  area or video mode.
 *  @param[in] refreshRate The desired refresh rate, in Hz, of the video mode,
 *  or `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The OpenGL or OpenGL ES context will not be destroyed or otherwise
 *  affected by any resizing or mode switching, although you may need to update
 *  your viewport if the framebuffer size has changed.
 *
 *  @remark @wayland The desired window position is ignored, as there is no way
 *  for an application to set this property.
 *
 *  @remark @wayland Setting the window to full screen will not attempt to
 *  change the mode, no matter what the requested size or refresh rate.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_monitor
 *  @sa @ref window_full_screen
 *  @sa @ref glfwGetWindowMonitor
 *  @sa @ref glfwSetWindowSize
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowMonitor(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

/*! @brief Returns an attribute of the specified window.
 *
 *  This function returns the value of an attribute of the specified window or
 *  its OpenGL or OpenGL ES context.
 *
 *  @param[in] window The window to query.
 *  @param[in] attrib The [window attribute](@ref window_attribs) whose value to
 *  return.
 *  @return The value of the attribute, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark Framebuffer related hints are not window attributes.  See @ref
 *  window_attribs_fb for more information.
 *
 *  @remark Zero is a valid value for many window and context related
 *  attributes so you cannot use a return value of zero as an indication of
 *  errors.  However, this function should not fail as long as it is passed
 *  valid arguments and the library has been [initialized](@ref intro_init).
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attribs
 *  @sa @ref glfwSetWindowAttrib
 *
 *  @since Added in version 3.0.  Replaces `glfwGetWindowParam` and
 *  `glfwGetGLVersion`.
 *
 *  @ingroup window
 */
GLFWAPI int glfwGetWindowAttrib(GLFWwindow* window, int attrib);

/*! @brief Sets an attribute of the specified window.
 *
 *  This function sets the value of an attribute of the specified window.
 *
 *  The supported attributes are [GLFW_DECORATED](@ref GLFW_DECORATED_attrib),
 *  [GLFW_RESIZABLE](@ref GLFW_RESIZABLE_attrib),
 *  [GLFW_FLOATING](@ref GLFW_FLOATING_attrib),
 *  [GLFW_AUTO_ICONIFY](@ref GLFW_AUTO_ICONIFY_attrib) and
 *  [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_attrib).
 *
 *  Some of these attributes are ignored for full screen windows.  The new
 *  value will take effect if the window is later made windowed.
 *
 *  Some of these attributes are ignored for windowed mode windows.  The new
 *  value will take effect if the window is later made full screen.
 *
 *  @param[in] window The window to set the attribute for.
 *  @param[in] attrib A supported window attribute.
 *  @param[in] value `GLFW_TRUE` or `GLFW_FALSE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM, @ref GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark Calling @ref glfwGetWindowAttrib will always return the latest
 *  value, even if that value is ignored by the current mode of the window.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attribs
 *  @sa @ref glfwGetWindowAttrib
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowAttrib(GLFWwindow* window, int attrib, int value);

/*! @brief Sets the user pointer of the specified window.
 *
 *  This function sets the user-defined pointer of the specified window.  The
 *  current value is retained until the window is destroyed.  The initial value
 *  is `NULL`.
 *
 *  @param[in] window The window whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_userptr
 *  @sa @ref glfwGetWindowUserPointer
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);

/*! @brief Returns the user pointer of the specified window.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified window.  The initial value is `NULL`.
 *
 *  @param[in] window The window whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_userptr
 *  @sa @ref glfwSetWindowUserPointer
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void* glfwGetWindowUserPointer(GLFWwindow* window);

/*! @brief Sets the position callback for the specified window.
 *
 *  This function sets the position callback of the specified window, which is
 *  called when the window is moved.  The callback is provided with the
 *  position, in screen coordinates, of the upper-left corner of the content
 *  area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int xpos, int ypos)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowposfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland This callback will never be called, as there is no way for
 *  an application to know its global position.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun callback);

/*! @brief Sets the size callback for the specified window.
 *
 *  This function sets the size callback of the specified window, which is
 *  called when the window is resized.  The callback is provided with the size,
 *  in screen coordinates, of the content area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowsizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);

/*! @brief Sets the close callback for the specified window.
 *
 *  This function sets the close callback of the specified window, which is
 *  called when the user attempts to close the window, for example by clicking
 *  the close widget in the title bar.
 *
 *  The close flag is set before this callback is called, but you can modify it
 *  at any time with @ref glfwSetWindowShouldClose.
 *
 *  The close callback is not triggered by @ref glfwDestroyWindow.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowclosefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @macos Selecting Quit from the application menu will trigger the
 *  close callback for all windows.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback);

/*! @brief Sets the refresh callback for the specified window.
 *
 *  This function sets the refresh callback of the specified window, which is
 *  called when the content area of the window needs to be redrawn, for example
 *  if the window has been exposed after having been covered by another window.
 *
 *  On compositing window systems such as Aero, Compiz, Aqua or Wayland, where
 *  the window contents are saved off-screen, this callback may be called only
 *  very infrequently or never at all.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window);
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowrefreshfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_refresh
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun callback);

/*! @brief Sets the focus callback for the specified window.
 *
 *  This function sets the focus callback of the specified window, which is
 *  called when the window gains or loses input focus.
 *
 *  After the focus callback is called for a window that lost input focus,
 *  synthetic key and mouse button release events will be generated for all such
 *  that had been pressed.  For more information, see @ref glfwSetKeyCallback
 *  and @ref glfwSetMouseButtonCallback.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int focused)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowfocusfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_focus
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun callback);

/*! @brief Sets the iconify callback for the specified window.
 *
 *  This function sets the iconification callback of the specified window, which
 *  is called when the window is iconified or restored.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int iconified)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowiconifyfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland The wl_shell protocol has no concept of iconification,
 *  this callback will never be called when using this deprecated protocol.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun callback);

/*! @brief Sets the maximize callback for the specified window.
 *
 *  This function sets the maximization callback of the specified window, which
 *  is called when the window is maximized or restored.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int maximized)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowmaximizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_maximize
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* window, GLFWwindowmaximizefun callback);

/*! @brief Sets the framebuffer resize callback for the specified window.
 *
 *  This function sets the framebuffer resize callback of the specified window,
 *  which is called when the framebuffer of the specified window is resized.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWframebuffersizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_fbsize
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback);

/*! @brief Sets the window content scale callback for the specified window.
 *
 *  This function sets the window content scale callback of the specified window,
 *  which is called when the content scale of the specified window changes.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, float xscale, float yscale)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowcontentscalefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwGetWindowContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);

/*! @brief Processes all pending events.
 *
 *  This function processes only those events that are already in the event
 *  queue and then returns immediately.  Processing events will cause the window
 *  and input callbacks associated with those events to be called.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwWaitEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwPollEvents(void);

/*! @brief Waits until events are queued and processes them.
 *
 *  This function puts the calling thread to sleep until at least one event is
 *  available in the event queue.  Once one or more events are available,
 *  it behaves exactly like @ref glfwPollEvents, i.e. the events in the queue
 *  are processed and the function then returns immediately.  Processing events
 *  will cause the window and input callbacks associated with those events to be
 *  called.
 *
 *  Since not all events are associated with callbacks, this function may return
 *  without a callback having been called even if you are monitoring all
 *  callbacks.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwPollEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 2.5.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWaitEvents(void);

/*! @brief Waits with timeout until events are queued and processes them.
 *
 *  This function puts the calling thread to sleep until at least one event is
 *  available in the event queue, or until the specified timeout is reached.  If
 *  one or more events are available, it behaves exactly like @ref
 *  glfwPollEvents, i.e. the events in the queue are processed and the function
 *  then returns immediately.  Processing events will cause the window and input
 *  callbacks associated with those events to be called.
 *
 *  The timeout value must be a positive finite number.
 *
 *  Since not all events are associated with callbacks, this function may return
 *  without a callback having been called even if you are monitoring all
 *  callbacks.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @param[in] timeout The maximum amount of time, in seconds, to wait.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwPollEvents
 *  @sa @ref glfwWaitEvents
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWaitEventsTimeout(double timeout);

/*! @brief Posts an empty event to the event queue.
 *
 *  This function posts an empty event from the current thread to the event
 *  queue, causing @ref glfwWaitEvents or @ref glfwWaitEventsTimeout to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwWaitEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup window
 */
GLFWAPI void glfwPostEmptyEvent(void);

/*! @brief Returns the value of an input option for the specified window.
 *
 *  This function returns the value of an input option for the specified window.
 *  The mode must be one of @ref GLFW_CURSOR, @ref GLFW_STICKY_KEYS,
 *  @ref GLFW_STICKY_MOUSE_BUTTONS, @ref GLFW_LOCK_KEY_MODS or
 *  @ref GLFW_RAW_MOUSE_MOTION.
 *
 *  @param[in] window The window to query.
 *  @param[in] mode One of `GLFW_CURSOR`, `GLFW_STICKY_KEYS`,
 *  `GLFW_STICKY_MOUSE_BUTTONS`, `GLFW_LOCK_KEY_MODS` or
 *  `GLFW_RAW_MOUSE_MOTION`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref glfwSetInputMode
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetInputMode(GLFWwindow* window, int mode);

/*! @brief Sets an input option for the specified window.
 *
 *  This function sets an input mode option for the specified window.  The mode
 *  must be one of @ref GLFW_CURSOR, @ref GLFW_STICKY_KEYS,
 *  @ref GLFW_STICKY_MOUSE_BUTTONS, @ref GLFW_LOCK_KEY_MODS or
 *  @ref GLFW_RAW_MOUSE_MOTION.
 *
 *  If the mode is `GLFW_CURSOR`, the value must be one of the following cursor
 *  modes:
 *  - `GLFW_CURSOR_NORMAL` makes the cursor visible and behaving normally.
 *  - `GLFW_CURSOR_HIDDEN` makes the cursor invisible when it is over the
 *    content area of the window but does not restrict the cursor from leaving.
 *  - `GLFW_CURSOR_DISABLED` hides and grabs the cursor, providing virtual
 *    and unlimited cursor movement.  This is useful for implementing for
 *    example 3D camera controls.
 *
 *  If the mode is `GLFW_STICKY_KEYS`, the value must be either `GLFW_TRUE` to
 *  enable sticky keys, or `GLFW_FALSE` to disable it.  If sticky keys are
 *  enabled, a key press will ensure that @ref glfwGetKey returns `GLFW_PRESS`
 *  the next time it is called even if the key had been released before the
 *  call.  This is useful when you are only interested in whether keys have been
 *  pressed but not when or in which order.
 *
 *  If the mode is `GLFW_STICKY_MOUSE_BUTTONS`, the value must be either
 *  `GLFW_TRUE` to enable sticky mouse buttons, or `GLFW_FALSE` to disable it.
 *  If sticky mouse buttons are enabled, a mouse button press will ensure that
 *  @ref glfwGetMouseButton returns `GLFW_PRESS` the next time it is called even
 *  if the mouse button had been released before the call.  This is useful when
 *  you are only interested in whether mouse buttons have been pressed but not
 *  when or in which order.
 *
 *  If the mode is `GLFW_LOCK_KEY_MODS`, the value must be either `GLFW_TRUE` to
 *  enable lock key modifier bits, or `GLFW_FALSE` to disable them.  If enabled,
 *  callbacks that receive modifier bits will also have the @ref
 *  GLFW_MOD_CAPS_LOCK bit set when the event was generated with Caps Lock on,
 *  and the @ref GLFW_MOD_NUM_LOCK bit when Num Lock was on.
 *
 *  If the mode is `GLFW_RAW_MOUSE_MOTION`, the value must be either `GLFW_TRUE`
 *  to enable raw (unscaled and unaccelerated) mouse motion when the cursor is
 *  disabled, or `GLFW_FALSE` to disable it.  If raw motion is not supported,
 *  attempting to set this will emit @ref GLFW_PLATFORM_ERROR.  Call @ref
 *  glfwRawMouseMotionSupported to check for support.
 *
 *  @param[in] window The window whose input mode to set.
 *  @param[in] mode One of `GLFW_CURSOR`, `GLFW_STICKY_KEYS`,
 *  `GLFW_STICKY_MOUSE_BUTTONS`, `GLFW_LOCK_KEY_MODS` or
 *  `GLFW_RAW_MOUSE_MOTION`.
 *  @param[in] value The new value of the specified input mode.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref glfwGetInputMode
 *
 *  @since Added in version 3.0.  Replaces `glfwEnable` and `glfwDisable`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetInputMode(GLFWwindow* window, int mode, int value);

/*! @brief Returns whether raw mouse motion is supported.
 *
 *  This function returns whether raw mouse motion is supported on the current
 *  system.  This status does not change after GLFW has been initialized so you
 *  only need to check this once.  If you attempt to enable raw motion on
 *  a system that does not support it, @ref GLFW_PLATFORM_ERROR will be emitted.
 *
 *  Raw mouse motion is closer to the actual motion of the mouse across
 *  a surface.  It is not affected by the scaling and acceleration applied to
 *  the motion of the desktop cursor.  That processing is suitable for a cursor
 *  while raw motion is better for controlling for example a 3D camera.  Because
 *  of this, raw mouse motion is only provided when the cursor is disabled.
 *
 *  @return `GLFW_TRUE` if raw mouse motion is supported on the current machine,
 *  or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref raw_mouse_motion
 *  @sa @ref glfwSetInputMode
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwRawMouseMotionSupported(void);

/*! @brief Returns the layout-specific name of the specified printable key.
 *
 *  This function returns the name of the specified printable key, encoded as
 *  UTF-8.  This is typically the character that key would produce without any
 *  modifier keys, intended for displaying key bindings to the user.  For dead
 *  keys, it is typically the diacritic it would add to a character.
 *
 *  __Do not use this function__ for [text input](@ref input_char).  You will
 *  break text input for many languages even if it happens to work for yours.
 *
 *  If the key is `GLFW_KEY_UNKNOWN`, the scancode is used to identify the key,
 *  otherwise the scancode is ignored.  If you specify a non-printable key, or
 *  `GLFW_KEY_UNKNOWN` and a scancode that maps to a non-printable key, this
 *  function returns `NULL` but does not emit an error.
 *
 *  This behavior allows you to always pass in the arguments in the
 *  [key callback](@ref input_key) without modification.
 *
 *  The printable keys are:
 *  - `GLFW_KEY_APOSTROPHE`
 *  - `GLFW_KEY_COMMA`
 *  - `GLFW_KEY_MINUS`
 *  - `GLFW_KEY_PERIOD`
 *  - `GLFW_KEY_SLASH`
 *  - `GLFW_KEY_SEMICOLON`
 *  - `GLFW_KEY_EQUAL`
 *  - `GLFW_KEY_LEFT_BRACKET`
 *  - `GLFW_KEY_RIGHT_BRACKET`
 *  - `GLFW_KEY_BACKSLASH`
 *  - `GLFW_KEY_WORLD_1`
 *  - `GLFW_KEY_WORLD_2`
 *  - `GLFW_KEY_0` to `GLFW_KEY_9`
 *  - `GLFW_KEY_A` to `GLFW_KEY_Z`
 *  - `GLFW_KEY_KP_0` to `GLFW_KEY_KP_9`
 *  - `GLFW_KEY_KP_DECIMAL`
 *  - `GLFW_KEY_KP_DIVIDE`
 *  - `GLFW_KEY_KP_MULTIPLY`
 *  - `GLFW_KEY_KP_SUBTRACT`
 *  - `GLFW_KEY_KP_ADD`
 *  - `GLFW_KEY_KP_EQUAL`
 *
 *  Names for printable keys depend on keyboard layout, while names for
 *  non-printable keys are the same across layouts but depend on the application
 *  language and should be localized along with other user interface text.
 *
 *  @param[in] key The key to query, or `GLFW_KEY_UNKNOWN`.
 *  @param[in] scancode The scancode of the key to query.
 *  @return The UTF-8 encoded, layout-specific name of the key, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The contents of the returned string may change when a keyboard
 *  layout change event is received.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key_name
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetKeyName(int key, int scancode);

/*! @brief Returns the platform-specific scancode of the specified key.
 *
 *  This function returns the platform-specific scancode of the specified key.
 *
 *  If the key is `GLFW_KEY_UNKNOWN` or does not exist on the keyboard this
 *  method will return `-1`.
 *
 *  @param[in] key Any [named key](@ref keys).
 *  @return The platform-specific scancode for the key, or `-1` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetKeyScancode(int key);

/*! @brief Returns the last reported state of a keyboard key for the specified
 *  window.
 *
 *  This function returns the last state reported for the specified key to the
 *  specified window.  The returned state is one of `GLFW_PRESS` or
 *  `GLFW_RELEASE`.  The higher-level action `GLFW_REPEAT` is only reported to
 *  the key callback.
 *
 *  If the @ref GLFW_STICKY_KEYS input mode is enabled, this function returns
 *  `GLFW_PRESS` the first time you call it for a key that was pressed, even if
 *  that key has already been released.
 *
 *  The key functions deal with physical keys, with [key tokens](@ref keys)
 *  named after their use on the standard US keyboard layout.  If you want to
 *  input text, use the Unicode character callback instead.
 *
 *  The [modifier key bit masks](@ref mods) are not key tokens and cannot be
 *  used with this function.
 *
 *  __Do not use this function__ to implement [text input](@ref input_char).
 *
 *  @param[in] window The desired window.
 *  @param[in] key The desired [keyboard key](@ref keys).  `GLFW_KEY_UNKNOWN` is
 *  not a valid key for this function.
 *  @return One of `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetKey(GLFWwindow* window, int key);

/*! @brief Returns the last reported state of a mouse button for the specified
 *  window.
 *
 *  This function returns the last state reported for the specified mouse button
 *  to the specified window.  The returned state is one of `GLFW_PRESS` or
 *  `GLFW_RELEASE`.
 *
 *  If the @ref GLFW_STICKY_MOUSE_BUTTONS input mode is enabled, this function
 *  returns `GLFW_PRESS` the first time you call it for a mouse button that was
 *  pressed, even if that mouse button has already been released.
 *
 *  @param[in] window The desired window.
 *  @param[in] button The desired [mouse button](@ref buttons).
 *  @return One of `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_mouse_button
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetMouseButton(GLFWwindow* window, int button);

/*! @brief Retrieves the position of the cursor relative to the content area of
 *  the window.
 *
 *  This function returns the position of the cursor, in screen coordinates,
 *  relative to the upper-left corner of the content area of the specified
 *  window.
 *
 *  If the cursor is disabled (with `GLFW_CURSOR_DISABLED`) then the cursor
 *  position is unbounded and limited only by the minimum and maximum values of
 *  a `double`.
 *
 *  The coordinate can be converted to their integer equivalents with the
 *  `floor` function.  Casting directly to an integer type works for positive
 *  coordinates, but fails for negative ones.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] window The desired window.
 *  @param[out] xpos Where to store the cursor x-coordinate, relative to the
 *  left edge of the content area, or `NULL`.
 *  @param[out] ypos Where to store the cursor y-coordinate, relative to the to
 *  top edge of the content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwSetCursorPos
 *
 *  @since Added in version 3.0.  Replaces `glfwGetMousePos`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);

/*! @brief Sets the position of the cursor, relative to the content area of the
 *  window.
 *
 *  This function sets the position, in screen coordinates, of the cursor
 *  relative to the upper-left corner of the content area of the specified
 *  window.  The window must have input focus.  If the window does not have
 *  input focus when this function is called, it fails silently.
 *
 *  __Do not use this function__ to implement things like camera controls.  GLFW
 *  already provides the `GLFW_CURSOR_DISABLED` cursor mode that hides the
 *  cursor, transparently re-centers it and provides unconstrained cursor
 *  motion.  See @ref glfwSetInputMode for more information.
 *
 *  If the cursor mode is `GLFW_CURSOR_DISABLED` then the cursor position is
 *  unconstrained and limited only by the minimum and maximum values of
 *  a `double`.
 *
 *  @param[in] window The desired window.
 *  @param[in] xpos The desired x-coordinate, relative to the left edge of the
 *  content area.
 *  @param[in] ypos The desired y-coordinate, relative to the top edge of the
 *  content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland This function will only work when the cursor mode is
 *  `GLFW_CURSOR_DISABLED`, otherwise it will do nothing.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwGetCursorPos
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMousePos`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);

/*! @brief Creates a custom cursor.
 *
 *  Creates a new custom cursor image that can be set for a window with @ref
 *  glfwSetCursor.  The cursor can be destroyed with @ref glfwDestroyCursor.
 *  Any remaining cursors are destroyed by @ref glfwTerminate.
 *
 *  The pixels are 32-bit, little-endian, non-premultiplied RGBA, i.e. eight
 *  bits per channel with the red channel first.  They are arranged canonically
 *  as packed sequential rows, starting from the top-left corner.
 *
 *  The cursor hotspot is specified in pixels, relative to the upper-left corner
 *  of the cursor image.  Like all other coordinate systems in GLFW, the X-axis
 *  points to the right and the Y-axis points down.
 *
 *  @param[in] image The desired cursor image.
 *  @param[in] xhot The desired x-coordinate, in pixels, of the cursor hotspot.
 *  @param[in] yhot The desired y-coordinate, in pixels, of the cursor hotspot.
 *  @return The handle of the created cursor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified image data is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwDestroyCursor
 *  @sa @ref glfwCreateStandardCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);

/*! @brief Creates a cursor with a standard shape.
 *
 *  Returns a cursor with a [standard shape](@ref shapes), that can be set for
 *  a window with @ref glfwSetCursor.
 *
 *  @param[in] shape One of the [standard shapes](@ref shapes).
 *  @return A new cursor ready to use or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwCreateCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursor* glfwCreateStandardCursor(int shape);

/*! @brief Destroys a cursor.
 *
 *  This function destroys a cursor previously created with @ref
 *  glfwCreateCursor.  Any remaining cursors will be destroyed by @ref
 *  glfwTerminate.
 *
 *  If the specified cursor is current for any window, that window will be
 *  reverted to the default cursor.  This does not affect the cursor mode.
 *
 *  @param[in] cursor The cursor object to destroy.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwCreateCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI void glfwDestroyCursor(GLFWcursor* cursor);

/*! @brief Sets the cursor for the window.
 *
 *  This function sets the cursor image to be used when the cursor is over the
 *  content area of the specified window.  The set cursor will only be visible
 *  when the [cursor mode](@ref cursor_mode) of the window is
 *  `GLFW_CURSOR_NORMAL`.
 *
 *  On some platforms, the set cursor may not be visible unless the window also
 *  has input focus.
 *
 *  @param[in] window The window to set the cursor for.
 *  @param[in] cursor The cursor to set, or `NULL` to switch back to the default
 *  arrow cursor.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);

/*! @brief Sets the key callback.
 *
 *  This function sets the key callback of the specified window, which is called
 *  when a key is pressed, repeated or released.
 *
 *  The key functions deal with physical keys, with layout independent
 *  [key tokens](@ref keys) named after their values in the standard US keyboard
 *  layout.  If you want to input text, use the
 *  [character callback](@ref glfwSetCharCallback) instead.
 *
 *  When a window loses input focus, it will generate synthetic key release
 *  events for all pressed keys.  You can tell these events from user-generated
 *  events by the fact that the synthetic ones are generated after the focus
 *  loss event has been processed, i.e. after the
 *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
 *
 *  The scancode of a key is specific to that platform or sometimes even to that
 *  machine.  Scancodes are intended to allow users to bind keys that don't have
 *  a GLFW key token.  Such keys have `key` set to `GLFW_KEY_UNKNOWN`, their
 *  state is not saved and so it cannot be queried with @ref glfwGetKey.
 *
 *  Sometimes GLFW needs to generate synthetic key events, in which case the
 *  scancode may be zero.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new key callback, or `NULL` to remove the currently
 *  set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWkeyfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);

/*! @brief Sets the Unicode character callback.
 *
 *  This function sets the character callback of the specified window, which is
 *  called when a Unicode character is input.
 *
 *  The character callback is intended for Unicode text input.  As it deals with
 *  characters, it is keyboard layout dependent, whereas the
 *  [key callback](@ref glfwSetKeyCallback) is not.  Characters do not map 1:1
 *  to physical keys, as a key may produce zero, one or more characters.  If you
 *  want to know whether a specific physical key was pressed or released, see
 *  the key callback instead.
 *
 *  The character callback behaves as system text input normally does and will
 *  not be called if modifier keys are held down that would prevent normal text
 *  input on that platform, for example a Super (Command) key on macOS or Alt key
 *  on Windows.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcharfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_char
 *
 *  @since Added in version 2.4.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun callback);

/*! @brief Sets the Unicode character with modifiers callback.
 *
 *  This function sets the character with modifiers callback of the specified
 *  window, which is called when a Unicode character is input regardless of what
 *  modifier keys are used.
 *
 *  The character with modifiers callback is intended for implementing custom
 *  Unicode character input.  For regular Unicode text input, see the
 *  [character callback](@ref glfwSetCharCallback).  Like the character
 *  callback, the character with modifiers callback deals with characters and is
 *  keyboard layout dependent.  Characters do not map 1:1 to physical keys, as
 *  a key may produce zero, one or more characters.  If you want to know whether
 *  a specific physical key was pressed or released, see the
 *  [key callback](@ref glfwSetKeyCallback) instead.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or an
 *  [error](@ref error_handling) occurred.
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcharmodsfun).
 *
 *  @deprecated Scheduled for removal in version 4.0.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_char
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);

/*! @brief Sets the mouse button callback.
 *
 *  This function sets the mouse button callback of the specified window, which
 *  is called when a mouse button is pressed or released.
 *
 *  When a window loses input focus, it will generate synthetic mouse button
 *  release events for all pressed mouse buttons.  You can tell these events
 *  from user-generated events by the fact that the synthetic ones are generated
 *  after the focus loss event has been processed, i.e. after the
 *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int button, int action, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWmousebuttonfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_mouse_button
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);

/*! @brief Sets the cursor position callback.
 *
 *  This function sets the cursor position callback of the specified window,
 *  which is called when the cursor is moved.  The callback is provided with the
 *  position, in screen coordinates, relative to the upper-left corner of the
 *  content area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, double xpos, double ypos);
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcursorposfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMousePosCallback`.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);

/*! @brief Sets the cursor enter/leave callback.
 *
 *  This function sets the cursor boundary crossing callback of the specified
 *  window, which is called when the cursor enters or leaves the content area of
 *  the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int entered)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcursorenterfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_enter
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun callback);

/*! @brief Sets the scroll callback.
 *
 *  This function sets the scroll callback of the specified window, which is
 *  called when a scrolling device is used, such as a mouse wheel or scrolling
 *  area of a touchpad.
 *
 *  The scroll callback receives all scrolling input, like that from a mouse
 *  wheel or a touchpad scrolling area.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new scroll callback, or `NULL` to remove the
 *  currently set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, double xoffset, double yoffset)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWscrollfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref scrolling
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMouseWheelCallback`.
 *
 *  @ingroup input
 */
GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);

/*! @brief Sets the path drop callback.
 *
 *  This function sets the path drop callback of the specified window, which is
 *  called when one or more dragged paths are dropped on the window.
 *
 *  Because the path array and its strings may have been generated specifically
 *  for that event, they are not guaranteed to be valid after the callback has
 *  returned.  If you wish to use them after the callback returns, you need to
 *  make a deep copy.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new file drop callback, or `NULL` to remove the
 *  currently set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int path_count, const char* paths[])
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWdropfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland File drop is currently unimplemented.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref path_drop
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun callback);

/*! @brief Returns whether the specified joystick is present.
 *
 *  This function returns whether the specified joystick is present.
 *
 *  There is no need to call this function before other functions that accept
 *  a joystick ID, as they all check for presence before performing any other
 *  work.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if the joystick is present, or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickParam`.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickPresent(int jid);

/*! @brief Returns the values of all axes of the specified joystick.
 *
 *  This function returns the values of all axes of the specified joystick.
 *  Each element in the array is a value between -1.0 and 1.0.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of axis values in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of axis values, or `NULL` if the joystick is not present or
 *  an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_axis
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickPos`.
 *
 *  @ingroup input
 */
GLFWAPI const float* glfwGetJoystickAxes(int jid, int* count);

/*! @brief Returns the state of all buttons of the specified joystick.
 *
 *  This function returns the state of all buttons of the specified joystick.
 *  Each element in the array is either `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  For backward compatibility with earlier versions that did not have @ref
 *  glfwGetJoystickHats, the button array also includes all hats, each
 *  represented as four buttons.  The hats are in the same order as returned by
 *  __glfwGetJoystickHats__ and are in the order _up_, _right_, _down_ and
 *  _left_.  To disable these extra buttons, set the @ref
 *  GLFW_JOYSTICK_HAT_BUTTONS init hint before initialization.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of button states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of button states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_button
 *
 *  @since Added in version 2.2.
 *  @glfw3 Changed to return a dynamic array.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickButtons(int jid, int* count);

/*! @brief Returns the state of all hats of the specified joystick.
 *
 *  This function returns the state of all hats of the specified joystick.
 *  Each element in the array is one of the following values:
 *
 *  Name                  | Value
 *  ----                  | -----
 *  `GLFW_HAT_CENTERED`   | 0
 *  `GLFW_HAT_UP`         | 1
 *  `GLFW_HAT_RIGHT`      | 2
 *  `GLFW_HAT_DOWN`       | 4
 *  `GLFW_HAT_LEFT`       | 8
 *  `GLFW_HAT_RIGHT_UP`   | `GLFW_HAT_RIGHT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_RIGHT_DOWN` | `GLFW_HAT_RIGHT` \| `GLFW_HAT_DOWN`
 *  `GLFW_HAT_LEFT_UP`    | `GLFW_HAT_LEFT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_LEFT_DOWN`  | `GLFW_HAT_LEFT` \| `GLFW_HAT_DOWN`
 *
 *  The diagonal directions are bitwise combinations of the primary (up, right,
 *  down and left) directions and you can test for these individually by ANDing
 *  it with the corresponding direction.
 *
 *  @code
 *  if (hats[2] & GLFW_HAT_RIGHT)
 *  {
 *      // State of hat 2 could be right-up, right or right-down
 *  }
 *  @endcode
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of hat states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of hat states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, this function is called again for that joystick or the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_hat
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickHats(int jid, int* count);

/*! @brief Returns the name of the specified joystick.
 *
 *  This function returns the name, encoded as UTF-8, of the specified joystick.
 *  The returned string is allocated and freed by GLFW.  You should not free it
 *  yourself.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_name
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickName(int jid);

/*! @brief Returns the SDL compatible GUID of the specified joystick.
 *
 *  This function returns the SDL compatible GUID, as a UTF-8 encoded
 *  hexadecimal string, of the specified joystick.  The returned string is
 *  allocated and freed by GLFW.  You should not free it yourself.
 *
 *  The GUID is what connects a joystick to a gamepad mapping.  A connected
 *  joystick will always have a GUID even if there is no gamepad mapping
 *  assigned to it.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  The GUID uses the format introduced in SDL 2.0.5.  This GUID tries to
 *  uniquely identify the make and model of a joystick but does not identify
 *  a specific unit, e.g. all wired Xbox 360 controllers will have the same
 *  GUID on that platform.  The GUID for a unit may vary between platforms
 *  depending on what hardware information the platform specific APIs provide.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded GUID of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickGUID(int jid);

/*! @brief Sets the user pointer of the specified joystick.
 *
 *  This function sets the user-defined pointer of the specified joystick.  The
 *  current value is retained until the joystick is disconnected.  The initial
 *  value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwGetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetJoystickUserPointer(int jid, void* pointer);

/*! @brief Returns the user pointer of the specified joystick.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified joystick.  The initial value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwSetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void* glfwGetJoystickUserPointer(int jid);

/*! @brief Returns whether the specified joystick has a gamepad mapping.
 *
 *  This function returns whether the specified joystick is both present and has
 *  a gamepad mapping.
 *
 *  If the specified joystick is present but does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check if a joystick is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if a joystick is both present and has a gamepad mapping,
 *  or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickIsGamepad(int jid);

/*! @brief Sets the joystick configuration callback.
 *
 *  This function sets the joystick configuration callback, or removes the
 *  currently set callback.  This is called when a joystick is connected to or
 *  disconnected from the system.
 *
 *  For joystick connection and disconnection events to be delivered on all
 *  platforms, you need to call one of the [event processing](@ref events)
 *  functions.  Joystick disconnection may also be detected and the callback
 *  called by joystick functions.  The function will then return whatever it
 *  returns if the joystick is not present.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWjoystickfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_event
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);

/*! @brief Adds the specified SDL_GameControllerDB gamepad mappings.
 *
 *  This function parses the specified ASCII encoded string and updates the
 *  internal list with any gamepad mappings it finds.  This string may
 *  contain either a single gamepad mapping or many mappings separated by
 *  newlines.  The parser supports the full format of the `gamecontrollerdb.txt`
 *  source file including empty lines and comments.
 *
 *  See @ref gamepad_mapping for a description of the format.
 *
 *  If there is already a gamepad mapping for a given GUID in the internal list,
 *  it will be replaced by the one passed to this function.  If the library is
 *  terminated and re-initialized the internal list will revert to the built-in
 *  default.
 *
 *  @param[in] string The string containing the gamepad mappings.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *  @sa @ref glfwGetGamepadName
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwUpdateGamepadMappings(const char* string);

/*! @brief Returns the human-readable gamepad name for the specified joystick.
 *
 *  This function returns the human-readable name of the gamepad from the
 *  gamepad mapping assigned to the specified joystick.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `NULL` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the gamepad, or `NULL` if the
 *  joystick is not present, does not have a mapping or an
 *  [error](@ref error_handling) occurred.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, the gamepad mappings are updated or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetGamepadName(int jid);

/*! @brief Retrieves the state of the specified joystick remapped as a gamepad.
 *
 *  This function retrieves the state of the specified joystick remapped to
 *  an Xbox-like gamepad.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  The Guide button may not be available for input as it is often hooked by the
 *  system or the Steam client.
 *
 *  Not all devices have all the buttons or axes provided by @ref
 *  GLFWgamepadstate.  Unavailable buttons and axes will always report
 *  `GLFW_RELEASE` and 0.0 respectively.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] state The gamepad input state of the joystick.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if no joystick is
 *  connected, it has no gamepad mapping or an [error](@ref error_handling)
 *  occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwUpdateGamepadMappings
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetGamepadState(int jid, GLFWgamepadstate* state);

/*! @brief Sets the clipboard to the specified string.
 *
 *  This function sets the system clipboard to the specified, UTF-8 encoded
 *  string.
 *
 *  @param[in] window Deprecated.  Any valid window or `NULL`.
 *  @param[in] string A UTF-8 encoded string.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified string is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa @ref glfwGetClipboardString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetClipboardString(GLFWwindow* window, const char* string);

/*! @brief Returns the contents of the clipboard as a string.
 *
 *  This function returns the contents of the system clipboard, if it contains
 *  or is convertible to a UTF-8 encoded string.  If the clipboard is empty or
 *  if its contents cannot be converted, `NULL` is returned and a @ref
 *  GLFW_FORMAT_UNAVAILABLE error is generated.
 *
 *  @param[in] window Deprecated.  Any valid window or `NULL`.
 *  @return The contents of the clipboard as a UTF-8 encoded string, or `NULL`
 *  if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the next call to @ref
 *  glfwGetClipboardString or @ref glfwSetClipboardString, or until the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa @ref glfwSetClipboardString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetClipboardString(GLFWwindow* window);

/*! @brief Returns the GLFW time.
 *
 *  This function returns the current GLFW time, in seconds.  Unless the time
 *  has been set using @ref glfwSetTime it measures time elapsed since GLFW was
 *  initialized.
 *
 *  This function and @ref glfwSetTime are helper functions on top of @ref
 *  glfwGetTimerFrequency and @ref glfwGetTimerValue.
 *
 *  The resolution of the timer is system dependent, but is usually on the order
 *  of a few micro- or nanoseconds.  It uses the highest-resolution monotonic
 *  time source on each supported platform.
 *
 *  @return The current time, in seconds, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Reading and
 *  writing of the internal base time is not atomic, so it needs to be
 *  externally synchronized with calls to @ref glfwSetTime.
 *
 *  @sa @ref time
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup input
 */
GLFWAPI double glfwGetTime(void);

/*! @brief Sets the GLFW time.
 *
 *  This function sets the current GLFW time, in seconds.  The value must be
 *  a positive finite number less than or equal to 18446744073.0, which is
 *  approximately 584.5 years.
 *
 *  This function and @ref glfwGetTime are helper functions on top of @ref
 *  glfwGetTimerFrequency and @ref glfwGetTimerValue.
 *
 *  @param[in] time The new value, in seconds.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @remark The upper limit of GLFW time is calculated as
 *  floor((2<sup>64</sup> - 1) / 10<sup>9</sup>) and is due to implementations
 *  storing nanoseconds in 64 bits.  The limit may be increased in the future.
 *
 *  @thread_safety This function may be called from any thread.  Reading and
 *  writing of the internal base time is not atomic, so it needs to be
 *  externally synchronized with calls to @ref glfwGetTime.
 *
 *  @sa @ref time
 *
 *  @since Added in version 2.2.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetTime(double time);

/*! @brief Returns the current value of the raw timer.
 *
 *  This function returns the current value of the raw timer, measured in
 *  1&nbsp;/&nbsp;frequency seconds.  To get the frequency, call @ref
 *  glfwGetTimerFrequency.
 *
 *  @return The value of the timer, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref time
 *  @sa @ref glfwGetTimerFrequency
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI uint64_t glfwGetTimerValue(void);

/*! @brief Returns the frequency, in Hz, of the raw timer.
 *
 *  This function returns the frequency, in Hz, of the raw timer.
 *
 *  @return The frequency of the timer, in Hz, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref time
 *  @sa @ref glfwGetTimerValue
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI uint64_t glfwGetTimerFrequency(void);

/*! @brief Makes the context of the specified window current for the calling
 *  thread.
 *
 *  This function makes the OpenGL or OpenGL ES context of the specified window
 *  current on the calling thread.  A context must only be made current on
 *  a single thread at a time and each thread can have only a single current
 *  context at a time.
 *
 *  When moving a context between threads, you must make it non-current on the
 *  old thread before making it current on the new one.
 *
 *  By default, making a context non-current implicitly forces a pipeline flush.
 *  On machines that support `GL_KHR_context_flush_control`, you can control
 *  whether a context performs this flush by setting the
 *  [GLFW_CONTEXT_RELEASE_BEHAVIOR](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_hint)
 *  hint.
 *
 *  The specified window must have an OpenGL or OpenGL ES context.  Specifying
 *  a window without a context will generate a @ref GLFW_NO_WINDOW_CONTEXT
 *  error.
 *
 *  @param[in] window The window whose context to make current, or `NULL` to
 *  detach the current context.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_WINDOW_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_current
 *  @sa @ref glfwGetCurrentContext
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
GLFWAPI void glfwMakeContextCurrent(GLFWwindow* window);

/*! @brief Returns the window whose context is current on the calling thread.
 *
 *  This function returns the window whose OpenGL or OpenGL ES context is
 *  current on the calling thread.
 *
 *  @return The window whose context is current, or `NULL` if no window's
 *  context is current.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_current
 *  @sa @ref glfwMakeContextCurrent
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
GLFWAPI GLFWwindow* glfwGetCurrentContext(void);

/*! @brief Swaps the front and back buffers of the specified window.
 *
 *  This function swaps the front and back buffers of the specified window when
 *  rendering with OpenGL or OpenGL ES.  If the swap interval is greater than
 *  zero, the GPU driver waits the specified number of screen updates before
 *  swapping the buffers.
 *
 *  The specified window must have an OpenGL or OpenGL ES context.  Specifying
 *  a window without a context will generate a @ref GLFW_NO_WINDOW_CONTEXT
 *  error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see `vkQueuePresentKHR` instead.
 *
 *  @param[in] window The window whose buffers to swap.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_WINDOW_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark __EGL:__ The context of the specified window must be current on the
 *  calling thread.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref buffer_swap
 *  @sa @ref glfwSwapInterval
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSwapBuffers(GLFWwindow* window);

/*! @brief Sets the swap interval for the current context.
 *
 *  This function sets the swap interval for the current OpenGL or OpenGL ES
 *  context, i.e. the number of screen updates to wait from the time @ref
 *  glfwSwapBuffers was called before swapping the buffers and returning.  This
 *  is sometimes called _vertical synchronization_, _vertical retrace
 *  synchronization_ or just _vsync_.
 *
 *  A context that supports either of the `WGL_EXT_swap_control_tear` and
 *  `GLX_EXT_swap_control_tear` extensions also accepts _negative_ swap
 *  intervals, which allows the driver to swap immediately even if a frame
 *  arrives a little bit late.  You can check for these extensions with @ref
 *  glfwExtensionSupported.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see the present mode of your swapchain instead.
 *
 *  @param[in] interval The minimum number of screen updates to wait for
 *  until the buffers are swapped by @ref glfwSwapBuffers.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark This function is not called during context creation, leaving the
 *  swap interval set to whatever is the default on that platform.  This is done
 *  because some swap interval extensions used by GLFW do not allow the swap
 *  interval to be reset to zero once it has been set to a non-zero value.
 *
 *  @remark Some GPU drivers do not honor the requested swap interval, either
 *  because of a user setting that overrides the application's request or due to
 *  bugs in the driver.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref buffer_swap
 *  @sa @ref glfwSwapBuffers
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI void glfwSwapInterval(int interval);

/*! @brief Returns whether the specified extension is available.
 *
 *  This function returns whether the specified
 *  [API extension](@ref context_glext) is supported by the current OpenGL or
 *  OpenGL ES context.  It searches both for client API extension and context
 *  creation API extensions.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  As this functions retrieves and searches one or more extension strings each
 *  call, it is recommended that you cache its results if it is going to be used
 *  frequently.  The extension strings will not change during the lifetime of
 *  a context, so there is no danger in doing this.
 *
 *  This function does not apply to Vulkan.  If you are using Vulkan, see @ref
 *  glfwGetRequiredInstanceExtensions, `vkEnumerateInstanceExtensionProperties`
 *  and `vkEnumerateDeviceExtensionProperties` instead.
 *
 *  @param[in] extension The ASCII encoded name of the extension.
 *  @return `GLFW_TRUE` if the extension is available, or `GLFW_FALSE`
 *  otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT, @ref GLFW_INVALID_VALUE and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwGetProcAddress
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI int glfwExtensionSupported(const char* extension);

/*! @brief Returns the address of the specified function for the current
 *  context.
 *
 *  This function returns the address of the specified OpenGL or OpenGL ES
 *  [core or extension function](@ref context_glext), if it is supported
 *  by the current context.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see @ref glfwGetInstanceProcAddress, `vkGetInstanceProcAddr` and
 *  `vkGetDeviceProcAddr` instead.
 *
 *  @param[in] procname The ASCII encoded name of the function.
 *  @return The address of the function, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark The address of a given function is not guaranteed to be the same
 *  between contexts.
 *
 *  @remark This function may return a non-`NULL` address despite the
 *  associated version or extension not being available.  Always check the
 *  context version or extension string first.
 *
 *  @pointer_lifetime The returned function pointer is valid until the context
 *  is destroyed or the library is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwExtensionSupported
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname);

/*! @brief Returns whether the Vulkan loader and an ICD have been found.
 *
 *  This function returns whether the Vulkan loader and any minimally functional
 *  ICD have been found.
 *
 *  The availability of a Vulkan loader and even an ICD does not by itself guarantee that
 *  surface creation or even instance creation is possible.  Call @ref
 *  glfwGetRequiredInstanceExtensions to check whether the extensions necessary for Vulkan
 *  surface creation are available and @ref glfwGetPhysicalDevicePresentationSupport to
 *  check whether a queue family of a physical device supports image presentation.
 *
 *  @return `GLFW_TRUE` if Vulkan is minimally available, or `GLFW_FALSE`
 *  otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_support
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI int glfwVulkanSupported(void);

/*! @brief Returns the Vulkan instance extensions required by GLFW.
 *
 *  This function returns an array of names of Vulkan instance extensions required
 *  by GLFW for creating Vulkan surfaces for GLFW windows.  If successful, the
 *  list will always contain `VK_KHR_surface`, so if you don't require any
 *  additional extensions you can pass this list directly to the
 *  `VkInstanceCreateInfo` struct.
 *
 *  If Vulkan is not available on the machine, this function returns `NULL` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available.
 *
 *  If Vulkan is available but no set of extensions allowing window surface
 *  creation was found, this function returns `NULL`.  You may still use Vulkan
 *  for off-screen rendering and compute work.
 *
 *  @param[out] count Where to store the number of extensions in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of ASCII encoded extension names, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_API_UNAVAILABLE.
 *
 *  @remark Additional extensions may be required by future versions of GLFW.
 *  You should check if any extensions you wish to enable are already in the
 *  returned array, as it is an error to specify an extension more than once in
 *  the `VkInstanceCreateInfo` struct.
 *
 *  @remark @macos GLFW currently supports both the `VK_MVK_macos_surface` and
 *  the newer `VK_EXT_metal_surface` extensions.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  library is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_ext
 *  @sa @ref glfwCreateWindowSurface
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI const char** glfwGetRequiredInstanceExtensions(uint32_t* count);

#if defined(VK_VERSION_1_0)

/*! @brief Returns the address of the specified Vulkan instance function.
 *
 *  This function returns the address of the specified Vulkan core or extension
 *  function for the specified instance.  If instance is set to `NULL` it can
 *  return any function exported from the Vulkan loader, including at least the
 *  following functions:
 *
 *  - `vkEnumerateInstanceExtensionProperties`
 *  - `vkEnumerateInstanceLayerProperties`
 *  - `vkCreateInstance`
 *  - `vkGetInstanceProcAddr`
 *
 *  If Vulkan is not available on the machine, this function returns `NULL` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available.
 *
 *  This function is equivalent to calling `vkGetInstanceProcAddr` with
 *  a platform-specific query of the Vulkan loader as a fallback.
 *
 *  @param[in] instance The Vulkan instance to query, or `NULL` to retrieve
 *  functions related to instance creation.
 *  @param[in] procname The ASCII encoded name of the function.
 *  @return The address of the function, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_API_UNAVAILABLE.
 *
 *  @pointer_lifetime The returned function pointer is valid until the library
 *  is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_proc
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI GLFWvkproc glfwGetInstanceProcAddress(VkInstance instance, const char* procname);

/*! @brief Returns whether the specified queue family can present images.
 *
 *  This function returns whether the specified queue family of the specified
 *  physical device supports presentation to the platform GLFW was built for.
 *
 *  If Vulkan or the required window surface creation instance extensions are
 *  not available on the machine, or if the specified instance was not created
 *  with the required extensions, this function returns `GLFW_FALSE` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available and @ref
 *  glfwGetRequiredInstanceExtensions to check what instance extensions are
 *  required.
 *
 *  @param[in] instance The instance that the physical device belongs to.
 *  @param[in] device The physical device that the queue family belongs to.
 *  @param[in] queuefamily The index of the queue family to query.
 *  @return `GLFW_TRUE` if the queue family supports presentation, or
 *  `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_API_UNAVAILABLE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos This function currently always returns `GLFW_TRUE`, as the
 *  `VK_MVK_macos_surface` and `VK_EXT_metal_surface` extensions do not provide
 *  a `vkGetPhysicalDevice*PresentationSupport` type function.
 *
 *  @thread_safety This function may be called from any thread.  For
 *  synchronization details of Vulkan objects, see the Vulkan specification.
 *
 *  @sa @ref vulkan_present
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI int glfwGetPhysicalDevicePresentationSupport(VkInstance instance, VkPhysicalDevice device, uint32_t queuefamily);

/*! @brief Creates a Vulkan surface for the specified window.
 *
 *  This function creates a Vulkan surface for the specified window.
 *
 *  If the Vulkan loader or at least one minimally functional ICD were not found,
 *  this function returns `VK_ERROR_INITIALIZATION_FAILED` and generates a @ref
 *  GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported to check whether
 *  Vulkan is at least minimally available.
 *
 *  If the required window surface creation instance extensions are not
 *  available or if the specified instance was not created with these extensions
 *  enabled, this function returns `VK_ERROR_EXTENSION_NOT_PRESENT` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref
 *  glfwGetRequiredInstanceExtensions to check what instance extensions are
 *  required.
 *
 *  The window surface cannot be shared with another API so the window must
 *  have been created with the [client api hint](@ref GLFW_CLIENT_API_attrib)
 *  set to `GLFW_NO_API` otherwise it generates a @ref GLFW_INVALID_VALUE error
 *  and returns `VK_ERROR_NATIVE_WINDOW_IN_USE_KHR`.
 *
 *  The window surface must be destroyed before the specified Vulkan instance.
 *  It is the responsibility of the caller to destroy the window surface.  GLFW
 *  does not destroy it for you.  Call `vkDestroySurfaceKHR` to destroy the
 *  surface.
 *
 *  @param[in] instance The Vulkan instance to create the surface in.
 *  @param[in] window The window to create the surface for.
 *  @param[in] allocator The allocator to use, or `NULL` to use the default
 *  allocator.
 *  @param[out] surface Where to store the handle of the surface.  This is set
 *  to `VK_NULL_HANDLE` if an error occurred.
 *  @return `VK_SUCCESS` if successful, or a Vulkan error code if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_API_UNAVAILABLE, @ref GLFW_PLATFORM_ERROR and @ref GLFW_INVALID_VALUE
 *
 *  @remark If an error occurs before the creation call is made, GLFW returns
 *  the Vulkan error code most appropriate for the error.  Appropriate use of
 *  @ref glfwVulkanSupported and @ref glfwGetRequiredInstanceExtensions should
 *  eliminate almost all occurrences of these errors.
 *
 *  @remark @macos This function currently only supports the
 *  `VK_MVK_macos_surface` extension from MoltenVK.
 *
 *  @remark @macos This function creates and sets a `CAMetalLayer` instance for
 *  the window content view, which is required for MoltenVK to function.
 *
 *  @thread_safety This function may be called from any thread.  For
 *  synchronization details of Vulkan objects, see the Vulkan specification.
 *
 *  @sa @ref vulkan_surface
 *  @sa @ref glfwGetRequiredInstanceExtensions
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance, GLFWwindow* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);

#endif /*VK_VERSION_1_0*/


/*************************************************************************
 * Global definition cleanup
 *************************************************************************/

/* ------------------- BEGIN SYSTEM/COMPILER SPECIFIC -------------------- */

#ifdef GLFW_WINGDIAPI_DEFINED
 #undef WINGDIAPI
 #undef GLFW_WINGDIAPI_DEFINED
#endif

#ifdef GLFW_CALLBACK_DEFINED
 #undef CALLBACK
 #undef GLFW_CALLBACK_DEFINED
#endif

/* Some OpenGL related headers need GLAPIENTRY, but it is unconditionally
 * defined by some gl.h variants (OpenBSD) so define it after if needed.
 */
#ifndef GLAPIENTRY
 #define GLAPIENTRY APIENTRY
#endif

/* -------------------- END SYSTEM/COMPILER SPECIFIC --------------------- */


#ifdef __cplusplus
}
#endif

#endif /* _glfw3_h_ */



	#if defined(HEXO_PLATFORM_OS_WINDOWS_10) || defined(HEXO_PLATFORM_OS_WINDOWS_11)
/*************************************************************************
 * GLFW 3.3 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef _glfw3_native_h_
#define _glfw3_native_h_

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************
 * Doxygen documentation
 *************************************************************************/

/*! @file glfw3native.h
 *  @brief The header of the native access functions.
 *
 *  This is the header file of the native access functions.  See @ref native for
 *  more information.
 */
/*! @defgroup native Native access
 *  @brief Functions related to accessing native handles.
 *
 *  **By using the native access functions you assert that you know what you're
 *  doing and how to fix problems caused by using them.  If you don't, you
 *  shouldn't be using them.**
 *
 *  Before the inclusion of @ref glfw3native.h, you may define zero or more
 *  window system API macro and zero or more context creation API macros.
 *
 *  The chosen backends must match those the library was compiled for.  Failure
 *  to do this will cause a link-time error.
 *
 *  The available window API macros are:
 *  * `GLFW_EXPOSE_NATIVE_WIN32`
 *  * `GLFW_EXPOSE_NATIVE_COCOA`
 *  * `GLFW_EXPOSE_NATIVE_X11`
 *  * `GLFW_EXPOSE_NATIVE_WAYLAND`
 *
 *  The available context API macros are:
 *  * `GLFW_EXPOSE_NATIVE_WGL`
 *  * `GLFW_EXPOSE_NATIVE_NSGL`
 *  * `GLFW_EXPOSE_NATIVE_GLX`
 *  * `GLFW_EXPOSE_NATIVE_EGL`
 *  * `GLFW_EXPOSE_NATIVE_OSMESA`
 *
 *  These macros select which of the native access functions that are declared
 *  and which platform-specific headers to include.  It is then up your (by
 *  definition platform-specific) code to handle which of these should be
 *  defined.
 */


/*************************************************************************
 * System headers and types
 *************************************************************************/

#if defined(GLFW_EXPOSE_NATIVE_WIN32) || defined(GLFW_EXPOSE_NATIVE_WGL)
 // This is a workaround for the fact that glfw3.h needs to export APIENTRY (for
 // example to allow applications to correctly declare a GL_KHR_debug callback)
 // but windows.h assumes no one will define APIENTRY before it does
 #if defined(GLFW_APIENTRY_DEFINED)
  #undef APIENTRY
  #undef GLFW_APIENTRY_DEFINED
 #endif
 #include <windows.h>
#elif defined(GLFW_EXPOSE_NATIVE_COCOA) || defined(GLFW_EXPOSE_NATIVE_NSGL)
 #if defined(__OBJC__)
  #import <Cocoa/Cocoa.h>
 #else
  #include <ApplicationServices/ApplicationServices.h>
  typedef void* id;
 #endif
#elif defined(GLFW_EXPOSE_NATIVE_X11) || defined(GLFW_EXPOSE_NATIVE_GLX)
 #include <X11/Xlib.h>
 #include <X11/extensions/Xrandr.h>
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
 #include <wayland-client.h>
#endif

#if defined(GLFW_EXPOSE_NATIVE_WGL)
 /* WGL is declared by windows.h */
#endif
#if defined(GLFW_EXPOSE_NATIVE_NSGL)
 /* NSGL is declared by Cocoa.h */
#endif
#if defined(GLFW_EXPOSE_NATIVE_GLX)
 #include <GL/glx.h>
#endif
#if defined(GLFW_EXPOSE_NATIVE_EGL)
 #include <EGL/egl.h>
#endif
#if defined(GLFW_EXPOSE_NATIVE_OSMESA)
 #include <GL/osmesa.h>
#endif


/*************************************************************************
 * Functions
 *************************************************************************/

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
/*! @brief Returns the adapter device name of the specified monitor.
 *
 *  @return The UTF-8 encoded adapter device name (for example `\\.\DISPLAY1`)
 *  of the specified monitor, or `NULL` if an [error](@ref error_handling)
 *  occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup native
 */
GLFWAPI const char* glfwGetWin32Adapter(GLFWmonitor* monitor);

/*! @brief Returns the display device name of the specified monitor.
 *
 *  @return The UTF-8 encoded display device name (for example
 *  `\\.\DISPLAY1\Monitor0`) of the specified monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup native
 */
GLFWAPI const char* glfwGetWin32Monitor(GLFWmonitor* monitor);

/*! @brief Returns the `HWND` of the specified window.
 *
 *  @return The `HWND` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark The `HDC` associated with the window can be queried with the
 *  [GetDC](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc)
 *  function.
 *  @code
 *  HDC dc = GetDC(glfwGetWin32Window(window));
 *  @endcode
 *  This DC is private and does not need to be released.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI HWND glfwGetWin32Window(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_WGL)
/*! @brief Returns the `HGLRC` of the specified window.
 *
 *  @return The `HGLRC` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @remark The `HDC` associated with the window can be queried with the
 *  [GetDC](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc)
 *  function.
 *  @code
 *  HDC dc = GetDC(glfwGetWin32Window(window));
 *  @endcode
 *  This DC is private and does not need to be released.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI HGLRC glfwGetWGLContext(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_COCOA)
/*! @brief Returns the `CGDirectDisplayID` of the specified monitor.
 *
 *  @return The `CGDirectDisplayID` of the specified monitor, or
 *  `kCGNullDirectDisplay` if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup native
 */
GLFWAPI CGDirectDisplayID glfwGetCocoaMonitor(GLFWmonitor* monitor);

/*! @brief Returns the `NSWindow` of the specified window.
 *
 *  @return The `NSWindow` of the specified window, or `nil` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI id glfwGetCocoaWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_NSGL)
/*! @brief Returns the `NSOpenGLContext` of the specified window.
 *
 *  @return The `NSOpenGLContext` of the specified window, or `nil` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI id glfwGetNSGLContext(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_X11)
/*! @brief Returns the `Display` used by GLFW.
 *
 *  @return The `Display` used by GLFW, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI Display* glfwGetX11Display(void);

/*! @brief Returns the `RRCrtc` of the specified monitor.
 *
 *  @return The `RRCrtc` of the specified monitor, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup native
 */
GLFWAPI RRCrtc glfwGetX11Adapter(GLFWmonitor* monitor);

/*! @brief Returns the `RROutput` of the specified monitor.
 *
 *  @return The `RROutput` of the specified monitor, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup native
 */
GLFWAPI RROutput glfwGetX11Monitor(GLFWmonitor* monitor);

/*! @brief Returns the `Window` of the specified window.
 *
 *  @return The `Window` of the specified window, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI Window glfwGetX11Window(GLFWwindow* window);

/*! @brief Sets the current primary selection to the specified string.
 *
 *  @param[in] string A UTF-8 encoded string.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified string is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa glfwGetX11SelectionString
 *  @sa glfwSetClipboardString
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup native
 */
GLFWAPI void glfwSetX11SelectionString(const char* string);

/*! @brief Returns the contents of the current primary selection as a string.
 *
 *  If the selection is empty or if its contents cannot be converted, `NULL`
 *  is returned and a @ref GLFW_FORMAT_UNAVAILABLE error is generated.
 *
 *  @return The contents of the selection as a UTF-8 encoded string, or `NULL`
 *  if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW. You
 *  should not free it yourself. It is valid until the next call to @ref
 *  glfwGetX11SelectionString or @ref glfwSetX11SelectionString, or until the
 *  library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa glfwSetX11SelectionString
 *  @sa glfwGetClipboardString
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup native
 */
GLFWAPI const char* glfwGetX11SelectionString(void);
#endif

#if defined(GLFW_EXPOSE_NATIVE_GLX)
/*! @brief Returns the `GLXContext` of the specified window.
 *
 *  @return The `GLXContext` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI GLXContext glfwGetGLXContext(GLFWwindow* window);

/*! @brief Returns the `GLXWindow` of the specified window.
 *
 *  @return The `GLXWindow` of the specified window, or `None` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup native
 */
GLFWAPI GLXWindow glfwGetGLXWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_WAYLAND)
/*! @brief Returns the `struct wl_display*` used by GLFW.
 *
 *  @return The `struct wl_display*` used by GLFW, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup native
 */
GLFWAPI struct wl_display* glfwGetWaylandDisplay(void);

/*! @brief Returns the `struct wl_output*` of the specified monitor.
 *
 *  @return The `struct wl_output*` of the specified monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup native
 */
GLFWAPI struct wl_output* glfwGetWaylandMonitor(GLFWmonitor* monitor);

/*! @brief Returns the main `struct wl_surface*` of the specified window.
 *
 *  @return The main `struct wl_surface*` of the specified window, or `NULL` if
 *  an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup native
 */
GLFWAPI struct wl_surface* glfwGetWaylandWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_EGL)
/*! @brief Returns the `EGLDisplay` used by GLFW.
 *
 *  @return The `EGLDisplay` used by GLFW, or `EGL_NO_DISPLAY` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI EGLDisplay glfwGetEGLDisplay(void);

/*! @brief Returns the `EGLContext` of the specified window.
 *
 *  @return The `EGLContext` of the specified window, or `EGL_NO_CONTEXT` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI EGLContext glfwGetEGLContext(GLFWwindow* window);

/*! @brief Returns the `EGLSurface` of the specified window.
 *
 *  @return The `EGLSurface` of the specified window, or `EGL_NO_SURFACE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup native
 */
GLFWAPI EGLSurface glfwGetEGLSurface(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_OSMESA)
/*! @brief Retrieves the color buffer associated with the specified window.
 *
 *  @param[in] window The window whose color buffer to retrieve.
 *  @param[out] width Where to store the width of the color buffer, or `NULL`.
 *  @param[out] height Where to store the height of the color buffer, or `NULL`.
 *  @param[out] format Where to store the OSMesa pixel format of the color
 *  buffer, or `NULL`.
 *  @param[out] buffer Where to store the address of the color buffer, or
 *  `NULL`.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup native
 */
GLFWAPI int glfwGetOSMesaColorBuffer(GLFWwindow* window, int* width, int* height, int* format, void** buffer);

/*! @brief Retrieves the depth buffer associated with the specified window.
 *
 *  @param[in] window The window whose depth buffer to retrieve.
 *  @param[out] width Where to store the width of the depth buffer, or `NULL`.
 *  @param[out] height Where to store the height of the depth buffer, or `NULL`.
 *  @param[out] bytesPerValue Where to store the number of bytes per depth
 *  buffer element, or `NULL`.
 *  @param[out] buffer Where to store the address of the depth buffer, or
 *  `NULL`.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup native
 */
GLFWAPI int glfwGetOSMesaDepthBuffer(GLFWwindow* window, int* width, int* height, int* bytesPerValue, void** buffer);

/*! @brief Returns the `OSMesaContext` of the specified window.
 *
 *  @return The `OSMesaContext` of the specified window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NO_WINDOW_CONTEXT and @ref
 *  GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup native
 */
GLFWAPI OSMesaContext glfwGetOSMesaContext(GLFWwindow* window);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _glfw3_native_h_ */


	#endif
#endif






#ifdef HEXO_GRAPHICS_ENABLE_ERRORS
	#define HX_GRAPHICS_ERROR_PRINT(str) HXRC_PRINT(str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_ERROR_RETURN(str) HXRC_RETURN_FATAL(str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_WARNING_RETURN(str) HXRC_RETURN_WARNING(str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_ERROR_ASSERT(x, str) HXRC_ASSERT_FATAL(x, str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_WARNING_ASSERT(x, str) HXRC_ASSERT_WARNING(x, str, HX_GRAPHICS_ERROR)

    #define HX_GRAPHICS_ERROR_RETURN_BOOL(str) HXRC_RETURN_BOOL_FATAL(str, HX_GRAPHICS_ERROR)
    #define HX_GRAPHICS_WARNING_RETURN_BOOL(str) HXRC_RETURN_BOOL_WARNING(str, HX_GRAPHICS_ERROR)
    #define HX_GRAPHICS_ERROR_ASSERT_RETURN_BOOL(x, str) HXRC_ASSERT_FATAL_RETURN_BOOL(x, str, HX_GRAPHICS_ERROR)
    #define HX_GRAPHICS_WARNING_ASSERT_RETURN_BOOL(x, str) HXRC_ASSERT_WARNING_RETURN_BOOL(x, str, HX_GRAPHICS_ERROR)

	#define HX_GRAPHICS_ERROR_NO_RETURN(str) HXRC_NO_RETURN_FATAL(str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_WARNING_NO_RETURN(str) HXRC_NO_RETURN_WARNING(str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_ERROR_ASSERT_NO_RETURN(x, str) HXRC_ASSERT_FATAL_NO_RETURN(x, str, HX_GRAPHICS_ERROR)
	#define HX_GRAPHICS_WARNING_ASSERT_NO_RETURN(x, str) HXRC_ASSERT_WARNING_NO_RETURN(x, str, HX_GRAPHICS_ERROR)

    #define HX_GRAPHICS_ERROR_ASSERT_MIN(x, str) HX_GRAPHICS_ERROR_ASSERT(x, str)
	#define HX_GRAPHICS_WARNING_ASSERT_MIN(x, str) HX_GRAPHICS_WARNING_ASSERT(x, str)
    #define HX_GRAPHICS_ERROR_ASSERT_NO_RETURN_MIN(x, str) HX_GRAPHICS_ERROR_ASSERT_NO_RETURN(x, str)
	#define HX_GRAPHICS_WARNING_ASSERT_NO_RETURN_MIN(x, str) HX_GRAPHICS_WARNING_ASSERT_NO_RETURN(x, str)

#else
	#define HX_GRAPHICS_ERROR_PRINT(str)
	#define HX_GRAPHICS_ERROR_RETURN(str) return HXRC_FATAL;
	#define HX_GRAPHICS_WARNING_RETURN(str) return HXRC_WARNING;
	#define HX_GRAPHICS_ERROR_ASSERT(x, str) if (!(x)){ return HXRC_FATAL; }
	#define HX_GRAPHICS_WARNING_ASSERT(x, str) if (!(x)){ return HXRC_WARNING; }

    #define HX_GRAPHICS_ERROR_RETURN_BOOL(str) return false;
    #define HX_GRAPHICS_WARNING_RETURN_BOOL(str) return false;
    #define HX_GRAPHICS_ERROR_ASSERT_RETURN_BOOL(x, str) if (!(x)){ return false; }
    #define HX_GRAPHICS_WARNING_ASSERT_RETURN_BOOL(x, str) if (!(x)){ return false; }

	#define HX_GRAPHICS_ERROR_NO_RETURN(str)
	#define HX_GRAPHICS_WARNING_NO_RETURN(str)
	#define HX_GRAPHICS_ERROR_ASSERT_NO_RETURN(x, str) (x);
	#define HX_GRAPHICS_WARNING_ASSERT_NO_RETURN(x, str) (x);

    #define HX_GRAPHICS_ERROR_ASSERT_MIN(x, str) (x);
    #define HX_GRAPHICS_WARNING_ASSERT_MIN(x, str) (x);
    #define HX_GRAPHICS_ERROR_ASSERT_NO_RETURN_MIN(x, str) (x);
	#define HX_GRAPHICS_WARNING_ASSERT_NO_RETURN_MIN(x, str) (x);
#endif











namespace Hexo {

    class GraphicsEngine;


    namespace Graphics {

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// Error app
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Register Error App for graphics engine
        struct ErrorApp {
            size_t hxrc_error_app = 0;
            static inline ErrorApp* Get(){ static ErrorApp e; return &e; }
        };

	#ifdef HEXO_GRAPHICS_ENABLE_ERRORS
        #define HX_GRAPHICS_ERROR Graphics::ErrorApp::Get()->hxrc_error_app
		template<typename F>
		static inline void SetErrorCallback(F&& func){
            HX_GRAPHICS_ERROR = HXRC_REGISTER_APP(func);
        }
	#else
		#define HX_GRAPHICS_ERROR 0
		template<typename F>
		static inline void SetErrorCallback(F&& func){}
	#endif

        /////////////////////////////////////////////////////////////////////////////////////////////////////


    }
}






#define HX_GRAPHICS_DEF_FRAME_COUNT 2
#define HX_GRAPHICS_DEF_MAX_RENDER_TARGETS_BOUND 8
#define HX_GRAPHICS_DEF_MAX_VERTEX_ATTRIBUTES 16




#ifndef HEXO_GRAPHICS_SHADER_H
#define HEXO_GRAPHICS_SHADER_H




namespace Hexo {
	namespace ShaderC {
        struct RHI_ShaderBinary;
        struct RHI_ShaderMetadata;


        typedef uint8_t ShaderUniformLayoutFlag;
        enum : ShaderUniformLayoutFlag {
            HX_SHADERC_UNIFORM_FLAG_DEFAULT = 0,
            HX_SHADERC_UNIFORM_FLAG_BINDLESS = (1 << 0),
            HX_SHADERC_UNIFORM_FLAG_EXPLICIT_VISIBILITY = (1 << 1),
            HX_SHADERC_UNIFORM_FLAG_USAGE_DYNAMIC = (1 << 2),
            HX_SHADERC_UNIFORM_FLAG_USE_HEAP = (1 << 3),
            HX_SHADERC_UNIFORM_FLAG_STATIC_SAMPLER = (1 << 3),
        };

        typedef uint8_t HXInputElementType;
        enum : HXInputElementType {
            HX_GRAPHICS_IA_INPUT_PER_VERTEX,
            HX_GRAPHICS_IA_INPUT_PER_INSTANCE,
        };


        struct HXShaderUniformLayout {
            const char* Name;
            ShaderUniformLayoutFlag Flags;
            HXUINTP Data;
        };


        struct HXInputElementLayout {
            uint8_t InputSlot;
        	HXSIZE Offset;
            HXInputElementType Type;
            HXSIZE InstancesPerElement;
        };


	#ifdef HEXO_GRAPHICS_RHI_GLES3
		typedef std::string RHI_ShaderBinary;
		struct RHI_ShaderMetadata {
			Utils::TypedVector<std::string> push_constants;
		};
	#endif

	#ifdef HEXO_GRAPHICS_RHI_VULKAN
		struct RHI_ShaderBinary {
            std::vector<uint32_t> data;
        };
		struct RHI_ShaderMetadata {};
	#endif

	#ifdef HEXO_GRAPHICS_RHI_D3D12
		struct RHI_ShaderBinary {
            void* ptr;
            size_t size;
        };

        struct RHI_ShaderMetadata {
            Utils::TypedVector<D3D12_INPUT_ELEMENT_DESC> inputElements;
            Utils::TypedVector<D3D12_DESCRIPTOR_RANGE1> descriptorRanges;
            Utils::TypedVector<D3D12_ROOT_PARAMETER1> rootParameters;
            Utils::TypedVector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
        };
	#endif

	#ifdef HEXO_GRAPHICS_RHI_MOLTENVK
		typedef char* RHI_ShaderBinary;
		struct RHI_ShaderMetadata {};
	#endif


		typedef uint8_t Language;
		enum : Language {
			HX_SHADERC_LANG_GLSL,
			HX_SHADERC_LANG_HLSL,
			HX_SHADERC_LANG_HXSL,
		};


		typedef uint8_t ShaderType;
		enum : ShaderType {
			HX_SHADERC_TYPE_VERTEX,
			HX_SHADERC_TYPE_TESSELATION_CONT,
			HX_SHADERC_TYPE_TESSELATION_EVAL,
			HX_SHADERC_TYPE_GEOMETRY,
			HX_SHADERC_TYPE_FRAGMENT,
			HX_SHADERC_TYPE_COMPUTE,
			HX_SHADERC_TYPE_NUMBER_OF_STAGES
		};


		struct CompileOptions {
			Language language;
			ShaderType type;
			std::string defines;
		};


		HXRC LoadShaderPipeline(
            HXUINTP data, HXUINTP pipelinePtr, HXUINTP pipelineConfigPtr, HXUINTP uniformMap,
            HXShaderUniformLayout* shaderLayout=NULL, HXSIZE shaderLayoutCount=0,
            HXInputElementLayout* inputLayout=NULL, HXSIZE inputLayoutCount=0
        ) noexcept;

	}
}




#endif /* end of include guard: HEXO_GRAPHICS_SHADER_H */



#ifndef HEXO_GRAPHICS_RHI_IMPL_H
#define HEXO_GRAPHICS_RHI_IMPL_H


#define HX_GRAPHICS_STORAGEBUFFER_TYPE(type) struct ALIGN(32) type



namespace Hexo {
    namespace Graphics {


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// RHI Settings and Enums
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        typedef uint8_t InputFormat;
        enum : InputFormat {
            HX_INPUT_FORMAT_NONE = 0,
            HX_R8_UINT,
            HX_R8_G8_UINT,
            HX_R8_G8_B8_A8_UINT,
            HX_R8_UNORM,
            HX_R8_G8_UNORM,
            HX_R8_G8_B8_A8_UNORM,

            HX_B8_G8_R8_UNORM,
            HX_B8_G8_R8_A8_UNORM,

            HX_R10_G10_B10_A2_UNORM,
            HX_R10_G10_B10_A2_UINT,
            HX_R11_G11_B10_FLOAT,

            HX_R16_UINT,
            HX_R16_G16_UINT,
            HX_R16_G16_B16_A16_UINT,
            HX_R16_FLOAT,
            HX_R16_G16_FLOAT,
            HX_R16_G16_B16_A16_FLOAT,

            HX_R32_UINT,
            HX_R32_G32_UINT,
            HX_R32_G32_B32_UINT,
            HX_R32_G32_B32_A32_UINT,
            HX_R32_FLOAT,
            HX_R32_G32_FLOAT,
            HX_R32_G32_B32_FLOAT,
            HX_R32_G32_B32_A32_FLOAT,

            HX_D16_UNORM,
            HX_D32_FLOAT,
            HX_D24_UNORM_S8_UINT,
            HX_D32_FLOAT_S8_UINT,
        };

        static inline Mathgl::uvec2 GetChannelsBitsFromInputFormat(const InputFormat& f) {
            using namespace Mathgl;
            switch (f) {
                case HX_R8_UINT:                return uvec2{1, 8};
                case HX_R8_G8_UINT:             return uvec2{2, 8};
                case HX_R8_G8_B8_A8_UINT:       return uvec2{4, 8};
        		case HX_R8_UNORM:               return uvec2{1, 8};
                case HX_R8_G8_UNORM:            return uvec2{2, 8};
                case HX_R8_G8_B8_A8_UNORM:      return uvec2{4, 8};

                case HX_B8_G8_R8_UNORM:         return uvec2{3, 8};
                case HX_B8_G8_R8_A8_UNORM:      return uvec2{4, 8};

                case HX_R10_G10_B10_A2_UNORM:   return uvec2{4, 10};
                case HX_R10_G10_B10_A2_UINT:    return uvec2{4, 10};
                case HX_R11_G11_B10_FLOAT:      return uvec2{3, 11};

                case HX_R16_UINT:               return uvec2{1, 16};
                case HX_R16_G16_UINT:           return uvec2{2, 16};
                case HX_R16_G16_B16_A16_UINT:   return uvec2{4, 16};
                case HX_R16_FLOAT:              return uvec2{1, 16};
                case HX_R16_G16_FLOAT:          return uvec2{2, 16};
                case HX_R16_G16_B16_A16_FLOAT:  return uvec2{4, 16};

                case HX_R32_UINT:               return uvec2{1, 32};
                case HX_R32_G32_UINT:           return uvec2{2, 32};
                case HX_R32_G32_B32_UINT:       return uvec2{3, 32};
                case HX_R32_G32_B32_A32_UINT:   return uvec2{4, 32};
                case HX_R32_FLOAT:              return uvec2{1, 32};
                case HX_R32_G32_FLOAT:          return uvec2{2, 32};
                case HX_R32_G32_B32_FLOAT:      return uvec2{3, 32};
                case HX_R32_G32_B32_A32_FLOAT:  return uvec2{4, 32};

                case HX_D16_UNORM:              return uvec2{1, 16};
                case HX_D32_FLOAT:              return uvec2{1, 32};
                case HX_D24_UNORM_S8_UINT:      return uvec2{2, 24};
                case HX_D32_FLOAT_S8_UINT:      return uvec2{2, 32};
            }

            return uvec2(0,0);
        }



        typedef uint8_t ShaderInputType;
        enum : ShaderInputType {
            HX_GRAPHICS_SHADER_IMAGE,
            HX_GRAPHICS_SHADER_IMAGE1D,
            HX_GRAPHICS_SHADER_IMAGE2D,
            HX_GRAPHICS_SHADER_IMAGE3D,
            HX_GRAPHICS_SHADER_IMAGECUBE,
            HX_GRAPHICS_SHADER_IMAGE1DARRAY,
            HX_GRAPHICS_SHADER_IMAGE2DARRAY,

            HX_GRAPHICS_SHADER_TEXTURE,
            HX_GRAPHICS_SHADER_TEXTURE1D,
            HX_GRAPHICS_SHADER_TEXTURE2D,
            HX_GRAPHICS_SHADER_TEXTURE3D,
            HX_GRAPHICS_SHADER_TEXTURECUBE,
            HX_GRAPHICS_SHADER_TEXTURE1DARRAY,
            HX_GRAPHICS_SHADER_TEXTURE2DARRAY,

            HX_GRAPHICS_SHADER_SAMPLER,

            HX_GRAPHICS_SHADER_UBUFFER,
            HX_GRAPHICS_SHADER_SSBUFFER,
            HX_GRAPHICS_SHADER_PUSHCONSTANT,
            HX_GRAPHICS_SHADER_ATOMICCOUNTER,

        };

        typedef uint8_t ShaderInputUsage;
        enum : ShaderInputUsage {
            HX_GRAPHICS_SHADER_INPUT_USAGE_STATIC,
            HX_GRAPHICS_SHADER_INPUT_USAGE_DYNAMIC
        };



        typedef uint8_t TextureFilteringMode;
        enum : TextureFilteringMode {
            HX_GRAPHICS_SAMPLE_LINEAR,
            HX_GRAPHICS_SAMPLE_MIPMAP,
            HX_GRAPHICS_SAMPLE_NEAREST,
            HX_GRAPHICS_SAMPLE_ANISOTROPIC,
        };

        typedef uint8_t TextureWrapMode;
        enum : TextureWrapMode {
            HX_GRAPHICS_WRAP_CLAMP,
            HX_GRAPHICS_WRAP_REPEAT,
            HX_GRAPHICS_WRAP_MIRROR_REPEAT
        };

        typedef uint8_t StorageResourceType;
        enum : StorageResourceType {
            HX_GRAPHICS_STORAGEBUFFER,
            HX_GRAPHICS_INDEXBUFFER,
            HX_GRAPHICS_VERTEXBUFFER,
            HX_GRAPHICS_SHADERBUFFER,
            HX_GRAPHICS_UNIFORMBUFFER,
            HX_GRAPHICS_INDIRECTBUFFER,
            HX_GRAPHICS_ATOMICCOUNTER_BUFFER,

            HX_GRAPHICS_TEXTURE_1D,
            HX_GRAPHICS_TEXTURE_2D,
            HX_GRAPHICS_TEXTURE_3D,
            HX_GRAPHICS_TEXTURE_RENDERTARGET,
            HX_GRAPHICS_TEXTURE_DEPTHSTENCIL,
            HX_GRAPHICS_TEXTURE_CUBE_POSITIVE_X,
            HX_GRAPHICS_TEXTURE_CUBE_NEGATIVE_X,
            HX_GRAPHICS_TEXTURE_CUBE_POSITIVE_Y,
            HX_GRAPHICS_TEXTURE_CUBE_NEGATIVE_Y,
            HX_GRAPHICS_TEXTURE_CUBE_POSITIVE_Z,
            HX_GRAPHICS_TEXTURE_CUBE_NEGATIVE_Z,
        };



        typedef uint8_t CommandBufferType;
        enum : CommandBufferType {
            HX_GRAPHICS_CMDBUFFER_GRAPHICS,
            HX_GRAPHICS_CMDBUFFER_COMPUTE,
            HX_GRAPHICS_CMDBUFFER_COPY,
            // HX_GRAPHICS_CMDBUFFER_RESOURCE_COPY
        };

        typedef uint8_t CommandBufferUsage;
        enum : CommandBufferUsage {
            HX_GRAPHICS_CMDBUFFER_ONCE,
            HX_GRAPHICS_CMDBUFFER_REUSE,
            HX_GRAPHICS_CMDBUFFER_SECONDARY,    // this type of command buffer must only be recorded in a single thread
        };


        typedef uint8_t ResourceUsage;
        enum : ResourceUsage {
            // HX_GRAPHICS_RESOURCE_USAGE_GPU_RW,          // shader writeable buffer, static vertex buffer, not mappable
            // HX_GRAPHICS_RESOURCE_USAGE_CPU_W_GPU_RW,    // uniform buffer (upload buffer), mappable
            // HX_GRAPHICS_RESOURCE_USAGE_CPU_R_GPU_RW,    // shader writeable buffer, mappable
            // HX_GRAPHICS_RESOURCE_USAGE_CPU_RW_GPU_RW,   // shader writeable buffer, fully mappable (performance penalty)

            HX_GRAPHICS_USAGE_GPU_R = (1 << 0),
            HX_GRAPHICS_USAGE_GPU_W = (1 << 1),
            HX_GRAPHICS_USAGE_CPU_R = (1 << 2),
            HX_GRAPHICS_USAGE_CPU_W = (1 << 3),
        };

        typedef uint32_t ResourceState;
        enum : ResourceState {
            HX_GRAPHICS_RESOURCE_STATE_DEFAULT = 1,
            HX_GRAPHICS_RESOURCE_STATE_VERTEX_BUFFER,
            HX_GRAPHICS_RESOURCE_STATE_INDEX_BUFFER,
            HX_GRAPHICS_RESOURCE_STATE_STORAGE_BUFFER,
            HX_GRAPHICS_RESOURCE_STATE_SHADER_RESOURCE,
            HX_GRAPHICS_RESOURCE_STATE_RENDER_TARGET,
            HX_GRAPHICS_RESOURCE_STATE_WINDOW_PRESENT,
            HX_GRAPHICS_RESOURCE_STATE_UNORDERED_ACCESS,
            HX_GRAPHICS_RESOURCE_STATE_COPY_DST,
            HX_GRAPHICS_RESOURCE_STATE_COPY_SRC,
        };

        typedef uint8_t MemoryAccess;
        enum : MemoryAccess {
            HX_GRAPHICS_MEM_ACCESS_R,
            HX_GRAPHICS_MEM_ACCESS_W,
            HX_GRAPHICS_MEM_ACCESS_RW,
        };

        typedef uint8_t CopyOperation;
        enum : CopyOperation {
            HX_GRAPHICS_COPY_CPU_GPU,
            HX_GRAPHICS_COPY_GPU_GPU,
            HX_GRAPHICS_COPY_GPU_CPU,
        };



        typedef uint8_t DrawMode;
        enum : DrawMode {
            HX_GRAPHICS_DRAW_TRIANGLE,
            HX_GRAPHICS_DRAW_TRIANGLE_STRIP,
            HX_GRAPHICS_DRAW_TRIANGLE_LIST,
            HX_GRAPHICS_DRAW_LINES,
            HX_GRAPHICS_DRAW_LINE_STRIP,
            HX_GRAPHICS_DRAW_LINE_LOOP,
            HX_GRAPHICS_DRAW_POINTS,
        };



        typedef uint8_t RenderPassFlags;
        enum : RenderPassFlags {
            HX_GRAPHICS_RENDERPASS_COLOUR = (1 << 0),
            HX_GRAPHICS_RENDERPASS_DEPTH = (1 << 1),
            HX_GRAPHICS_RENDERPASS_STENCIL = (1 << 2),
        };



        typedef uint8_t ColorBlendType;
        enum : ColorBlendType {
            HX_GRAPHICS_BLEND_NONE,
            HX_GRAPHICS_BLEND_ZERO,
            HX_GRAPHICS_BLEND_ONE,
            HX_GRAPHICS_BLEND_SRC_COLOR,
            HX_GRAPHICS_BLEND_ONE_MINUS_SRC_COLOR,
            HX_GRAPHICS_BLEND_DST_COLOR,
            HX_GRAPHICS_BLEND_ONE_MINUS_DST_COLOR,
            HX_GRAPHICS_BLEND_SRC_ALPHA,
            HX_GRAPHICS_BLEND_ONE_MINUS_SRC_ALPHA,
            HX_GRAPHICS_BLEND_DST_ALPHA,
            HX_GRAPHICS_BLEND_ONE_MINUS_DST_ALPHA,
            HX_GRAPHICS_BLEND_CONSTANT_COLOR,
            HX_GRAPHICS_BLEND_ONE_MINUS_CONSTANT_COLOR,
            HX_GRAPHICS_BLEND_CONSTANT_ALPHA,
            HX_GRAPHICS_BLEND_ONE_MINUS_CONSTANT_ALPHA,
            HX_GRAPHICS_BLEND_SRC_ALPHA_SATURATE,
        };

        typedef uint8_t ColorBlendOperation;
        enum : ColorBlendOperation {
            HX_GRAPHICS_BLEND_OP_NONE,
            HX_GRAPHICS_BLEND_OP_ADD,
            HX_GRAPHICS_BLEND_OP_SUBTRACT,
            HX_GRAPHICS_BLEND_OP_REV_SUBTRACT,
            HX_GRAPHICS_BLEND_OP_MIN,
            HX_GRAPHICS_BLEND_OP_MAX,
        };

        typedef uint8_t CompareOperation;
        enum : CompareOperation {
            HX_GRAPHICS_COMP_OP_NONE,
            HX_GRAPHICS_COMP_OP_NEVER,
            HX_GRAPHICS_COMP_OP_LESS,
            HX_GRAPHICS_COMP_OP_EQUAL,
            HX_GRAPHICS_COMP_OP_LESS_EQUAL,
            HX_GRAPHICS_COMP_OP_GREATER,
            HX_GRAPHICS_COMP_OP_NOT_EQUAL,
            HX_GRAPHICS_COMP_OP_GREATER_EQUAL,
            HX_GRAPHICS_COMP_OP_ALWAYS,
        };

        typedef uint8_t CullOperation;
        enum : CullOperation {
            HX_GRAPHICS_CULL_OP_NONE,
            HX_GRAPHICS_CULL_OP_FRONT,
            HX_GRAPHICS_CULL_OP_BACK,
        };

        typedef uint8_t FillOperation;
        enum : FillOperation {
            HX_GRAPHICS_FILL_OP_SOLID,
            HX_GRAPHICS_FILL_OP_WIREFRAME,
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////















        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// RHI Structs
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        struct RHI_Device;
        struct RHI_GPUContext;
        typedef HXUINTP HXGPUContext;
        struct RHI_Window;
        typedef HXUINTP HXWindow;

        struct HXGraphicsPipeline;
        struct HXMeshShaderPipeline;
        struct HXRayTracingPipeline;
        struct HXComputePipeline;

        struct RHI_Resource;
        struct HXStorageBuffer;
        struct HXTexture;
        struct HXDescriptorHeap;

        struct HXRenderTargetView;
        struct HXDepthStencilView;
        struct HXVertexBufferView;
        template<uint8_t C> struct HXVertexBufferViewPack;
        struct HXIndexBufferView;
        struct HXTextureSampler;
        struct HXIndirectBufferView;

        struct HXCommandBuffer;
        struct HXGPUFence;

        struct HXVertexIndirectCommand;
        struct HXIndexedIndirectCommand;

        struct HXFrameBuffer;

        #define HX_GRAPHICS_CMDS_FUNC_PARAMS RHI_Device& Device, RHI_GPUContext& context, HXCommandBuffer& buffer, HXUINTP data
        typedef HXRC (*RHI_CommandFunction)(HX_GRAPHICS_CMDS_FUNC_PARAMS);

        /////////////////////////////////////////////////////////////////////////////////////////////////////













        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// standard RHI
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        struct HXWindowConfig {
        	std::string Name;
        	Mathgl::uvec2 Size;
        	bool Fullscreen;
        	float RefreshRate;
        	bool VSync;
        	bool Transparency;

        	std::function<HXRC(GraphicsEngine*, HXWindow)> OnCreate;
        	std::function<HXRC(GraphicsEngine*, HXWindow, Mathgl::uvec2, Mathgl::uvec2)> OnResize;
        	std::function<HXRC(GraphicsEngine*, HXWindow)> OnDestroy;
        };



        struct HXCommandBufferConfig {
            CommandBufferType Type;
            CommandBufferUsage Usage;
        };



        struct HXTextureMipDesc {
            HXUINTP Data;
            HXSIZE Width;
            HXSIZE Height;
        };

        struct HXCommandExecutionState {
            HXCommandBuffer* CommandBuffer = NULL;
            HXUINTP Context = NULL;
            bool StartCommandBuffer = true;
            bool EndCommandBuffer = true;
            // do not touch
            HXUINTP _reservedState;
        };

        struct HXStorageResourceConfig {
            StorageResourceType Type;
            HXSIZE Width;
            HXSIZE Height;
            HXSIZE Depth;
            uint8_t Channels;
            uint8_t UnitStride;
            uint8_t MipCount;
            ResourceUsage Usage;
            ResourceState State;
            InputFormat Format;
            HXUINTP Data = 0;
            HXTextureMipDesc* Mips = NULL;
            RHI_Resource* Output = NULL;
        };



        struct HXVBufferDesc {
            HXStorageBuffer* buffer;
            HXSIZE stride;
            HXSIZE count;
        };



        struct HXSamplerConfig {
            TextureFilteringMode Filter = HX_GRAPHICS_SAMPLE_LINEAR;
            TextureWrapMode Wrap = HX_GRAPHICS_WRAP_REPEAT;
            uint8_t MaxAnisotropy = 1; /// 1 to 16
            HXTextureSampler* Output = NULL;
        };



        struct HXSRVElement {
            HXUINTP Resource;
            HXSIZE Size;
        };



        struct HXBlendConfig {
            ColorBlendType SrcFactor = HX_GRAPHICS_BLEND_ONE;
            ColorBlendType DstFactor = HX_GRAPHICS_BLEND_ZERO;
            ColorBlendType SrcAlphaFactor = HX_GRAPHICS_BLEND_ONE;
            ColorBlendType DstAlphaFactor = HX_GRAPHICS_BLEND_ZERO;
            ColorBlendOperation BlendOp = HX_GRAPHICS_BLEND_OP_ADD;
            ColorBlendOperation BlendAlphaOp = HX_GRAPHICS_BLEND_OP_ADD;
        };

        struct HXRasterizerConfig {
            DrawMode Mode = HX_GRAPHICS_DRAW_TRIANGLE;
            CompareOperation DepthOp = HX_GRAPHICS_COMP_OP_NONE;
            CompareOperation StencilOp = HX_GRAPHICS_COMP_OP_NONE;
            CullOperation CullOp = HX_GRAPHICS_CULL_OP_NONE;
            FillOperation FillOp = HX_GRAPHICS_FILL_OP_SOLID;
            bool CounterClockwise = true;
            bool LineAntialiasing = false;
            uint8_t MultisamplingCount = 1;
        };

        struct HXGraphicsPipelineStateConfig {
            HXRasterizerConfig RasterizerConfig;

            HXBlendConfig BlendConfigs[HX_GRAPHICS_DEF_MAX_RENDER_TARGETS_BOUND];
            uint8_t BlendCount = 0;

            HXSamplerConfig* StaticSamplers = NULL;
            uint8_t StaticSamplersCount = 0;

            InputFormat RenderTargetFormats[HX_GRAPHICS_DEF_MAX_RENDER_TARGETS_BOUND];
            InputFormat DepthStencilFormat = 0;
            uint8_t RenderTargetCount = 0;
        };

        struct HXGraphicsPipelineConfig {
            ShaderC::RHI_ShaderBinary VertexShader;
            ShaderC::RHI_ShaderBinary TesselationEvalShader;
            ShaderC::RHI_ShaderBinary TesselationContShader;
            ShaderC::RHI_ShaderBinary GeometryShader;
            ShaderC::RHI_ShaderBinary FragmentShader;
            ShaderC::RHI_ShaderMetadata Metadata;
        };

        struct HXComputePipelineConfig {
            ShaderC::RHI_ShaderBinary ComputeShader;
            ShaderC::RHI_ShaderMetadata Metadata;
        };

        struct HXMeshShaderPipelineConfig {
            ShaderC::RHI_ShaderBinary MeshShader;
            ShaderC::RHI_ShaderBinary TaskShader;
            ShaderC::RHI_ShaderMetadata Metadata;
        };

        /// TODO
        struct HXRayTracingPipelineConfig {
            ShaderC::RHI_ShaderBinary ComputeShader;
            ShaderC::RHI_ShaderMetadata Metadata;
        };

        typedef uint32_t ShaderInputBinding;
        struct HXShaderUniform {
            ShaderInputBinding binding;
            ShaderInputType type;
            ShaderC::ShaderUniformLayoutFlag flags;
            uint32_t count;
            size_t size;
        };

        struct HXShaderInput {
            struct BufferRange {
                uint32_t Start;
                uint32_t Size;
            };

            HXShaderUniform* Uniform;
            HXUINTP Data;
            uint32_t Count;
            union {
                HXTextureSampler* Sampler;
                BufferRange Range;
                BufferRange* Ranges;
                // HXUINTP Data2;
            };
            ShaderInputUsage Usage;
        };

        struct HXShaderInputsDesc {
            HXShaderInput* Inputs;
            HXSIZE Count;
        };

        typedef Utils::TypedHashMap<std::string, HXShaderUniform> HXShaderUniformMap;



        struct HXFrameBufferConfig {
            RenderPassFlags Flags;
            RenderPassFlags ClearFlags;
            HXDepthStencilView* DepthStencil;
            HXRenderTargetView* RenderTargets[HX_GRAPHICS_DEF_MAX_RENDER_TARGETS_BOUND];
            uint8_t RenderTargetCount;
            Mathgl::vec4 ClearColor;
            float ClearDepth = 1.0f;
            int8_t ClearStencil = 0;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////














        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        //////
        ////// Standard Commands for all RHIs
        //////
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        #define _HXG_RHI_MK_CMD(name, type, def) HXRC name(HX_GRAPHICS_CMDS_FUNC_PARAMS); struct type { static constexpr RHI_CommandFunction Func = &Graphics::Commands::name; def; };

        namespace Commands {
            _HXG_RHI_MK_CMD( ClearRenderTarget, HXClearRenderTarget,
                HXRenderTargetView* target;
                Mathgl::vec4 color;
            );
            _HXG_RHI_MK_CMD( ClearDepthStencil, HXClearDepthStencil,
                HXDepthStencilView* target;
                float depthValue;
                uint8_t stencilValue;
                bool clearDepth;
                bool clearStencil;
            );


            _HXG_RHI_MK_CMD( ChangeResourceState, HXChangeResourceState,
                ResourceState beforeState;
                ResourceState state;
                RHI_Resource* resource;
            );
            _HXG_RHI_MK_CMD( WaitForShaderWriteOp, HXWaitForShaderWriteOp,
                RHI_Resource* resource;
            );
            // _HXG_RHI_MK_CMD( FillBuffer, HXFillBuffer,
            //     HXStorageBuffer* buffer;
            //     HXSIZE offset;
            //     uint value;
            // );


            _HXG_RHI_MK_CMD( SetGraphicsPipeline, HXSetGraphicsPipeline,
                HXGraphicsPipeline* pipeline;
                HXShaderInputsDesc uniforms;
            );
            _HXG_RHI_MK_CMD( SetComputePipeline, HXSetComputePipeline,
                HXComputePipeline* pipeline;
                HXShaderInputsDesc uniforms;
            );
            _HXG_RHI_MK_CMD( SetGraphicsUniforms, HXSetGraphicsUniforms,
                HXShaderInputsDesc uniforms;
            );
            _HXG_RHI_MK_CMD( SetComputeUniforms, HXSetComputeUniforms,
                HXShaderInputsDesc uniforms;
            );
            _HXG_RHI_MK_CMD( StartRenderPass, HXStartRenderPass,
                HXFrameBuffer* framebuffer;
                Mathgl::vec4 viewport;
            );
            _HXG_RHI_MK_CMD( EndRenderPass, HXEndRenderPass, );


            _HXG_RHI_MK_CMD( DrawVertices, HXVertexDraw,
                HXSIZE StartVertex;
                HXSIZE StartInstance;
                HXSIZE VertexCount;
                HXSIZE InstanceCount;
                HXVertexBufferView* VDesc;
                uint8_t BufferCount;
            );
            _HXG_RHI_MK_CMD( DrawInidces, HXIndexDraw,
                HXSIZE StartIndex;
                HXSIZE StartInstance;
                HXSIZE IndexCount;
                HXSIZE InstanceCount;
                HXVertexBufferView* VDesc;
                uint8_t BufferCount;
                HXIndexBufferView* IDesc;
            );
            _HXG_RHI_MK_CMD( DrawVerticesIndirect, HXVertexDrawIndirect,
                HXSIZE StartVertex;
                HXStorageBuffer* VBuffer;
                HXVertexBufferView* VDesc;
                HXStorageBuffer* IndirectBuffer;
            );
            _HXG_RHI_MK_CMD( DrawInidcesIndirect, HXIndexDrawIndirect,
                HXSIZE StartIndex;
                HXVertexBufferView* VDesc;
                HXIndexBufferView* IDesc;
                HXStorageBuffer* IndirectBuffer;
            );
            // _HXG_RHI_MK_CMD( DrawFullscreen, HXFullscreenDraw, );


            _HXG_RHI_MK_CMD( CopyStorageBuffer, HXCopyStorageBuffer,
                HXStorageBuffer* src;
                HXUINTP dst;
                HXSIZE srcOffset;
                HXSIZE dstOffset;
                HXSIZE size;
            );
            _HXG_RHI_MK_CMD( CopyTexture, HXCopyTexture,
                HXTexture* src;
                HXUINTP dst;
                Mathgl::vec2 rect;
            );


            _HXG_RHI_MK_CMD( DispatchComputeWork, HXDispatchComputeWork,
                HXSIZE x;
                HXSIZE y;
                HXSIZE z;
            );


            _HXG_RHI_MK_CMD( ExecuteSecondaryCmdBuffer, HXRunSecondaryCmdBuffer,
                HXCommandBuffer* buffers;
                HXSIZE count;
            );


            /// Misc functions (Dont require the device and are for abstraction only)
            HXRC CreateVertexIndirectCommandObject(HXVertexIndirectCommand*, uint32_t, uint32_t, uint32_t);
            HXRC CreateIndexedIndirectCommandObject(HXIndexedIndirectCommand*, uint32_t, uint32_t, uint32_t, uint32_t);

            /////////////////////////////////////////////////////////////////////////////////////////////////////














            //////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            //////
            ////// RHI Helpers
            //////
            /////////////////////////////////////////////////////////////////////////////////////////////////////

            template<typename T, HXSIZE Count>
            struct SwapHelper {
            public:
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
            	constexpr14 inline T& previous(){
            		return objects[(current_index - 1) % Count];
            	}
                constexpr inline HXSIZE size() const {
                    return Count;
                }
                constexpr inline T& operator[](const HXSIZE& i) {
                    return objects[i];
                }
            };

            /////////////////////////////////////////////////////////////////////////////////////////////////////

        }


    }
}




#endif /* end of include guard: HEXO_GRAPHICS_RHI_IMPL_H */


#ifdef HEXO_GRAPHICS_RHI_WIN32_D3D12_GLFW

#ifndef HEXO_GRAPHICS_WIN32_IMPL_H
#define HEXO_GRAPHICS_WIN32_IMPL_H


#define HX_GRAPHICS_DEF_SHADERMODEL_VER D3D_SHADER_MODEL_6_5
#define HX_GRAPHICS_DEF_SHADERMODEL_VER_STR "6.5"


#define HX_GRAPHICS_CMDBUFFER_RESOURCE_COPY HX_GRAPHICS_CMDBUFFER_GRAPHICS



namespace Hexo {
    namespace Graphics {

        struct _DX12_CommandBuffer {
            ID3D12GraphicsCommandList4* internal;
        };

        struct _DX12_DescriptorHeap {
            ID3D12DescriptorHeap* internal = NULL;
            UINT realsize = 0;
            UINT size = 0;
        };

        struct _DX12_GPU_Descriptor {
            UINT64 handle;
        };



        struct RHI_GPUContext {
            ID3D12Fence* fence;
            UINT64 fence_value;
            HANDLE fence_event;

            D3D12_COMMAND_LIST_TYPE type;
            ID3D12CommandAllocator* allocator;
            ID3D12CommandQueue* queue_mirror;

            _DX12_DescriptorHeap rtv_heap;
            _DX12_DescriptorHeap dsv_heap;
        };



        struct RHI_Window {
            static constexpr UINT backbufferCount = HX_GRAPHICS_DEF_FRAME_COUNT;

            RHI_Window() : config({}), active(false), win(NULL) {}
            RHI_Window(const HXWindowConfig& s) : config(s), active(false), win(NULL) {}

            HXWindowConfig config;
            bool active;

            GLFWwindow* win;

            DXGI_FORMAT format;
            IDXGISwapChain3* swapchain = nullptr;
        	ID3D12Resource* renderTargets[backbufferCount];
        	UINT frameIndex = 0;
            HXGPUContext context;
        };



        struct RHI_Device {
            ID3D12Device4* device = nullptr;
            IDXGIFactory6* factory = nullptr;
            IDXGIAdapter1* adapter = nullptr;

        #ifdef HEXO_GRAPHICS_ENABLE_ERRORS
            ID3D12Debug1* debugController = nullptr;
            ID3D12DebugDevice* debugDevice = nullptr;
        #endif

            ID3D12CommandQueue* window_queue = nullptr;
            ID3D12CommandQueue* graphics_queue = nullptr;
            ID3D12CommandQueue* compute_queue = nullptr;
            ID3D12CommandQueue* copy_queue = nullptr;
            ID3D12CommandAllocator* bundle_allocator = nullptr;

            HXGPUContext copyContext;
            Utils::MiniTypedVector<RHI_GPUContext> contexts;

            Utils::TypedVector<RHI_Window> windows;
            HXSIZE numberOfWindows;

            D3D12MA::Allocator* memAlloc = NULL;

            _DX12_DescriptorHeap samplerHeap;
        };



        struct HXFrameBuffer : HXFrameBufferConfig {};

        struct _DX12_ShaderPipeline {
            ID3D12PipelineState* internal = NULL;
            ID3D12RootSignature* rootSignature = NULL;
        };

        struct HXGraphicsPipeline : _DX12_ShaderPipeline {
            DrawMode drawMode;
        };

        struct HXComputePipeline : _DX12_ShaderPipeline {

        };

        struct HXMeshShaderPipeline : _DX12_ShaderPipeline {

        };

        struct HXRayTracingPipeline : _DX12_ShaderPipeline {

        };



        struct RHI_Resource {
            D3D12_RESOURCE_STATES state;
            ID3D12Resource* internal = NULL;
            D3D12MA::Allocation* heap;
        };

        struct HXStorageBuffer : RHI_Resource {
        };

        struct HXTexture : RHI_Resource {
            StorageResourceType type;
            DXGI_FORMAT format;
        };



        struct HXRenderTargetView {
            size_t handle; // this value is the descriptor offset + 1 so that "0" will mean it is NULL
            DXGI_FORMAT format;
            uint32_t mipSlice;
        };

        struct HXDepthStencilView {
            size_t handle; // this value is the descriptor offset + 1 so that "0" will mean it is NULL
            DXGI_FORMAT format;
            uint32_t mipSlice;
        };

        struct HXVertexBufferView {
            uint8_t count;
            UINT vcount;
        };
        template<uint8_t C>
        struct HXVertexBufferViewPack : HXVertexBufferView {
            D3D12_VERTEX_BUFFER_VIEW descs[C];
        };

        struct HXIndexBufferView {
            UINT icount;
            D3D12_INDEX_BUFFER_VIEW desc;
        };

        struct HXTextureSampler {
            D3D12_GPU_DESCRIPTOR_HANDLE handle;
        };



        struct HXCommandBuffer : _DX12_CommandBuffer {
            enum : uint8_t {
                RECORDING_FIRSTTIME = (1 << 0),
                RECORDING_RELOAD_UNIFORMS = (1 << 1), // is set when the command buffer is used for a new set of commands
                RECORDING_RENDERPASS = (1 << 2),
            };
            HXCommandBufferConfig config;
            DrawMode last_draw_mode;
            HXSIZE context = 0;
            uint8_t state = 0;
            _DX12_DescriptorHeap srv_heap;
        };

        struct HXGPUFence {
            ID3D12Fence* ptr = NULL; // the object in this pointer is never created, the pointer is only assigned
            UINT64 value = 0;
            HANDLE event = 0;
        };

    }
}


#endif /* end of include guard: HEXO_GRAPHICS_WIN32_IMPL_H */

#endif

#ifdef HEXO_GRAPHICS_RHI_WIN32_VULKAN_GLFW

#ifndef HEXO_GRAPHICS_WIN32_VULKAN_H
#define HEXO_GRAPHICS_WIN32_VULKAN_H



#define HX_GRAPHICS_VK_MAX_QUEUE_FAMILY_INDICES 64



namespace Hexo {
    namespace Graphics {

        struct RHI_Window {
            static constexpr UINT backbufferCount = HX_GRAPHICS_DEF_FRAME_COUNT;

            RHI_Window() : config({}), active(false), win(NULL) {}
            RHI_Window(const HXWindowConfig& s) : config(s), active(false), win(NULL) {}

            HXWindowConfig config;
            bool active;

            GLFWwindow* win;
            VkSurfaceKHR surface;
            VkSwapchainKHR swapChain;

        	UINT frameIndex = 0;
            HXGPUContext context;
        };



        struct _VK_DescriptorPool {
            VkDescriptorPool internal = NULL;
            UINT realsize = 0;
            UINT size = 0;
        };

        struct _VK_CPU_Fence {
            VKFence fence;
            UINT64 value = 0;
            HANDLE event = 0;
        };


        struct RHI_GPUContext {
            _VK_DescriptorPool samplerPool;
            VkDeviceMemory* memoryPools;
            HXSIZE memoryPoolSize;
        };



        struct RHI_Device {
            enum {
                _HXG_VK_GRAPHICS_QUEUE_ARR_INDEX = 0,
                _HXG_VK_COMPUTE_QUEUE_ARR_INDEX = 1,
                _HXG_VK_TRANSFER_QUEUE_ARR_INDEX = 2,
                _HXG_VK_PRESENT_QUEUE_ARR_INDEX = 3,
            };
            static constexpr uint8_t AllQueueTypesCount = 4;
            static constexpr uint32_t AllQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
            static constexpr const char* RequiredExtensions[] = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
            static constexpr int RequiredExtensionsCount = sizeof(RequiredExtensions)/sizeof(const char*);

            HXGPUContext copyContext;
            Utils::MiniTypedVector<RHI_GPUContext> contexts;

            Utils::TypedVector<RHI_Window> windows;
            HXSIZE numberOfWindows;

            bool released = false;
            VkInstance instance;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkDevice device;

            VkQueue window_queue;
            VkQueue graphics_queue;
            VkQueue compute_queue;
            VkQueue transfer_queue;
            uint16_t finalQueueIndices[AllQueueTypesCount] = { 0 };

        #ifdef HEXO_GRAPHICS_ENABLE_ERRORS
            VkDebugUtilsMessengerEXT debugMessenger;
        #endif
        };



        struct HXFrameBuffer : HXFrameBufferConfig {
            VkFramebuffer framebuffer;
        };

        struct HXGraphicsPipeline {
            VkPipelineLayout layout;
            VkPipeline pipeline;
            Utils::TypedHashMap<std::string, HXShaderUniform> uniforms;
        };

        struct HXComputePipeline {

        };

        struct HXMeshShaderPipeline {

        };

        struct HXRayTracingPipeline {

        };

        struct HXStorageBuffer {
            HXSIZE memory;
            HXSIZE poolIndex;
            VkBuffer buffer;
        };

        struct HXTexture {
            HXSIZE memory;
            HXSIZE poolIndex;
            VkImage image;
        };

        struct HXRenderTargetView {
            VkImageView view;
        };

        struct HXDepthStencilView {
            VkImageView view;
        };

        struct HXVertexBufferView {
            uint8_t count;
            UINT vcount;
        };
        template<uint8_t C>
        struct HXVertexBufferViewPack {
            HXVertexBufferView views[C];
        };

        struct HXIndexBufferView {
            UINT icount;
        };

        struct HXTextureSampler {
            HXSIZE handle;
        };

        struct HXCommandBuffer {
            enum : uint8_t {
                RECORDING_FIRSTTIME = (1 << 0),
                RECORDING_RELOAD_UNIFORMS = (1 << 1), // is set when the command buffer is used for a new set of commands
                RECORDING_RENDERPASS = (1 << 2),
            };
            HXCommandBufferConfig config;
            HXSIZE context = 0;
            uint8_t state = 0;
            _VK_DescriptorPool* descriptor_pools;
            VkDescriptorSet descriptor_set;
            _VK_CPU_Fence cpufence;
        };

        struct HXGPUFence {
            VkSemaphore gpufence;
            UINT64 value = 0;
        };

    }
}


#endif /* end of include guard: HEXO_GRAPHICS_WIN32_VULKAN_H */

#endif

namespace Hexo {

    using namespace Graphics;

    class GraphicsEngine {
    private:
        RHI_Device Device;

    public:
        GraphicsEngine() : Device() {}
        ~GraphicsEngine(){
            /// do not call release here, it must be explicitly called by the end user
        }

        HX_MAKE_NON_COPYABLE_TYPE(GraphicsEngine)

        inline RHI_Device* GetInternalDevice(){ return &Device; }

    #ifdef HEXO_GRAPHICS_ENABLE_ERRORS
        bool enableValidationLayer = false;
    #endif



        /// implementation defined
        HXRC InitInThread();
        HXRC Update();
        HXRC UpdateInThread();
        HXRC Release();





        HXRC CreateGPUContext(HXGPUContext&, CommandBufferType);
        template<typename... T>
        HXRC ExecuteCommandBuffers(HXGPUContext& context, HXGPUFence* fence, T&&... p){
            HXCommandBuffer* ap[sizeof...(T)] = { (&p)... };
            return ExecuteCommandBuffersFromArray(context, fence, ap, sizeof...(T));
        }
        HXRC ExecuteCommandBuffersFromArray(HXGPUContext&, HXGPUFence*, HXCommandBuffer**, HXSIZE);
        HXRC FenceWaitCPU(HXGPUFence&);
        HXRC FenceWaitGPU(HXGPUContext&, HXGPUFence&);
        HXRC FenceUpdateCPU(HXGPUFence&);
        HXRC WaitForExecution(HXGPUContext&);
        HXRC ResetGPUContext(HXGPUContext&);
        HXRC GetContextFence(HXGPUContext&, HXGPUFence&);
        HXRC FlushContexts();





        HXRC CreateNewWindow(HXWindow&, const HXWindowConfig&);
        HXRC DestroyWindow(HXWindow&);
        HXWindowConfig GetConfig(HXWindow&);
        HXRC ResizeWindow(HXWindow&, Mathgl::uvec2);
        HXUINTP GetNativeWindowHandle(HXWindow&);
        HXRC UpdateWindowConfig(HXWindow&, HXWindowConfig&);
        HXSIZE GetNumberOfWindows();
        HXGPUContext* GetWindowGPUContext(const HXWindow&);
        HXTexture GetWindowTexture(HXWindow&, uint8_t index);
        HXRenderTargetView GetWindowRenderTarget(HXWindow&, uint8_t index);
        HXTexture GetCurrentWindowTexture(HXWindow& w);
        HXRenderTargetView GetCurrentWindowRenderTarget(HXWindow& w);
        uint8_t GetFrameIndex(HXWindow&);
        HXRC PresentToWindow(HXWindow&);





        HXRC CreateGraphicsPipeline(HXGraphicsPipeline&, const HXGraphicsPipelineConfig&, const HXGraphicsPipelineStateConfig&);
        HXRC CreateComputePipeline(HXComputePipeline&, const HXComputePipelineConfig&);
        HXRC DestroyGraphicsPipeline(HXGraphicsPipeline&);
        HXRC DestroyComputePipeline(HXComputePipeline&);
        HXShaderUniform* GetUniform(HXShaderUniformMap&, const char*);

        HXRC CreateFrameBuffer(const HXFrameBufferConfig&, HXFrameBuffer&);
        HXRC DestroyFrameBuffer(HXFrameBuffer&);





        HXRC CreateCommandBuffer(HXCommandBuffer&, const HXCommandBufferConfig&);
        HXRC StopRecording(HXCommandExecutionState&);
        HXRC StartRecordingCopy(HXCommandExecutionState&);
        HXRC StartRecording(HXCommandExecutionState&, uint32_t uniformCount=0);
        HXRC StartRecording(HXCommandExecutionState&, HXGraphicsPipeline&, HXShaderInputsDesc);
        HXRC StartRecording(HXCommandExecutionState&, HXComputePipeline&, HXShaderInputsDesc);
        HXRC ResetCommandBuffer(HXCommandBuffer&);

        template<typename... T>
        HXRC InsertCommands(
            HXCommandExecutionState& state, T&&... commands
        ){
            constexpr auto count = sizeof...(T);
            RHI_CommandFunction funcs[count] = { (std::remove_reference<T>::type::Func)... };
            HXUINTP cmds[count] = { (&commands)... };
            return InsertCommandsFromArray(state, funcs, cmds, count);
        }
        HXRC InsertCommandsFromArray(
            HXCommandExecutionState&, RHI_CommandFunction*, HXUINTP*, HXSIZE
        );





        template<typename... T>
        HXRC CreateStorageResources(HXCommandExecutionState& copy_state, T&&... p){
            HXStorageResourceConfig ap[sizeof...(T)] = { p... };
            return CreateStorageResourcesFromArray(copy_state, ap, sizeof...(T));
        }
        HXRC CreateStorageResourcesFromArray(HXCommandExecutionState&, HXStorageResourceConfig*, HXSIZE);

        template<typename... T>
        HXRC CleanUpResourceCopyStates(T&&... p){
            HXCommandExecutionState ap[sizeof...(T)] = { p... };
            return CleanUpResourceCopyStatesFromArray(ap, sizeof...(T));
        }
        HXRC CleanUpResourceCopyStatesFromArray(HXCommandExecutionState*, HXSIZE);

        HXRC MapStorageBuffer(HXStorageBuffer&, const HXSIZE&, const HXSIZE&, void**);
        HXRC UnmapStorageBuffer(HXStorageBuffer&, const HXSIZE&, const HXSIZE&);
        HXRC MapTexture(HXTexture&, const Mathgl::uvec3&, const Mathgl::uvec3&, void**);
        HXRC UnmapTexture(HXTexture&, const Mathgl::uvec3&, const Mathgl::uvec3&);

        template<typename... T>
        HXRC CreateSamplers(T&&... p){
            HXSamplerConfig ap[sizeof...(T)] = { p... };
            return CreateSamplersFromArray(ap, sizeof...(T));
        }
        HXRC CreateSamplersFromArray(HXSamplerConfig*, HXSIZE);





        HXRC CreateRenderTargetView(HXRenderTargetView&, HXGPUContext&, const HXTexture&, uint8_t);
        HXRC CreateDepthStencilView(HXDepthStencilView&, HXGPUContext&, const HXTexture&, uint8_t);
        template<typename View_t, typename... T>
        HXRC CreateVertexBufferView(View_t& view, T&&... p){
    		HXVBufferDesc ap[sizeof...(T)] = { p... };
            return CreateVertexBufferViewFromArray(&view, ap, sizeof...(T));
    	}
        HXRC CreateVertexBufferViewFromArray(HXVertexBufferView*, HXVBufferDesc*, uint8_t);
        HXRC CreateIndexBufferView(HXIndexBufferView&, const HXStorageBuffer&, HXSIZE, InputFormat);





        template<typename... T>
        HXRC DestroyStorageBuffers(T&&... p){
            HXStorageBuffer* ap[sizeof...(T)] = { (&p)... };
            return DestroyStorageBuffersFromArray(ap, sizeof...(T));
        }
        template<typename... T>
        HXRC DestroyTextures(T&&... p){
            HXTexture* ap[sizeof...(T)] = { (&p)... };
            return DestroyTexturesFromArray(ap, sizeof...(T));
        }
        HXRC DestroyStorageBuffersFromArray(HXStorageBuffer**, HXSIZE);
        HXRC DestroyTexturesFromArray(HXTexture**, HXSIZE);

    };

}








#endif /* end of include guard: HEXO_GRAPHICS_H */
