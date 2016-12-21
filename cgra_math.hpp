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

	// use like: inf<float>()
	// only for floating point types
	template <typename T>
	inline T inf() {
		return std::numeric_limits<T>::infinity();
	}

	// use like: nan<float>()
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
		basic_vec() { }

		const T & operator[](size_t i) const {
			assert(i < N);
			return m_data[i];
		}

		T & operator[](size_t i) {
			assert(i < N);
			return m_data[i];
		}

		T * data() { return &m_data[0]; }
		const T * data() const { return &m_data[0]; }

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

		basic_vec() { }
		template <typename U>
		basic_vec(const basic_vec<U, 0> &v) { }

		const T & operator[](size_t i) const {
			assert(false);
			return reinterpret_cast<const T &>(*this);
		}

		T & operator[](size_t i) {
			assert(false);
			return reinterpret_cast<T &>(*this);
		}

		T * data() { return reinterpret_cast<T *>(this); }
		const T * data() const { return reinterpret_cast<const T *>(this); }

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

		basic_vec() : x(0) { }
		explicit basic_vec(T s) : x(s) { }
		explicit basic_vec(const T *p) : x(p[0]) { }
		template <typename U>
		basic_vec(const basic_vec<U, 1> &v) : x(v.x) { }

		const T & operator[](size_t i) const {
			assert(i < 1);
			return (&x)[i];
		}

		T & operator[](size_t i) {
			assert(i < 1);
			return (&x)[i];
		}

		T * data() { return &x; }
		const T * data() const { return &x; }

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

		basic_vec() : x(0), y(0) { }
		basic_vec(T _x, T _y) : x(_x), y(_y) { }
		explicit basic_vec(T s) : x(s), y(s) { }
		explicit basic_vec(const T *p) : x(p[0]), y(p[1]) { }
		template <typename U>
		basic_vec(const basic_vec<U, 2> &v) : x(v.x), y(v.y) { }

		const T & operator[](size_t i) const {
			assert(i < 2);
			return (&x)[i];
		}

		T & operator[](size_t i) {
			assert(i < 2);
			return (&x)[i];
		}

		T * data() { return &x; }
		const T * data() const { return &x; }

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

		basic_vec() : x(0), y(0), z(0) { }
		basic_vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }
		explicit basic_vec(T s) : x(s), y(s), z(s) { }
		explicit basic_vec(const T *p) : x(p[0]), y(p[1]), z(p[2]) { }
		template <typename U>
		basic_vec(const basic_vec<U, 3> &v) : x(v.x), y(v.y), z(v.z) { }

		const T & operator[](size_t i) const {
			assert(i < 3);
			return (&x)[i];
		}

		T & operator[](size_t i) {
			assert(i < 3);
			return (&x)[i];
		}

		T * data() { return &x; }
		const T * data() const { return &x; }

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

		basic_vec() : x(0), y(0), z(0), w(0) { }
		basic_vec(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
		explicit basic_vec(T s) : x(s), y(s), z(s), w(s) { }
		explicit basic_vec(const T *p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) { }
		template <typename U>
		basic_vec(const basic_vec<U, 4> &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

		const T & operator[](size_t i) const {
			assert(i < 4);
			return (&x)[i];
		}

		T & operator[](size_t i) {
			assert(i < 4);
			return (&x)[i];
		}

		T * data() { return &x; }
		const T * data() const { return &x; }

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

	void zip_with() {

	}




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

	template <typename T> inline T radians(T x) {
		return x * pi / T(180.0);
	}

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

	// base 2 exponentiation
	template <typename T> inline T exp2(const T &x) {
		using namespace std;
		return pow(T(2), x);
	}

	// base 2 logarithm
	template <typename T> inline T log2(const T &x) {
		using namespace std;
		return log(x) * T(1.4426950408889634073599246810019);
	}

	// inverse square root
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





