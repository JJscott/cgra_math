//----------------------------------------------------------------------------
// 
// CGRA Math Library
//
//
//----------------------------------------------------------------------------



/**
=================
	  TODO
=================

- constexpr everything
- zip_with
- vector/matrix/quaternion functions
- transform functions

*/

#pragma once

#include <cassert> // assert, static_assert
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility> // std::declval




// Visual Studio versions prior to 2015 lack constexpr support
#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(constexpr)
#define constexpr
#endif

// We undefine min and max macros if they exist
// so it doesn't interfer with our function overloads
// @$#! macros...
#undef MIN
#undef min
#undef MAX
#undef max


namespace cgra {

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////     _____  _____          _               _____                                                                   ////
	////    / ____|/ ____|   /\   | |        /\   |  __ \                                                                ////
	////   | (___ | |       /  \  | |       /  \  | |__) |                                                             ////
	////    \___ \| |      / /\ \ | |      / /\ \ |  _  /                                                            ////
	////    ____) | |____ / ____ \| |____ / ____ \| | \ \                                                              ////
	////   |_____/ \_____/_/    \_\______/_/    \_\_|  \_\                                                               ////
	////                                                                                                                   ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// pi
	constexpr double pi = 3.1415926535897932384626433832795;

	// natural log base
	constexpr double e = 2.7182818284590452353602874713527;

	// golden ratio
	constexpr double phi = 1.61803398874989484820458683436563811;


	// Angle and Trigonometry Functions
	// 

	using std::sin;

	using std::cos;

	using std::tan;

	using std::asin;

	using std::acos;

	using std::atan;

	template <typename T>
	inline auto atan(const T &y, const T &x) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return std::atan2(y, x);
	}

	using std::sinh;

	using std::cosh;

	using std::tanh;

	using std::asinh;

	using std::acosh;

	using std::atanh;
	

	// Exponential Functions
	//

	using std::pow;

	using std::exp;

	using std::log;

	using std::sqrt;
	
	
	// Common Functions
	//

	using std::abs;

	template <typename T>
	inline auto sign(T a) -> std::enable_if_t<std::is_arithmetic<T>::value, int> {
		return (T(0) < a) - (a < T(0));
	}

	using std::floor;

	using std::ceil;

	template <typename T>
	inline auto mod(const T &a, const T &b) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return a - b * std::floor(a/b);
	}

	using std::min;

	using std::max;

	// clamp scalars between b and c
	template <typename T>
	inline auto clamp(const T &a, const T &b, const T &c) -> std::enable_if_t<std::is_arithmetic<T>::value, T>  {
		return std::min(std::max(a, b), c);
	}

	// linear blend of scalars : x*(1-a) + y*a
	template <typename T>
	inline auto mix(const T &a, const T &b, const T &c) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return (T(1)-a) * b + a * c;
	}

	// 0.0 if edge < v, else 1.0
	template <typename T>
	inline auto step(const T &a, const T &b) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return (a<b) ? T(0) : T(1);
	}

	// smooth hermit interpolation
	template <typename T>
	inline auto smoothstep(const T &a, const T &b, const T &c) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		auto t = clamp((c-a)/(b-a), T(0), T(1));
		return t * t * (T(3)-T(2)*t);
	}

	using std::isinf;

	using std::isnan;

	// eg: inf<float>()
	// only for floating point types
	template <typename T>
	inline T inf() {
		return std::numeric_limits<T>::infinity();
	}

	// eg: nan<float>()
	// only for floating point types
	template <typename T>
	inline T nan() {
		return std::numeric_limits<T>::quiet_NaN();
	}

	// return a random value of T in range [lower, upper)
	template <typename T>
	inline T random(T lower, T upper) {
		// create a distrubution object based on the type T
		using dist_t = std::conditional_t<
			std::is_floating_point<T>::value,
			std::uniform_real_distribution<T>,
			std::conditional_t<
				std::is_integral<T>::value,
				std::uniform_int_distribution<
					std::conditional_t<
						sizeof(T) < sizeof(short),
						std::conditional_t<
							std::is_signed<T>::value,
							short,
							unsigned short
						>,
						T
					>
				>,
				void
			>
		>;
		static thread_local std::default_random_engine re { std::random_device()() };
		dist_t dist(lower, upper);
		return dist(re);
	}

	// return a random value of T in range [0, upper)
	template <typename T> inline T random(T upper = T(1)) {
		return random<T>(T(0), upper);
	}





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  __      __       _                                                                                               ////
	////  \ \    / /      | |                                                                                            ////
	////   \ \  / /__  ___| |_ ___  _ __                                                                               ////
	////    \ \/ / _ \/ __| __/ _ \| '__|                                                                            ////
	////     \  /  __/ (__| || (_) | |                                                                                 ////
	////      \/ \___|\___|\__\___/|_|                                                                                   ////
	////                                                                                                                   ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename, size_t> class basic_vec;
	template <typename, size_t, size_t> class basic_mat;
	template <typename> class basic_quat;

	// Aliases
	// 

#ifdef CGRA_INITIAL3D_NAMES // Intial3D naming convention
	using vec0f = basic_vec<float,    0>;
	using vec0d = basic_vec<double,   0>;
	using vec0i = basic_vec<int,      0>;
	using vec0u = basic_vec<unsigned, 0>;
	using vec0b = basic_vec<bool,     0>;

	using vec1f = basic_vec<float,    1>;
	using vec1d = basic_vec<double,   1>;
	using vec1i = basic_vec<int,      1>;
	using vec1u = basic_vec<unsigned, 1>;
	using vec1b = basic_vec<bool,     1>;

	using vec2f = basic_vec<float,    2>;
	using vec2d = basic_vec<double,   2>;
	using vec2i = basic_vec<int,      2>;
	using vec2u = basic_vec<unsigned, 2>;
	using vec2b = basic_vec<bool,     2>;

	using vec3f = basic_vec<float,    3>;
	using vec3d = basic_vec<double,   3>;
	using vec3i = basic_vec<int,      3>;
	using vec3u = basic_vec<unsigned, 3>;
	using vec3b = basic_vec<bool,     3>;

	using vec4f = basic_vec<float,    4>;
	using vec4d = basic_vec<double,   4>;
	using vec4i = basic_vec<int,      4>;
	using vec4u = basic_vec<unsigned, 4>;
	using vec4b = basic_vec<bool,     4>;

	using mat2f = basic_mat<float,  2, 2>;
	using mat2d = basic_mat<double, 2, 2>;
	using mat2i = basic_mat<int,    2, 2>;
	using mat3f = basic_mat<float,  3, 3>;
	using mat3d = basic_mat<double, 3, 3>;
	using mat3i = basic_mat<int,    3, 3>;
	using mat4f = basic_mat<float,  4, 4>;
	using mat4d = basic_mat<double, 4, 4>;
	using mat4i = basic_mat<int,    4, 4>;

	using mat2x3f = basic_mat<float,  2, 3>;
	using mat2x3d = basic_mat<double, 2, 3>;
	using mat2x3i = basic_mat<int,    2, 3>;
	using mat2x4f = basic_mat<float,  2, 4>;
	using mat2x4d = basic_mat<double, 2, 4>;
	using mat2x4i = basic_mat<int,    2, 4>;

	using mat3x2f = basic_mat<float,  3, 2>;
	using mat3x2d = basic_mat<double, 3, 2>;
	using mat3x2i = basic_mat<int,    3, 2>;
	using mat3x4f = basic_mat<float,  3, 4>;
	using mat3x4d = basic_mat<double, 3, 4>;
	using mat3x4i = basic_mat<int,    3, 4>;

	using mat4x2f = basic_mat<float,  4, 2>;
	using mat4x2d = basic_mat<double, 4, 2>;
	using mat4x2i = basic_mat<int,    4, 2>;
	using mat4x3f = basic_mat<float,  4, 3>;
	using mat4x3d = basic_mat<double, 4, 3>;
	using mat4x3i = basic_mat<int,    4, 3>;

	using quatf = basic_quat<float>;
	using quatd = basic_quat<double>;

#else // GLSL naming convention
	using vec0  = basic_vec<float,    0>;
	using dvec0 = basic_vec<double,   0>;
	using ivec0 = basic_vec<int,      0>;
	using uvec0 = basic_vec<unsigned, 0>;
	using bvec0 = basic_vec<bool,     0>;

	using vec1  = basic_vec<float,    1>;
	using dvec1 = basic_vec<double,   1>;
	using ivec1 = basic_vec<int,      1>;
	using uvec1 = basic_vec<unsigned, 1>;
	using bvec1 = basic_vec<bool,     1>;

	using vec2  = basic_vec<float,    2>;
	using dvec2 = basic_vec<double,   2>;
	using ivec2 = basic_vec<int,      2>;
	using uvec2 = basic_vec<unsigned, 2>;
	using bvec2 = basic_vec<bool,     2>;

	using vec3  = basic_vec<float,    3>;
	using dvec3 = basic_vec<double,   3>;
	using ivec3 = basic_vec<int,      3>;
	using uvec3 = basic_vec<unsigned, 3>;
	using bvec3 = basic_vec<bool,     3>;

	using vec4  = basic_vec<float,    4>;
	using dvec4 = basic_vec<double,   4>;
	using ivec4 = basic_vec<int,      4>;
	using uvec4 = basic_vec<unsigned, 4>;
	using bvec4 = basic_vec<bool,     4>;

	using mat2  = basic_mat<float,  2, 2>;
	using dmat2 = basic_mat<double, 2, 2>;
	using imat2 = basic_mat<int,    2, 2>;
	using mat3  = basic_mat<float,  3, 3>;
	using dmat3 = basic_mat<double, 3, 3>;
	using imat3 = basic_mat<int,    3, 3>;
	using mat4  = basic_mat<float,  4, 4>;
	using dmat4 = basic_mat<double, 4, 4>;
	using imat4 = basic_mat<int,    4, 4>;

	using mat2x3  = basic_mat<float,  2, 3>;
	using dmat2x3 = basic_mat<double, 2, 3>;
	using imat2x3 = basic_mat<int,    2, 3>;
	using mat2x4  = basic_mat<float,  2, 4>;
	using dmat2x4 = basic_mat<double, 2, 4>;
	using imat2x4 = basic_mat<int,    2, 4>;

	using mat3x2  = basic_mat<float,  3, 2>;
	using dmat3x2 = basic_mat<double, 3, 2>;
	using imat3x2 = basic_mat<int,    3, 2>;
	using mat3x4  = basic_mat<float,  3, 4>;
	using dmat3x4 = basic_mat<double, 3, 4>;
	using imat3x4 = basic_mat<int,    3, 4>;

	using mat4x2  = basic_mat<float,  4, 2>;
	using dmat4x2 = basic_mat<double, 4, 2>;
	using imat4x2 = basic_mat<int,    4, 2>;
	using mat4x3  = basic_mat<float,  4, 3>;
	using dmat4x3 = basic_mat<double, 4, 3>;
	using imat4x3 = basic_mat<int,    4, 3>;

	using quat = basic_quat<float>;
	using dquat = basic_quat<double>;

#endif


	// N-dimensional vector class of type T
	template <typename T, size_t N>
	class basic_vec {
	private:
		T m_data[N];

	public:
		using value_t = T;
		static constexpr size_t size = N;

		// TODO more constructors
		constexpr basic_vec() { }

		template <typename ...ArgTs>
		constexpr basic_vec(ArgTs &&...args) : m_data{std::forward<ArgTs>(args)...} { }

		T & operator[](size_t i) {
			assert(i < N);
			return m_data[i];
		}

		constexpr const T & operator[](size_t i) const {
			assert(i < N);
			return m_data[i];
		}

		T * data() { return &m_data[0]; }

		constexpr const T * data() const { return &m_data[0]; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v[0];
			for (size_t i = 1; i < N; ++i)
				out << ", " << v[i];
			out << ')';
		}
	};


	// 0-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 0> {
	public:
		using value_t = T;
		static constexpr size_t size = 0;

		constexpr basic_vec() { }
		template <typename U>
		constexpr basic_vec(const basic_vec<U, 0> &v) { }

		T & operator[](size_t i) {
			assert(false);
			return reinterpret_cast<T &>(*this);
		}

		constexpr const T & operator[](size_t i) const {
			assert(false);
			return reinterpret_cast<const T &>(*this);
		}

		T * data() { return reinterpret_cast<T *>(this); }

		constexpr const T * data() const { return reinterpret_cast<const T *>(this); }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << "( )";
		}
	};


	// 1-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 1> {
	public:
		using value_t = T;
		static constexpr size_t size = 1;

		union{ T x; T r; T s; };

		constexpr basic_vec() : x(0) { }
		constexpr explicit basic_vec(T s) : x(s) { }
		constexpr explicit basic_vec(const T *p) : x(p[0]) { }
		template <typename U>
		constexpr basic_vec(const basic_vec<U, 1> &v) : x(v.x) { }

		T & operator[](size_t i) {
			assert(i < 1);
			return (&x)[i];
		}

		constexpr const T & operator[](size_t i) const {
			assert(i < 1);
			return (&x)[i];
		}

		T * data() { return &x; }

		constexpr const T * data() const { return &x; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ')';
		}
	};


	// 2-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 2> {
	public:
		using value_t = T;
		static constexpr size_t size = 2;

		union{ T x; T r; T s; };
		union{ T y; T g; T t; };

		constexpr basic_vec() : x(0), y(0) { }
		constexpr basic_vec(T _x, T _y) : x(_x), y(_y) { }
		constexpr explicit basic_vec(T s) : x(s), y(s) { }
		constexpr explicit basic_vec(const T *p) : x(p[0]), y(p[1]) { }
		template <typename U>
		constexpr basic_vec(const basic_vec<U, 2> &v) : x(v.x), y(v.y) { }

		T & operator[](size_t i) {
			assert(i < 2);
			return (&x)[i];
		}

		constexpr const T & operator[](size_t i) const {
			assert(i < 2);
			return (&x)[i];
		}

		T * data() { return &x; }

		constexpr const T * data() const { return &x; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ')';
		}
	};


	// 3-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 3> {
	public:
		using value_t = T;
		static constexpr size_t size = 3;

		union{ T x; T r; T s; };
		union{ T y; T g; T t; };
		union{ T z; T b; T p; };

		constexpr basic_vec() : x(0), y(0), z(0) { }
		constexpr basic_vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }
		constexpr explicit basic_vec(T s) : x(s), y(s), z(s) { }
		constexpr explicit basic_vec(const T *p) : x(p[0]), y(p[1]), z(p[2]) { }
		template <typename U>
		constexpr basic_vec(const basic_vec<U, 3> &v) : x(v.x), y(v.y), z(v.z) { }

		T & operator[](size_t i) {
			assert(i < 3);
			return (&x)[i];
		}

		constexpr const T & operator[](size_t i) const {
			assert(i < 3);
			return (&x)[i];
		}

		T * data() { return &x; }

		constexpr const T * data() const { return &x; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
		}
	};


	// 4-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 4> {
	public:
		using value_t = T;
		static constexpr size_t size = 4;

		union{ T x; T r; T s; };
		union{ T y; T g; T t; };
		union{ T z; T b; T p; };
		union{ T w; T a; T q; };

		constexpr basic_vec() : x(0), y(0), z(0), w(0) { }
		constexpr basic_vec(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
		constexpr explicit basic_vec(T s) : x(s), y(s), z(s), w(s) { }
		constexpr explicit basic_vec(const T *p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) { }
		template <typename U>
		constexpr basic_vec(const basic_vec<U, 4> &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

		T & operator[](size_t i) {
			assert(i < 4);
			return (&x)[i];
		}

		constexpr const T & operator[](size_t i) const {
			assert(i < 4);
			return (&x)[i];
		}

		T * data() { return &x; }

		constexpr const T * data() const { return &x; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
		}
	};


	// M,N-dimensional matrix class of type T
	template <typename T, size_t M, size_t N>
	class basic_mat {
	private:
		T m_data[M*N];

	public:
		using value_t = T;
		static constexpr size_t cols = M;
		static constexpr size_t rows = N;
		static constexpr size_t size = M * N;

		// TODO more constructors
		basic_mat() { }

		const T & operator[](size_t i) const {
			assert(i < M);
			return reinterpret_cast<const basic_vec<T, N> &>(m_data[N*i]);
		}

		T & operator[](size_t i) {
			assert(i < M);
			return reinterpret_cast<basic_vec<T, N> &>(m_data[N*i]);
		}

		T * data() { return &m_data[0]; }
		const T * data() const { return &m_data[0]; }

		// stream insertion
		inline friend std::ostream & operator<<(std::ostream &out, const basic_mat &m) {
			const size_t field_width = 10;
			std::ostringstream oss;
			oss << std::setprecision(4);
			for (size_t r = 0; r < N; ++r) {
				oss << '[' << std::setw(field_width) << m[0][r];
				for (size_t c = 1; c < M; ++c)
					oss << ", " << std::setw(field_width) << m[c][r];
				oss << ']';
				if (r < N-1)
					oss << std::endl;
			}
			return out << oss.str();
		}
	};


	// Quaternion class of type T
	template <typename T>
	class basic_quat {
	public:
		using value_t = T;
		static constexpr size_t size = 4;

		T w, x, y, z;

		basic_quat() : w(1), x(0), y(0), z(0) { }
		basic_quat(T _w, T _x, T _y, T _z) :  w(_w), x(_x), y(_y), z(_z) { }
		template <typename U> 
		basic_quat(const basic_quat<U>& q) : w(q.w), x(q.x), y(q.y), z(q.z) { }
		template <typename U>
		basic_quat(T _w, const basic_vec<U, 3> &v) : w(_w), x(v.x), y(v.y), z(v.z) { }
		template <typename U1, typename U2>
		basic_quat(const basic_vec<U1, 1> &v1, const basic_vec<U2, 3> &v2) : w(v1.x), x(v2.x), y(v2.y), z(v2.z) { }
		template <typename U>
		basic_quat(const basic_vec<U, 4> &v) : w(v.w), x(v.x), y(v.y), z(v.z) { }

		const T & operator[](size_t i) const {
			assert(i < 4);
			return (&w)[i];
		}

		T & operator[](size_t i) {
			assert(i < 4);
			return (&w)[i];
		}

		T * data() { return &w; }
		const T * data() const { return &w; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_quat &v) {
			return out << '(' << v.w << ", " << v.x << ", " << v.y << ", " << v.z << ')';
		}

	};


	// Iterator functions
	// 

	template<typename T, size_t N>
	T * begin(basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	const T * begin(const basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	const T * cbegin(const basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	T * end(basic_vec<T, N> &v) { return begin(v) + N; }

	template<typename T, size_t N>
	const T * end(const basic_vec<T, N> &v) { return begin(v) + N; }

	template<typename T, size_t N>
	const T * cend(const basic_vec<T, N> &v) { return begin(v) + N; }

	
	template<typename T, size_t M, size_t N>
	T * begin(basic_mat<T, M, N> &m) { return m.data(); }
	
	template<typename T, size_t M, size_t N>
	const T * begin(const basic_mat<T, M, N> &m) { return m.data(); }

	template<typename T, size_t M, size_t N>
	const T * cbegin(const basic_mat<T, M, N> &m) { return m.data(); }
	
	template<typename T, size_t M, size_t N>
	T * end(basic_mat<T, M, N> &m) { return begin(m) + M * N; }
	
	template<typename T, size_t M, size_t N>
	const T * end(const basic_mat<T, M, N> &m) { return begin(m) + M * N; }

	template<typename T, size_t M, size_t N>
	const T * cend(const basic_mat<T, M, N> &m) { return begin(m) + M * N; }


	template<typename T>
	T * begin(basic_quat<T> &q) { return q.data(); }

	template<typename T>
	const T * begin(const basic_quat<T> &q) { return q.data(); }

	template<typename T>
	const T * cbegin(const basic_quat<T> &q) { return q.data(); }

	template<typename T>
	T * end(basic_quat<T> &q) { return begin(q) + 4; }

	template<typename T>
	const T * end(const basic_quat<T> &q) { return begin(q) + 4; }

	template<typename T>
	const T * cend(const basic_quat<T> &q) { return begin(q) + 4; }









	// ZipWith functions
	//

	template <typename ...VecTs>
	struct vec_min_size { };

	template <typename VecT>
	struct vec_min_size<VecT> : std::integral_constant<size_t, VecT::size> { };

	template <typename VecT1, typename VecT2, typename ...VecTs>
	struct vec_min_size<VecT1, VecT2, VecTs...> :
		std::integral_constant<
			size_t,
			(vec_min_size<VecT1>::value < vec_min_size<VecT2>::value) ?
			vec_min_size<VecT1, VecTs...>::value : vec_min_size<VecT2, VecTs...>::value
		>
	{ };

	template <size_t I, typename F, typename ...ArgTs>
	constexpr auto zip_with_impl_impl(F f, ArgTs &&...args) {
		return f(std::forward<ArgTs>(args)[I]...);
	}

	template <typename ResT, typename F, size_t ...Is, typename ...ArgTs>
	constexpr ResT zip_with_impl(F f, std::index_sequence<Is...>, ArgTs &&...args) {
		return ResT(zip_with_impl_impl<Is>(f, std::forward<ArgTs>(args)...)...);
	}

	template <typename F, typename ...ArgTs>
	constexpr auto zip_with(F f, ArgTs &&...args) {
		using value_t = decltype(f(std::forward<ArgTs>(args)[0]...));
		using size = vec_min_size<std::decay_t<ArgTs>...>;
		using vec_t = basic_vec<value_t, size::value>;
		using iseq = std::make_index_sequence<size::value>;
		return zip_with_impl<vec_t>(f, iseq(), std::forward<ArgTs>(args)...);
	}

	// struct op_add {
	// 	template <typename T1, typename T2>
	// 	constexpr auto operator()(T1 &&t1, T2 &&t2) const {
	// 		return std::forward<T1>(t1) + std::forward<T2>(t2);
	// 	}
	// };

	// constexpr vec3i c = zip_with(op_add(), a, b);
	// auto d = zip_with([](auto g, auto h) { return std::max(g, h); }, a, b);






	// Fold function
	//
	// Produce a scalar by applying f(T,T) -> T to adjacent pairs of elements
	// from vector a in left-to-right order starting with f(z, v[0])
	template<typename F, typename T, size_t N>
	T fold(F f, T z, const basic_vec<T, N> &v) {
		T r = f(z, v[0]);
		for (size_t i = 1; i < N; ++i)
			r = f(r, v[i]);
		return r;
	}





	// Functions
	// 

	// Angle and Trigonometry Functions
	// 

	// Element-wise function for x in v
	// The standard trigonometric sine function
	template <typename T, size_t N>
	inline basic_vec<T, N> sin(const basic_vec<T, N> &v) {
		return zip_with(sin, v);
	}

	// Element-wise function for x in v
	// The standard trigonometric cosine function
	template <typename T, size_t N>
	inline basic_vec<T, N> cos(const basic_vec<T, N> &v) {
		return zip_with(sin, v);
	}

	// Element-wise function for x in v
	// The standard trigonometric tangent
	template <typename T, size_t N>
	inline basic_vec<T, N> tan(const basic_vec<T, N> &v) {
		return zip_with(sin, v);
	}

	// Element-wise function for x in v
	// Arc sine. Returns an angle whose sine is x
	// The range of values returned by this function is [-pi/2, pi/2]
	// Results are undefined if ∣x∣>1
	template <typename T, size_t N>
	inline basic_vec<T, N> asin(const basic_vec<T, N> &v) {
		return zip_with(asin, v);
	}

	// Element-wise function for x in v
	// Arc cosine. Returns an angle whose cosine is x
	// The range of values returned by this function is [0, p]
	// Results are undefined if ∣x∣>1
	template <typename T, size_t N>
	inline basic_vec<T, N> acos(const basic_vec<T, N> &v) {
		return zip_with(acos, v);
	}

	// Element-wise function for y in v1 and x in v2
	// Arc tangent. Returns an angle whose tangent is y/x
	// The signs of x and y are used to determine what quadrant the angle is in
	// The range of values returned by this function is [−pi, pi]
	// Results are undefined if x and y are both 0
	template <typename T1, typename T2, size_t N>
	inline auto atan(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with(atan2, v2, v1);
	}

	// Element-wise function for x in v
	// Arc tangent. Returns an angle whose tangent is y_over_x.
	// The range of values returned by this function is [-pi/2, pi/2] 
	template <typename T, size_t N>
	inline basic_vec<T, N> atan(const basic_vec<T, N> &v) {
		return zip_with(atan, v);
	}

	// Element-wise function for x in v
	// Returns the hyperbolic sine function (e^x - e^-x)/2
	template <typename T, size_t N>
	inline basic_vec<T, N> sinh(const basic_vec<T, N> &v) {
		return zip_with(sinh, v);
	}

	// Element-wise function for x in v
	// Returns the hyperbolic cosine function (e^x + e^-x)/2
	template <typename T, size_t N>
	inline basic_vec<T, N> cosh(const basic_vec<T, N> &v) {
		return zip_with(cosh, v);
	}

	// Element-wise function for x in v
	// Returns the hyperbolic tangent function sinh(x)/cosh(x)
	template <typename T, size_t N>
	inline basic_vec<T, N> tanh(const basic_vec<T, N> &v) {
		return zip_with(tanh, v);
	}

	// Element-wise function for x in v
	// Arc hyperbolic sine; returns the inverse of sinh(x)
	template <typename T, size_t N>
	inline basic_vec<T, N> asinh(const basic_vec<T, N> &v) {
		return zip_with(asinh, v);
	}

	// Element-wise function for x in v
	// Arc hyperbolic cosine; returns the non-negative inverse of cosh(x)
	// Results are undefined if x < 1
	template <typename T, size_t N>
	inline basic_vec<T, N> acosh(const basic_vec<T, N> &v) {
		return zip_with(acosh, v);
	}

	// Element-wise function for x in v
	// Arc hyperbolic tangent; returns the inverse of tanh(x)
	// Results are undefined if ∣x∣>=1
	template <typename T, size_t N>
	inline basic_vec<T, N> atanh(const basic_vec<T, N> &v) {
		return zip_with(atanh, v);
	}


	// Exponential Functions
	// 

	// Element-wise function for x in v1 and y in v2
	// Returns x raised to the y power, i.e., x^y
	// Results are undefined if x < 0
	// Results are undefined if x = 0 and y <= 0
	template <typename T1, typename T2, size_t N>
	inline auto pow(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with(pow, v1, v2);
	}

	// Element-wise function for x in v
	// Returns the natural exponentiation of x, i.e., e^x
	template <typename T, size_t N>
	inline basic_vec<T, N> exp(const basic_vec<T, N> &v) {
		return zip_with(exp, v);
	}

	// Element-wise function for x in v
	// Returns the natural logarithm of x, i.e., the value y which satisfies the equation x = e^y
	// Results are undefined if x <= 0.
	template <typename T, size_t N>
	inline basic_vec<T, N> log(const basic_vec<T, N> &v) {
		return zip_with(log, v);
	}

	// Element-wise function for x in v
	// Returns 2 raised to the x power, i.e., 2^x
	template <typename T, size_t N>
	inline basic_vec<T, N> exp2(const basic_vec<T, N> &v) {
		return zip_with(exp2, v);
	}

	// Element-wise function for x in v
	// Returns the base 2 logarithm of x, i.e., the value y which satisfies the equation x = 2^y
	// Results are undefined if x <= 0
	template <typename T, size_t N>
	inline basic_vec<T, N> log2(const basic_vec<T, N> &v) {
		return zip_with(log2, v);
	}

	// Element-wise function for x in v
	// Returns sqrt(x)
	// Results are undefined if x < 0
	template <typename T, size_t N>
	inline basic_vec<T, N> sqrt(const basic_vec<T, N> &v) {
		return zip_with(sqrt, v);
	}

	// Element-wise function for x in v
	// Returns 1/sqrt(x)
	// Results are undefined if x <= 0
	template <typename T, size_t N>
	inline basic_vec<T, N> inversesqrt(const basic_vec<T, N> &v) {
		return checknan(T(1) / zip_with(sqrt, v));
	}


	// Common Functions
	// 

	// Element-wise function for x in v
	// Returns x if x >= 0; otherwise it returns –x
	template <typename T, size_t N>
	inline basic_vec<T, N> abs(const basic_vec<T, N> &v) {
		return zip_with(abs, v);
	}

	// Element-wise function for x in v
	// Returns 1 if x > 0, 0 if x = 0, or –1 if x < 0
	template <typename T, size_t N>
	inline basic_vec<T, N> sign(const basic_vec<T, N> &v) {
		return zip_with(sign, v);
	}

	// Element-wise function for x in v
	// Returns a value equal to the nearest integer that is less than or equal to x
	template <typename T, size_t N>
	inline basic_vec<T, N> floor(const basic_vec<T, N> &v) {
		return zip_with(floor, v);
	}

	//TODO trunc
	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x whose
	// absolute value is not larger than the absolute value of x

	//TODO round
	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x
	// The fraction 0.5 will round in a direction chosen by the
	// implementation, presumably the direction that is fastest
	// This includes the possibility that round(x) returns the
	// same value as roundEven(x) for all values of x

	//TODO roundEven
	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x
	// A fractional part of 0.5 will round toward the nearest even integer
	// (Both 3.5 and 4.5 for x will return 4.0)

	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x whose
	// absolute value is not larger than the absolute value of x
	template <typename T, size_t N>
	inline basic_vec<T, N> ceil(const basic_vec<T, N> &v) {
		return zip_with(ceil, v);
	}

	// Element-wise function for x in v
	// Returns x – floor (x)
	template <typename T, size_t N>
	inline basic_vec<T, N> fract(const basic_vec<T, N> &v) {
		return v-floor(v);
	}

	// Element-wise function for x in v
	// Modulus. Returns v-m*floor(v/m)
	template <typename T1, typename T2, size_t N>
	inline auto mod(const basic_vec<T1, N> &v, T2 m) {
		return v-m*floor(v/m);
	}

	// Element-wise function for x in v and m in mv
	// Modulus. Returns v-m*floor(v/m)
	template <typename T1, typename T2, size_t N>
	inline auto mod(const basic_vec<T1, N> &v, const basic_vec<T2, N> &mv) {
		return v-mv*floor(v/mv);
	}

	//TODO modf
	// Element-wise function for x in v and m in mv
	// Returns the fractional part of x and sets i to the integer
	// part (as a whole number floating-point value)
	// Both the return value and the output parameter will have the same sign as x
	// 
	// template <typename T, size_t N>
	// inline auto mod(const basic_vec<T1> &v, basic_vec<T2> &mv) {
	// 	return ;
	// }

	// Element-wise function for x in lhs
	// Returns rhs if rhs < x; otherwise it returns x
	template <typename T1, size_t N, typename T2, typename=std::enable_if_t<std::is_arithmetic<T2>::value>>
	inline auto min(const basic_vec<T1, N> &lhs, T2 rhs) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto x, auto y) { return min<common_t>(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in lhs and y in rhs
	// x and y are different types
	// Returns y if y < x; otherwise it returns x
	template <typename T1, typename T2, size_t N>
	inline auto min(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto x, auto y) { return min<common_t>(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in lhs and y in rhs
	// x and y are the same type
	// Returns y if y < x; otherwise it returns x
	template <typename T, size_t N>
	inline auto min(const basic_vec<T, N> &lhs, const basic_vec<T, N> &rhs) {
		return zip_with([](auto x, auto y) { return min(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in lhs
	// Returns rhs if rhs > x; otherwise it returns x
	template <typename T1, size_t N, typename T2, typename=std::enable_if_t<std::is_arithmetic<T2>::value>>
	inline auto max(const basic_vec<T1, N> &lhs, T2 rhs) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto x, auto y) { return max<common_t>(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in lhs and y in rhs
	// x and y are different types
	// Returns y if y > x; otherwise it returns x
	template <typename T1, typename T2, size_t N>
	inline auto max(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto x, auto y) { return max<common_t>(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in lhs and y in rhs
	// x and y are the same type
	// Returns y if y > x; otherwise it returns x
	template <typename T, size_t N>
	inline auto max(const basic_vec<T, N> &lhs, const basic_vec<T, N> &rhs) {
		return zip_with([](auto x, auto y) { return max(x, y); }, lhs, rhs);
	}

	// Element-wise function for x in v
	// Returns min(max(x, minVal), maxVal)
	// Results are undefined if minVal > maxVal
	template <typename T1, size_t N, typename T2, typename T3>
	inline auto clamp(const basic_vec<T1, N> &v, T2 minVal, T3 maxVal) {
		return min(max(v, minVal), maxVal);
	}

	// Element-wise function for x in v, minVal in vminVal and maxVal in vmaxVal
	// Returns min(max(x, minVal), maxVal)
	// Results are undefined if minVal > maxVal
	template <typename T1, typename T2, typename T3, size_t N>
	inline auto clamp(const basic_vec<T1, N> &v, const basic_vec<T2, N> &vminVal, const basic_vec<T3, N> &vmaxVal) {
		return min(max(v, vminVal), vmaxVal);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the linear blend of x and y, i.e., x*(1−a) + y*a
	template <typename T1, typename T2, size_t N, typename T3>
	inline auto mix(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2, T3 a) {
		using common_t = std::common_type_t<T1, T2, T3>;
		return v1*(common_t(1)-a)+v2*a;
	}

	// Element-wise function for x in v1, y in v2 and a in va
	// Returns the linear blend of x and y, i.e., x*(1−a) + y*a
	template <typename T1, typename T2, typename T3, size_t N>
	inline auto mix(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2, const basic_vec<T3, N> &va) {
		using common_t = std::common_type_t<T1, T2, T3>;
		return v1*(common_t(1)-va)+v2*va;
	}

	// Element-wise function for x in v1, y in v2 and a in va
	// Selects which vector each returned component comes from
	// For a component of a that is false, the corresponding component of x is returned
	// For a component of a that is true, the corresponding component of y is returned.
	// Components of x and y that are not selected are allowed to be invalid floating-point
	// values and will have no effect on the results
	// Thus, this provides different functionality than, for example, 
	//     genType mix(genType x, genType y, genType(a))
	// where a is a Boolean vector.
	template <typename T1, typename T2, size_t N>
	inline auto mix(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2, const basic_vec<bool, N> &va) {
		return zip_with([](auto x, auto y, auto a) { return (!a) ? x : y; }, v1, v2, va);
	}

	// Element-wise function for x in v
	// Returns 0.0 if x < edge; otherwise it returns 1.0
	template <typename T1, typename T2, size_t N>
	inline auto step(T1 edge, const basic_vec<T2, N> &v) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto edge, auto x) { return (x < edge) ? common_t(0) : common_t(1); }, basic_vec<common_t, N>(edge), v);
	}

	// Element-wise function for edge in vedge and x in v
	// Returns 0.0 if x < edge; otherwise it returns 1.0
	template <typename T1, typename T2, size_t N>
	inline auto step(const basic_vec<T1, N> &vedge, const basic_vec<T2, N> &v) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto edge, auto x) { return (x < edge) ? common_t(0) : common_t(1); }, vedge, v);
	}

	// Element-wise function for edge0 in vedge0, edge1 in vedge1
	// Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth
	// Hermite interpolation between 0 and 1 when edge0 < x < edge1
	// This is useful in cases where you would want a threshold
	// function with a smooth transition and is equivalent to:
	//     T t;
	//     t = clamp ((x – edge0) / (edge1 – edge0), 0, 1);
	//     return t * t * (3 – 2 * t);
	// Results are undefined if edge0 >= edge1.
	template <typename T1, typename T2, typename T3, size_t N>
	inline auto smoothstep(const basic_vec<T1, N> &edge0, const basic_vec<T2, N> &edge1, T3 x) {
		using common_t = std::common_type_t<T1, T2, T3>;
		auto t = clamp((x-edge0)/(edge1-edge0),0, 1);
		return t * t * (common_t(3)-common_t(2)*t);
	}

	// Element-wise function for edge0 in vedge0, edge1 in vedge1 and x in v
	// Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth
	// Hermite interpolation between 0 and 1 when edge0 < x < edge1
	// This is useful in cases where you would want a threshold
	// function with a smooth transition and is equivalent to:
	//     T t;
	//     t = clamp ((x – edge0) / (edge1 – edge0), 0, 1);
	//     return t * t * (3 – 2 * t);
	// Results are undefined if edge0 >= edge1.
	template <typename T1, typename T2, typename T3, size_t N>
	inline auto smoothstep(const basic_vec<T1, N> &vedge0, const basic_vec<T2, N> &vedge1, const basic_vec<T3, N> &v) {
		using common_t = std::common_type_t<T1, T2, T3>;
		auto t = clamp((v-vedge0)/(vedge1-vedge0),0, 1);
		return t * t * (common_t(3)-common_t(2)*t);
	}

	// Element-wise function for x in v
	// Returns true if x holds a NaN. Returns false otherwise
	// Always returns false if NaNs are not implemented
	template <typename T, size_t N>
	inline auto isnan(const basic_vec<T, N> &v) {
		return zip_with([](auto x) { return isnan(x); }, v);
	}

	// Element-wise function for x in v
	// Returns true if x holds a positive infinity or negative infinity
	// Returns false otherwise
	template <typename T, size_t N>
	inline auto isinf(const basic_vec<T, N> &v) {
		return zip_with([](auto x) { return isinf(x); }, v);
	}

	//TODO
	// floatBitsToInt
	// floatBitsToUint
	// Returns a signed or unsigned integer value representing the encoding of a float
	// The float value's bit-level representation is preserved

	//TODO
	// intBitsToFloat
	// uintBitsToFloat
	// Returns a float value corresponding to a signed or unsigned integer encoding of a float
	// If a NaN is passed in, it will not signal, and the resulting value is unspecified.
	// If an Inf is passed in, the resulting value is the corresponding Inf.

	//TODO
	// fma
	// Computes and returns a*b + c.
	// In uses where the return value is eventually consumed by
	// a variable declared as precise:
	//
	//  - fma() is considered a single operation, whereas the
	//    expression “a*b + c” consumed by a variable
	//    declared precise is considered two operations.
	//
	//  - The precision of fma() can differ from the precision
	//    of the expression “a*b + c”.
	//
	//  - fma() will be computed with the same precision as
	//    any other fma() consumed by a precise variable,
	//    giving invariant results for the same input values of
	//    a, b, and c.
	//
	// Otherwise, in the absence of precise consumption, there
	// are no special constraints on the number of operations or
	// difference in precision between fma() and the expression a*b+c.

	//TODO
	// frexp
	// Splits x into a floating-point significand in the range
	// [0.5, 1.0) and an integral exponent of two, such that:
	//     x = significand*2^exponent
	// The significand is returned by the function and the
	// exponent is returned in the parameter exp.
	// For a floating-point value of zero, the significand and exponent are both zero.
	// For a floating-point value that is an infinity or is not a number, the results are undefined.
	// If an implementation supports negative 0, frexp(-0)
	// should return -0; otherwise it will return 0.

	//TODO
	// ldexp
	// Builds a floating-point number from x and the
	// corresponding integral exponent of two in exp, returning:
	//     significand*2^exponent
	// If this product is too large to be represented in the
	// floating-point type, the result is undefined.
	// If exp is greater than +128 (single-precision) or +1024
	// (double-precision), the value returned is undefined.
	// If exp is less than -126 (single-precision) or -1022 (doubleprecision),
	// the value returned may be flushed to zero.
	// Additionally, splitting the value into a significand and
	// exponent using frexp() and then reconstructing a
	// floating-point value using ldexp() should yield the
	// original input for zero and all finite non-denormized values.

	// Returns the sum of all x in v, i.e., v[0] + v[1] + ...
	template <typename T, size_t N>
	inline T sum(const basic_vec<T, N> &v) {
		return fold( [](auto a, auto b) { return a + b; }, T(0), v);
	}

	// Returns the product of all x in v, i.e., v[0] * v[1] * ...
	template <typename T, size_t N>
	inline T product(const basic_vec<T, N> &v) {
		return fold( [](auto a, auto b) { return a * b; }, T(0), v);
	}

	//TODO
	// Floating-Point Pack and Unpack Functions (all)

	// Geometric Functions
	// 

	// Returns the length of vector v, i.e.,sqrt(v[0]^2 + v[1]^2  + ...)
	template <typename T, size_t N>
	inline T length(const basic_vec<T, N> &v) {
		return sqrt(sum(v*v));
	}

	// Returns the distance between p1 and p2, i.e., length (p1 – p2)
	template <typename T1, typename T2, size_t N>
	inline auto distance(const basic_vec<T1, N> &p1, const basic_vec<T2, N> &p2) {
		return length(p1-p2);
	}

	// Returns the dot product of v1 and v2, i.e., v1[0]*v1[0] + v1[1]*v2[1] + ...
	template <typename T1, typename T2, size_t N>
	inline auto dot(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return sum(v1*v2);
	}

	// Returns a vector in the same direction as v but with a length of 1
	template <typename T, size_t N>
	inline basic_vec<T, N> normalize(const basic_vec<T, N> &v) {
		return v / length(v);
	}

	// If dot(nref, i) < 0 return n, otherwise return -n
	template <typename T1, typename T2, typename T3, size_t N>
	inline auto faceforward(const basic_vec<T1, N> &n, const basic_vec<T2, N> &i, const basic_vec<T3, N> &nref) {
		using common_t = std::common_type_t<T1, T2, T3>;
		return (dot(nref, i) < common_t(0)) ? n : -n ;
	}

	// For the incident vector i and surface orientation n,
	// returns the reflection direction:
	//     I – 2 * dot(N, I) * N
	// n must already be normalized in order to achieve the desired result.
	template <typename T1, typename T2, size_t N>
	inline auto reflect(const basic_vec<T1, N> &i, const basic_vec<T2, N> &n) {
		using common_t = std::common_type_t<T1, T2>;
		return i - common_t(2) * dot(n, i) * n;
	}

	// For the incident vector i and surface normal n, and the
	// ratio of indices of refraction eta, return the refraction vector.
	// The result is computed by :
	//     k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i))
	//     if (k < 0.0)
	//         return genType(0.0) // or genDType(0.0)
	//     else
	//         return eta * i - (eta * dot(n, i) + sqrt(k)) * n
	// The input parameters for the incident vector i and the surface
	// normal n must already be normalized to get the desired results.
	template <typename T1, typename T2, size_t N, typename T3>
	inline auto refract(const basic_vec<T1, N> &i, const basic_vec<T2, N> &n, T3 eta) {
		using common_t = std::common_type_t<T1, T2, T3>;
		auto k = common_t(1) - eta * eta * (common_t(1) - dot(n, i) * dot(n, i));
		if (k < common_t(0)) {
			return basic_vec<common_t, N>();
		}
		return eta * i - (eta * dot(n, i) + std::sqrt(k)) * n;
	}


	// Vector Relational Functions
	//

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x < y
	template <typename T1, typename T2, size_t N>
	inline auto lessThan(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x < y; }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x <= y
	template <typename T1, typename T2, size_t N>
	inline auto lessThanEqual(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x <= y; }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x > y
	template <typename T1, typename T2, size_t N>
	inline auto greaterThan(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x > y; }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x >= y
	template <typename T1, typename T2, size_t N>
	inline auto greaterThanEqual(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x >= y; }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x == y
	template <typename T1, typename T2, size_t N>
	inline auto equal(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x == y; }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x != y
	template <typename T1, typename T2, size_t N>
	inline auto notEqual(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto x, auto y) { return x != y; }, v1, v2);
	}

	// Returns true if any component of v is true
	template <size_t N>
	inline bool any(const basic_vec<bool, N> &v) {
		return fold([](auto x, auto y) { return x || y; }, false, v);
	}

	// Returns true only if all components of x are true
	template <size_t N>
	inline bool all(const basic_vec<bool, N> &v) {
		return fold([](auto x, auto y) { return x && y; }, true, v);
	}

	// Note : C++ does not support "not" as a function name, hence it has been omitted

	//TODO
	// 8 Integer Functions





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////     _____           _                  __ __      __       _                                                      ////
	////    / ____|         | |                / / \ \    / /      | |                                                   ////
	////   | (___   ___ __ _| | __ _ _ __     / /   \ \  / /__  ___| |_ ___  _ __                                      ////
	////    \___ \ / __/ _` | |/ _` | '__|   / /     \ \/ / _ \/ __| __/ _ \| '__|                                   ////
	////    ____) | (_| (_| | | (_| | |     / /       \  /  __/ (__| || (_) | |                                        ////
	////   |_____/ \___\__,_|_|\__,_|_|    /_/         \/ \___|\___|\__\___/|_|                                          ////
	////                                                                                                                   ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Angle and Trigonometry Functions
	// 

	// Converts degrees to radians, i.e., degrees * pi/180
	template <typename T> inline T radians(T x) {
		return x * pi / T(180.0);
	}

	// Converts radians to degrees, i.e., radians * 180/pi
	template <typename T> inline T degrees(T x) {
		return x / pi * T(180.0);
	}

	template <typename T> inline T csc(const T &x) {
		using namespace std;
		return T(1) / sin(x);
	}

	template <typename T> inline T sec(const T &x) {
		using namespace std;
		return T(1) / cos(x);
	}

	template <typename T> inline T cot(const T &x) {
		using namespace std;
		return T(1) / tan(x);
	}

	template <typename T> inline T acsc(const T &x) {
		using namespace std;
		return asin(T(1) / x);
	}

	template <typename T> inline T asec(const T &x) {
		using namespace std;
		return acos(T(1) / x);
	}

	template <typename T> inline T acot(const T &x) {
		using namespace std;
		return atan(T(1) / x);
	}

	template <typename T> inline T csch(const T &x) {
		using namespace std;
		return T(1) / sinh(x);
	}
	
	template <typename T> inline T sech(const T &x) {
		using namespace std;
		return T(1) / cosh(x);
	}
	
	template <typename T> inline T coth(const T &x) {
		using namespace std;
		return cosh(x) / sinh(x);
	}

	template <typename T> inline T acsch(const T &x) {
		using namespace std;
		return log(T(1) / x + sqrt((x*x) + T(1)) / abs(x));
	}
	
	template <typename T> inline T asech(const T &x) {
		using namespace std;
		return log(T(1) / x + sqrt(T(1) - (x*x)) / x);
	}
	
	template <typename T> inline T acoth(const T &x) {
		using namespace std;
		return T(0.5) * log((x + T(1)) / (x - T(1)));
	}


	// Exponential Functions
	// 

	// Returns 2 raised to the x power, i.e., 2^x
	template <typename T> inline T exp2(const T &x) {
		using namespace std;
		return pow(T(2), x);
	}

	// Returns the base 2 logarithm of x, i.e., returns the value
	// y which satisfies the equation x=2^y
	// Results are undefined if x <= 0
	template <typename T> inline T log2(const T &x) {
		using namespace std;
		return log(x) * T(1.4426950408889634073599246810019);
	}

	// Returns 1/sqrt(x)
	// Results are undefined if x < 0
	template <typename T> inline T inversesqrt(const T &x) {
		using namespace std;
		return T(1)/sqrt(x);
	}


	// Common Functions
	// 

	// fractional part in terms of floor : x-floor(x)
	template <typename T> inline T fract(const T &x) {
		using namespace std;
		return x-floor(x);
	}









}





