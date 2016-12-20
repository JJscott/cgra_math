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



}





