//----------------------------------------------------------------------------
// 
// CGRA Math Library
//
//
//----------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>






namespace cgra {


	template <typename, size_t> class basic_vec;
	template <typename, size_t, size_t> class basic_mat;
	template <typename> class basic_quat;

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

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << ')';
		}
	};


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
			return *(this);
		}

		T & operator[](size_t i) {
			assert(false);
			return *(this);
		}

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << ')';
		}
	};


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

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ')';
		}
	};


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


		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ')';
		}
	};


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

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
		}
	};


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

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
		}
	};


	template <typename T, size_t M, size_t N>
	class basic_mat {
	private:
		T m_data[M*N];

	public:
		using value_t = T;
		static constexpr size_t cols = M;
		static constexpr size_t rows = N;
		static constexpr size_t size = M*N;

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

		// TODO stream insertion
	};


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

	};


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









}





