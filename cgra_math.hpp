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
- clean up common function definitions and remove duplicates
- fix generic constructor for vectors (concat etc)
- comments and documentation
- create inputstream operators for data types to recreate from output
- TODO implement alternitives to body-3-2-1 euler rotation
- - http://www.geometrictools.com/Documentation/EulerAngles.pdf
*/

#pragma once

#include <cassert> // assert
#include <cmath>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility> // std::declval


// Visual Studio prior to 2017 doesn't support constexpr well enough for our types
#if defined(_MSC_VER) && _MSC_VER < 1910
#define CGRA_CONSTEXPR_FUNCTION
#endif

// we require constexpr for variables, but it is optional for functions
#ifndef CGRA_CONSTEXPR_FUNCTION
#define CGRA_CONSTEXPR_FUNCTION constexpr
#endif

// We undefine min and max macros if they exist
// so it doesn't interfer with our function overloads
// @$#! macros...
#undef MIN
#undef min
#undef MAX
#undef max


namespace cgra {

	// Forward declarations
	template <typename T> class not_nan;
	template <typename, size_t> class basic_vec;
	template <typename, size_t, size_t> class basic_mat;
	template <typename> class basic_quat;
	

	// pi
	constexpr double pi = 3.1415926535897932384626433832795;

	// natural log base
	constexpr double e = 2.7182818284590452353602874713527;

	// golden ratio
	constexpr double phi = 1.61803398874989484820458683436563811;

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

	// not-NaN type
	// asserts using ctor that value is non-nan
	template <typename T>
	class not_nan {
	private:
		T m_v;

		void check() const {
			assert(m_v == m_v && "nan");
		}

	public:
		not_nan() : m_v{} {}

		not_nan(const T &t_) : m_v{ t_ } {
			check();
		}

		not_nan(T &&t_) : m_v(std::move(t_)) {
			check();
		}

		operator const T & () const {
			check();
			return m_v;
		}

		friend void swap(not_nan &a, not_nan &b) {
			using std::swap;
			swap(a.m_v, b.m_v);
		}

		not_nan & operator=(const T &t) {
			m_v = t;
			check();
			return *this;
		}

		not_nan & operator=(T &&t) {
			m_v = std::move(t);
			check();
			return *this;
		}

		not_nan & operator+=(const T &rhs) {
			m_v += rhs;
			check();
			return *this;
		}

		not_nan & operator-=(const T &rhs) {
			m_v -= rhs;
			check();
			return *this;
		}

		not_nan & operator*=(const T &rhs) {
			m_v *= rhs;
			check();
			return *this;
		}

		not_nan & operator/=(const T &rhs) {
			m_v /= rhs;
			check();
			return *this;
		}

		not_nan & operator++() {
			m_v++;
			return *this;
		}
		
		not_nan operator++(int) {
			T temp = m_v;
			++*this;
			return temp;
		}
  
		not_nan & operator--() {
			m_v--;
			return *this;
		}
		
		not_nan operator--(int) {
			T temp = m_v;
			--*this;
			return temp;
		}
	};

	template <typename T>
	inline not_nan<T> operator+(const not_nan<T> &lhs, const T &rhs) {
		return not_nan<T>(lhs) += rhs;
	}

	template <typename T>
	inline not_nan<T> operator+(const T &lhs, const not_nan<T> &rhs) {
		return not_nan<T>(lhs) += rhs;
	}

	template <typename T>
	inline not_nan<T> operator-(const not_nan<T> &lhs, const T &rhs) {
		return not_nan<T>(lhs) -= rhs;
	}

	template <typename T>
	inline not_nan<T> operator-(const T &lhs, const not_nan<T> &rhs) {
		return not_nan<T>(lhs) -= rhs;
	}

	template <typename T>
	inline not_nan<T> operator*(const not_nan<T> &lhs, const T &rhs) {
		return not_nan<T>(lhs) *= rhs;
	}

	template <typename T>
	inline not_nan<T> operator*(const T &lhs, const not_nan<T> &rhs) {
		return not_nan<T>(lhs) *= rhs;
	}

	template <typename T>
	inline not_nan<T> operator/(const not_nan<T> &lhs, const T &rhs) {
		return not_nan<T>(lhs) /= rhs;
	}

	template <typename T>
	inline not_nan<T> operator/(const T &lhs, const not_nan<T> &rhs) {
		return not_nan<T>(lhs) /= rhs;
	}


	using nnfloat = not_nan<float>;
	using nndouble = not_nan<double>;







	//TODO insert lol RANDOM xD header here

	//  .______          ___      .__   __.  _______   ______   .___  ___.  //
	//  |   _  \        /   \     |  \ |  | |       \ /  __  \  |   \/   |  //
	//  |  |_)  |      /  ^  \    |   \|  | |  .--.  |  |  |  | |  \  /  |  //
	//  |      /      /  /_\  \   |  . `  | |  |  |  |  |  |  | |  |\/|  |  //
	//  |  |\  \----./  _____  \  |  |\   | |  '--'  |  `--'  | |  |  |  |  //
	//  | _| `._____/__/     \__\ |__| \__| |_______/ \______/  |__|  |__|  //
	//                                                                      //
	//======================================================================//

	namespace detail {

		template <typename T, typename = void>
		struct distribution {};

		template <typename T>
		using distribution_t = typename distribution<T>::type;

	}


	template <typename T, size_t N>
	class uniform_vec_distribution {
	public:
		using result_type = basic_vec<T, N>;
		using elem_dist_type = detail::distribution_t<T>;

		class param_type {
		private:
			result_type m_a;
			result_type m_b;

		public:
			using distribution_type = uniform_vec_distribution;

			param_type(const T &a = T(0), const T &b = T(1)) : m_a(a), m_b(b) { }
			param_type(const result_type &a = result_type(0), const result_type &b = result_type(1)) : m_a(a), m_b(b) { }

			result_type const a() { return m_a; }
			result_type const b() { return m_b; }

			friend bool operator==(const param_type &p1, const param_type &p2) {
				return p1.m_a == p2.m_a && p1.m_b == p2.m_b;
			}
		};

	private:
		param_type m_param;
		elem_dist_type m_elem_dist;

	public: 
		uniform_vec_distribution() : m_param(result_type(0), result_type(1)) { }
		uniform_vec_distribution(const param_type& param) : m_param(param) { }

		result_type const a() { return m_param.a(); }
		result_type const b() { return m_param.b(); }

		void reset() { }

		param_type const param() { return m_param; }
		
		void param(const param_type &param) { m_param = param; }

		result_type const min() { return this->a(); }
		result_type const max() { return this->b(); }

		template <typename Generator>
		result_type operator()(Generator& g) {
			return (*this)(g, this->param());
		}

		template <typename Generator>
		result_type operator()(Generator& g, param_type param) {
			result_type r;
			for (int i = 0; i < r.size; ++i)
				r[i] = m_elem_dist(g, typename elem_dist_type::param_type(param.a()[i], param.b()[i]));
			return r;
		}

		friend bool operator==(const uniform_vec_distribution &d1, const uniform_vec_distribution &d2) {
			return d1.param() == d1.param();
		}

		friend bool operator!=(const uniform_vec_distribution &d1, const uniform_vec_distribution &d2) {
			return !(d1 == d2);
		}

		//TODO 
		// << >>
	};


	template <typename T, size_t Cols, size_t Rows>
	class uniform_mat_distribution {
	public:
		using result_type = basic_mat<T, Cols, Rows>;
		using elem_dist_type = detail::distribution_t<T>;

		class param_type {
		private:
			result_type m_a;
			result_type m_b;

		public:
			using distribution_type = uniform_mat_distribution;

			param_type(const T &a = T(0), const T &b = T(1)) : m_a(a), m_b(b) { }
			param_type(const result_type &a = result_type(0), const result_type &b = result_type(1)) : m_a(a), m_b(b) { }

			result_type const a() { return m_a; }
			result_type const b() { return m_b; }

			friend bool operator==(const param_type &p1, const param_type &p2) {
				return p1.m_a == p2.m_a && p1.m_b == p2.m_b;
			}
		};

	private:
		param_type m_param;
		elem_dist_type m_elem_dist;

	public: 
		uniform_mat_distribution() : m_param(result_type(0), result_type(1)) { }
		uniform_mat_distribution(const param_type& param) : m_param(param) { }

		result_type const a() { return m_param.a(); }
		result_type const b() { return m_param.b(); }

		void reset() { }

		param_type const param() { return m_param; }
		
		void param(const param_type &param) { m_param = param; }

		result_type const min() { return this->a(); }
		result_type const max() { return this->b(); }

		template <typename Generator>
		result_type operator()(Generator& g) {
			return (*this)(g, this->param());
		}

		template <typename Generator>
		result_type operator()(Generator& g, param_type param) {
			result_type r;
			for (int j = 0; j < r.cols; ++j)
				for (int i = 0; i < r.rows; ++i)
					r[j][i] = m_elem_dist(g, typename elem_dist_type::param_type(param.a()[j][i], param.b()[j][i]));
			return r;
		}

		friend bool operator==(const uniform_mat_distribution &d1, const uniform_mat_distribution &d2) {
			return d1.param() == d1.param();
		}

		friend bool operator!=(const uniform_mat_distribution &d1, const uniform_mat_distribution &d2) {
			return !(d1 == d2);
		}

		//TODO 
		// << >>
	};


	template <typename T>
	class uniform_quat_distribution {
		public:
		using result_type = basic_quat<T>;
		using elem_dist_type = detail::distribution_t<T>;
		using vec_dist_type = detail::distribution_t<basic_vec<T, 3>>;

		class param_type {
		private:
			T m_a;
			T m_b;

		public:
			using distribution_type = uniform_quat_distribution;

			param_type(const T &a = T(0), const T &b = T(pi)) : m_a(a), m_b(b) { }

			T const a() { return m_a; }
			T const b() { return m_b; }

			friend bool operator==(const param_type &p1, const param_type &p2) {
				return p1.m_a == p2.m_a && p1.m_b == p2.m_b;
			}
		};

	private:
		param_type m_param;
		elem_dist_type m_elem_dist;
		vec_dist_type m_vec_dist;

	public: 
		uniform_quat_distribution() : m_param(T(0), T(pi)) { }
		uniform_quat_distribution(const param_type& param) : m_param(param) { }

		T const a() { return m_param.a(); }
		T const b() { return m_param.b(); }

		void reset() { }

		param_type const param() { return m_param; }
		
		void param(const param_type &param) { m_param = param; }

		// These methods can not be implemented sensibly due to the nature of quaternions
		result_type const min() { assert(false); return basic_quat<T>(); }
		result_type const max() { assert(false); return basic_quat<T>(); }

		template <typename Generator>
		result_type operator()(Generator& g) {
			return (*this)(g, this->param());
		}

		template <typename Generator>
		result_type operator()(Generator& g, param_type param) {
			result_type r;
			// TODO make this true uniformally random
			T angle = m_elem_dist(g, elem_dist_type::param_type(param.a(), param.b()));
			basic_vec<T, 3> axis = normalize(m_vec_dist(g, vec_dist_type::param_type(basic_vec<T, 3>(-1), basic_vec<T, 3>(1))));
			r = axisangle<basic_quat<T>>(normalize(axis), angle);
			return r;
		}

		friend bool operator==(const uniform_quat_distribution &d1, const uniform_quat_distribution &d2) {
			return d1.param() == d1.param();
		}

		friend bool operator!=(const uniform_quat_distribution &d1, const uniform_quat_distribution &d2) {
			return !(d1 == d2);
		}

		//TODO 
		// << >>
	};


	// distribution specializations
	namespace detail {

		template <typename T>
		struct distribution<T, std::enable_if_t<std::is_integral<T>::value>> {
			using type = std::uniform_int_distribution<
				std::conditional_t<
					sizeof(T) < sizeof(short),
					std::conditional_t<
						std::is_signed<T>::value,
						short,
						unsigned short
					>,
					T
				>
			>;
		};

		template <typename T>
		struct distribution<T, std::enable_if_t<std::is_floating_point<T>::value>> {
			using type = std::uniform_real_distribution<T>;
		};

		template <typename T, size_t N>
		struct distribution<basic_vec<T, N>> {
			using type = uniform_vec_distribution<T, N>;
		};

		template <typename T, size_t Cols, size_t Rows>
		struct distribution<basic_mat<T, Cols, Rows>> {
			using type = uniform_mat_distribution<T, Cols, Rows>;
		};

		template <typename T>
		struct distribution<basic_quat<T>> {
			using type = uniform_quat_distribution<T>;
		};

		// singleton for random engine
		inline auto & random_engine() {
			static thread_local std::default_random_engine re { std::random_device()() };
			return re;
		}
	}

	// return a random value of T in range [lower, upper)
	template <typename T, typename P>
	inline T random(P lower, P upper) {
		using dist_t = detail::distribution_t<T>;
		dist_t dist(typename dist_t::param_type(lower, upper));
		return dist(detail::random_engine());
	}

	// return a random value of T in range [0, upper)
	template <typename T, typename P>
	inline T random(P upper) {
		return random<T, P>(std::decay_t<P>(0), upper);
	}

	// return a random value of T in the range defined by associated uniform distribution
	template <typename T>
	inline T random() {
		using dist_t = detail::distribution_t<T>;
		dist_t dist;
		return dist(detail::random_engine());
	}



//   _______       ___   .___________.    ___              _______.___________..______       __    __    ______ .___________. __    __  .______       _______     _______.  //
//  |       \     /   \  |           |   /   \            /       |           ||   _  \     |  |  |  |  /      ||           ||  |  |  | |   _  \     |   ____|   /       |  //
//  |  .--.  |   /  ^  \ `---|  |----`  /  ^  \          |   (----`---|  |----`|  |_)  |    |  |  |  | |  ,----'`---|  |----`|  |  |  | |  |_)  |    |  |__     |   (----`  //
//  |  |  |  |  /  /_\  \    |  |      /  /_\  \          \   \       |  |     |      /     |  |  |  | |  |         |  |     |  |  |  | |      /     |   __|     \   \      //
//  |  '--'  | /  _____  \   |  |     /  _____  \     .----)   |      |  |     |  |\  \----.|  `--'  | |  `----.    |  |     |  `--'  | |  |\  \----.|  |____.----)   |     //
//  |_______/ /__/     \__\  |__|    /__/     \__\    |_______/       |__|     | _| `._____| \______/   \______|    |__|      \______/  | _| `._____||_______|_______/      //
//                                                                                                                                                                          //
//==========================================================================================================================================================================//

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

	// Constructor magics
	// TODO description
	namespace detail {

		// foward decl
		template<typename T, size_t N> class repeat_vec;

		// note: msvc has some trouble with expansion of template (type) parameter packs that result in integers,
		// so we use integral_constant-like types instead (especially for seq_repeat, make_index_sequence).

		template <typename ...Ts>
		using void_t = void;

		template <size_t X>
		using index_constant = std::integral_constant<size_t, X>;

		struct vec_element_ctor_tag { };

		template <typename VecT>
		struct vec_traits { };

		template <typename T, size_t N>
		struct vec_traits<basic_vec<T, N>> {
			using value_t = T;
			static const size_t size = N;

			template <size_t I, typename VecT>
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) get(VecT &&v) {
				return std::forward<VecT>(v)[I];
			}
		};

		template <typename T, size_t N>
		struct vec_traits<repeat_vec<T, N>> : vec_traits<basic_vec<T, N>> { };

		template <typename T, size_t Cols, size_t Rows>
		struct vec_traits<basic_mat<T, Cols, Rows>> {
			using value_t = basic_vec<T, Rows>;
			static const size_t size = Cols;

			template <size_t I, typename VecT>
			constexpr static decltype(auto) get(VecT &&v) {
				return std::forward<VecT>(v)[I];
			}
		};

		// TODO vec traits for std::array, std::tuple

		template <typename VecT>
		using vec_value_t = typename vec_traits<std::decay_t<VecT>>::value_t;

		template <typename VecT, typename T, typename = void>
		struct is_relatively_scalar : std::false_type { };

		template <typename VecT, typename T>
		struct is_relatively_scalar<VecT, T, void_t<vec_value_t<VecT>>> :
			std::is_convertible<T, vec_value_t<VecT>>
		{ };

		template <typename VecT0, typename VecT1, typename = void, typename = void>
		struct is_relatively_vector : std::true_type { };

		template <typename VecT0, typename VecT1, typename X>
		struct is_relatively_vector<VecT0, VecT1, void_t<vec_value_t<VecT0>>, X> : std::false_type { };

		template <typename VecT0, typename VecT1, typename X>
		struct is_relatively_vector<VecT0, VecT1, X, void_t<vec_value_t<VecT1>>> : std::false_type { };

		template <typename VecT0, typename VecT1>
		struct is_relatively_vector<VecT0, VecT1, void_t<vec_value_t<VecT0>>, void_t<vec_value_t<VecT1>>> :
			std::is_convertible<vec_value_t<VecT1>, vec_value_t<VecT0>>
		{ };

		template <typename ...Seqs>
		struct seq_cat { };

		template <typename ...Seqs>
		using seq_cat_t = typename seq_cat<Seqs...>::type;

		template <typename Seq>
		struct seq_cat<Seq> {
			using type = Seq;
		};

		template <typename Seq0, typename Seq1, typename Seq2, typename ...Seqs>
		struct seq_cat<Seq0, Seq1, Seq2, Seqs...> {
			using type = seq_cat_t<seq_cat_t<Seq0, Seq1>, Seq2, Seqs...>;
		};

		template <typename T, T ...Is, T ...Js>
		struct seq_cat<std::integer_sequence<T, Is...>, std::integer_sequence<T, Js...>> {
			using type = std::integer_sequence<T, Is..., Js...>;
		};

		template <typename Seq, typename N>
		struct seq_repeat {
			using type = seq_cat_t<Seq, typename seq_repeat<Seq, index_constant<N::value - 1>>::type>;
		};

		template <typename Seq, typename N>
		using seq_repeat_t = typename seq_repeat<Seq, N>::type;

		template <typename Seq>
		struct seq_repeat<Seq, index_constant<0>> {
			using type = std::integer_sequence<typename Seq::value_type>;
		};

		template <typename Seq>
		struct seq_repeat<Seq, index_constant<1>> {
			using type = Seq;
		};

		template <typename Seq>
		struct seq_repeat<Seq, index_constant<2>> {
			using type = seq_cat_t<Seq, Seq>;
		};

		template <typename SeqR, typename Seq0, typename N, typename = void>
		struct seq_trim_impl {};

		template <typename SeqR, typename Seq0>
		struct seq_trim_impl<SeqR, Seq0, index_constant<0>> {
			using type = SeqR;
		};

		template <typename T, T ...Rs, T I0, T ...Is, typename N>
		struct seq_trim_impl<
			std::integer_sequence<T, Rs...>,
			std::integer_sequence<T, I0, Is...>,
			N, std::enable_if_t<(N::value > 0)>
		> {
			using type = typename seq_trim_impl<
				std::integer_sequence<T, Rs..., I0>,
				std::integer_sequence<T, Is...>,
				index_constant<N::value - 1>
			>::type;
		};

		template <typename Seq, typename N>
		struct seq_trim {
			static_assert(N::value <= Seq::size(), "sequence too small");
			using type = typename seq_trim_impl<std::integer_sequence<typename Seq::value_type>, Seq, N>::type;
		};

		template <typename Seq, typename N>
		using seq_trim_t = typename seq_trim<Seq, N>::type;

		template <typename T, bool IsVec>
		struct cat_arg_vec_size_impl : index_constant<1> { };

		template <typename T>
		struct cat_arg_vec_size_impl<T, true> : index_constant<vec_traits<std::decay_t<T>>::size> { };

		template <typename CatT, typename T>
		struct cat_arg_vec_size : cat_arg_vec_size_impl<T, is_relatively_vector<CatT, T>::value> { };

		template <typename CatT, typename Seq, typename Tup>
		struct cat_arg_seq { };

		template <typename CatT, size_t ...Is, typename ...Ts>
		struct cat_arg_seq<CatT, std::index_sequence<Is...>, std::tuple<Ts...>> {
			using type = seq_cat_t<std::index_sequence<>, seq_repeat_t<std::index_sequence<Is>, cat_arg_vec_size<CatT, Ts>>...>;
		};

		template <typename N>
		struct make_index_sequence {
			using type = std::make_index_sequence<N::value>;
		};

		template <typename N>
		using make_index_sequence_t = typename make_index_sequence<N>::type;

		template <typename CatT, typename Tup>
		struct cat_val_seq { };

		template <typename CatT, typename ...Ts>
		struct cat_val_seq<CatT, std::tuple<Ts...>> {
			using type = seq_cat_t<std::index_sequence<>, make_index_sequence_t<cat_arg_vec_size<CatT, Ts>>...>;
		};

		template <size_t I, typename T, bool IsVec>
		struct vec_get_impl {
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) go(T &&t) {
				return std::forward<T>(t);
			}
		};

		template <size_t I, typename T>
		struct vec_get_impl<I, T, true> {
			constexpr static decltype(auto) go(T &&t) {
				return vec_traits<std::decay_t<T>>::template get<I>(std::forward<T>(t));
			}
		};

		template <typename CatT, size_t I, typename T>
		CGRA_CONSTEXPR_FUNCTION decltype(auto) vec_get(T &&t) {
			static_assert(
				is_relatively_vector<CatT, T>::value || is_relatively_scalar<CatT, T>::value,
				"vector cat argument is not relatively vector or scalar"
				);
			return vec_get_impl<I, T, is_relatively_vector<CatT, T>::value>::go(std::forward<T>(t));
		}

		template <typename CatT, typename ArgTupT, size_t ...Js, size_t ...Ks>
		CGRA_CONSTEXPR_FUNCTION CatT cat_impl_impl(ArgTupT &&args, std::index_sequence<Js...>, std::index_sequence<Ks...>) {
			return CatT(detail::vec_element_ctor_tag(), vec_get<CatT, Ks>(std::get<Js>(std::forward<ArgTupT>(args)))...);
		}

		template <typename CatT, typename ...Ts>
		CGRA_CONSTEXPR_FUNCTION CatT cat_impl(Ts &&...args) {
			using argseq0 = typename cat_arg_seq<CatT, std::index_sequence_for<Ts...>, std::tuple<Ts...>>::type;
			using valseq0 = typename cat_val_seq<CatT, std::tuple<Ts...>>::type;
			// trim to size so that extra components are ignored
			using argseq = seq_trim_t<argseq0, index_constant<CatT::size>>;
			using valseq = seq_trim_t<valseq0, index_constant<CatT::size>>;
			// calling forward_as_tuple calls tuple's defaulted move ctor
			// this compiles fine in vs2017, but intellisense thinks it isn't constexpr
			// so, we call tuple's ctor directly to stop it complaining
			return cat_impl_impl<CatT>(std::tuple<Ts &&...>{std::forward<Ts>(args)...}, argseq(), valseq());
		}

		template <typename T>
		CGRA_CONSTEXPR_FUNCTION const T & constify(const T &t) {
			return t;
		}

		template <typename T>
		struct forward_or_cast_impl {
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) go(T &t) { return t; }
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) go(T &&t) { return std::move(t); };
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) go(const T &t) { return t; }

			template <typename U>
			CGRA_CONSTEXPR_FUNCTION static T go(U &&u) {
				return static_cast<T>(std::forward<U>(u));
			}
		};

		template <typename T, typename U>
		CGRA_CONSTEXPR_FUNCTION decltype(auto) forward_or_cast(U &&u) {
			return forward_or_cast_impl<T>::go(std::forward<U>(u));
		}

		template<typename T, size_t N>
		class repeat_vec {
		private:
			T m_v;

		public:
			using value_t = T;
			static constexpr size_t size = N;

			CGRA_CONSTEXPR_FUNCTION repeat_vec() { }

			template <typename U>
			CGRA_CONSTEXPR_FUNCTION explicit repeat_vec(U &&u) : m_v(std::forward<U>(u)) { }

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				assert(i < N);
				return m_v;
			}

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				assert(i < N);
				return m_v;
			}

			inline friend std::ostream & operator<<(std::ostream &out, const repeat_vec &v) {
				return out << '(' << v.m_v << ", ... , n=" << size << ")";
			}
		};

		template <typename T, size_t Cols, size_t Rows>
		CGRA_CONSTEXPR_FUNCTION auto make_mat(basic_vec<basic_vec<T, Rows>, Cols> &&v) {
			return basic_mat<T, Cols, Rows>(std::move(v));
		}
	}


	template <typename T, size_t N>
	class basic_vec {
	private:
		// not using std::array for better constexpr behaviour
		T m_data[N];

	public:
		using value_t = T;
		static constexpr size_t size = N;

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_vec() : basic_vec(T()) { }

		// tagged ctor, used by cat
		template <typename ...ArgTs>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(detail::vec_element_ctor_tag, ArgTs &&...args) :
#ifdef __EDG__
			// intellisense needs a hand here
			m_data{ detail::constify(detail::forward_or_cast<T>(std::forward<ArgTs>(args)))... }
#else
			// but for actual compilation, we should allow moving
			m_data { detail::forward_or_cast<T>(std::forward<ArgTs>(args))... }
#endif
		{ }

		// magic ctor
		// this is marked explicit mainly so that `is_relatively_*' don't determine that random things are convertible to this
		// enforcing >= 2 args is helpful but not strictly necessary
		// this is only a better match for 1 arg than the 1-arg ctors if they sfinae out
		// direct enforcement with explicit 1st and 2nd args results in this being used instead of the above tagged ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...args) :
			basic_vec(detail::cat_impl<basic_vec>(std::forward<ArgTs>(args)...))
		{ }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&v) :
			basic_vec(detail::cat_impl<basic_vec>(std::forward<VecT>(v)))
		{ }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) :
			// constify is to help intellisense, but should have no impact on actual compilation
			basic_vec(detail::constify(detail::repeat_vec<const T &, N>(t)))
		{ }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < N);
			return m_data[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < N);
			return m_data[i];
		}

		CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0]; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0]; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			out << '(' << v[0];
			for (size_t i = 1; i < N; ++i) {
				out << ", " << v[i];
			}
			out << ')';
			return out;
		}
	};



	// 0-dimesional vector explicit template specialization
	template <typename T>
	class basic_vec<T, 0> {
	public:
		using value_t = T;
		static constexpr size_t size = 0;

		CGRA_CONSTEXPR_FUNCTION basic_vec() { }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...) { }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&) { }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &) { }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(false);
			return reinterpret_cast<T &>(*this);
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(false);
			return reinterpret_cast<const T &>(*this);
		}

		T * data() { return reinterpret_cast<T *>(this); }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return reinterpret_cast<const T *>(this); }

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

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_vec() : x{} { }

		// tagged ctor, used by cat
		template <typename U1>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(detail::vec_element_ctor_tag, U1 &&x_) :
#ifdef __EDG__
			// intellisense needs a hand here
			x{ detail::constify(detail::forward_or_cast<T>(std::forward<U1>(x_))) }
#else
			// but for actual compilation, we should allow moving
			x{ detail::forward_or_cast<T>(std::forward<U1>(x_)) }
#endif
		{ }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...args) :	basic_vec(detail::cat_impl<basic_vec>(std::forward<ArgTs>(args)...)) { }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&v) :	basic_vec(detail::cat_impl<basic_vec>(std::forward<VecT>(v))) { }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) : basic_vec(detail::constify(detail::repeat_vec<const T &, 1>(t))) { }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < 1);
			return (&x)[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < 1);
			return (&x)[i];
		}

		T * data() { return &x; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &x; }

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

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_vec() : x{}, y{} { }

		// tagged ctor, used by cat
		template <typename U1, typename U2>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(detail::vec_element_ctor_tag, U1 &&x_, U2 &&y_) :
#ifdef __EDG__
			// intellisense needs a hand here
			x{ detail::constify(detail::forward_or_cast<T>(std::forward<U1>(x_))) },
			y{ detail::constify(detail::forward_or_cast<T>(std::forward<U2>(y_))) }
#else
			// but for actual compilation, we should allow moving
			x{ detail::forward_or_cast<T>(std::forward<U1>(x_)) },
			y{ detail::forward_or_cast<T>(std::forward<U2>(y_)) }
#endif
		{ }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...args) : basic_vec(detail::cat_impl<basic_vec>(std::forward<ArgTs>(args)...)) { }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&v) :	basic_vec(detail::cat_impl<basic_vec>(std::forward<VecT>(v))) { }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) : 	basic_vec(detail::constify(detail::repeat_vec<const T &, 2>(t))) { }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < 2);
			return (&x)[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < 2);
			return (&x)[i];
		}

		T * data() { return &x; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &x; }

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

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_vec() : x{}, y{}, z{} { }
		
		// tagged ctor, used by cat
		template <typename U1, typename U2, typename U3>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(detail::vec_element_ctor_tag, U1 &&x_, U2 &&y_, U3 &&z_) :
#ifdef __EDG__
			// intellisense needs a hand here
			x{ detail::constify(detail::forward_or_cast<T>(std::forward<U1>(x_))) },
			y{ detail::constify(detail::forward_or_cast<T>(std::forward<U2>(y_))) },
			z{ detail::constify(detail::forward_or_cast<T>(std::forward<U3>(z_))) }
#else
			// but for actual compilation, we should allow moving
			x{ detail::forward_or_cast<T>(std::forward<U1>(x_)) },
			y{ detail::forward_or_cast<T>(std::forward<U2>(y_)) },
			z{ detail::forward_or_cast<T>(std::forward<U3>(z_))	}
#endif
		{ }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...args) : basic_vec(detail::cat_impl<basic_vec>(std::forward<ArgTs>(args)...)) { }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&v) : basic_vec(detail::cat_impl<basic_vec>(std::forward<VecT>(v))) { }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) : basic_vec(detail::constify(detail::repeat_vec<const T &, 3>(t))) { }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < 3);
			return (&x)[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < 3);
			return (&x)[i];
		}

		T * data() { return &x; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &x; }

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

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_vec() : x{}, y{}, z{}, w{} { }
		
		// tagged ctor, used by cat
		template <typename U1, typename U2, typename U3, typename U4>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(detail::vec_element_ctor_tag, U1 &&x_, U2 &&y_, U3 &&z_, U4 &&w_) :
#ifdef __EDG__
			// intellisense needs a hand here
			x{ detail::constify(detail::forward_or_cast<T>(std::forward<U1>(x_))) },
			y{ detail::constify(detail::forward_or_cast<T>(std::forward<U2>(y_))) },
			z{ detail::constify(detail::forward_or_cast<T>(std::forward<U3>(z_))) },
			w{ detail::constify(detail::forward_or_cast<T>(std::forward<U4>(w_))) }
#else
			// but for actual compilation, we should allow moving
			x{ detail::forward_or_cast<T>(std::forward<U1>(x_)) },
			y{ detail::forward_or_cast<T>(std::forward<U2>(y_)) },
			z{ detail::forward_or_cast<T>(std::forward<U3>(z_)) },
			w{ detail::forward_or_cast<T>(std::forward<U4>(w_))	}
#endif
		{ }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(ArgTs &&...args) : basic_vec(detail::cat_impl<basic_vec>(std::forward<ArgTs>(args)...)) { }

		// 1-arg magic ctor
		template <typename VecT, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec, VecT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_vec(VecT &&v) : basic_vec(detail::cat_impl<basic_vec>(std::forward<VecT>(v))) { }

		// scalar broadcast ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) : basic_vec(detail::constify(detail::repeat_vec<const T &, 4>(t)))	{ }

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < 4);
			return (&x)[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < 4);
			return (&x)[i];
		}

		T * data() { return &x; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &x; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_vec &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
		}
	};


	// [Cols, Rows]-dimensional matrix class of type T
	template <typename T, size_t Cols, size_t Rows>
	class basic_mat {
	private:
		basic_vec<basic_vec<T, Rows>, Cols> m_data; // avoids reinterpret_cast

	public:
		using value_t = T;
		static constexpr size_t cols = Cols;
		static constexpr size_t rows = Rows;
		static constexpr size_t size = Cols * Rows;

		// default ctor
		CGRA_CONSTEXPR_FUNCTION basic_mat() { }

		// magic ctor
		template <typename ...ArgTs, typename = std::enable_if_t<(sizeof...(ArgTs) >= 2)>>
		CGRA_CONSTEXPR_FUNCTION explicit basic_mat(ArgTs &&...args) : m_data{ std::forward<ArgTs>(args)... } {}

		// 1-arg magic ctor
		template <typename MatT, typename = std::enable_if_t<detail::is_relatively_vector<basic_mat, MatT>::value>, typename = void>
		CGRA_CONSTEXPR_FUNCTION basic_mat(MatT &&v) : m_data{ std::forward<MatT>(v) } {}

		// identity ctor
		CGRA_CONSTEXPR_FUNCTION explicit basic_mat(const value_t &v_) {
			for (size_t i = 0; i < std::min(Cols, Rows); i++) {
				(*this)[i][i] = v_;
			}
		}

		CGRA_CONSTEXPR_FUNCTION auto & operator[](size_t i) {
			assert(i < Cols);
			return m_data[i];
		}

		CGRA_CONSTEXPR_FUNCTION const auto & operator[](size_t i) const {
			assert(i < Cols);
			return m_data[i];
		}

		CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0][0]; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0][0]; }
		
		CGRA_CONSTEXPR_FUNCTION auto & as_vec() { return m_data; }

		CGRA_CONSTEXPR_FUNCTION const auto & as_vec() const { return m_data; }

		// stream insertion
		inline friend std::ostream & operator<<(std::ostream &out, const basic_mat &m) {
			const size_t field_width = 10;
			std::ostringstream oss;
			oss << std::setprecision(4);
			for (size_t r = 0; r < Rows; ++r) {
				oss << '[' << std::setw(field_width) << m[0][r];
				for (size_t c = 1; c < Cols; ++c)
					oss << ", " << std::setw(field_width) << m[c][r];
				oss << ']';
				if (r < Rows-1)
					oss << std::endl;
			}
			return out << oss.str();
		}
	};


	// Quaternion class of type T
	template <typename T>
	class basic_quat : private basic_vec<T, 4> {
	public:
		using value_t = T;
		static constexpr size_t size = 4;

		using basic_vec<T, 4>::x;
		using basic_vec<T, 4>::y;
		using basic_vec<T, 4>::z;
		using basic_vec<T, 4>::w;

		CGRA_CONSTEXPR_FUNCTION basic_quat() : basic_vec<T, 4>(0, 0, 0, 1) { }

		CGRA_CONSTEXPR_FUNCTION basic_quat(T _x, T _y, T _z, T _w) :  basic_vec<T, 4>(_x, _y, _z, _w) { }

		template <typename U>
		CGRA_CONSTEXPR_FUNCTION basic_quat(const basic_quat<U>& q) : basic_vec<T, 4>(q.as_vec()) { }

		CGRA_CONSTEXPR_FUNCTION basic_quat(const basic_vec<T, 3> &v, T _w) : basic_vec<T, 4>(v, _w) { }

		CGRA_CONSTEXPR_FUNCTION basic_quat(const basic_vec<T, 3> &v1, const basic_vec<T, 1> &v2) :  basic_vec<T, 4>(v1, v2) { }

		CGRA_CONSTEXPR_FUNCTION basic_quat(const basic_vec<T, 4> &v) : basic_vec<T, 4>(v) { }

		// basic_mat<U, 3, 3> converter
		template <typename U>
		explicit operator basic_mat<U, 3, 3>() const {
			basic_mat<U, 3, 3> m;

			m[0][0] = w * w + x * x - y * y - z * z;
			m[0][1] = 2 * x * y + 2 * w * z;
			m[0][2] = 2 * x * z - 2 * w * y;

			m[1][0] = 2 * x * y - 2 * w * z;
			m[1][1] = w * w - x * x + y * y - z * z;
			m[1][2] = 2 * y * z + 2 * w * x;

			m[2][0] = 2 * x * z + 2 * w * y;
			m[2][1] = 2 * y * z - 2 * w * x;
			m[2][2] = w * w - x * x - y * y + z * z;

			return m;
		}

		// basic_mat<U, 4, 4> converter
		template <typename U>
		explicit operator basic_mat<U, 4, 4>() const {
			basic_mat<U, 4, 4> m;

			m[0][0] = w * w + x * x - y * y - z * z;
			m[0][1] = 2 * x * y + 2 * w * z;
			m[0][2] = 2 * x * z - 2 * w * y;
			m[0][3] = 0;

			m[1][0] = 2 * x * y - 2 * w * z;
			m[1][1] = w * w - x * x + y * y - z * z;
			m[1][2] = 2 * y * z + 2 * w * x;
			m[1][3] = 0;

			m[2][0] = 2 * x * z + 2 * w * y;
			m[2][1] = 2 * y * z - 2 * w * x;
			m[2][2] = w * w - x * x - y * y + z * z;
			m[2][3] = 0;

			m[3][0] = 0;
			m[3][1] = 0;
			m[3][2] = 0;
			m[3][3] = w * w + x * x + y * y + z * z;

			return m;
		}

		CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
			assert(i < 4);
			return (&x)[i];
		}

		CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
			assert(i < 4);
			return (&x)[i];
		}

		T * data() { return &x; }

		CGRA_CONSTEXPR_FUNCTION const T * data() const { return &x; }

		basic_vec<T, 4> & as_vec() { return *this; }

		CGRA_CONSTEXPR_FUNCTION const basic_vec<T, 4> & as_vec() const { return *this; }

		inline friend std::ostream & operator<<(std::ostream &out, const basic_quat &v) {
			return out << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
		}

	};



	//    ______   .______    _______ .______          ___   .___________.  ______   .______           ______   ____    ____  _______ .______       __        ______        ___       _______       _______.  //
	//   /  __  \  |   _  \  |   ____||   _  \        /   \  |           | /  __  \  |   _  \         /  __  \  \   \  /   / |   ____||   _  \     |  |      /  __  \      /   \     |       \     /       |  //
	//  |  |  |  | |  |_)  | |  |__   |  |_)  |      /  ^  \ `---|  |----`|  |  |  | |  |_)  |       |  |  |  |  \   \/   /  |  |__   |  |_)  |    |  |     |  |  |  |    /  ^  \    |  .--.  |   |   (----`  //
	//  |  |  |  | |   ___/  |   __|  |      /      /  /_\  \    |  |     |  |  |  | |      /        |  |  |  |   \      /   |   __|  |      /     |  |     |  |  |  |   /  /_\  \   |  |  |  |    \   \      //
	//  |  `--'  | |  |      |  |____ |  |\  \----./  _____  \   |  |     |  `--'  | |  |\  \----.   |  `--'  |    \    /    |  |____ |  |\  \----.|  `----.|  `--'  |  /  _____  \  |  '--'  |.----)   |     //
	//   \______/  | _|      |_______|| _| `._____/__/     \__\  |__|      \______/  | _| `._____|    \______/      \__/     |_______|| _| `._____||_______| \______/  /__/     \__\ |_______/ |_______/      //
	//                                                                                                                                                                                                        //
	//========================================================================================================================================================================================================//

	namespace detail {
		struct nothing { };

		namespace op {
			struct neg {
				template <typename T>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T &&t) const {
					return -std::forward<T>(t);
				}
			};

			struct add {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) + std::forward<T2>(t2);
				}
			};

			struct sub {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) - std::forward<T2>(t2);
				}
			};

			struct mul {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) * std::forward<T2>(t2);
				}
			};

			struct div {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) / std::forward<T2>(t2);
				}
			};

			struct mod {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) % std::forward<T2>(t2);
				}
			};


			struct add_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) += std::forward<T2>(t2); return{};
				}
			};

			struct sub_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) -= std::forward<T2>(t2); return{};
				}
			};

			struct mul_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) *= std::forward<T2>(t2); return{};
				}
			};

			struct div_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) /= std::forward<T2>(t2); return{};
				}
			};

			struct mod_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) %= std::forward<T2>(t2); return{};
				}
			};

			
			struct lshift {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) << std::forward<T2>(t2);
				}
			};

			struct rshift {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) >> std::forward<T2>(t2);
				}
			};

			struct lshift_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) <<= std::forward<T2>(t2); return{};
				}
			};

			struct rshift_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) >>= std::forward<T2>(t2); return{};
				}
			};


			struct logical_not {
				template <typename T>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T &&t) const {
					return !std::forward<T>(t);
				}
			};

			struct logical_or {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) || std::forward<T2>(t2);
				}
			};

			struct logical_and {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) && std::forward<T2>(t2);
				}
			};


			struct bitwise_not {
				template <typename T>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T &&t) const {
					return ~std::forward<T>(t);
				}
			};

			struct bitwise_or {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) | std::forward<T2>(t2);
				}
			};

			struct bitwise_xor {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) ^ std::forward<T2>(t2);
				}
			};

			struct bitwise_and {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) & std::forward<T2>(t2);
				}
			};

			struct bitwise_or_assign  {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) |= std::forward<T2>(t2); return{};
				}
			};

			struct bitwise_xor_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) ^= std::forward<T2>(t2); return{};
				}
			};

			struct bitwise_and_assign {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION nothing operator()(T1 &&t1, T2 &&t2) const {
					std::forward<T1>(t1) &= std::forward<T2>(t2); return{};
				}
			};


			struct equal {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) == std::forward<T2>(t2);
				}
			};

			struct nequal {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) != std::forward<T2>(t2);
				}
			};

			struct less {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) < std::forward<T2>(t2);
				}
			};

			struct greater {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) > std::forward<T2>(t2);
				}
			};

			struct less_equal {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) <= std::forward<T2>(t2);
				}
			};

			struct greater_equal {
				template <typename T1, typename T2>
				CGRA_CONSTEXPR_FUNCTION auto operator()(T1 &&t1, T2 &&t2) const {
					return std::forward<T1>(t1) >= std::forward<T2>(t2);
				}
			};

		}
	}

	// Vector assign operators
	// 

	// addition

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator+=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::add_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator+=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::add_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// subtraction

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator-=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::sub_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator-=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::sub_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// multiplication

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator*=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::mul_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator*=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::mul_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// division

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator/=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::div_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator/=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::div_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// remainder (mod)

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator%=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::mod_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator%=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::mod_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// left-shift

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator<<=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::lshift_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator<<=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::lshift_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// right-shift

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator>>=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::rshift_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator>>=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::rshift_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// logical or

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator|=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::bitwise_or_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator|=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_or_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// logical xor

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator^=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::bitwise_xor_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator^=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_xor_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}

	// logical and

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator&=(basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		zip_with(detail::op::bitwise_and_assign(), lhs, rhs);
		return lhs;
	}

	template<typename T1, size_t N, typename T2>
	inline basic_vec<T1, N> & operator&=(basic_vec<T1, N> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_and_assign(), lhs, basic_vec<T2, N>(rhs));
		return lhs;
	}


	// Vector operators
	// 

	// negate

	template<typename T, size_t N>
	inline auto operator-(const basic_vec<T, N> &rhs) {
		return zip_with(detail::op::neg(), rhs);
	}

	// addition

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator+(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::add(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator+(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::add(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator+(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::add(), basic_vec<T1, N>(lhs), rhs);
	}

	// subtraction

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator-(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::sub(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator-(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::sub(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator-(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::sub(), basic_vec<T1, N>(lhs), rhs);
	}

	// multiplication

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator*(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::mul(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator*(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::mul(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator*(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::mul(), basic_vec<T1, N>(lhs), rhs);
	}

	// division

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator/(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::div(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator/(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::div(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator/(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::div(), basic_vec<T1, N>(lhs), rhs);
	}

	// remainder (mod)

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator%(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::mod(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator%(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::mod(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator%(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::mod(), basic_vec<T1, N>(lhs), rhs);
	}

	// left-shift

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator<<(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::lshift(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator<<(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::lshift(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator<<(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::lshift(), basic_vec<T1, N>(lhs), rhs);
	}

	// right-shift

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator>>(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::rshift(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator>>(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::rshift(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator>>(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::rshift(), basic_vec<T1, N>(lhs), rhs);
	}

	// logical not

	template<typename T, size_t N>
	inline auto operator!(const basic_vec<T, N> &rhs) {
		return zip_with(detail::op::logical_not(), rhs);
	}

	// logical or

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator||(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::logical_or(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator||(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::logical_or(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator||(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::logical_or(), basic_vec<T1, N>(lhs), rhs);
	}

	// logical and

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator&&(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::logical_and(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator&&(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::logical_and(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator&&(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::logical_and(), basic_vec<T1, N>(lhs), rhs);
	}

	// bitwise not

	template<typename T, size_t N>
	inline auto operator~(const basic_vec<T, N> &rhs) {
		return zip_with(detail::op::bitwise_not(), rhs);
	}

	// bitwise or

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator|(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_or(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator|(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::bitwise_or(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator|(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_or(), basic_vec<T1, N>(lhs), rhs);
	}

	// bitwise xor

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator^(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_xor(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator^(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::bitwise_xor(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator^(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_xor(), basic_vec<T1, N>(lhs), rhs);
	}

	// bitwise and

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_vector<basic_vec<T1, N>, basic_vec<T2, N>>::value>>
	inline auto operator&(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_and(), lhs, rhs);
	}

	template<typename T1, size_t N, typename T2, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T1, N>, T2>::value>>
	inline auto operator&(const basic_vec<T1, N> &lhs, const T2 &rhs) {
		return zip_with(detail::op::bitwise_and(), lhs, basic_vec<T2, N>(rhs));
	}

	template<typename T1, typename T2, size_t N, typename = std::enable_if_t<detail::is_relatively_scalar<basic_vec<T2, N>, T1>::value>>
	inline auto operator&(const T1 &lhs, const basic_vec<T2, N> &rhs) {
		return zip_with(detail::op::bitwise_and(), basic_vec<T1, N>(lhs), rhs);
	}

	// equal

	template<typename T1, typename T2, size_t N>
	inline auto operator==(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return fold(detail::op::logical_and(), true, zip_with(detail::op::equal(), lhs, rhs));
	}

	// not equal

	template<typename T1, typename T2, size_t N>
	inline auto operator!=(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return fold(detail::op::logical_or(), false, zip_with(detail::op::nequal(), lhs, rhs));
	}

	// less than

	template<typename T1, typename T2, size_t N>
	inline auto operator<(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		return reinterpret_cast<const std::array<T1, N> &>(lhs) < reinterpret_cast<const std::array<T2, N> &>(rhs);
	}



	// Matrix assign operators
	// 

	// addition

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator+=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::add_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator+=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::add_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// subtraction

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator-=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::sub_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator-=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::sub_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// multiplication
	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator*=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Rows, Cols> &rhs) {
		lhs = lhs * rhs;
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator*=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::mul_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// division

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator/=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::div_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator/=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::div_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// remainder (mod)

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator%=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::mod_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator%=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::mod_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// left-shift

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator<<=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::lshift_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator<<=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::lshift_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// right-shift

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator>>=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::rshift_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator>>=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::rshift_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// logical or

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator|=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::bitwise_or_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator|=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_or_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// logical xor

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator^=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::bitwise_xor_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator^=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_xor_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}

	// logical and

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator&=(basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		zip_with(detail::op::bitwise_and_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline basic_mat<T1, Cols, Rows> & operator&=(basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		zip_with(detail::op::bitwise_and_assign(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec());
		return lhs;
	}


	// Matrix operators
	// 

	// negate

	template<typename T, size_t Cols, size_t Rows>
	inline auto operator-(const basic_mat<T, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::neg(), rhs.as_vec()));
	}

	// addition

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator+(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::add(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator+(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::add(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator+(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::add(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// subtraction

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator-(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::sub(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator-(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::sub(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator-(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::sub(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// multiplication
	template<typename T1, typename T2, size_t CommonSize, size_t Cols, size_t Rows>
	inline auto operator*(const basic_mat<T1, CommonSize, Rows> &lhs, const basic_mat<T2, Cols, CommonSize> &rhs) {
		return detail::make_mat(zip_with([&](auto &&rcol) { return dot(lhs.as_vec(), decltype(rcol)(rcol)); }, rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator*(const basic_mat<T1, Cols, Rows> &lhs, const basic_vec<T2, Cols> &rhs) {
		return dot(lhs.as_vec(), rhs);
		//return fold(detail::op::add(), basic_vec<T1, Rows>{}, zip_with(detail::op::mul(), lhs.as_vec(), rhs));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator*(const basic_vec<T1, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		//return dot(repeat_vec(lhs), rhs.as_vec());
		return fold(detail::op::add(), basic_vec<T1, Rows>(0), zip_with(detail::op::mul(), detail::repeat_vec<basic_vec<T1, Cols>, Rows>(lhs), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator*(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::mul(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator*(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::mul(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// division

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator/(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::div(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator/(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::div(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator/(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::div(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// remainder (mod)

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator%(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::mod(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator%(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::mod(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator%(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::mod(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// left-shift

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator<<(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::lshift(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator<<(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::lshift(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator<<(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::lshift(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// right-shift

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator>>(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::rshift(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator>>(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::rshift(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator>>(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::rshift(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// logical not

	template<typename T, size_t Cols, size_t Rows>
	inline auto operator!(const basic_mat<T, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_not(), rhs.as_vec().as_vec()));
	}

	// logical or

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator||(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_or(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator||(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_or(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator||(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_or(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// logical and

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator&&(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_and(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator&&(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_and(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator&&(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::logical_and(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// bitwise not

	template<typename T, size_t Cols, size_t Rows>
	inline auto operator~(const basic_mat<T, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_not(), rhs.as_vec().as_vec()));
	}

	// bitwise or

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator|(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_or(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator|(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_or(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator|(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_or(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// bitwise xor

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator^(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_xor(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator^(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_xor(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator^(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_xor(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// bitwise and

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator&(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_and(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, size_t Cols, size_t Rows, typename T2>
	inline auto operator&(const basic_mat<T1, Cols, Rows> &lhs, const T2 &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_and(), lhs.as_vec(), basic_mat<T2, Cols, Rows>(rhs).as_vec()));
	}

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator&(const T1 &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return detail::make_mat(zip_with(detail::op::bitwise_and(), basic_mat<T1, Cols, Rows>(lhs).as_vec(), rhs.as_vec()));
	}

	// equal

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator==(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return fold(detail::op::logical_and(), true, detail::make_mat(zip_with(detail::op::equal(), lhs.as_vec(), rhs.as_vec())));
	}

	// not equal

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator!=(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return fold(detail::op::logical_or(), false, detail::make_mat(zip_with(detail::op::nequal(), lhs.as_vec(), rhs.as_vec())));
	}

	// less than

	template<typename T1, typename T2, size_t Cols, size_t Rows>
	inline auto operator<(const basic_mat<T1, Cols, Rows> &lhs, const basic_mat<T2, Cols, Rows> &rhs) {
		return lhs.as_vec() < rhs.as_vec();
	}


	// Quaternion assign operators
	//

	// addition

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator+=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		zip_with(detail::op::add_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	// subtraction

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator-=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		zip_with(detail::op::sub_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	// multiplication

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator*=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		using common_t = std::common_type_t<T1, T2>;
		common_t x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
		common_t y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
		common_t z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
		common_t w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
		lhs.x = x;
		lhs.y = y;
		lhs.z = z;
		lhs.w = w;
		return lhs;
	}

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator*=(basic_quat<T1> &lhs, const T2 &rhs) {
		zip_with(detail::op::mul_assign(), lhs.as_vec(), basic_vec<T2, 4>(rhs));
		return lhs;
	}

	// division

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator/=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		zip_with(detail::op::div_assign(), lhs.as_vec(), rhs.as_vec());
		return lhs;
	}

	template<typename T1, typename T2>
	inline basic_quat<T1> & operator/=(basic_quat<T1> &lhs, const T2 &rhs) {
		zip_with(detail::op::div_assign(), lhs.as_vec(), basic_vec<T2, 4>(rhs));
		return lhs;
	}


	// Quaternion operators
	//

	// negate

	template<typename T>
	inline auto operator-(const basic_quat<T> &rhs) {
		return conjugate(rhs);
	}

	// addition

	template<typename T1, typename T2>
	inline auto operator+(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return basic_quat<T1>(zip_with(detail::op::add(), lhs.as_vec(), rhs.as_vec()));
	}

	// subtraction

	template<typename T1, typename T2>
	inline auto operator-(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return basic_quat<T1>(zip_with(detail::op::sub(), lhs.as_vec(), rhs.as_vec()));
	}

	// multiplication

	template<typename T1, typename T2>
	inline auto operator*(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		basic_quat<std::common_type_t<T1, T2>> r = lhs;
		r *= rhs;
		return r;
	}

	template<typename T1, typename T2>
	inline auto operator*(const basic_quat<T1> &lhs, const basic_vec<T2, 3> &rhs) {
		using common_t = std::common_type_t<T1, T2>;
		basic_quat<common_t> q = lhs;
		basic_quat<common_t> p(rhs, 0);
		return basic_vec<common_t, 3>(q * p * inverse(q));
	}

	template<typename T1, typename T2>
	inline auto operator*(const basic_quat<T1> &lhs, const T2 &rhs) {
		return basic_quat<std::common_type_t<T1, T2>>(zip_with(detail::op::mul(), lhs.as_vec(), detail::repeat_vec<T2, 4>(rhs)));
	}

	template<typename T1, typename T2>
	inline auto operator*(const T1 &lhs, const basic_quat<T2> &rhs) {
		return basic_quat<std::common_type_t<T1, T2>>(zip_with(detail::op::mul(), detail::repeat_vec<T1, 4>(lhs), rhs.as_vec()));
	}

	// division

	template<typename T1, typename T2>
	inline auto operator/(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return basic_quat<std::common_type_t<T1, T2>>(zip_with(detail::op::div(), lhs.as_vec(), rhs.as_vec()));
	}

	template<typename T1, typename T2>
	inline auto operator/(const basic_quat<T1> &lhs, const T2 &rhs) {
		return  basic_quat<std::common_type_t<T1, T2>>(zip_with(detail::op::div(), lhs.as_vec(), detail::repeat_vec<T2, 4>(rhs)));
	}

	// equal

	template<typename T1, typename T2>
	inline auto operator==(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return fold(detail::op::logical_and(), true, zip_with(detail::op::equal(), lhs.as_vec(), rhs.as_vec()));
	}

	// not equal

	template<typename T1, typename T2>
	inline auto operator!=(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return fold(detail::op::logical_or(), false, zip_with(detail::op::nequal(), lhs.as_vec(), rhs.as_vec()));
	}

	// less than

	template<typename T1, typename T2>
	inline auto operator<(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return lhs.as_vec() < rhs.as_vec();
	}




	// Iterator functions
	// 

	template<typename T, size_t N>
	inline T * begin(basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	inline const T * begin(const basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	inline const T * cbegin(const basic_vec<T, N> &v) { return v.data(); }

	template<typename T, size_t N>
	inline T * end(basic_vec<T, N> &v) { return begin(v) + N; }

	template<typename T, size_t N>
	inline const T * end(const basic_vec<T, N> &v) { return begin(v) + N; }

	template<typename T, size_t N>
	inline const T * cend(const basic_vec<T, N> &v) { return begin(v) + N; }

	
	template<typename T, size_t Cols, size_t Rows>
	inline T * begin(basic_mat<T, Cols, Rows> &m) { return m.data(); }
	
	template<typename T, size_t Cols, size_t Rows>
	inline const T * begin(const basic_mat<T, Cols, Rows> &m) { return m.data(); }

	template<typename T, size_t Cols, size_t Rows>
	inline const T * cbegin(const basic_mat<T, Cols, Rows> &m) { return m.data(); }
	
	template<typename T, size_t Cols, size_t Rows>
	inline T * end(basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }
	
	template<typename T, size_t Cols, size_t Rows>
	inline const T * end(const basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }

	template<typename T, size_t Cols, size_t Rows>
	inline const T * cend(const basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }


	template<typename T>
	inline T * begin(basic_quat<T> &q) { return q.data(); }

	template<typename T>
	inline const T * begin(const basic_quat<T> &q) { return q.data(); }

	template<typename T>
	inline const T * cbegin(const basic_quat<T> &q) { return q.data(); }

	template<typename T>
	inline T * end(basic_quat<T> &q) { return begin(q) + 4; }

	template<typename T>
	inline const T * end(const basic_quat<T> &q) { return begin(q) + 4; }

	template<typename T>
	inline const T * cend(const basic_quat<T> &q) { return begin(q) + 4; }



	// Swap overloads
	//

	template <typename T, size_t N>
	void swap(basic_vec<T, N> &lhs, basic_vec<T, N> &rhs) {
		for (size_t i = 0; i < N; i++) {
			using std::swap;
			swap(lhs[i], rhs[i]);
		}
	}


	template <typename T, size_t Cols, size_t Rows>
	void swap(basic_mat<T, Cols, Rows> &lhs, basic_mat<T, Cols, Rows> &rhs) {
		for (size_t i = 0; i < Cols; i++) {
			using std::swap;
			swap(lhs[i], rhs[i]);
		}
	}

	template <typename T>
	void swap(basic_quat<T> &lhs, basic_quat<T> &rhs) {
		for (size_t i = 0; i < lhs.size; i++) {
			using std::swap;
			swap(lhs[i], rhs[i]);
		}
	}





	//       ___      .__   __.   _______  __       _______     ___.___________..______       __    _______     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//      /   \     |  \ |  |  /  _____||  |     |   ____|   /  /|           ||   _  \     |  |  /  _____|   |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//     /  ^  \    |   \|  | |  |  __  |  |     |  |__     /  / `---|  |----`|  |_)  |    |  | |  |  __     |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//    /  /_\  \   |  . `  | |  | |_ | |  |     |   __|   /  /      |  |     |      /     |  | |  | |_ |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//   /  _____  \  |  |\   | |  |__| | |  `----.|  |____ /  /       |  |     |  |\  \----.|  | |  |__| |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  /__/     \__\ |__| \__|  \______| |_______||_______/__/        |__|     | _| `._____||__|  \______|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                      //
	//======================================================================================================================================================================================================//

	// Converts degrees to radians, i.e., degrees * pi/180
	template <typename T> inline T radians(T x) {
		return x * T(pi / 180.0);
	}

	// Converts radians to degrees, i.e., radians * 180/pi
	template <typename T> inline T degrees(T x) {
		return x * T(180.0 / pi);
	}

	// Returns the angle between 2 vectors in radians
	template <typename T1, typename T2, size_t N>
	inline auto angle(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return acos(dot(v1, v2) / (length(v1) * length(v2)));
	}

	using std::sin;

	// Element-wise function for x in v
	// The standard trigonometric sine function
	template <typename T, size_t N>
	inline basic_vec<T, N> sin(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return sin(decltype(x)(x)); }, v);
	}

	using std::cos;

	// Element-wise function for x in v
	// The standard trigonometric cosine function
	template <typename T, size_t N>
	inline basic_vec<T, N> cos(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return cos(decltype(x)(x)); }, v);
	}

	using std::tan;

	// Element-wise function for x in v
	// The standard trigonometric tangent
	template <typename T, size_t N>
	inline basic_vec<T, N> tan(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return tan(decltype(x)(x)); }, v);
	}

	using std::asin;

	// Element-wise function for x in v
	// Arc sine. Returns an angle whose sine is x
	// The range of values returned by this function is [-pi/2, pi/2]
	// Results are undefined if ∣x∣>1
	template <typename T, size_t N>
	inline basic_vec<T, N> asin(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return asin(decltype(x)(x)); }, v);
	}

	using std::acos;

	// Element-wise function for x in v
	// Arc cosine. Returns an angle whose cosine is x
	// The range of values returned by this function is [0, p]
	// Results are undefined if ∣x∣>1
	template <typename T, size_t N>
	inline basic_vec<T, N> acos(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return acos(decltype(x)(x)); }, v);
	}

	// Arc tangent. Returns an angle whose tangent is y/x
	// The signs of x and y are used to determine what quadrant the angle is in
	// The range of values returned by this function is [−pi, pi]
	// Results are undefined if x and y are both 0
	template <typename T>
	inline auto atan(const T &y, const T &x) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return std::atan2(y, x);
	}

	// Element-wise function for y in v1 and x in v2
	// Arc tangent. Returns an angle whose tangent is y/x
	// The signs of x and y are used to determine what quadrant the angle is in
	// The range of values returned by this function is [−pi, pi]
	// Results are undefined if x and y are both 0
	template <typename T1, typename T2, size_t N>
	inline auto atan(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return atan2(decltype(x)(x), decltype(y)(y)); }, v2, v1);
	}

	using std::atan;

	// Element-wise function for x in v
	// Arc tangent. Returns an angle whose tangent is y_over_x.
	// The range of values returned by this function is [-pi/2, pi/2] 
	template <typename T, size_t N>
	inline basic_vec<T, N> atan(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return atan(decltype(x)(x)); }, v);
	}

	using std::sinh;

	// Element-wise function for x in v
	// Returns the hyperbolic sine function (e^x - e^-x)/2
	template <typename T, size_t N>
	inline basic_vec<T, N> sinh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return sinh(decltype(x)(x)); }, v);
	}

	using std::cosh;

	// Element-wise function for x in v
	// Returns the hyperbolic cosine function (e^x + e^-x)/2
	template <typename T, size_t N>
	inline basic_vec<T, N> cosh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return cosh(decltype(x)(x)); }, v);
	}

	using std::tanh;

	// Element-wise function for x in v
	// Returns the hyperbolic tangent function sinh(x)/cosh(x)
	template <typename T, size_t N>
	inline basic_vec<T, N> tanh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return tanh(decltype(x)(x)); }, v);
	}

	using std::asinh;

	// Element-wise function for x in v
	// Arc hyperbolic sine; returns the inverse of sinh(x)
	template <typename T, size_t N>
	inline basic_vec<T, N> asinh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return asinh(decltype(x)(x)); }, v);
	}

	using std::acosh;

	// Element-wise function for x in v
	// Arc hyperbolic cosine; returns the non-negative inverse of cosh(x)
	// Results are undefined if x < 1
	template <typename T, size_t N>
	inline basic_vec<T, N> acosh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return acosh(decltype(x)(x)); }, v);
	}

	using std::atanh;

	// Element-wise function for x in v
	// Arc hyperbolic tangent; returns the inverse of tanh(x)
	// Results are undefined if ∣x∣>=1
	template <typename T, size_t N>
	inline basic_vec<T, N> atanh(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return atanh(decltype(x)(x)); }, v);
	}

	//TODO
	// description
	// csc for both scalar x or elements in vector x
	template <typename T> inline T csc(const T &x) {
		return T(1) / sin(x);
	}

	//TODO
	// description
	// sec for both scalar x or elements in vector x
	template <typename T> inline T sec(const T &x) {
		return T(1) / cos(x);
	}

	//TODO
	// description
	// cot for both scalar x or elements in vector x
	template <typename T> inline T cot(const T &x) {
		return T(1) / tan(x);
	}

	//TODO
	// description
	// acsc for both scalar x or elements in vector x
	template <typename T> inline T acsc(const T &x) {
		return asin(T(1) / x);
	}

	//TODO
	// description
	// asec for both scalar x or elements in vector x
	template <typename T> inline T asec(const T &x) {
		return acos(T(1) / x);
	}

	//TODO
	// description
	// acot for both scalar x or elements in vector x
	template <typename T> inline T acot(const T &x) {
		return atan(T(1) / x);
	}

	//TODO
	// description
	// csch for both scalar x or elements in vector x
	template <typename T> inline T csch(const T &x) {
		return T(1) / sinh(x);
	}
	
	//TODO
	// description
	// sech for both scalar x or elements in vector x
	template <typename T> inline T sech(const T &x) {
		return T(1) / cosh(x);
	}
	
	//TODO
	// description
	// coth for both scalar x or elements in vector x
	template <typename T> inline T coth(const T &x) {
		return cosh(x) / sinh(x);
	}

	//TODO
	// description
	// acsch for both scalar x or elements in vector x
	template <typename T> inline T acsch(const T &x) {
		return log(T(1) / x + sqrt((x*x) + T(1)) / abs(x));
	}
	
	//TODO
	// description
	// asech for both scalar x or elements in vector x
	template <typename T> inline T asech(const T &x) {
		return log(T(1) / x + sqrt(T(1) - (x*x)) / x);
	}
	
	//TODO
	// description
	// acoth for both scalar x or elements in vector x
	template <typename T> inline T acoth(const T &x) {
		return T(0.5) * log((x + T(1)) / (x - T(1)));
	}



	//   __________   ___ .______     ______   .__   __.  _______ .__   __. .___________. __       ___       __          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   ____\  \ /  / |   _  \   /  __  \  |  \ |  | |   ____||  \ |  | |           ||  |     /   \     |  |        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |__   \  V  /  |  |_)  | |  |  |  | |   \|  | |  |__   |   \|  | `---|  |----`|  |    /  ^  \    |  |        |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |   __|   >   <   |   ___/  |  |  |  | |  . `  | |   __|  |  . `  |     |  |     |  |   /  /_\  \   |  |        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |____ /  .  \  |  |      |  `--'  | |  |\   | |  |____ |  |\   |     |  |     |  |  /  _____  \  |  `----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |_______/__/ \__\ | _|       \______/  |__| \__| |_______||__| \__|     |__|     |__| /__/     \__\ |_______|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                               //
	//===============================================================================================================================================================================================================//

	using std::pow;

	// Element-wise function for x in v1 and y in v2
	// Returns x raised to the y power, i.e., x^y
	// Results are undefined if x < 0
	// Results are undefined if x = 0 and y <= 0
	template <typename T1, typename T2, size_t N>
	inline auto pow(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return pow(decltype(x)(x), decltype(y)(y)); }, v1, v2);
	}

	using std::exp;

	// Element-wise function for x in v
	// Returns the natural exponentiation of x, i.e., e^x
	template <typename T, size_t N>
	inline basic_vec<T, N> exp(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return exp(decltype(x)(x)); }, v);
	}

	using std::log;

	// Element-wise function for x in v
	// Returns the natural logarithm of x, i.e., the value y which satisfies the equation x = e^y
	// Results are undefined if x <= 0.
	template <typename T, size_t N>
	inline basic_vec<T, N> log(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return log(decltype(x)(x)); }, v);
	}

	// exp2 for both scalar x or elements in vector x
	// Returns 2 raised to the x power, i.e., 2^x
	template <typename T>
	inline T exp2(const T &x) {
		return zip_with([](auto &&x) { return pow(decltype(x)(2), decltype(x)(x)); }, x);
	}

	// log2 for both scalar x or elements in vector x
	// Returns the base 2 logarithm of x, i.e., returns the value
	// y which satisfies the equation x=2^y
	// Results are undefined if x <= 0
	template <typename T>
	inline T log2(const T &x) {
		return log(x) * T(1.4426950408889634073599246810019);
	}

	using std::sqrt;

	// Element-wise function for x in v
	// Returns sqrt(x)
	// Results are undefined if x < 0
	template <typename T, size_t N>
	inline basic_vec<T, N> sqrt(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return sqrt(decltype(x)(x)); }, v);
	}

	// inversesqrt for both scalar x or elements in vector x
	// Returns 1/sqrt(x)
	// Results are undefined if x < 0
	template <typename T>
	inline T inversesqrt(const T &x) {
		return T(1) / sqrt(x);
	}



	//    ______   ______   .___  ___. .___  ___.   ______   .__   __.     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//   /      | /  __  \  |   \/   | |   \/   |  /  __  \  |  \ |  |    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  ,----'|  |  |  | |  \  /  | |  \  /  | |  |  |  | |   \|  |    |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |     |  |  |  | |  |\/|  | |  |\/|  | |  |  |  | |  . `  |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  `----.|  `--'  | |  |  |  | |  |  |  | |  `--'  | |  |\   |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//   \______| \______/  |__|  |__| |__|  |__|  \______/  |__| \__|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                 //
	//=================================================================================================================================================================//

	using std::abs;

	// Element-wise function for x in v
	// Returns x if x >= 0; otherwise it returns –x
	template <typename T, size_t N>
	inline basic_vec<T, N> abs(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return abs(decltype(x)(x)); }, v);
	}

	// Returns 1 if x > 0, 0 if x = 0, or –1 if x < 0
	template <typename T>
	inline auto sign(T x) -> std::enable_if_t<std::is_arithmetic<T>::value, int> {
		return (T(0) < x) - (x < T(0));
	}

	// Element-wise function for x in v
	// Returns 1 if x > 0, 0 if x = 0, or –1 if x < 0
	template <typename T, size_t N>
	inline basic_vec<T, N> sign(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return sign(decltype(x)(x)); }, v);
	}

	using std::floor;

	// Element-wise function for x in v
	// Returns a value equal to the nearest integer that is less than or equal to x
	template <typename T, size_t N>
	inline basic_vec<T, N> floor(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return floor(decltype(x)(x)); }, v);
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
	// same value as round_even(x) for all values of x

	//TODO round_even
	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x
	// A fractional part of 0.5 will round toward the nearest even integer
	// (Both 3.5 and 4.5 for x will return 4.0)

	using std::ceil;

	// Element-wise function for x in v
	// Returns a value equal to the nearest integer to x whose
	// absolute value is not larger than the absolute value of x
	template <typename T, size_t N>
	inline basic_vec<T, N> ceil(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return ceil(decltype(x)(x)); }, v);
	}

	// fract for both scalar x or elements in vector x
	// Returns x – floor (x)
	template <typename T>
	inline T fract(const T &x) {
		return x-floor(x);
	}

	// mod for both scalar x or elements in vector x
	// Modulus. Returns x-m*floor(x/m)
	template <typename T1, typename T2>
	inline auto mod(const T1 &x, const T2 &m) {
		return x-m * floor(x/m);
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

	using std::min;

	// Element-wise function for x in v
	// Returns y if y < x; otherwise it returns x
	template <typename T1, size_t N, typename T2, typename=std::enable_if_t<std::is_arithmetic<T2>::value>>
	inline auto min(const basic_vec<T1, N> &v, T2 y) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&x, auto &&y) { return min<common_t>(decltype(x)(x), decltype(y)(y)); }, v, basic_vec<T2, N>(y));
	}

	// Element-wise function for x in v1 and y in v2
	// x and y are different types
	// Returns y if y < x; otherwise it returns x
	template <typename T1, typename T2, size_t N>
	inline auto min(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&x, auto &&y) { return min<common_t>(decltype(x)(x), decltype(y)(y)); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// x and y are the same type
	// Returns y if y < x; otherwise it returns x
	template <typename T, size_t N>
	inline auto min(const basic_vec<T, N> &v1, const basic_vec<T, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return min(decltype(x)(x), decltype(y)(y)); }, v1, v2);
	}

	using std::max;

	// Element-wise function for x in v
	// Returns y if y > x; otherwise it returns x
	template <typename T1, size_t N, typename T2, typename=std::enable_if_t<std::is_arithmetic<T2>::value>>
	inline auto max(const basic_vec<T1, N> &v, T2 y) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&x, auto &&y) { return max<common_t>(decltype(x)(x), decltype(y)(y)); }, v, basic_vec<T2, N>(y));
	}

	// Element-wise function for x in v1 and y in v2
	// x and y are different types
	// Returns y if y > x; otherwise it returns x
	template <typename T1, typename T2, size_t N>
	inline auto max(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&x, auto &&y) { return max<common_t>(decltype(x)(x), decltype(y)(y)); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// x and y are the same type
	// Returns y if y > x; otherwise it returns x
	template <typename T, size_t N>
	inline auto max(const basic_vec<T, N> &v1, const basic_vec<T, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return max(decltype(x)(x), decltype(y)(y)); }, v1, v2);
	}

	// clamp for both scalar a, minVal, maxVal or elements in vector a, minVal, maxVal
	// Returns min(max(x, minVal), maxVal)
	// Results are undefined if minVal > maxVal
	template <typename T1, typename T2, typename T3>
	inline auto clamp(const T1 &a, const T2 &minVal, const T3 &maxVal) {
		return min(max(a, minVal), maxVal);
	}

	// Returns the linear blend of x and y, i.e., x*(1−a) + y*a
	template <typename T>
	inline auto mix(const T &x, const T &y, const T &a) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return (T(1)-a) * x + a * y;
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
	//     vecT mix(vecT x, vecT y, vecT(a))
	// where a is a Boolean vector.
	template <typename T1, typename T2, size_t N>
	inline auto mix(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2, const basic_vec<bool, N> &va) {
		return zip_with([](auto &&x, auto &&y, auto &&a) { return (!decltype(a)(a)) ? decltype(x)(x) : decltype(y)(y); }, v1, v2, va);
	}

	// Returns 0.0 if x < edge; otherwise it returns 1.0
	template <typename T>
	inline auto step(const T &edge, const T &x) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		return (x<edge) ? T(0) : T(1);
	}

	// Element-wise function for x in v
	// Returns 0.0 if x < edge; otherwise it returns 1.0
	template <typename T1, typename T2, size_t N>
	inline auto step(T1 edge, const basic_vec<T2, N> &v) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&edge, auto &&x) { return (decltype(x)(x) < decltype(edge)(edge)) ? common_t(0) : common_t(1); }, basic_vec<common_t, N>(edge), v);
	}

	// Element-wise function for edge in vedge and x in v
	// Returns 0.0 if x < edge; otherwise it returns 1.0
	template <typename T1, typename T2, size_t N>
	inline auto step(const basic_vec<T1, N> &vedge, const basic_vec<T2, N> &v) {
		using common_t = std::common_type_t<T1, T2>;
		return zip_with([](auto &&edge, auto &&x) { return (decltype(x)(x) < decltype(edge)(edge)) ? common_t(0) : common_t(1); }, vedge, v);
	}

	// Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth
	// Hermite interpolation between 0 and 1 when edge0 < x < edge1
	// This is useful in cases where you would want a threshold
	// function with a smooth transition and is equivalent to:
	//     T t;
	//     t = clamp ((x – edge0) / (edge1 – edge0), 0, 1);
	//     return t * t * (3 – 2 * t);
	// Results are undefined if edge0 >= edge1.
	template <typename T>
	inline auto smoothstep(const T &edge0, const T &edge1, const T &x) -> std::enable_if_t<std::is_arithmetic<T>::value, T> {
		auto t = clamp((x-edge0)/(edge1-edge0), T(0), T(1));
		return t * t * (T(3)-T(2)*t);
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

	using std::isnan;

	// Element-wise function for x in v
	// Returns true if x holds a NaN. Returns false otherwise
	// Always returns false if NaNs are not implemented
	template <typename T, size_t N>
	inline auto isnan(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return isnan(decltype(x)(x)); }, v);
	}

	using std::isinf;

	// Element-wise function for x in v
	// Returns true if x holds a positive infinity or negative infinity
	// Returns false otherwise
	template <typename T, size_t N>
	inline auto isinf(const basic_vec<T, N> &v) {
		return zip_with([](auto &&x) { return isinf(decltype(x)(x)); }, v);
	}

	//TODO
	// float_bits_to_int
	// float_bits_to_uint
	// Returns a signed or unsigned integer value representing the encoding of a float
	// The float value's bit-level representation is preserved

	//TODO
	// int_bits_to_float
	// uint_bits_to_float
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
		return fold(detail::op::add(), T(0), v);
	}

	// Returns the product of all x in v, i.e., v[0] * v[1] * ...
	template <typename T, size_t N>
	inline T product(const basic_vec<T, N> &v) {
		return fold(detail::op::mul(), T(1), v);
	}



	//  .______      ___       ______  __  ___  __  .__   __.   _______     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   _  \    /   \     /      ||  |/  / |  | |  \ |  |  /  _____|   |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |_)  |  /  ^  \   |  ,----'|  '  /  |  | |   \|  | |  |  __     |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |   ___/  /  /_\  \  |  |     |    <   |  | |  . `  | |  | |_ |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |     /  _____  \ |  `----.|  .  \  |  | |  |\   | |  |__| |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  | _|    /__/     \__\ \______||__|\__\ |__| |__| \__|  \______|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                  //
	//==================================================================================================================================================================//

	//TODO



	//    _______  _______   ______   .___  ___.  _______ .___________..______       __    ______     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//   /  _____||   ____| /  __  \  |   \/   | |   ____||           ||   _  \     |  |  /      |   |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |  __  |  |__   |  |  |  | |  \  /  | |  |__   `---|  |----`|  |_)  |    |  | |  ,----'   |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  | |_ | |   __|  |  |  |  | |  |\/|  | |   __|      |  |     |      /     |  | |  |        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |__| | |  |____ |  `--'  | |  |  |  | |  |____     |  |     |  |\  \----.|  | |  `----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//   \______| |_______| \______/  |__|  |__| |_______|    |__|     | _| `._____||__|  \______|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                            //
	//============================================================================================================================================================================================//

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

	// Returns the dot product of v1 and v2, i.e., v1[0]*v2[0] + v1[1]*v2[1] + ...
	template <typename T1, typename T2, size_t N>
	inline auto dot(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return fold(detail::op::add(), T1{}, zip_with(detail::op::mul(), v1, v2));
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

	// TODO more complete description
	// project lhs onto rhs
	template <typename T1, typename T2, size_t N>
	inline auto project(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return v2 * (dot(v1, v2) / dot(v2, v2));
	}

	// TODO more complete description
	// project lhs onto the plane defined by the normal rhs
	template <typename T1, typename T2, size_t N>
	inline auto reject(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return v1 - project(v1, v2);
	}

	// TODO description
	// Returns the cross product of 2 vectors
	template <typename T1, typename T2>
	inline auto cross(const basic_vec<T1, 3> &v1, const basic_vec<T2, 3> &v2) {
		return basic_vec<std::common_type_t<T1, T2>, 3>(
			v1[1] * v2[2] - v1[2] * v2[1],
			v1[2] * v2[0] - v1[0] * v2[2],
			v1[0] * v2[1] - v1[1] * v2[0]
		);
	}



	//  .______       _______  __          ___   .___________. __    ______   .__   __.      ___       __          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   _  \     |   ____||  |        /   \  |           ||  |  /  __  \  |  \ |  |     /   \     |  |        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |_)  |    |  |__   |  |       /  ^  \ `---|  |----`|  | |  |  |  | |   \|  |    /  ^  \    |  |        |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |      /     |   __|  |  |      /  /_\  \    |  |     |  | |  |  |  | |  . `  |   /  /_\  \   |  |        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |\  \----.|  |____ |  `----./  _____  \   |  |     |  | |  `--'  | |  |\   |  /  _____  \  |  `----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  | _| `._____||_______||_______/__/     \__\  |__|     |__|  \______/  |__| \__| /__/     \__\ |_______|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                         //
	//=========================================================================================================================================================================================================//

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x < y
	template <typename T1, typename T2, size_t N>
	inline auto less_than(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) < decltype(y)(y); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x <= y
	template <typename T1, typename T2, size_t N>
	inline auto less_than_equal(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) <= decltype(y)(y); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x > y
	template <typename T1, typename T2, size_t N>
	inline auto greater_than(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) > decltype(y)(y); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x >= y
	template <typename T1, typename T2, size_t N>
	inline auto greater_than_equal(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) >= decltype(y)(y); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x == y
	template <typename T1, typename T2, size_t N>
	inline auto equal(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) == decltype(y)(y); }, v1, v2);
	}

	// Element-wise function for x in v1 and y in v2
	// Returns the comparison of x != y
	template <typename T1, typename T2, size_t N>
	inline auto not_equal(const basic_vec<T1, N> &v1, const basic_vec<T2, N> &v2) {
		return zip_with([](auto &&x, auto &&y) { return decltype(x)(x) != decltype(y)(y); }, v1, v2);
	}

	// Returns true if any component of v is true
	template <size_t N>
	inline bool any(const basic_vec<bool, N> &v) {
		return fold([](auto &&x, auto &&y) { return decltype(x)(x) || decltype(y)(y); }, false, v);
	}

	// Returns true only if all components of x are true
	template <size_t N>
	inline bool all(const basic_vec<bool, N> &v) {
		return fold([](auto &&x, auto &&y) { return decltype(x)(x) && decltype(y)(y); }, true, v);
	}

	// Note : C++ does not support "not" as a function name, hence it has been omitted



	
	//   __  .__   __. .___________. _______   _______  _______ .______          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |  | |  \ |  | |           ||   ____| /  _____||   ____||   _  \        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  | |   \|  | `---|  |----`|  |__   |  |  __  |  |__   |  |_)  |       |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  | |  . `  |     |  |     |   __|  |  | |_ | |   __|  |      /        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  | |  |\   |     |  |     |  |____ |  |__| | |  |____ |  |\  \----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |__| |__| \__|     |__|     |_______| \______| |_______|| _| `._____|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                       //
	//=======================================================================================================================================================================//

	//TODO




	//  .___  ___.      ___   .___________..______       __  ___   ___     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   \/   |     /   \  |           ||   _  \     |  | \  \ /  /    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  \  /  |    /  ^  \ `---|  |----`|  |_)  |    |  |  \  V  /     |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |\/|  |   /  /_\  \    |  |     |      /     |  |   >   <      |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |  |  |  /  _____  \   |  |     |  |\  \----.|  |  /  .  \     |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |__|  |__| /__/     \__\  |__|     | _| `._____||__| /__/ \__\    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                 //
	//=================================================================================================================================================================//

	namespace detail {
		template <typename T>
		inline T det2x2(
			T e00, T e01,
			T e10, T e11
		) {
			return e00 * e11 - e10 * e01;
		}

		template <typename T>
		inline T det3x3(
			T e00, T e01, T e02,
			T e10, T e11, T e12,
			T e20, T e21, T e22
		) {
			T d = 0;
			d += e00 * e11 * e22;
			d += e01 * e12 * e20;
			d += e02 * e10 * e21;
			d -= e00 * e12 * e21;
			d -= e01 * e10 * e22;
			d -= e02 * e11 * e20;
			return d;
		}
	}

	// returns a matrix with all values set to the given argument
	// TODO description
	template <typename MatT>
	inline MatT fill(MatT::val_t v) {
		MatT m;
		for (size_t j = 0; j < MatT::cols; ++j)
			for (size_t i = 0; i < MatT::row; ++i)
				m[j][i] = v;
		return m;
	}

	// inverse of matrix (error if not invertible)
	// generic case
	// TODO description
	template <typename T, size_t Cols, size_t Rows>
	inline basic_mat<T, Cols, Rows> inverse(const basic_mat<T, Cols, Rows> &m) {
		static_assert(Cols == Rows, "only square matrices are invertible");
		using std::abs;
		basic_mat<T, Cols, Rows> mtemp{ m };
		basic_mat<T, Cols, Rows> mr{ 1 }; // @josh this needs an identity ctor
										  // run column-wise gauss-jordan elimination on mtemp, apply same ops to mr
		size_t col = 0;
		for (size_t r = 0; r < Rows; r++) {
			// swap cols so (r, col) is as large in magnitude as possible
			size_t swcol = col;
			T swmax = abs(mtemp[col][r]);
			for (size_t j = col + 1; j < Cols; j++) {
				T t = abs(mtemp[j][r]);
				if (t > swmax) {
					swmax = t;
					swcol = j;
				}
			}
			if (col != swcol) {
				// found something to swap with
				swap(mr[col], mr[swcol]);
				swap(mtemp[col], mtemp[swcol]);
			}
			if (swmax > 0) {
				// largest usable abs value was > 0, continue => zero rest of row
				T q = T(1) / mtemp[col][r];
				for (size_t j = 0; j < Cols; j++) {
					if (j != col && abs(mtemp[j][r]) > 0) {
						T f = mtemp[j][r] * q;
						mr[j] -= mr[col] * f;
						mtemp[j] -= mtemp[col] * f;
					}
				}
				// get leading 1 in this col
				mr[col] *= q;
				mtemp[col] *= q;
				// pivot isolated, move to next col
				col++;
			}
		}
		// after above routine, col == rank
		assert(Cols == col && "singular matrix");
		// mr is now the inverse
		return mr;
	}

	// inverse of matrix (error if not invertible)
	template <typename T>
	inline basic_mat<T, 1, 1> inverse(const basic_mat<T, 1, 1> &m) {
		// TODO (error if not invertible)
		return 1 / m;
	}

	// inverse of matrix (error if not invertible)
	template <typename T>
	inline basic_mat<T, 2, 2> inverse(const basic_mat<T, 2, 2> &m) {
		basic_mat<T, 2, 2> r;
		T invdet = 1 / determinant(m);
		// FIXME proper detect infinite determinant
		assert(!isinf(invdet) && invdet == invdet && invdet != 0);
		r[0][0] =  m[1][1] * invdet;
		r[0][1] = -m[0][1] * invdet;
		r[1][0] = -m[1][0] * invdet;
		r[1][1] =  m[0][0] * invdet;
		return r;
	}

	// inverse of matrix (error if not invertible)
	template <typename T>
	inline basic_mat<T, 3, 3> inverse(const basic_mat<T, 3, 3> &m) {
		basic_mat<T, 3, 3> r;
		// first column of cofactors, can use for determinant
		T c00 =  detail::det2x2(m[1][1], m[1][2], m[2][1], m[2][2]);
		T c01 = -detail::det2x2(m[1][0], m[1][2], m[2][0], m[2][2]);
		T c02 =  detail::det2x2(m[1][0], m[1][1], m[2][0], m[2][1]);
		// get determinant by expanding about first column
		T invdet = 1 / (m[0][0] * c00 + m[0][1] * c01 + m[0][2] * c02);
		// FIXME proper detect infinite determinant
		assert(!isinf(invdet) && invdet == invdet && invdet != 0);
		// transpose of cofactor matrix * (1 / det)
		r[0][0] = c00 * invdet;
		r[1][0] = c01 * invdet;
		r[2][0] = c02 * invdet;
		r[0][1] = -detail::det2x2(m[0][1], m[0][2], m[2][1], m[2][2]) * invdet;
		r[1][1] =  detail::det2x2(m[0][0], m[0][2], m[2][0], m[2][2]) * invdet;
		r[2][1] = -detail::det2x2(m[0][0], m[0][1], m[2][0], m[2][1]) * invdet;
		r[0][2] =  detail::det2x2(m[0][1], m[0][2], m[1][1], m[1][2]) * invdet;
		r[1][2] = -detail::det2x2(m[0][0], m[0][2], m[1][0], m[1][2]) * invdet;
		r[2][2] =  detail::det2x2(m[0][0], m[0][1], m[1][0], m[1][1]) * invdet;
		return r;
	}


	// inverse of matrix (error if not invertible)
	template <typename T>
	inline basic_mat<T, 4, 4> inverse(const basic_mat<T, 4, 4> &m) {
		basic_mat<T, 4, 4> r;
		// first column of cofactors, can use for determinant
		T c0 =  detail::det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
		T c1 = -detail::det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
		T c2 =  detail::det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
		T c3 = -detail::det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
		// get determinant by expanding about first column
		T invdet = 1 / (m[0][0] * c0 + m[0][1] * c1 + m[0][2] * c2 + m[0][3] * c3);
		// FIXME proper detect infinite determinant
		assert(!isinf(invdet) && invdet == invdet && invdet != 0);
		// transpose of cofactor matrix * (1 / det)
		r[0][0] = c0 * invdet;
		r[1][0] = c1 * invdet;
		r[2][0] = c2 * invdet;
		r[3][0] = c3 * invdet;
		r[0][1] = -detail::det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]) * invdet;
		r[1][1] =  detail::det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]) * invdet;
		r[2][1] = -detail::det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]) * invdet;
		r[3][1] =  detail::det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]) * invdet;
		r[0][2] =  detail::det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]) * invdet;
		r[1][2] = -detail::det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]) * invdet;
		r[2][2] =  detail::det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]) * invdet;
		r[3][2] = -detail::det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]) * invdet;
		r[0][3] = -detail::det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]) * invdet;
		r[1][3] =  detail::det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]) * invdet;
		r[2][3] = -detail::det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]) * invdet;
		r[3][3] =  detail::det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]) * invdet;
		return r;
	}


	// determinant of matrix
	template <typename T>
	inline T determinant(const basic_mat<T, 2, 2> &m) {
		return m[0][0] * m[1][1] - m[1][0] * m[0][1];
	}

	// determinant of matrix
	template <typename T>
	inline T determinant(const basic_mat<T, 3, 3> &m) {
		T d = 0;
		d += m[0][0] * m[1][1] * m[2][2];
		d += m[0][1] * m[1][2] * m[2][0];
		d += m[0][2] * m[1][0] * m[2][1];
		d -= m[0][0] * m[1][2] * m[2][1];
		d -= m[0][1] * m[1][0] * m[2][2];
		d -= m[0][2] * m[1][1] * m[2][0];
		return d;
	}

	//TODO
	// determinant of matrix
	template <typename T>
	inline T determinant(const basic_mat<T, 4, 4> &m) {
		T d = 0;
		// expand about first column
		d += m[0][0] * detail::det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
		d -= m[0][1] * detail::det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
		d += m[0][2] * detail::det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
		d -= m[0][3] * detail::det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
		return d;
	}


	// transpose of matrix
	// TODO
	template <typename T, size_t Cols, size_t Rows>
	inline basic_mat<T, Rows, Cols> transpose(const basic_mat<T, Rows, Cols> &m) {
		basic_mat<T, Rows, Cols> r;
		for (size_t j = 0; j < Cols; ++j)
			for (size_t i = 0; i < Rows; ++i)
				r[i][j] = m[j][i];
		return r;
	}

	// component-wise multiplication 
	// see (*) operator overload for matrix product
	// TODO
	template <typename T1, typename T2, size_t Rows, size_t Cols>
	inline auto matrixCompMult(const basic_mat<T1, Rows, Cols> &lhs, const basic_mat<T2, Rows, Cols> &rhs) {
		return zip_with(detail::op::mul(), lhs.as_vec(), rhs.as_vec());
	}

	// outerProduct
	// TODO
	template <typename T1, typename T2, size_t N>
	inline auto outerProduct(const basic_vec<T1, N> &lhs, const basic_vec<T2, N> &rhs) {
		basic_mat<std::common_type_t<T1, T2>, N, N> r;
		for (size_t j = 0; j < N; ++j)
			for (size_t i = 0; i < N; ++i)
				r[j][i] = lhs[j] * rhs[i];
		return r;
	}





	//  .___________..______          ___      .__   __.      _______. _______   ______   .______      .___  ___.     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |           ||   _  \        /   \     |  \ |  |     /       ||   ____| /  __  \  |   _  \     |   \/   |    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  `---|  |----`|  |_)  |      /  ^  \    |   \|  |    |   (----`|  |__   |  |  |  | |  |_)  |    |  \  /  |    |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//      |  |     |      /      /  /_\  \   |  . `  |     \   \    |   __|  |  |  |  | |      /     |  |\/|  |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//      |  |     |  |\  \----./  _____  \  |  |\   | .----)   |   |  |     |  `--'  | |  |\  \----.|  |  |  |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//      |__|     | _| `._____/__/     \__\ |__| \__| |_______/    |__|      \______/  | _| `._____||__|  |__|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                            //
	//============================================================================================================================================================================================================//


	// general transformations
	//

	template<typename MatT>
	inline auto identity() {
		return MatT{1};
	}

	template <typename MatT>
	inline auto shear(int t_dim, int s_dim, typename MatT::value_t f) {
		MatT m{ 1 };
		m[t_dim][s_dim] = f;
		return m;
	}


	// Functions for constructing 2d transformations
	//

	template <typename MatT>
	inline auto rotate2(typename MatT::value_t v) {
		static_assert(MatT::cols >= 2, "Matrix type must have 2 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		typename MatT::value_t cos_v = cos(v);
		typename MatT::value_t sin_v = sin(v);
		r[0][0] = cos_v;
		r[1][0] = -sin_v;
		r[1][1] = cos_v;
		r[0][1] = sin_v;
		return r;
	}

	template <typename MatT>
	inline auto scale2(typename MatT::value_t v) {
		static_assert(MatT::cols >= 2, "Matrix type must have 2 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		r[0][0] = v;
		r[1][1] = v;
		return r;
	}


	template <typename MatT>
	inline auto scale2(const basic_vec<typename MatT::value_t, 2> &v) {
		static_assert(MatT::cols >= 2, "Matrix type must have 2 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		r[0][0] = v[0];
		r[1][1] = v[1];
		return r;
	}

	template <typename MatT>
	inline auto translate2(typename MatT::value_t v) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		r[3][0] = v;
		r[3][1] = v;
		return r;
	}

	template <typename MatT>
	inline auto translate2(typename MatT::value_t vx, typename MatT::value_t vy) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		r[3][0] = vx;
		r[3][1] = vy;
		return r;
	}

	template <typename MatT>
	inline auto translate2(const basic_vec<typename MatT::value_t, 2> &v) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 2, "Matrix type must have 2 or more rows");
		MatT r{ 1 };
		r[3][0] = v[0];
		r[3][1] = v[1];
		return r;
	}





	// Functions for constructing 3d transformations
	//

	// TODO Nan check
	template <typename MatT>
	inline auto look_at(
		const basic_vec<typename MatT::value_t, 3> &eye,
		const basic_vec<typename MatT::value_t, 3> &lookAt,
		const basic_vec<typename MatT::value_t, 3> &up
	) {
		static_assert(MatT::cols == 4, "Matrix type must have exactly 4 columns");
		static_assert(MatT::rows == 4, "Matrix type must have exactly 4 rows");
		using vec3_t = basic_vec<typename MatT::value_t, 3>;
		using vec4_t = basic_vec<typename MatT::value_t, 4>;
		vec3_t vz = normalize(eye - lookAt);
		vec3_t vx = normalize(cross(up, vz));
		vec3_t vy = normalize(cross(vz, vx));
		MatT m{
			vec4_t(vx, 0),
			vec4_t(vy, 0),
			vec4_t(vz, 0),
			vec4_t(eye, 1) };
		return inverse(m);
	}

	template <typename MatT>
	inline auto look_at(
		typename MatT::value_t ex, typename MatT::value_t ey, typename MatT::value_t ez,
		typename MatT::value_t lx, typename MatT::value_t ly, typename MatT::value_t lz,
		typename MatT::value_t ux, typename MatT::value_t uy, typename MatT::value_t uz
	) {
		static_assert(MatT::cols == 4, "Matrix type must have exactly 4 columns");
		static_assert(MatT::rows == 4, "Matrix type must have exactly 4 rows");
		return look_at( { ex, ey, ez }, { lx, ly, lz }, { ux, uy, uz } );
	}

	// fovy in radians veritical feild of view as an angle
	// aspect is w/h
	// TODO Nan check
	template <typename MatT>
	inline auto perspective(
		typename MatT::value_t fovy,
		typename MatT::value_t aspect,
		typename MatT::value_t zNear,
		typename MatT::value_t zFar
	) {
		static_assert(MatT::cols == 4, "Matrix type must have exactly 4 columns");
		static_assert(MatT::rows == 4, "Matrix type must have exactly 4 rows");
		// typename MatT::value_t f = typename MatT::value_t(1) / (fovy / typename MatT::value_t(2)); // lol wtf, fast approximation
		typename MatT::value_t f = cot(fovy / typename MatT::value_t(2)); // real equation
		MatT r{ 0 };
		r[0][0] = f / aspect;
		r[1][1] = f;
		r[2][2] = (zFar + zNear) / (zNear - zFar);
		r[3][2] = (2 * zFar * zNear) / (zNear - zFar);
		r[2][3] = -1;
		return r;
	}

	// TODO Nan check
	template <typename MatT>
	inline auto orthographic(
		typename MatT::value_t left, 
		typename MatT::value_t right, 
		typename MatT::value_t bottom, 
		typename MatT::value_t top, 
		typename MatT::value_t nearVal, 
		typename MatT::value_t farVal
	) {
		static_assert(MatT::cols == 4, "Matrix type must have exactly 4 columns");
		static_assert(MatT::rows == 4, "Matrix type must have exactly 4 rows");
		MatT r{ 0 };
		r[0][0] = typename MatT::value_t(2) / (right - left);
		r[3][0] = (right + left) / (right - left);
		r[1][1] = typename MatT::value_t(2) / (top - bottom);
		r[3][1] = (top + bottom) / (top - bottom);
		r[2][2] = -typename MatT::value_t(2) / (farVal - nearVal);
		r[3][2] = (farVal + nearVal) / (farVal - nearVal);
		r[3][3] = typename MatT::value_t(1);
		return r;
	}

	template <typename MatT>
	inline auto rotate3x(typename MatT::value_t angle) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[1][1] = cos(angle);
		r[2][1] = -sin(angle);
		r[1][2] = sin(angle);
		r[2][2] = cos(angle);
		return r;
	}

	template <typename MatT>
	inline auto rotate3y(typename MatT::value_t angle) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[0][0] = cos(angle);
		r[2][0] = sin(angle);
		r[0][2] = -sin(angle);
		r[2][2] = cos(angle);
		return r;
	}

	template <typename MatT>
	inline auto rotate3z(typename MatT::value_t angle) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[0][0] = cos(angle);
		r[1][0] = -sin(angle);
		r[0][1] = sin(angle);
		r[1][1] = cos(angle);
		return r;
	}

	template <typename MatT>
	inline auto rotate3(const basic_quat<typename MatT::value_t> &q) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		basic_mat<typename MatT::value_t, 4, 4> rotation(q);
		MatT r{ 1 };

		// copy rotation part of the quaternion matrix over
		for (int j = 0; j < MatT::cols && j < rotation.cols; ++j)
			for (int i = 0; i < MatT::rows && i < rotation.rows; ++i)
				r[j][i] = rotation[j][i];

		return r;
	}


	template <typename MatT>
	inline auto scale3(typename MatT::value_t v) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[0][0] = v;
		r[1][1] = v;
		r[2][2] = v;
		return r;
	}

	template <typename MatT>
	inline auto scale3(typename MatT::value_t vx, typename MatT::value_t vy, typename MatT::value_t vz) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[0][0] = vx;
		r[1][1] = vy;
		r[2][2] = vz;
		return r;
	}


	template <typename MatT>
	inline auto scale3(const basic_vec<typename MatT::value_t, 3> &v) {
		static_assert(MatT::cols >= 3, "Matrix type must have 3 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[0][0] = v[0];
		r[1][1] = v[1];
		r[2][2] = v[2];
		return r;
	}

	template <typename MatT>
	inline auto translate3(typename MatT::value_t v) {
		static_assert(MatT::cols >= 4, "Matrix type must have 4 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[3][0] = v;
		r[3][1] = v;
		r[3][2] = v;
		return r;
	}

	template <typename MatT>
	inline auto translate3(typename MatT::value_t vx, typename MatT::value_t vy, typename MatT::value_t vz) {
		static_assert(MatT::cols >= 4, "Matrix type must have 4 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[3][0] = vx;
		r[3][1] = vy;
		r[3][2] = vz;
		return r;
	}


	template <typename MatT>
	inline auto translate3(const basic_vec<typename MatT::value_t, 3> &v) {
		static_assert(MatT::cols >= 4, "Matrix type must have 4 or more columns");
		static_assert(MatT::rows >= 3, "Matrix type must have 3 or more rows");
		MatT r{ 1 };
		r[3][0] = v[0];
		r[3][1] = v[1];
		r[3][2] = v[2];
		return r;
	}





	//    ______      __    __       ___   .___________. _______ .______      .__   __.  __    ______   .__   __.     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//   /  __  \    |  |  |  |     /   \  |           ||   ____||   _  \     |  \ |  | |  |  /  __  \  |  \ |  |    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |  |  |   |  |  |  |    /  ^  \ `---|  |----`|  |__   |  |_)  |    |   \|  | |  | |  |  |  | |   \|  |    |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |  |  |   |  |  |  |   /  /_\  \    |  |     |   __|  |      /     |  . `  | |  | |  |  |  | |  . `  |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  `--'  '--.|  `--'  |  /  _____  \   |  |     |  |____ |  |\  \----.|  |\   | |  | |  `--'  | |  |\   |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//   \_____\_____\\______/  /__/     \__\  |__|     |_______|| _| `._____||__| \__| |__|  \______/  |__| \__|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                            //
	//============================================================================================================================================================================================================//

	// Euler angle constuctor
	// body-3-2-1 euler rotation
	// TODO nan checking
	template <typename QuatT>
	inline auto euler(typename QuatT::value_t rx, typename QuatT::value_t ry, typename QuatT::value_t rz) {

		basic_vec<typename QuatT::value_t, 4> rotx(sin(rx / 2), 0, 0, cos(rx / 2));
		basic_vec<typename QuatT::value_t, 4> roty(0, sin(ry / 2), 0, cos(ry / 2));
		basic_vec<typename QuatT::value_t, 4> rotz(0, 0, sin(rz / 2), cos(rz / 2));

		QuatT q(rotx * outerProduct(roty, rotz));

		return q;
	}

	// rotation around a given axis
	// normalizes the axis for you
	// TODO nan checking
	template <typename QuatT, typename U>
	inline auto axisangle(const basic_vec<U, 3> &axis, typename QuatT::value_t angle) {
		basic_vec<typename QuatT::value_t, 3> a = normalize(axis);
		typename QuatT::value_t s = sin(angle / 2);

		return QuatT(s * a, cos(angle / 2));
	}

	// rotation around a given axis using axis's magnitude as angle
	// TODO nan checking
	template <typename QuatT, typename U>
	inline auto axisangle(const basic_vec<U, 3>& axis) {
		typename QuatT::value_t angle = length(axis);
		typename QuatT::value_t s = sin(angle / 2);
		basic_vec<typename QuatT::value_t, 3> a = axis / angle; // normalize

		return QuatT(s * a, cos(angle / 2));
	}

	// rotation between 2 vectors
	// TODO nan checking
	template <typename QuatT, typename U1, typename U2>
	inline auto fromto(const basic_vec<U1, 3> &from, const basic_vec<U2, 3> &to) {
		typename QuatT::value_t angle = acos(dot(normalize(from), normalize(to)));
		typename QuatT::value_t s = sin(angle / 2);
		basic_vec<typename QuatT::value_t, 3> a = normalize(cross(from, to));

		return QuatT(s * a, cos(angle / 2));
	}

	// dot product
	template <typename T1, typename T2>
	inline auto dot(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	// inverse rotation
	template <typename T>
	inline auto conjugate(const basic_quat<T>& q) {
		return basic_quat<T>{ -q.x, -q.y, -q.z, q.w };
	}

	// multiplicative inverse
	template <typename T>
	inline auto inverse(const basic_quat<T>& q) {
		T ilen2 = 1 / dot(q, q);
		return conjugate(q) * ilen2;
	}

	template <typename T>
	inline auto pow(const basic_quat<T> &q, T power) {
		T qm = length(q);
		T theta = acos(q.w / qm);
		T ivm = 1 / sqrt(q.x * q.x + q.y * q.y + q.z * q.z);

		if (isinf(ivm)) return basic_quat<T>() * pow(qm, power);

		T ivm_power_theta = ivm * power * theta;
		basic_quat<T> p(0, q.x * ivm_power_theta, q.y * ivm_power_theta, q.z * ivm_power_theta);
		return exp(p) * pow(qm, power);
	}


	template <typename T>
	inline auto exp(const basic_quat<T> &q) {
		T expw = exp(q.w);
		T vm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
		T ivm = 1 / vm;

		if (isinf(ivm)) return basic_quat<T>(expw * cos(vm), 0, 0, 0);

		T vf = expw * sin(vm) * ivm;
		return basic_quat<T>(expw * cos(vm), q.x * vf, q.y * vf, q.z * vf);
	}

	template <typename T>
	inline auto log(const basic_quat<T> &q) {
		T qm = length(q);
		T ivm = 1 / sqrt(q.x * q.x + q.y * q.y + q.z * q.z);

		if (isinf(ivm)) return basic_quat<T>(log(qm), 0, 0, 0);

		T vf = acos(q.w / qm) * ivm;
		return basic_quat<T>(log(qm), q.x * vf, q.y * vf, q.z * vf);
	}

	// length/magnitude of vector from quaternion
	template <typename T>
	inline T length(const basic_quat<T> &q) {
		return sqrt(dot(q, q));
	}

	// returns unit quaternion
	template <typename T>
	inline auto normalize(const basic_quat<T> &q) {
		return q / length(q);
	}

	// normalized linear interpolation
	// linear blend of quaternions : x*(1-a) + y*a
	template <typename T1, typename T2, typename T3>
	inline auto mix(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs, T3 a) {
		using common_t = std::common_type_t<T1, T2, T3>;
		return lhs*(common_t(1) - a) + rhs*a;
	}

	// spherical linear interpolation
	template <typename T1, typename T2, typename T3>
	inline auto slerp(const basic_quat<T1>& lhs, const basic_quat<T2>& rhs, T3 a) {
		using common_t = std::common_type_t<T1, T2, T3>;
		basic_quat<common_t> q = normalize(lhs);
		basic_quat<common_t> p = normalize(rhs);
		common_t epsilon = 0.0001;
		if (dot(p, q) < common_t(0)) {
			q = q * common_t(-1);
		}
		common_t dpq = dot(p, q);

		if ((common_t(1) - dpq) > epsilon) {
			common_t w = acos(dpq);
			return ((sin((common_t(1) - a) * w) * p) + (sin(a * w) * q)) / sin(w);
		}

		return (common_t(1) - a) * p + a * q;
	}


	// returns the rotation (in radians) of the quaternion around a given axis
	template <typename T1, typename T2>
	inline auto project(const basic_quat<T1> &q, const basic_vec<T2, 3> &v) {
		using common_t = std::common_type_t<T1, T2>;

		basic_vec<common_t, 3> nv = normalize(v);

		// find the the tangent to nv
		basic_vec<common_t, 3> tangent{ 1, 0, 0 };
		if (abs(dot(tangent, nv)) > 0.7331) // anti-leet
			tangent = basic_vec<common_t, 3> { 0, 1, 0 };
		tangent = normalize(cross(nv, tangent));

		// transform, reject and dot values to get angle
		basic_vec<common_t, 3> transformed = q * tangent;
		basic_vec<common_t, 3> projection = normalize(reject(transformed, nv));

		return angle(tangent, projection);
	}




	//  .___  ___.  _______ .___________.    ___          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   \/   | |   ____||           |   /   \        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  \  /  | |  |__   `---|  |----`  /  ^  \       |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |\/|  | |   __|      |  |      /  /_\  \      |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |  |  | |  |____     |  |     /  _____  \     |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |__|  |__| |_______|    |__|    /__/     \__\    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                //
	//================================================================================================================================================//

	template <typename ...VecTs>
	struct min_size { };

	template <typename VecT>
	struct min_size<VecT> : std::integral_constant<size_t, VecT::size> { };

	// A meta struct with an integral_constant value equal to the minimum size of its template arguments
	template <typename VecT1, typename VecT2, typename ...VecTs>
	struct min_size<VecT1, VecT2, VecTs...> :
		std::integral_constant<
			size_t,
			(min_size<VecT1>::value < min_size<VecT2>::value) ?
			min_size<VecT1, VecTs...>::value : min_size<VecT2, VecTs...>::value
		>
	{ };





	//   __    __   __    _______  __    __       ______   .______       _______   _______ .______          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |  |  |  | |  |  /  _____||  |  |  |     /  __  \  |   _  \     |       \ |   ____||   _  \        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |__|  | |  | |  |  __  |  |__|  |    |  |  |  | |  |_)  |    |  .--.  ||  |__   |  |_)  |       |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |   __   | |  | |  | |_ | |   __   |    |  |  |  | |      /     |  |  |  ||   __|  |      /        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |  |  | |  | |  |__| | |  |  |  |    |  `--'  | |  |\  \----.|  '--'  ||  |____ |  |\  \----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |__|  |__| |__|  \______| |__|  |__|     \______/  | _| `._____||_______/ |_______|| _| `._____|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                  //
	//==================================================================================================================================================================================================//

	namespace detail {
		// A helper function used internally by zip_with
		template <size_t I, typename F, typename ...ArgTs>
		CGRA_CONSTEXPR_FUNCTION decltype(auto) zip_with_impl_impl(F f, ArgTs &&...args) {
			return f(std::forward<ArgTs>(args)[I]...);
		}

		// A helper function used internally by zip_with
		template <typename ResT, typename F, size_t ...Is, typename ...ArgTs>
		CGRA_CONSTEXPR_FUNCTION ResT zip_with_impl(F f, std::index_sequence<Is...>, ArgTs &&...args) {
			return ResT(zip_with_impl_impl<Is>(f, std::forward<ArgTs>(args)...)...);
		}

		// A helper function used internally by fold
		template <size_t I, size_t N>
		struct fold_impl {
			template <typename F, typename T1, typename ArgT>
			static CGRA_CONSTEXPR_FUNCTION decltype(auto) apply(F f, T1 &&t1, ArgT &&v) {
				return fold_impl<I + 1, N>::apply(f, f(std::forward<T1>(t1), std::forward<ArgT>(v)[I]), std::forward<ArgT>(v));
			}
		};

		// A helper function used internally by fold
		template <size_t N>
		struct fold_impl<N, N> {
			template <typename F, typename T1, typename ArgT>
			static CGRA_CONSTEXPR_FUNCTION decltype(auto) apply(F, T1 &&t1, ArgT &&) {
				return std::forward<T1>(t1);
			}
		};
	}

	//TODO
	// decsription
	template <typename F, typename ...ArgTs>
	CGRA_CONSTEXPR_FUNCTION auto zip_with(F f, ArgTs &&...args) {
		using value_t = std::decay_t<decltype(f(std::forward<ArgTs>(args)[0]...))>;
		using size = min_size<std::decay_t<ArgTs>...>;
		using vec_t = basic_vec<value_t, size::value>;
		using iseq = std::make_index_sequence<size::value>;
		return detail::zip_with_impl<vec_t>(f, iseq(), std::forward<ArgTs>(args)...);
	}

	// Produce a scalar by applying f(T1,T2) -> T3 to adjacent pairs of elements
	// from vector a in left-to-right order starting with f(z, v[0])
	// Typically T1 = T3 and T2 is a vector of some T
	template <typename F, typename T1, typename ArgT>
	CGRA_CONSTEXPR_FUNCTION decltype(auto) fold(F f, T1 &&t1, ArgT &&v) {
		return detail::fold_impl<0, std::decay_t<ArgT>::size>::apply(f, std::forward<T1>(t1), std::forward<ArgT>(v));
	}

}




//   __    __       ___           _______. __    __   __  .__   __.   _______   //
//  |  |  |  |     /   \         /       ||  |  |  | |  | |  \ |  |  /  _____|  //
//  |  |__|  |    /  ^  \       |   (----`|  |__|  | |  | |   \|  | |  |  __    //
//  |   __   |   /  /_\  \       \   \    |   __   | |  | |  . `  | |  | |_ |   //
//  |  |  |  |  /  _____  \  .----)   |   |  |  |  | |  | |  |\   | |  |__| |   //
//  |__|  |__| /__/     \__\ |_______/    |__|  |__| |__| |__| \__|  \______|   //
//                                                                              //
//==============================================================================//

namespace cgra {
	//TODO
	// decsription
	template <class T>
	inline size_t hash_combine(std::size_t seed, T v) {
		std::hash<T> h;
		return seed ^ (h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}
}

namespace std {
	//TODO
	// decsription
	template<typename T, size_t N>
	struct hash<cgra::basic_vec<T, N>> {
		inline size_t operator()(const cgra::basic_vec<T, N> &v) const {
			return cgra::fold(cgra::hash_combine, 73, v); // 73 Sheldon Cooper's favorite number
		}
	};

	//TODO
	// decsription
	template<typename T, size_t Cols, size_t Rows>
	struct hash<cgra::basic_mat<T, Cols, Rows>> {
		inline size_t operator()(const cgra::basic_mat<T, Cols, Rows> &m) const {
			return cgra::fold(cgra::hash_combine, 73, m.as_vec());
		}
	};

	//TODO
	// decsription
	template<typename T>
	struct hash<cgra::basic_quat<T>> {
		inline size_t operator()(const cgra::basic_quat<T> &q) const {
			return cgra::fold(cgra::hash_combine, 73, q.as_vec());
		}
	};
}