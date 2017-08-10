//----------------------------------------------------------------------------
// 
// CGRA Math Library
//
//
//----------------------------------------------------------------------------



/*
//-----------------
//      TODO
//-----------------

- FIXME examine all advanced quat functions for correctness
- FIXME quat random
- FIXME min/max for vectors vs std (overloading only for basic_vec is dangerous)
- FIXME unbreak constexpr for msvc and intellisense
- axis/angle function names
- factory function constraints?
- vector etc comparison result magic
- ndc z-bounds for projection matrices (gl vs d3d)
- infinite perspective projection
- defend more things against vectors of vectors (scalar compatibility?)
- defend more things against integer arguments by promoting to floating point
- test is_vector_compatible etc with static asserts
- constexpr everything
- clean up common function definitions and remove duplicates
- implement generic cat functions (with vec_cast?)
- comments and documentation
- create inputstream operators for data types to recreate from output
- implement alternatives to body-3-2-1 euler rotation
	http://www.geometrictools.com/Documentation/EulerAngles.pdf
- noexceptness

*/

#pragma once

// we undefine min and max macros if they exist
// so they don't interfere with our function overloads
// @$#! macros...
#undef MIN
#undef min
#undef MAX
#undef max

#include <cassert>
#include <cmath>
#include <climits>

#include <algorithm>
#include <array>
#include <complex>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <exception>
#include <stdexcept>
#include <initializer_list>

// MSVC and GCC-likes (including Clang) support anonymous structs which
// we use for vector members while still allowing constexpr operator[]
#if !defined(CGRA_NO_ANONYMOUS_STRUCT) && (defined(_MSC_VER) || defined(__GNUC__))
#define CGRA_HAVE_ANONYMOUS_STRUCT
#else
// otherwise, we have to disable constexprness for functions
#define CGRA_NO_CONSTEXPR_FUNCTIONS
#endif

// MSVC prior to vs2017 doesn't support constexpr well enough for our types
#if defined(_MSC_VER) && _MSC_VER < 1910
#define CGRA_NO_CONSTEXPR_FUNCTIONS
#endif

// turn off constexpr functions if needed
// we require constexpr for variables, but it is optional for functions
#ifdef CGRA_NO_CONSTEXPR_FUNCTIONS
#define CGRA_CONSTEXPR_FUNCTION
#endif

// turn on constexpr functions if possible
#ifndef CGRA_CONSTEXPR_FUNCTION
#define CGRA_CONSTEXPR_FUNCTION constexpr
#endif

// we may need these macros to define ctors that intellisense can constexpr-eval

// normal magic ctor definition is dragged in from base class
#ifndef CGRA_DEFINE_MAGIC_CTOR
#define CGRA_DEFINE_MAGIC_CTOR(thisclass_t, baseclass_t, element_t, this_size) using baseclass_t::baseclass_t;
#endif

// normal default ctor delegates to base class
#ifndef CGRA_DEFINE_DEFAULT_CTOR
#define CGRA_DEFINE_DEFAULT_CTOR(thisclass_t, baseclass_t, element_t, this_size) \
CGRA_CONSTEXPR_FUNCTION thisclass_t() : baseclass_t{} {};
#endif

namespace cgra {

	namespace detail {
		namespace scalars {
			template <typename T> class not_nan;
			template <typename T> class basic_quat;
			inline namespace functions {
				// inline for ADL
			}
		}
		namespace vectors {
			template <typename T, size_t N> class basic_vec;
			inline namespace functions {
				// inline for ADL
			}
		}
		namespace matrices {
			template <typename T, size_t Cols, size_t Rows> class basic_mat;
			inline namespace functions {
				// inline for ADL
			}
		}
	}

	template <typename T>
	using not_nan = detail::scalars::not_nan<T>;

	template <typename T>
	using basic_quat = detail::scalars::basic_quat<T>;

	template <typename T, size_t N>
	using basic_vec = detail::vectors::basic_vec<T, N>;

	template <typename T, size_t Cols, size_t Rows>
	using basic_mat = detail::matrices::basic_mat<T, Cols, Rows>;

	using namespace detail::scalars::functions;
	using namespace detail::vectors::functions;
	using namespace detail::matrices::functions;

	using nnfloat = not_nan<float>;
	using nndouble = not_nan<double>;

	class singular_matrix_error : public std::runtime_error {
	public:
		singular_matrix_error() : std::runtime_error("singular matrix") {}
	};

#ifdef CGRA_INITIAL3D_NAMES
	// aliases: Intial3D naming convention

	using vec0f = basic_vec<float, 0>;
	using vec0d = basic_vec<double, 0>;
	using vec0i = basic_vec<int, 0>;
	using vec0u = basic_vec<unsigned, 0>;
	using vec0b = basic_vec<bool, 0>;

	using vec1f = basic_vec<float, 1>;
	using vec1d = basic_vec<double, 1>;
	using vec1i = basic_vec<int, 1>;
	using vec1u = basic_vec<unsigned, 1>;
	using vec1b = basic_vec<bool, 1>;

	using vec2f = basic_vec<float, 2>;
	using vec2d = basic_vec<double, 2>;
	using vec2i = basic_vec<int, 2>;
	using vec2u = basic_vec<unsigned, 2>;
	using vec2b = basic_vec<bool, 2>;

	using vec3f = basic_vec<float, 3>;
	using vec3d = basic_vec<double, 3>;
	using vec3i = basic_vec<int, 3>;
	using vec3u = basic_vec<unsigned, 3>;
	using vec3b = basic_vec<bool, 3>;

	using vec4f = basic_vec<float, 4>;
	using vec4d = basic_vec<double, 4>;
	using vec4i = basic_vec<int, 4>;
	using vec4u = basic_vec<unsigned, 4>;
	using vec4b = basic_vec<bool, 4>;

	using mat2f = basic_mat<float, 2, 2>;
	using mat2d = basic_mat<double, 2, 2>;
	using mat2i = basic_mat<int, 2, 2>;
	using mat3f = basic_mat<float, 3, 3>;
	using mat3d = basic_mat<double, 3, 3>;
	using mat3i = basic_mat<int, 3, 3>;
	using mat4f = basic_mat<float, 4, 4>;
	using mat4d = basic_mat<double, 4, 4>;
	using mat4i = basic_mat<int, 4, 4>;

	using mat2x3f = basic_mat<float, 2, 3>;
	using mat2x3d = basic_mat<double, 2, 3>;
	using mat2x3i = basic_mat<int, 2, 3>;
	using mat2x4f = basic_mat<float, 2, 4>;
	using mat2x4d = basic_mat<double, 2, 4>;
	using mat2x4i = basic_mat<int, 2, 4>;

	using mat3x2f = basic_mat<float, 3, 2>;
	using mat3x2d = basic_mat<double, 3, 2>;
	using mat3x2i = basic_mat<int, 3, 2>;
	using mat3x4f = basic_mat<float, 3, 4>;
	using mat3x4d = basic_mat<double, 3, 4>;
	using mat3x4i = basic_mat<int, 3, 4>;

	using mat4x2f = basic_mat<float, 4, 2>;
	using mat4x2d = basic_mat<double, 4, 2>;
	using mat4x2i = basic_mat<int, 4, 2>;
	using mat4x3f = basic_mat<float, 4, 3>;
	using mat4x3d = basic_mat<double, 4, 3>;
	using mat4x3i = basic_mat<int, 4, 3>;

	using quatf = basic_quat<float>;
	using quatd = basic_quat<double>;

#else
	// aliases: GLSL naming convention

	using vec0 = basic_vec<float, 0>;
	using dvec0 = basic_vec<double, 0>;
	using ivec0 = basic_vec<int, 0>;
	using uvec0 = basic_vec<unsigned, 0>;
	using bvec0 = basic_vec<bool, 0>;

	using vec1 = basic_vec<float, 1>;
	using dvec1 = basic_vec<double, 1>;
	using ivec1 = basic_vec<int, 1>;
	using uvec1 = basic_vec<unsigned, 1>;
	using bvec1 = basic_vec<bool, 1>;

	using vec2 = basic_vec<float, 2>;
	using dvec2 = basic_vec<double, 2>;
	using ivec2 = basic_vec<int, 2>;
	using uvec2 = basic_vec<unsigned, 2>;
	using bvec2 = basic_vec<bool, 2>;

	using vec3 = basic_vec<float, 3>;
	using dvec3 = basic_vec<double, 3>;
	using ivec3 = basic_vec<int, 3>;
	using uvec3 = basic_vec<unsigned, 3>;
	using bvec3 = basic_vec<bool, 3>;

	using vec4 = basic_vec<float, 4>;
	using dvec4 = basic_vec<double, 4>;
	using ivec4 = basic_vec<int, 4>;
	using uvec4 = basic_vec<unsigned, 4>;
	using bvec4 = basic_vec<bool, 4>;

	using mat2 = basic_mat<float, 2, 2>;
	using dmat2 = basic_mat<double, 2, 2>;
	using imat2 = basic_mat<int, 2, 2>;
	using mat3 = basic_mat<float, 3, 3>;
	using dmat3 = basic_mat<double, 3, 3>;
	using imat3 = basic_mat<int, 3, 3>;
	using mat4 = basic_mat<float, 4, 4>;
	using dmat4 = basic_mat<double, 4, 4>;
	using imat4 = basic_mat<int, 4, 4>;

	using mat2x3 = basic_mat<float, 2, 3>;
	using dmat2x3 = basic_mat<double, 2, 3>;
	using imat2x3 = basic_mat<int, 2, 3>;
	using mat2x4 = basic_mat<float, 2, 4>;
	using dmat2x4 = basic_mat<double, 2, 4>;
	using imat2x4 = basic_mat<int, 2, 4>;

	using mat3x2 = basic_mat<float, 3, 2>;
	using dmat3x2 = basic_mat<double, 3, 2>;
	using imat3x2 = basic_mat<int, 3, 2>;
	using mat3x4 = basic_mat<float, 3, 4>;
	using dmat3x4 = basic_mat<double, 3, 4>;
	using imat3x4 = basic_mat<int, 3, 4>;

	using mat4x2 = basic_mat<float, 4, 2>;
	using dmat4x2 = basic_mat<double, 4, 2>;
	using imat4x2 = basic_mat<int, 4, 2>;
	using mat4x3 = basic_mat<float, 4, 3>;
	using dmat4x3 = basic_mat<double, 4, 3>;
	using imat4x3 = basic_mat<int, 4, 3>;

	using quat = basic_quat<float>;
	using dquat = basic_quat<double>;

#endif

	inline namespace constants {
		// pi
		constexpr double pi = 3.1415926535897932384626433832795;

		// natural log base
		constexpr double e = 2.7182818284590452353602874713527;

		// golden ratio
		constexpr double phi = 1.61803398874989484820458683436563811;
	}

	namespace detail {
		namespace scalars {

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

				not_nan(const T &t_) : m_v{t_} {
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

		}
	}




	//   __  .__   __. .___________. _______ .______      .__   __.      ___       __          _______.  //
	//  |  | |  \ |  | |           ||   ____||   _  \     |  \ |  |     /   \     |  |        /       |  //
	//  |  | |   \|  | `---|  |----`|  |__   |  |_)  |    |   \|  |    /  ^  \    |  |       |   (----`  //
	//  |  | |  . `  |     |  |     |   __|  |      /     |  . `  |   /  /_\  \   |  |        \   \      //
	//  |  | |  |\   |     |  |     |  |____ |  |\  \----.|  |\   |  /  _____  \  |  `----.----)   |     //
	//  |__| |__| \__|     |__|     |_______|| _| `._____||__| \__| /__/     \__\ |_______|_______/      //
	//                                                                                                   //
	//===================================================================================================//
	
	namespace detail {

		// note: msvc has some trouble with expansion of template (type) parameter packs that result in integers,
		// so we use integral_constant-like types instead (especially for seq_repeat, make_index_sequence).
		// however, this breaks specialization for specific values in the context of inheritance,
		// so we use the _t alias to unpack the value from the type before specialization.

		template <typename ...Ts>
		using void_t = void;

		template <typename ...Ts>
		struct dependent_false : std::false_type {};

		template <size_t X>
		using index_constant = std::integral_constant<size_t, X>;

		template <bool X>
		using bool_constant = std::integral_constant<bool, X>;

		struct vec_element_ctor_tag {};
		struct vec_dead_ctor_tag {};

		namespace vectors {
			template <typename T, size_t N> class repeat_vec;
			template <typename T, size_t N, bool RequireExactSize, typename ArgTupT, typename BaseDataT> class basic_vec_ctor_proxy;
		}

		template <typename T, size_t N>
		using repeat_vec = vectors::repeat_vec<T, N>;

		template <typename T, size_t Cols, size_t Rows>
		using repeat_vec_vec = repeat_vec<repeat_vec<T, Rows>, Cols>;

		template <typename T, size_t N, bool RequireExactSize, typename ArgTupT, typename BaseDataT>
		using basic_vec_ctor_proxy = vectors::basic_vec_ctor_proxy<T, N, RequireExactSize, ArgTupT, BaseDataT>;

		// got integers? promote them to doubles
		template <typename T, typename = void>
		struct fpromote {
			using type = T;
		};

		template <typename T>
		using fpromote_t = typename fpromote<T>::type;

		template <typename T, typename = void>
		struct scalar_traits {
			static constexpr bool is_scalar = false;
			static constexpr bool want_real_fns = false;
			static constexpr bool want_trig_fns = false;
			static constexpr bool want_exp_fns = false;
			static constexpr bool want_linear_fns = false;
			static constexpr bool want_bool_fns = false;
		};

		template <typename T>
		struct scalar_traits<T, std::enable_if_t<std::numeric_limits<T>::is_specialized>> {
			using fpromote_t = std::conditional_t<std::numeric_limits<T>::is_integer, decltype(std::declval<T>() + 1.0), T>;
			static constexpr bool is_scalar = true;
			static constexpr bool want_real_fns = true;
			static constexpr bool want_trig_fns = true;
			static constexpr bool want_exp_fns = true;
			static constexpr bool want_linear_fns = true;
			static constexpr bool want_bool_fns = true;
		};

		template <typename T>
		struct scalar_traits<std::complex<T>, void> {
			using fpromote_t = std::complex<detail::fpromote_t<T>>;
			static constexpr bool is_scalar = true;
			static constexpr bool want_real_fns = false;
			static constexpr bool want_trig_fns = true;
			static constexpr bool want_exp_fns = true;
			static constexpr bool want_linear_fns = true;
			static constexpr bool want_bool_fns = false;
		};

		template <typename T>
		struct scalar_traits<basic_quat<T>, void> {
			using fpromote_t = basic_quat<detail::fpromote_t<T>>;
			static constexpr bool is_scalar = true;
			static constexpr bool want_real_fns = false;
			static constexpr bool want_trig_fns = false;
			static constexpr bool want_exp_fns = true;
			static constexpr bool want_linear_fns = true;
			static constexpr bool want_bool_fns = false;
		};

		template <typename T>
		struct is_scalar : bool_constant<scalar_traits<std::decay_t<T>>::is_scalar> {};

		template <typename T>
		struct fpromote<T, std::enable_if_t<is_scalar<T>::value>> {
			using type = typename scalar_traits<std::decay_t<T>>::fpromote_t;
		};

		template <typename T>
		struct want_real_fns : bool_constant<scalar_traits<std::decay_t<T>>::want_real_fns> {};

		template <typename T>
		struct want_trig_fns : bool_constant<scalar_traits<std::decay_t<T>>::want_trig_fns> {};

		template <typename T>
		struct want_exp_fns : bool_constant<scalar_traits<std::decay_t<T>>::want_exp_fns> {};

		template <typename T>
		struct want_linear_fns : bool_constant<scalar_traits<std::decay_t<T>>::want_linear_fns> {};

		template <typename T>
		struct want_bool_fns : bool_constant<scalar_traits<std::decay_t<T>>::want_bool_fns> {};

		template <typename T, typename = void>
		struct array_traits {
			// cannot have value_t type; absence required for sfinae
			static constexpr size_t size = 0;

			static constexpr bool is_array = false;
			static constexpr bool is_vector = false;
			static constexpr bool is_matrix = false;
		};

		template <typename T, size_t N>
		struct array_traits<basic_vec<T, N>, void> {
			using value_t = T;
			using copy_t = basic_vec<T, N>;
			using fpromote_t = basic_vec<detail::fpromote_t<T>, N>;
			static constexpr size_t size = N;

			static constexpr bool is_array = true;
			static constexpr bool is_vector = true;
			static constexpr bool is_matrix = false;

			template <size_t I, typename VecT>
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) get(VecT &&v) {
				return std::forward<VecT>(v)[I];
			}
		};

		template <typename T, size_t N>
		struct array_traits<repeat_vec<T, N>, void> : array_traits<basic_vec<T, N>> {};

		template <typename T, size_t N, bool RequireExactSize, typename ExArgTupT, typename BaseDataT>
		struct array_traits<basic_vec_ctor_proxy<T, N, RequireExactSize, ExArgTupT, BaseDataT>, void> : array_traits<basic_vec<T, N>> {};

		template <typename T, size_t Cols, size_t Rows>
		struct array_traits<basic_mat<T, Cols, Rows>, void> : array_traits<basic_vec<basic_vec<T, Rows>, Cols>> {
			using copy_t = basic_mat<T, Cols, Rows>;
			using fpromote_t = basic_mat<detail::fpromote_t<T>, Cols, Rows>;
			static constexpr bool is_vector = false;
			static constexpr bool is_matrix = true;
		};

		// TODO array_traits for std::array, std::tuple

		template <typename VecT>
		using array_value_t = typename array_traits<std::decay_t<VecT>>::value_t;

		template <typename MatT>
		using matrix_value_t = array_value_t<array_value_t<MatT>>;

		template <typename T, typename = void>
		struct array_value_or_void {
			using type = void;
		};

		template <typename T>
		struct array_value_or_void<T, void_t<array_value_t<T>>> {
			using type = array_value_t<T>;
		};

		template <typename T>
		using array_value_or_void_t = typename array_value_or_void<T>::type;

		template <typename VecT>
		struct array_size : index_constant<array_traits<std::decay_t<VecT>>::size> {};

		template <typename T>
		struct is_array : bool_constant<array_traits<std::decay_t<T>>::is_array> {};

		template <typename T>
		struct is_vector : bool_constant<array_traits<std::decay_t<T>>::is_vector> {};

		template <typename T>
		struct is_matrix : bool_constant<array_traits<std::decay_t<T>>::is_matrix> {};

		template <typename VecT>
		struct fpromote<VecT, std::enable_if_t<is_array<VecT>::value>> {
			using type = typename array_traits<std::decay_t<VecT>>::fpromote_t;
		};

		template <typename T, typename = void>
		struct copy_type {
			using type = T;
		};

		template <typename T>
		using copy_type_t = typename copy_type<T>::type;

		template <typename VecT>
		struct copy_type<VecT, void_t<array_value_t<VecT>>> {
			using type = typename array_traits<std::decay_t<VecT>>::copy_t;
		};

		template <typename N>
		struct make_index_sequence {
			using type = std::make_index_sequence<N::value>;
		};

		template <typename N>
		using make_index_sequence_t = typename make_index_sequence<N>::type;

		template <template <typename ...Ts> class F>
		struct meta_quote {
			template <typename ...Us>
			struct apply {
				using type = typename F<Us...>::type;
			};
		};

		template <template <typename ...Ts> class F>
		struct meta_fquote {
			template <typename ...Us>
			struct apply {
				using type = F<Us...>;
			};
		};

		template <typename F, typename Tup>
		struct tup_apply {};

		template <typename F, typename Tup>
		using tup_apply_t = typename tup_apply<F, Tup>::type;

		template <typename F, typename ...Ts>
		struct tup_apply<F, std::tuple<Ts...>> {
			using type = typename F::template apply<Ts...>::type;
		};

		template <typename F, typename T1, typename Tup>
		struct meta_fold {};

		template <typename F, typename T1, typename Tup>
		using meta_fold_t = typename meta_fold<F, T1, Tup>::type;

		template <typename F, typename T1>
		struct meta_fold<F, T1, std::tuple<>> {
			using type = T1;
		};

		template <typename F, typename T1, typename T2>
		struct meta_fold<F, T1, std::tuple<T2>> {
			using type = typename F::template apply<T1, T2>::type;
		};

		template <typename F, typename T1, typename T2, typename T3, typename ...Ts>
		struct meta_fold<F, T1, std::tuple<T2, T3, Ts...>> {
			using type = typename meta_fold<F, typename F::template apply<T1, T2>::type, std::tuple<T3, Ts...>>::type;
		};

		template <typename N1, typename N2>
		struct meta_min {
			using type = index_constant<(N1::value < N2::value) ? N1::value : N2::value>;
		};

		template <typename N1, typename N2>
		using meta_min_t = typename meta_min<N1, N2>::type;

		template <typename N1, typename N2>
		struct meta_max {
			using type = index_constant<(N1::value > N2::value) ? N1::value : N2::value>;
		};

		template <typename N1, typename N2>
		using meta_max_t = typename meta_max<N1, N2>::type;

		template <typename N1, typename N2>
		struct meta_add {
			using type = index_constant<(N1::value + N2::value)>;
		};

		template <typename N1, typename N2>
		using meta_add_t = typename meta_add<N1, N2>::type;

		template <typename N1, typename N2>
		struct meta_sub {
			using type = index_constant<(N1::value - N2::value)>;
		};

		template <typename N1, typename N2>
		using meta_sub_t = typename meta_sub<N1, N2>::type;

		template <typename N>
		struct meta_not {
			using type = bool_constant<!N::value>;
		};

		template <typename N>
		using meta_not_t = typename meta_not<N>::type;

		template <typename N1, typename N2>
		struct meta_and {
			using type = bool_constant<(N1::value && N2::value)>;
		};

		template <typename N1, typename N2>
		using meta_and_t = typename meta_and<N1, N2>::type;

		template <typename N1, typename N2>
		struct meta_or {
			using type = bool_constant<(N1::value || N2::value)>;
		};

		template <typename N1, typename N2>
		using meta_or_t = typename meta_or<N1, N2>::type;

		template <typename F, typename ...Tups>
		struct meta_zip_with {
			template <size_t I>
			struct impl2 {
				using type = typename F::template apply<std::tuple_element_t<I, Tups>...>::type;
			};
			template <typename ISeq>
			struct impl {};
			template <size_t ...Is>
			struct impl<std::index_sequence<Is...>> {
				using type = std::tuple<typename impl2<Is>::type...>;
			};
			using size = meta_fold_t<meta_quote<meta_min>, index_constant<SIZE_MAX>, std::tuple<std::tuple_size<Tups>...>>;
			using type = typename impl<make_index_sequence_t<size>>::type;
		};

		template <typename F, typename ...Tups>
		using meta_zip_with_t = typename meta_zip_with<F, Tups...>::type;

		template <typename ...Tups>
		using meta_zip = meta_zip_with_t<meta_fquote<std::tuple>, Tups...>;

		template <typename ...Tups>
		struct tup_cat {
			using type = decltype(std::tuple_cat(std::declval<Tups>()...));
		};

		template <typename ...Tups>
		using tup_cat_t = typename tup_cat<Tups...>::type;

		template <typename Tup1, typename Tup2>
		struct meta_cartesian_product {
			// yes, this could be generalized to an arbitrary number of tuples.
			// no, im not going to unless i have to.
			template <typename T1>
			struct impl {
				template <typename T2>
				struct impl2 {
					using type = std::tuple<T1, T2>;
				};
				using type = meta_zip_with_t<meta_quote<impl2>, Tup2>;
			};
			using type = tup_apply_t<meta_quote<tup_cat>, meta_zip_with_t<meta_quote<impl>, Tup1>>;
		};

		template <typename Tup1, typename Tup2>
		using meta_cartesian_product_t = typename meta_cartesian_product<Tup1, Tup2>::type;

		template <typename F, typename Tup1, typename Tup2>
		struct meta_cartesian_zip_with {
			template <typename TupX>
			struct impl {
				using type = tup_apply_t<F, TupX>;
			};
			using type = meta_zip_with_t<meta_quote<impl>, meta_cartesian_product_t<Tup1, Tup2>>;
		};

		template <typename F, typename Tup1, typename Tup2>
		using meta_cartesian_zip_with_t = typename meta_cartesian_zip_with<F, Tup1, Tup2>::type;

		template <typename ...VecTs>
		struct array_min_size : meta_fold_t<meta_quote<meta_min>, index_constant<SIZE_MAX>, std::tuple<array_size<VecTs>...>> {};
		
		template <typename MatT>
		struct mat_cols : array_size<MatT> {};

		template <typename MatT>
		struct mat_rows : array_size<array_value_or_void_t<MatT>> {};

		template <typename T1, typename T2>
		struct arith_result2 {
			using type = decltype(std::declval<T1>() + std::declval<T2>());
		};

		template <typename ...Ts>
		using fpromote_arith_t = meta_fold_t<meta_quote<arith_result2>, float, std::tuple<fpromote_t<Ts>...>>;

		template <typename T1, typename T2>
		struct is_mutually_convertible :
			bool_constant<std::is_convertible<T1, T2>::value && std::is_convertible<T2, T1>::value> {};

		template <typename T1, typename T2>
		struct is_mutually_constructible :
			bool_constant<std::is_constructible<T1, T2>::value && std::is_constructible<T2, T1>::value> {};

		// is T a compatible scalar type for array VecT?
		template <typename VecT, typename T, bool Explicit = false, typename = void>
		struct is_array_scalar_compatible : std::false_type {};

		template <typename VecT, typename T>
		struct is_array_scalar_compatible<VecT, T, false, void_t<array_value_t<VecT>>> :
			is_mutually_convertible<array_value_t<VecT>, std::decay_t<T>> {};

		template <typename VecT, typename T>
		struct is_array_scalar_compatible<VecT, T, true, void_t<array_value_t<VecT>>> :
			is_mutually_constructible<array_value_t<VecT>, std::decay_t<T>> {};

		// are the element types of these arrays compatible?
		template <typename VecT0, typename VecT1, bool Explicit = false, typename = void, typename = void>
		struct is_element_compatible : std::false_type {};

		template <typename VecT0, typename VecT1>
		struct is_element_compatible<VecT0, VecT1, false, void_t<array_value_t<VecT0>>, void_t<array_value_t<VecT1>>> :
			bool_constant<is_mutually_convertible<array_value_t<VecT0>, array_value_t<VecT1>>::value> {};

		template <typename VecT0, typename VecT1>
		struct is_element_compatible<VecT0, VecT1, true, void_t<array_value_t<VecT0>>, void_t<array_value_t<VecT1>>> :
			// we still need the 2-way check to avoid scalar broadcast problems etc.
			bool_constant<is_mutually_constructible<array_value_t<VecT0>, array_value_t<VecT1>>::value> {};

		// are these arrays of the same (outer) size and are they element-compatible?
		template <typename VecT0, typename VecT1, bool Explicit = false, typename = void, typename = void>
		struct is_array_compatible : std::true_type {};

		template <typename VecT0, typename VecT1, bool Explicit, typename X>
		struct is_array_compatible<VecT0, VecT1, Explicit, void_t<array_value_t<VecT0>>, X> : std::false_type {};

		template <typename VecT0, typename VecT1, bool Explicit, typename X>
		struct is_array_compatible<VecT0, VecT1, Explicit, X, void_t<array_value_t<VecT1>>> : std::false_type {};

		template <typename VecT0, typename VecT1>
		struct is_array_compatible<VecT0, VecT1, false, void_t<array_value_t<VecT0>>, void_t<array_value_t<VecT1>>> :
			std::integral_constant<bool,
				is_element_compatible<VecT0, VecT1>::value
				&& array_size<VecT0>::value == array_size<VecT1>::value
			>
		{};

		template <typename VecT0, typename VecT1>
		struct is_array_compatible<VecT0, VecT1, true, void_t<array_value_t<VecT0>>, void_t<array_value_t<VecT1>>> :
			std::integral_constant<bool,
				is_element_compatible<VecT0, VecT1>::value
				&& array_size<VecT0>::value == array_size<VecT1>::value
			>
		{};

		// is T element-compatible or scalar-compatible with array CatT? (ie, usable by cat)
		template <typename CatT, typename T, bool Explicit>
		struct is_cat_compatible :
			bool_constant<is_element_compatible<CatT, T, Explicit>::value || is_array_scalar_compatible<CatT, T, Explicit>::value>
		{};

		template <typename VecT0, typename VecT1>
		struct is_vector_compatible :
			bool_constant<is_array_compatible<VecT0, VecT1>::value && is_vector<VecT0>::value && is_vector<VecT1>::value>
		{};

		template <typename VecT, typename T>
		struct is_vector_scalar_compatible :
			bool_constant<is_array_scalar_compatible<VecT, T>::value && is_vector<VecT>::value>
		{};

		template <typename MatT0, typename MatT1>
		struct is_matrix_compatible :
			bool_constant<
				is_array_compatible<MatT0, MatT1>::value
				&& is_array_compatible<array_value_or_void_t<MatT0>, array_value_or_void_t<MatT1>>::value
				&& is_matrix<MatT0>::value
				&& is_matrix<MatT1>::value
			>
		{};

		template <typename MatT0, typename MatT1>
		struct is_matrix_mul_compatible :
			bool_constant<
				is_element_compatible<array_value_or_void_t<MatT0>, array_value_or_void_t<MatT1>>::value
				&& mat_cols<MatT0>::value == mat_rows<MatT1>::value
				&& is_matrix<MatT0>::value
				&& is_matrix<MatT1>::value
			>
		{};

		template <typename MatT, typename T>
		struct is_matrix_scalar_compatible :
			bool_constant<is_array_scalar_compatible<array_value_or_void_t<MatT>, T>::value && is_matrix<MatT>::value>
		{};

		template <typename MatT, typename VecT>
		struct is_matrix_mul_col_compatible :
			bool_constant<
				is_element_compatible<array_value_or_void_t<MatT>, VecT>::value
				&& mat_cols<MatT>::value == array_size<VecT>::value
				&& is_matrix<MatT>::value
				&& is_vector<VecT>::value
			>
		{};

		template <typename MatT, typename VecT>
		struct is_matrix_mul_row_compatible :
			bool_constant<
				is_element_compatible<array_value_or_void_t<MatT>, VecT>::value
				&& mat_rows<MatT>::value == array_size<VecT>::value
				&& is_matrix<MatT>::value
				&& is_vector<VecT>::value
			>
		{};

		template <typename F, typename ...Ts>
		using enable_if_all_t = std::enable_if_t<
			meta_fold_t<
				meta_quote<meta_and>,
				std::true_type,
				std::tuple<typename F::template apply<Ts>::type...>
			>::value,
			int
		>;

		template <typename ...Ts>
		using enable_if_arithmetic_t = enable_if_all_t<meta_fquote<std::is_arithmetic>, Ts...>;

		template <typename ...Ts>
		using enable_if_want_real_fns_t = enable_if_all_t<meta_fquote<want_real_fns>, Ts...>;

		template <typename ...Ts>
		using enable_if_want_trig_fns_t = enable_if_all_t<meta_fquote<want_trig_fns>, Ts...>;

		template <typename ...Ts>
		using enable_if_want_exp_fns_t = enable_if_all_t<meta_fquote<want_exp_fns>, Ts...>;

		template <typename ...Ts>
		using enable_if_want_linear_fns_t = enable_if_all_t<meta_fquote<want_linear_fns>, Ts...>;

		template <typename ...Ts>
		using enable_if_want_bool_fns_t = enable_if_all_t<meta_fquote<want_bool_fns>, Ts...>;

		template <typename ...VecTs>
		using enable_if_array_t = enable_if_all_t<meta_fquote<is_array>, VecTs...>;

		template <typename ...VecTs>
		using enable_if_vector_t = enable_if_all_t<meta_fquote<is_vector>, VecTs...>;
		
		template <typename ...MatTs>
		using enable_if_matrix_t = enable_if_all_t<meta_fquote<is_matrix>, MatTs...>;

		template <typename F, typename ...Ts>
		using enable_if_cartesian_all_t = std::enable_if_t<
			meta_fold_t<
				meta_quote<meta_and>,
				std::true_type,
				meta_cartesian_zip_with_t<
					F,
					std::tuple<Ts...>,
					std::tuple<Ts...>
				>
			>::value,
			int
		>;

		template <typename F, typename ...Ts>
		using enable_if_cartesian_not_all_t = std::enable_if_t<
			!meta_fold_t<
				meta_quote<meta_and>,
				std::true_type,
				meta_cartesian_zip_with_t<
					F,
					std::tuple<Ts...>,
					std::tuple<Ts...>
				>
			>::value,
			int
		>;

		template <typename ...Ts>
		using enable_if_mutually_convertible_t = enable_if_cartesian_all_t<meta_fquote<is_mutually_convertible>, Ts...>;

		template <typename ...Ts>
		using enable_if_not_mutually_convertible_t = enable_if_cartesian_not_all_t<meta_fquote<is_mutually_convertible>, Ts...>;

		template <typename ...Ts>
		using enable_if_mutually_constructible_t = enable_if_cartesian_all_t<meta_fquote<is_mutually_constructible>, Ts...>;

		template <typename ...VecTs>
		using enable_if_vector_compatible_t = enable_if_cartesian_all_t<meta_fquote<is_vector_compatible>, VecTs...>;

		template <typename VecT, typename T>
		using enable_if_vector_scalar_compatible_t = std::enable_if_t<is_vector_scalar_compatible<VecT, T>::value, int>;

		template <typename ...MatTs>
		using enable_if_matrix_compatible_t = enable_if_cartesian_all_t<meta_fquote<is_matrix_compatible>, MatTs...>;

		template <typename MatT0, typename MatT1>
		using enable_if_matrix_mul_compatible_t = std::enable_if_t<is_matrix_mul_compatible<MatT0, MatT1>::value, int>;

		template <typename MatT, typename T>
		using enable_if_matrix_scalar_compatible_t = std::enable_if_t<is_matrix_scalar_compatible<MatT, T>::value, int>;

		template <typename MatT, typename VecT>
		using enable_if_matrix_mul_col_compatible_t = std::enable_if_t<is_matrix_mul_col_compatible<MatT, VecT>::value, int>;

		template <typename MatT, typename VecT>
		using enable_if_matrix_mul_row_compatible_t = std::enable_if_t<is_matrix_mul_row_compatible<MatT, VecT>::value, int>;

		template <typename Tup, size_t N>
		struct tup_repeat {
			using type = tup_cat_t<Tup, typename tup_repeat<Tup, N - 1>::type>;
		};

		template <typename Tup, typename N>
		using tup_repeat_t = typename tup_repeat<Tup, N::value>::type;

		template <typename Tup>
		struct tup_repeat<Tup, 0> {
			using type = std::tuple<>;
		};

		template <typename Tup>
		struct tup_repeat<Tup, 1> {
			using type = Tup;
		};

		template <typename Tup>
		struct tup_repeat<Tup, 2> {
			using type = tup_cat_t<Tup, Tup>;
		};

		template <typename Seq>
		struct seq_pop_front {};

		template <typename Seq>
		using seq_pop_front_t = typename seq_pop_front<Seq>::type;

		template <typename T>
		struct seq_pop_front<std::integer_sequence<T>> {
			using type = std::integer_sequence<T>;
		};

		template <typename T, T V0>
		struct seq_pop_front<std::integer_sequence<T, V0>> {
			using type = std::integer_sequence<T>;
		};

		template <typename T, T V0, T ...Vs>
		struct seq_pop_front<std::integer_sequence<T, V0, Vs...>> {
			using type = std::integer_sequence<T, Vs...>;
		};

		template <typename Seq, size_t I>
		struct seq_get : seq_get<seq_pop_front_t<Seq>, I - 1> {};

		template <typename T, T V0, T ...Vs>
		struct seq_get<std::integer_sequence<T, V0, Vs...>, 0> : std::integral_constant<T, V0> {};

		template <typename ...Seqs>
		struct seq_cat {};

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

		template <typename Seq, size_t N>
		struct seq_repeat {
			using type = seq_cat_t<Seq, typename seq_repeat<Seq, N - 1>::type>;
		};

		template <typename Seq, typename N>
		using seq_repeat_t = typename seq_repeat<Seq, N::value>::type;

		template <typename Seq>
		struct seq_repeat<Seq, 0> {
			using type = std::integer_sequence<typename Seq::value_type>;
		};

		template <typename Seq>
		struct seq_repeat<Seq, 1> {
			using type = Seq;
		};

		template <typename Seq>
		struct seq_repeat<Seq, 2> {
			using type = seq_cat_t<Seq, Seq>;
		};

		template <typename SeqR, typename Seq0, size_t N, typename = void>
		struct seq_trim_impl {};

		template <typename SeqR, typename Seq0>
		struct seq_trim_impl<SeqR, Seq0, 0> {
			using type = SeqR;
		};

		template <typename T, T ...Rs, T I0, T ...Is, size_t N>
		struct seq_trim_impl<
			std::integer_sequence<T, Rs...>,
			std::integer_sequence<T, I0, Is...>,
			N, std::enable_if_t<(N > 0)>
		> {
			using type = typename seq_trim_impl<
				std::integer_sequence<T, Rs..., I0>,
				std::integer_sequence<T, Is...>,
				N - 1
			>::type;
		};

		template <typename Seq, typename N>
		struct seq_trim {
			static_assert(N::value <= Seq::size(), "sequence too small");
			using type = typename seq_trim_impl<std::integer_sequence<typename Seq::value_type>, Seq, N::value>::type;
		};

		template <typename Seq, typename N>
		using seq_trim_t = typename seq_trim<Seq, N>::type;

		template <typename T, bool IsVec>
		struct cat_arg_vec_size_impl : index_constant<1> {};

		template <typename T>
		struct cat_arg_vec_size_impl<T, true> : index_constant<array_traits<std::decay_t<T>>::size> {};

		template <typename CatT, typename T>
		struct cat_arg_vec_size : cat_arg_vec_size_impl<T, is_element_compatible<CatT, T, true>::value> {};

		template <typename CatT, typename ...ArgTs>
		struct cat_arg_vec_total_size :
			meta_fold_t<meta_quote<meta_add>, index_constant<0>, std::tuple<cat_arg_vec_size<CatT, ArgTs>...>> {};

		template <typename CatT, typename Seq, typename Tup>
		struct cat_arg_seq {};

		template <typename CatT, size_t ...Is, typename ...Ts>
		struct cat_arg_seq<CatT, std::index_sequence<Is...>, std::tuple<Ts...>> {
			using type = seq_cat_t<std::index_sequence<>, seq_repeat_t<std::index_sequence<Is>, cat_arg_vec_size<CatT, Ts>>...>;
		};

		template <typename CatT, typename Tup>
		struct cat_val_seq {};

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
			CGRA_CONSTEXPR_FUNCTION static decltype(auto) go(T &&t) {
				return array_traits<std::decay_t<T>>::template get<I>(std::forward<T>(t));
			}
		};

		template <typename CatT, size_t I, typename T>
		CGRA_CONSTEXPR_FUNCTION decltype(auto) vec_get(T &&t) {
			static_assert(
				is_cat_compatible<CatT, T, true>::value,
				"vector cat argument is not element-compatible or scalar-compatible"
			);
			return vec_get_impl<I, T, is_element_compatible<CatT, T, true>::value>::go(std::forward<T>(t));
		}

		template <typename CatT, typename ArgTupT, size_t ...Js, size_t ...Ks>
		CGRA_CONSTEXPR_FUNCTION CatT cat_impl_impl(ArgTupT &&args, std::index_sequence<Js...>, std::index_sequence<Ks...>) {
			return CatT(detail::vec_element_ctor_tag(), vec_get<CatT, Ks>(std::get<Js>(std::forward<ArgTupT>(args)))...);
		}

		template <typename CatT, typename ...Ts>
		CGRA_CONSTEXPR_FUNCTION CatT cat_impl(Ts &&...args) {
			using padsize = meta_sub_t<array_size<CatT>, meta_min_t<array_size<CatT>, cat_arg_vec_total_size<CatT, Ts...>>>;
			using padvec_t = repeat_vec<array_value_t<CatT>, padsize::value>;
			using argseq0 = typename cat_arg_seq<CatT, std::index_sequence_for<Ts..., padvec_t>, std::tuple<Ts..., padvec_t>>::type;
			using valseq0 = typename cat_val_seq<CatT, std::tuple<Ts..., padvec_t>>::type;
			// trim to size so that extra components are ignored
			using argseq = seq_trim_t<argseq0, index_constant<CatT::size>>;
			using valseq = seq_trim_t<valseq0, index_constant<CatT::size>>;
			// prepare padding, default constructed
			padvec_t pad{array_value_t<CatT>()};
			// calling forward_as_tuple calls tuple's defaulted move ctor
			// this compiles fine in vs2017, but intellisense thinks it isn't constexpr
			// so, we call tuple's ctor directly to stop it complaining
			return cat_impl_impl<CatT>(std::tuple<Ts &&..., const padvec_t &>{std::forward<Ts>(args)..., pad}, argseq(), valseq());
		}

		template <typename CatT, typename ...ArgTs>
		struct can_have_implicit_magic_ctor :
			bool_constant<
				array_size<CatT>::value == cat_arg_vec_total_size<CatT, ArgTs...>::value
				&& meta_fold_t<
					meta_quote<meta_and>,
					std::true_type,
					std::tuple<is_cat_compatible<CatT, ArgTs, false>...>
				>::value
			>
		{};

		// single arg -> array only, don't compete with default copy/move
		template <typename CatT, typename ArgT>
		struct can_have_implicit_magic_ctor<CatT, ArgT> : 
			meta_and_t<
				is_array_compatible<CatT, ArgT, false>,
				meta_not_t<std::is_same<CatT, std::decay_t<ArgT>>>
			>
		{};

		template <typename CatT, bool RequireExactSize, typename ...ArgTs>
		struct can_have_explicit_magic_ctor :
			meta_fold_t<
				meta_quote<meta_and>,
				std::true_type,
				std::tuple<
					bool_constant<
						!RequireExactSize
						|| array_size<CatT>::value == cat_arg_vec_total_size<CatT, ArgTs...>::value
					>,
					is_cat_compatible<CatT, ArgTs, true>...
				>
			>
		{};

		// single arg -> array only, don't compete with default copy/move
		template <typename CatT, bool RequireExactSize, typename ArgT>
		struct can_have_explicit_magic_ctor<CatT, RequireExactSize, ArgT> :
			meta_and_t<
				meta_and_t<
					bool_constant<
						!RequireExactSize
						|| array_size<CatT>::value == array_size<ArgT>::value
					>,
					is_element_compatible<CatT, ArgT, true>
				>,
				meta_not_t<std::is_same<CatT, std::decay_t<ArgT>>>
			>
		{};

		template <typename CatT, typename ...ArgTs>
		struct can_have_any_magic_ctor :
			meta_or_t<
				can_have_explicit_magic_ctor<CatT, false, ArgTs...>,
				can_have_implicit_magic_ctor<CatT, ArgTs...>
			>
		{};

		template <typename T, typename ...ArgTs>
		struct can_have_element_ctor :
			meta_fold_t<
				meta_quote<meta_and>,
				std::true_type,
				std::tuple<std::is_same<T, std::decay_t<ArgTs>>...>
			>
		{};

#ifdef __INTELLISENSE__
		// this sometimes helps intellisense make sense of things
		template <typename T>
		CGRA_CONSTEXPR_FUNCTION const T & intellisense_constify(const T &t) {
			return t;
		}
#else
		template <typename T>
		CGRA_CONSTEXPR_FUNCTION decltype(auto) intellisense_constify(T &&t) {
			return std::forward<T>(t);
		}
#endif

		template <typename T, size_t Cols, size_t Rows>
		CGRA_CONSTEXPR_FUNCTION auto make_mat(basic_vec<basic_vec<T, Rows>, Cols> &&v) {
			return basic_mat<T, Cols, Rows>(std::move(v));
		}

		template <typename T, size_t N>
		struct vec_exarg_tup {
			using type = tup_repeat_t<std::tuple<T>, index_constant<N>>;
		};

		// if we don't do this, the explicit arg ctor conflicts with the default ctor for vec0
		template <typename T>
		struct vec_exarg_tup<T, 0> {
			using type = std::tuple<vec_dead_ctor_tag>;
		};

		template <typename T, size_t N>
		using vec_exarg_tup_t = typename vec_exarg_tup<T, N>::type;

		template <typename T, size_t N>
		struct simple_array {
			T data[N];

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				return data[i];
			}

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				return data[i];
			}
		};

		// base type for vector data storage
		// specializations must be default constructible, copyable, movable and destructible;
		// this means care must be taken to handle unions correctly.
		template <typename T, size_t N, typename = void>
		class basic_vec_data {
		public:
			// not using std::array for better constexpr behaviour
			simple_array<T, N> m_data;

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : m_data{} {}

			template <typename ...ArgTs, typename = std::enable_if_t<can_have_element_ctor<T, ArgTs...>::value>>
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(ArgTs &&...args) : m_data{intellisense_constify(std::forward<ArgTs>(args))...} {}

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
		};

		template <typename T>
		class basic_vec_data<T, 0> {
		public:
			CGRA_CONSTEXPR_FUNCTION basic_vec_data() {}
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(vec_dead_ctor_tag) {}

			CGRA_CONSTEXPR_FUNCTION T operator[](size_t) const {
				throw std::logic_error("0-size basic_vec has no elements");
			}

			CGRA_CONSTEXPR_FUNCTION T * data() { return nullptr; }

			CGRA_CONSTEXPR_FUNCTION const T * data() const { return nullptr; }
		};

		template <typename T, typename X>
		class basic_vec_data<T, 1, X> {
		public:
			union { T x, r, s; };

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : x{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) :
				x{other.x} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				x{std::move(other.x)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				x = other.x;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				x = std::move(other.x);
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION explicit basic_vec_data(T x_) :
				x{std::move(x_)} {}

			const T & operator[](size_t i) const {
				assert(i == 0);
				return x;
			}

			T & operator[](size_t i) {
				assert(i == 0);
				return x;
			}

			CGRA_CONSTEXPR_FUNCTION const T * data() const {
				return &x;
			}

			CGRA_CONSTEXPR_FUNCTION T * data() {
				return &x;
			}

			~basic_vec_data() {
				x.~T();
			}
		};

		template <typename T, typename X>
		class basic_vec_data<T, 2, X> {
		public:
			union { T x, r, s; };
			union { T y, g, t; };

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : x{}, y{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) :
				x{other.x}, y{other.y} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				x{std::move(other.x)}, y{std::move(other.y)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				x = other.x;
				y = other.y;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				x = std::move(other.x);
				y = std::move(other.y);
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION explicit basic_vec_data(T x_, T y_) :
				x{std::move(x_)}, y{std::move(y_)} {}

			const T & operator[](size_t i) const {
				assert(i < 2);
				return *(&x + i);
			}

			T & operator[](size_t i) {
				assert(i < 2);
				return *(&x + i);
			}

			CGRA_CONSTEXPR_FUNCTION const T * data() const {
				return &x;
			}

			CGRA_CONSTEXPR_FUNCTION T * data() {
				return &x;
			}

			~basic_vec_data() {
				x.~T();
				y.~T();
			}
		};

		template <typename T, typename X>
		class basic_vec_data<T, 3, X> {
		public:
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : x{}, y{}, z{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) :
				x{other.x}, y{other.y}, z{other.z} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				x{std::move(other.x)}, y{std::move(other.y)}, z{std::move(other.z)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				x = std::move(other.x);
				y = std::move(other.y);
				z = std::move(other.z);
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION explicit basic_vec_data(T x_, T y_, T z_) :
				x{std::move(x_)}, y{std::move(y_)}, z{std::move(z_)} {}

			const T & operator[](size_t i) const {
				assert(i < 3);
				return *(&x + i);
			}

			T & operator[](size_t i) {
				assert(i < 3);
				return *(&x + i);
			}

			CGRA_CONSTEXPR_FUNCTION const T * data() const {
				return &x;
			}

			CGRA_CONSTEXPR_FUNCTION T * data() {
				return &x;
			}

			~basic_vec_data() {
				x.~T();
				y.~T();
				z.~T();
			}
		};

		template <typename T, typename X>
		class basic_vec_data<T, 4, X> {
		public:
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
			union { T w, a, q; };

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : x{}, y{}, z{}, w{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) :
				x{other.x}, y{other.y}, z{other.z}, w{other.w} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				x{std::move(other.x)}, y{std::move(other.y)}, z{std::move(other.z)}, w{std::move(other.w)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				x = std::move(other.x);
				y = std::move(other.y);
				z = std::move(other.z);
				w = std::move(other.w);
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION explicit basic_vec_data(T x_, T y_, T z_, T w_) :
				x{std::move(x_)}, y{std::move(y_)}, z{std::move(z_)}, w{std::move(w_)} {}

			const T & operator[](size_t i) const {
				assert(i < 4);
				return *(&x + i);
			}

			T & operator[](size_t i) {
				assert(i < 4);
				return *(&x + i);
			}

			CGRA_CONSTEXPR_FUNCTION const T * data() const {
				return &x;
			}

			CGRA_CONSTEXPR_FUNCTION T * data() {
				return &x;
			}

			~basic_vec_data() {
				x.~T();
				y.~T();
				z.~T();
				w.~T();
			}
		};

#ifdef CGRA_HAVE_ANONYMOUS_STRUCT
		// we turn off some warnings around these specializations
		// because otherwise we get warnings about our anonymous structs
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)
#endif

		template <typename T>
		class basic_vec_data<T, 1, std::enable_if_t<std::is_trivially_destructible<T>::value>> {
		public:
			union {
				simple_array<T, 1> m_data;
				struct { T x; };
				struct { T r; };
				struct { T s; };
			};

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : m_data{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) : m_data{other.m_data} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				m_data{std::move(other.m_data)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				m_data = other.m_data;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				m_data = std::move(other.m_data);
				return *this;
			}
			
			template <typename ...ArgTs, typename = std::enable_if_t<can_have_element_ctor<T, ArgTs...>::value>>
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(ArgTs &&...args) : m_data{intellisense_constify(std::forward<ArgTs>(args))...} {}

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				assert(i < 1);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				assert(i < 1);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0]; }

			CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0]; }
		};

		template <typename T>
		class basic_vec_data<T, 2, std::enable_if_t<std::is_trivially_destructible<T>::value>> {
		public:
			union {
				simple_array<T, 2> m_data;
				struct { T x; T y; };
				struct { T r; T g; };
				struct { T s; T t; };
			};

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : m_data{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) : m_data{other.m_data} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				m_data{std::move(other.m_data)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				m_data = other.m_data;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				m_data = std::move(other.m_data);
				return *this;
			}

			template <typename ...ArgTs, typename = std::enable_if_t<can_have_element_ctor<T, ArgTs...>::value>>
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(ArgTs &&...args) : m_data{intellisense_constify(std::forward<ArgTs>(args))...} {}

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				assert(i < 2);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				assert(i < 2);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0]; }

			CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0]; }
		};

		template <typename T>
		class basic_vec_data<T, 3, std::enable_if_t<std::is_trivially_destructible<T>::value>> {
		public:
			union {
				simple_array<T, 3> m_data;
				struct { T x; T y; T z; };
				struct { T r; T g; T b; };
				struct { T s; T t; T p; };
			};

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : m_data{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) : m_data{other.m_data} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				m_data{std::move(other.m_data)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				m_data = other.m_data;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				m_data = std::move(other.m_data);
				return *this;
			}

			template <typename ...ArgTs, typename = std::enable_if_t<can_have_element_ctor<T, ArgTs...>::value>>
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(ArgTs &&...args) : m_data{intellisense_constify(std::forward<ArgTs>(args))...} {}

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				assert(i < 3);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				assert(i < 3);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0]; }

			CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0]; }
		};

		template <typename T>
		class basic_vec_data<T, 4, std::enable_if_t<std::is_trivially_destructible<T>::value>> {
		public:
			union {
				simple_array<T, 4> m_data;
				struct { T x; T y; T z; T w; };
				struct { T r; T g; T b; T a; };
				struct { T s; T t; T p; T q; };
			};

			CGRA_CONSTEXPR_FUNCTION basic_vec_data() : m_data{} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(const basic_vec_data &other) : m_data{other.m_data} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data(basic_vec_data &&other) noexcept(std::is_nothrow_move_constructible<T>::value) :
				m_data{std::move(other.m_data)} {}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(const basic_vec_data &other) {
				m_data = other.m_data;
				return *this;
			}

			CGRA_CONSTEXPR_FUNCTION basic_vec_data & operator=(basic_vec_data &&other) noexcept(std::is_nothrow_move_assignable<T>::value) {
				m_data = std::move(other.m_data);
				return *this;
			}

			template <typename ...ArgTs, typename = std::enable_if_t<can_have_element_ctor<T, ArgTs...>::value>>
			CGRA_CONSTEXPR_FUNCTION basic_vec_data(ArgTs &&...args) : m_data{intellisense_constify(std::forward<ArgTs>(args))...} {}

			CGRA_CONSTEXPR_FUNCTION T & operator[](size_t i) {
				assert(i < 4);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION const T & operator[](size_t i) const {
				assert(i < 4);
				return m_data[i];
			}

			CGRA_CONSTEXPR_FUNCTION T * data() { return &m_data[0]; }

			CGRA_CONSTEXPR_FUNCTION const T * data() const { return &m_data[0]; }
		};

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // CGRA_HAVE_ANONYMOUS_STRUCT
		
		namespace vectors {

			template <typename T, size_t N>
			class repeat_vec {
			private:
				T m_v;

			public:
				using value_t = T;
				static constexpr size_t size = N;

				CGRA_CONSTEXPR_FUNCTION repeat_vec() { }

				template <typename U>
				CGRA_CONSTEXPR_FUNCTION explicit repeat_vec(U &&u) : m_v(intellisense_constify(std::forward<U>(u))) { }

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

			template <typename T, size_t N, bool RequireExactSize, typename ArgTupT, typename BaseDataT>
			class basic_vec_ctor_proxy {};

			template <typename T, size_t N, bool RequireExactSize, typename ...ExArgTs, typename BaseDataT>
			class basic_vec_ctor_proxy<T, N, RequireExactSize, std::tuple<ExArgTs...>, BaseDataT> : protected BaseDataT {
			private:
				using base_data_t = BaseDataT;

				static_assert(
					std::is_nothrow_move_constructible<base_data_t>::value == std::is_nothrow_move_constructible<T>::value,
					"basic_vec_data should be as nothrow-move-constructible as its elements"
				);

				static_assert(
					std::is_nothrow_move_assignable<base_data_t>::value == std::is_nothrow_move_assignable<T>::value,
					"basic_vec_data should be as nothrow-move-assignable as its elements"
				);

			public:
				using value_t = T;
				static constexpr size_t size = N;

				// MSVC VS2017: this has to be public (compiler bug?)
				static constexpr bool require_exact_size = RequireExactSize;

				using base_data_t::operator[];
				using base_data_t::data;

				// default ctor
				// explictly defining this to delegate to the superclass prevents MSVC in VS2015
				// from silently producing bad code in a certain case, and instead causes an ICE
				// the bit of our code that caused this (in the general magic ctor) is now fixed
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy() : base_data_t{} {}

				// explictly default the copy and move ops to ensure the base class supplies them
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy(const basic_vec_ctor_proxy &other) = default;
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy(basic_vec_ctor_proxy &&other) = default;
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy & operator=(const basic_vec_ctor_proxy &other) = default;
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy & operator=(basic_vec_ctor_proxy &&other) = default;

				// explict arg ctor for nested brace initializers
				// vec_exarg_tup_t produces {vec_dead_ctor_tag} for vec0 to avoid conflict with the default ctor
				// FIXME this needs to be implicit without breaking anything for >2 levels of braces
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec_ctor_proxy(ExArgTs ...ts) :
					base_data_t{intellisense_constify(std::move(ts))...}
				{}

				// tagged ctor, used by cat
				// this must use generic forwarding references to avoid being out-competed by the magic ctor
				template <typename ...ArgTs>
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec_ctor_proxy(vec_element_ctor_tag, ArgTs &&...args) :
					base_data_t{intellisense_constify(static_cast<T>(std::forward<ArgTs>(args)))...}
				{}

				// general magic ctor (implicit)
				// enforcing >= 2 args is necessary to prevent unwanted implicit conversions
				// direct enforcement with explicit 1st and 2nd args results in this being used instead of the above tagged ctor
				// this implicit version checks that the number of elements is correct and that they are implicitly convertible
				// this is callable from within nested braced init lists
				// for MSVC in VS2015, we have to have an explicit 1st arg to avoid some kind of weird conflict with the default ctor
				template <
					typename ArgT0,
					typename ...ArgTs,
					typename = std::enable_if_t<
						(sizeof...(ArgTs) >= 1)
						&& can_have_implicit_magic_ctor<basic_vec_ctor_proxy, ArgT0, ArgTs...>::value
					>
				>
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy(ArgT0 &&arg0, ArgTs &&...args) :
					// note that cat_impl constructs a basic_vec_ctor_proxy, not a basic_vec
					// the latter would result in recursively delegating to this ctor
					// in correct operation, the (default) move ctor is delegated to
					basic_vec_ctor_proxy{intellisense_constify(cat_impl<basic_vec_ctor_proxy>(std::forward<ArgT0>(arg0), std::forward<ArgTs>(args)...))}
				{}

				// general magic ctor (explicit)
				// this explicit version allows arguments with a different total number of elements and
				// element types that are only explicitly convertible
				template <
					typename ArgT0,
					typename ...ArgTs,
					typename = std::enable_if_t<
						(sizeof...(ArgTs) >= 1)
						&& can_have_explicit_magic_ctor<basic_vec_ctor_proxy, require_exact_size, ArgT0, ArgTs...>::value
						// second condition needed to not conflict with implicit general magic ctor
						&& !can_have_implicit_magic_ctor<basic_vec_ctor_proxy, ArgT0, ArgTs...>::value
					>,
					typename = void
				>
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec_ctor_proxy(ArgT0 &&arg0, ArgTs &&...args) :
					basic_vec_ctor_proxy{intellisense_constify(cat_impl<basic_vec_ctor_proxy>(std::forward<ArgT0>(arg0), std::forward<ArgTs>(args)...))}
				{}

				// 1-arg magic ctor (implicit)
				// this is separate from the general magic ctor to restrict single args to only vectors
				// ie, it disables implicit conversion of scalars to vectors with the magic ctors
				// this implicit version checks that the vector sizes are the same and the element types are implicitly convertible
				template <
					typename VecT,
					typename = std::enable_if_t<
						can_have_implicit_magic_ctor<basic_vec_ctor_proxy, VecT>::value
					>
				>
				CGRA_CONSTEXPR_FUNCTION basic_vec_ctor_proxy(VecT &&v) :
					basic_vec_ctor_proxy{intellisense_constify(cat_impl<basic_vec_ctor_proxy>(std::forward<VecT>(v)))}
				{}

				// 1-arg magic ctor (explicit)
				// this explicit version also restricts single args to only vectors, but allows vectors of different sizes
				// and those whose elements are only explicitly convertible
				template <
					typename VecT,
					typename = std::enable_if_t<
						can_have_explicit_magic_ctor<basic_vec_ctor_proxy, require_exact_size, VecT>::value
						// second condition needed to not conflict with implicit 1-arg magic ctor
						&& !can_have_implicit_magic_ctor<basic_vec_ctor_proxy, VecT>::value
					>,
					typename = void
				>
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec_ctor_proxy(VecT &&v) :
					basic_vec_ctor_proxy{intellisense_constify(cat_impl<basic_vec_ctor_proxy>(std::forward<VecT>(v)))}
				{}

			};

		}



	}




	// 
	// operator function classes
	// 
	// 
	// 
	// 
	// 
	//=================

	namespace detail {

		// used in place of a void return to be usable with zip_with
		struct nothing {};

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

			struct bitwise_or_assign {
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

			struct not_equal {
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




	//   _______       ___   .___________.    ___              _______.___________..______       __    __    ______ .___________. __    __  .______       _______     _______.  //
	//  |       \     /   \  |           |   /   \            /       |           ||   _  \     |  |  |  |  /      ||           ||  |  |  | |   _  \     |   ____|   /       |  //
	//  |  .--.  |   /  ^  \ `---|  |----`  /  ^  \          |   (----`---|  |----`|  |_)  |    |  |  |  | |  ,----'`---|  |----`|  |  |  | |  |_)  |    |  |__     |   (----`  //
	//  |  |  |  |  /  /_\  \    |  |      /  /_\  \          \   \       |  |     |      /     |  |  |  | |  |         |  |     |  |  |  | |      /     |   __|     \   \      //
	//  |  '--'  | /  _____  \   |  |     /  _____  \     .----)   |      |  |     |  |\  \----.|  `--'  | |  `----.    |  |     |  `--'  | |  |\  \----.|  |____.----)   |     //
	//  |_______/ /__/     \__\  |__|    /__/     \__\    |_______/       |__|     | _| `._____| \______/   \______|    |__|      \______/  | _| `._____||_______|_______/      //
	//                                                                                                                                                                          //
	//==========================================================================================================================================================================//

	namespace detail {

		namespace scalars {

			// quaternion of type T
			template <typename T>
			class basic_quat {
			public:
				using value_t = T;

				T w, x, y, z;

				// default ctor
				CGRA_CONSTEXPR_FUNCTION basic_quat() :
					w(), x(), y(), z() {}

				// cross-type copy ctor (implicit)
				template <typename U, enable_if_mutually_convertible_t<T, U> = 0>
				CGRA_CONSTEXPR_FUNCTION basic_quat(const basic_quat<U> &q) :
					w(q.w), x(q.x), y(q.y), z(q.z) {}

				// cross-type copy ctor (explicit)
				template <typename U, enable_if_mutually_constructible_t<T, U> = 0, enable_if_not_mutually_convertible_t<T, U> = 0>
				CGRA_CONSTEXPR_FUNCTION explicit basic_quat(const basic_quat<U> &q) :
					w(q.w), x(q.x), y(q.y), z(q.z) {}

				// real ctor
				CGRA_CONSTEXPR_FUNCTION basic_quat(T t) :
					w(std::move(t)), x(), y(), z() {}

				// real + vector imag ctor
				CGRA_CONSTEXPR_FUNCTION basic_quat(T w_, basic_vec<T, 3> xyz) :
					w(std::move(w_)), x(std::move(xyz.x)), y(std::move(xyz.y)), z(std::move(xyz.y)) {}

				// TODO complex ctor?

				// all values ctor (prefer the real + vector imag ctor)
				CGRA_CONSTEXPR_FUNCTION explicit basic_quat(T w_, T x_, T y_, T z_) :
					w(std::move(w_)), x(std::move(x_)), y(std::move(y_)), z(std::move(z_)) {}

				CGRA_CONSTEXPR_FUNCTION explicit basic_quat(basic_vec<T, 4> v) :
					w(std::move(v[0])), x(std::move(v[1])), y(std::move(v[2])), z(std::move(v[3])) {}

				CGRA_CONSTEXPR_FUNCTION explicit operator basic_vec<T, 4>() const {
					return basic_vec<T, 4>{w, x, y, z};
				}

				// TODO proper access to real/imag components

				// basic_mat<U, 3, 3> conversion
				template <typename U>
				CGRA_CONSTEXPR_FUNCTION explicit operator basic_mat<U, 3, 3>() const {
					basic_mat<U, 3, 3> m;
					// col 0
					m[0][0] = w * w + x * x - y * y - z * z;
					m[0][1] = 2 * x * y + 2 * w * z;
					m[0][2] = 2 * x * z - 2 * w * y;
					// col 1
					m[1][0] = 2 * x * y - 2 * w * z;
					m[1][1] = w * w - x * x + y * y - z * z;
					m[1][2] = 2 * y * z + 2 * w * x;
					// col 2
					m[2][0] = 2 * x * z + 2 * w * y;
					m[2][1] = 2 * y * z - 2 * w * x;
					m[2][2] = w * w - x * x - y * y + z * z;
					return m;
				}

				// basic_mat<U, 4, 4> conversion
				template <typename U>
				CGRA_CONSTEXPR_FUNCTION explicit operator basic_mat<U, 4, 4>() const {
					basic_mat<U, 4, 4> m;
					// col 0
					m[0][0] = w * w + x * x - y * y - z * z;
					m[0][1] = 2 * x * y + 2 * w * z;
					m[0][2] = 2 * x * z - 2 * w * y;
					m[0][3] = 0;
					// col 1
					m[1][0] = 2 * x * y - 2 * w * z;
					m[1][1] = w * w - x * x + y * y - z * z;
					m[1][2] = 2 * y * z + 2 * w * x;
					m[1][3] = 0;
					// col 2
					m[2][0] = 2 * x * z + 2 * w * y;
					m[2][1] = 2 * y * z - 2 * w * x;
					m[2][2] = w * w - x * x - y * y + z * z;
					m[2][3] = 0;
					// col 3
					m[3][0] = 0;
					m[3][1] = 0;
					m[3][2] = 0;
					m[3][3] = w * w + x * x + y * y + z * z;
					return m;
				}
			};

			template <typename T>
			CGRA_CONSTEXPR_FUNCTION inline void swap(basic_quat<T> &lhs, basic_quat<T> &rhs) {
				using std::swap;
				// TODO noexcept
				swap(lhs.w, rhs.w);
				swap(lhs.x, rhs.x);
				swap(lhs.y, rhs.y);
				swap(lhs.z, rhs.z);
			}

			template <typename T>
			inline std::ostream & operator<<(std::ostream &out, const basic_quat<T> &v) {
				return out << '(' << v.w << " + " << v.x << "i + " << v.y << "j + " << v.z << "k)";
			}

		}

		namespace vectors {

			// size N vector of type T
			template <typename T, size_t N>
			class basic_vec : protected detail::basic_vec_ctor_proxy<T, N, false, detail::vec_exarg_tup_t<T, N>, detail::basic_vec_data<T, N>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, N, false, detail::vec_exarg_tup_t<T, N>, detail::basic_vec_data<T, N>>;

			public:
				using value_t = T;
				static constexpr size_t size = N;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// scalar broadcast ctor
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) :
					ctor_proxy_t{detail::intellisense_constify(detail::repeat_vec<const T &, N>(t))} {}

			};

			template <typename T>
			class basic_vec<T, 0> : protected detail::basic_vec_ctor_proxy<T, 0, false, detail::vec_exarg_tup_t<T, 0>, detail::basic_vec_data<T, 0>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, 0, false, detail::vec_exarg_tup_t<T, 0>, detail::basic_vec_data<T, 0>>;

			public:
				using value_t = T;
				static constexpr size_t size = 0;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// scalar broadcast ctor
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &) {}

			};

			template <typename T>
			class basic_vec<T, 1> : protected detail::basic_vec_ctor_proxy<T, 1, false, detail::vec_exarg_tup_t<T, 1>, detail::basic_vec_data<T, 1>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, 1, false, detail::vec_exarg_tup_t<T, 1>, detail::basic_vec_data<T, 1>>;

			public:
				using value_t = T;
				static constexpr size_t size = 1;

				using detail::basic_vec_data<T, 1>::x;
				using detail::basic_vec_data<T, 1>::r;
				using detail::basic_vec_data<T, 1>::s;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// vec1 has no scalar broadcast ctor to avoid conflict with the explicit arg ctor

			};

			template <typename T>
			class basic_vec<T, 2> : protected detail::basic_vec_ctor_proxy<T, 2, false, detail::vec_exarg_tup_t<T, 2>, detail::basic_vec_data<T, 2>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, 2, false, detail::vec_exarg_tup_t<T, 2>, detail::basic_vec_data<T, 2>>;

			public:
				using value_t = T;
				static constexpr size_t size = 2;

				using detail::basic_vec_data<T, 2>::x;
				using detail::basic_vec_data<T, 2>::r;
				using detail::basic_vec_data<T, 2>::s;
				using detail::basic_vec_data<T, 2>::y;
				using detail::basic_vec_data<T, 2>::g;
				using detail::basic_vec_data<T, 2>::t;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// scalar broadcast ctor
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) :
					ctor_proxy_t{detail::intellisense_constify(detail::repeat_vec<const T &, 2>(t))} {}

			};

			template <typename T>
			class basic_vec<T, 3> : protected detail::basic_vec_ctor_proxy<T, 3, false, detail::vec_exarg_tup_t<T, 3>, detail::basic_vec_data<T, 3>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, 3, false, detail::vec_exarg_tup_t<T, 3>, detail::basic_vec_data<T, 3>>;

			public:
				using value_t = T;
				static constexpr size_t size = 3;

				using detail::basic_vec_data<T, 3>::x;
				using detail::basic_vec_data<T, 3>::r;
				using detail::basic_vec_data<T, 3>::s;
				using detail::basic_vec_data<T, 3>::y;
				using detail::basic_vec_data<T, 3>::g;
				using detail::basic_vec_data<T, 3>::t;
				using detail::basic_vec_data<T, 3>::z;
				using detail::basic_vec_data<T, 3>::b;
				using detail::basic_vec_data<T, 3>::p;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// scalar broadcast ctor
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) :
					ctor_proxy_t{detail::intellisense_constify(detail::repeat_vec<const T &, 3>(t))} {}

			};

			template <typename T>
			class basic_vec<T, 4> : protected detail::basic_vec_ctor_proxy<T, 4, false, detail::vec_exarg_tup_t<T, 4>, detail::basic_vec_data<T, 4>> {
			private:
				using ctor_proxy_t = detail::basic_vec_ctor_proxy<T, 4, false, detail::vec_exarg_tup_t<T, 4>, detail::basic_vec_data<T, 4>>;

			public:
				using value_t = T;
				static constexpr size_t size = 4;

				using detail::basic_vec_data<T, 4>::x;
				using detail::basic_vec_data<T, 4>::r;
				using detail::basic_vec_data<T, 4>::s;
				using detail::basic_vec_data<T, 4>::y;
				using detail::basic_vec_data<T, 4>::g;
				using detail::basic_vec_data<T, 4>::t;
				using detail::basic_vec_data<T, 4>::z;
				using detail::basic_vec_data<T, 4>::b;
				using detail::basic_vec_data<T, 4>::p;
				using detail::basic_vec_data<T, 4>::w;
				using detail::basic_vec_data<T, 4>::a;
				using detail::basic_vec_data<T, 4>::q;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_vec, ctor_proxy_t, T, size)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_vec, ctor_proxy_t, T, size)

				// make inherited functions visible
				using ctor_proxy_t::operator[];
				using ctor_proxy_t::data;

				// scalar broadcast ctor
				CGRA_CONSTEXPR_FUNCTION explicit basic_vec(const T &t) :
					ctor_proxy_t{detail::intellisense_constify(detail::repeat_vec<const T &, 4>(t))} {}

			};

			namespace functions {

				template <typename T, size_t N>
				CGRA_CONSTEXPR_FUNCTION void swap(basic_vec<T, N> &lhs, basic_vec<T, N> &rhs) {
					// TODO c++17: noexcept(std::is_nothrow_swappable<T>::value)
					for (size_t i = 0; i < N; i++) {
						using std::swap;
						swap(lhs[i], rhs[i]);
					}
				}

				template <typename T, size_t N>
				inline std::ostream & operator<<(std::ostream &out, const basic_vec<T, N> &v) {
					out << '(';
					if (N > 0) out << v[0];
					for (size_t i = 1; i < N; ++i) {
						out << ", " << v[i];
					}
					out << ')';
					return out;
				}

				template <typename T, size_t N>
				inline T * begin(basic_vec<T, N> &v) { return v.data(); }

				template <typename T, size_t N>
				inline const T * begin(const basic_vec<T, N> &v) { return v.data(); }

				template <typename T, size_t N>
				inline const T * cbegin(const basic_vec<T, N> &v) { return v.data(); }

				template <typename T, size_t N>
				inline T * end(basic_vec<T, N> &v) { return begin(v) + N; }

				template <typename T, size_t N>
				inline const T * end(const basic_vec<T, N> &v) { return begin(v) + N; }

				template <typename T, size_t N>
				inline const T * cend(const basic_vec<T, N> &v) { return begin(v) + N; }

			}

		}

		namespace matrices {

			// Cols x Rows matrix of type T
			template <typename T, size_t Cols, size_t Rows>
			class basic_mat : protected basic_vec<basic_vec<T, Rows>, Cols> {
			private:
				using vec_t = basic_vec<basic_vec<T, Rows>, Cols>;
				using element_t = basic_vec<T, Rows>;

			public:
				using value_t = T;
				static constexpr size_t cols = Cols;
				static constexpr size_t rows = Rows;

				// TODO should this member exist? is it useful? could be confusing vs. basic_vec::size
				//static constexpr size_t size = Cols * Rows;

				// define default ctor
				CGRA_DEFINE_DEFAULT_CTOR(basic_mat, vec_t, element_t, cols)

				// define magic ctor
				CGRA_DEFINE_MAGIC_CTOR(basic_mat, vec_t, element_t, cols)

				// make inherited functions visible
				using vec_t::operator[];

				// identity ctor
				// this does not hide the inherited scalar broadcast ctor
				// (that takes a vec<T>, this takes a T)
				CGRA_CONSTEXPR_FUNCTION explicit basic_mat(const T &t) {
					for (size_t i = 0; i < std::min(Cols, Rows); i++) {
						(*this)[i][i] = t;
					}
				}

				CGRA_CONSTEXPR_FUNCTION T * data() { return &(*this)[0][0]; }

				CGRA_CONSTEXPR_FUNCTION const T * data() const { return &(*this)[0][0]; }

				CGRA_CONSTEXPR_FUNCTION vec_t & as_vec() { return *this; }

				CGRA_CONSTEXPR_FUNCTION const vec_t & as_vec() const { return *this; }

			};

			namespace functions {

				template <typename T, size_t Cols, size_t Rows>
				void swap(basic_mat<T, Cols, Rows> &lhs, basic_mat<T, Cols, Rows> &rhs) {
					// TODO c++17: noexcept(std::is_nothrow_swappable<T>::value)
					for (size_t i = 0; i < Cols; i++) {
						using std::swap;
						swap(lhs[i], rhs[i]);
					}
				}

				template <typename T, size_t Cols, size_t Rows>
				inline std::ostream & operator<<(std::ostream &out, const basic_mat<T, Cols, Rows> &m) {
					static const char *toplinestart = Rows > 1 ? "/ " : "[ ";
					static const char *toplineend = Rows > 1 ? " \\" : " ]";
					static const char *midlinestart = "| ";
					static const char *midlineend = " |";
					static const char *bottomlinestart = Rows > 1 ? "\\ " : "[ ";
					static const char *bottomlineend = Rows > 1 ? " /" : " ]";
					// calculate column widths
					size_t maxwidths[Cols];
					std::ostringstream tempss;
					tempss.precision(out.precision());
					for (size_t i = 0; i < Cols; ++i) {
						maxwidths[i] = 0;
						for (size_t j = 0; j < Rows; ++j) {
							// msvc: seeking to 0 on empty stream sets failbit
							tempss.seekp(0);
							tempss.clear();
							tempss << m[i][j];
							maxwidths[i] = size_t(tempss.tellp()) > maxwidths[i] ? size_t(tempss.tellp()) : maxwidths[i];
						}
					}
					// print
					for (size_t i = 0; i < Rows; ++i) {
						if (i == 0) {
							out << toplinestart;
						} else if (i + 1 == Rows) {
							out << bottomlinestart;
						} else {
							out << midlinestart;
						}
						for (size_t j = 0; j < Cols; ++j) {
							out.width(maxwidths[j]);
							out << m[j][i];
							if (j + 1 < Cols) out << ", ";
						}
						if (i == 0) {
							out << toplineend;
						} else if (i + 1 == Rows) {
							out << bottomlineend;
						} else {
							out << midlineend;
						}
						if (i + 1 < Rows) out << '\n';
					}
					return out;
				}

				template <typename T, size_t Cols, size_t Rows>
				inline T * begin(basic_mat<T, Cols, Rows> &m) { return m.data(); }

				template <typename T, size_t Cols, size_t Rows>
				inline const T * begin(const basic_mat<T, Cols, Rows> &m) { return m.data(); }

				template <typename T, size_t Cols, size_t Rows>
				inline const T * cbegin(const basic_mat<T, Cols, Rows> &m) { return m.data(); }

				template <typename T, size_t Cols, size_t Rows>
				inline T * end(basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }

				template <typename T, size_t Cols, size_t Rows>
				inline const T * end(const basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }

				template <typename T, size_t Cols, size_t Rows>
				inline const T * cend(const basic_mat<T, Cols, Rows> &m) { return begin(m) + Cols * Rows; }

			}

		}

	}
	



	// 
	// core functions
	// 
	// 
	// 
	// 
	// 
	//=================

	namespace detail {

		template <size_t I, typename F, typename ...ArgTs>
		CGRA_CONSTEXPR_FUNCTION auto zip_with_impl_impl(F f, ArgTs &&...args) {
			return f(array_traits<std::decay_t<ArgTs>>::template get<I>(std::forward<ArgTs>(args))...);
		}

		template <typename ResT, typename F, size_t ...Is, typename ...ArgTs>
		CGRA_CONSTEXPR_FUNCTION ResT zip_with_impl(F f, std::index_sequence<Is...>, ArgTs &&...args) {
			return ResT(zip_with_impl_impl<Is>(f, std::forward<ArgTs>(args)...)...);
		}

		template <size_t I, size_t N>
		struct fold_impl {
			template <typename F, typename T1, typename ArgT>
			static CGRA_CONSTEXPR_FUNCTION auto apply(F f, T1 &&t1, ArgT &&v) {
				return fold_impl<I + 1, N>::apply(
					f,
					f(std::forward<T1>(t1), array_traits<std::decay_t<ArgT>>::template get<I>(std::forward<ArgT>(v))),
					std::forward<ArgT>(v)
				);
			}
		};

		template <size_t N>
		struct fold_impl<N, N> {
			template <typename F, typename T1, typename ArgT>
			static CGRA_CONSTEXPR_FUNCTION decltype(auto) apply(F, T1 &&t1, ArgT &&) {
				return std::forward<T1>(t1);
			}
		};

		// fill, but as a repeat_vec
		template <typename VecT, typename T>
		CGRA_CONSTEXPR_FUNCTION auto fill_repeat(const T &);

		template <typename VecT, typename T, typename = void>
		struct fill_impl {
			CGRA_CONSTEXPR_FUNCTION static auto go(const T &t) {
				auto val = fill_repeat<array_value_t<VecT>>(t);
				return repeat_vec<decltype(val), array_size<VecT>::value>(std::move(val));
			}
		};

		template <typename VecT, typename T>
		struct fill_impl<VecT, T, std::enable_if_t<is_array_compatible<VecT, T>::value>> {
			CGRA_CONSTEXPR_FUNCTION static auto go(const T &t) {
				return VecT(t);
			}
		};

		template <typename VecT, typename T>
		CGRA_CONSTEXPR_FUNCTION inline auto fill_repeat(const T &t) {
			return fill_impl<VecT, T>::go(t);
		}

	}

	// metafunction class: convert array-like type to vector
	struct type_to_vec {
		template <typename VecT>
		struct apply {
			using type = basic_vec<detail::copy_type_t<detail::array_value_t<VecT>>, detail::array_size<VecT>::value>;
		};
	};

	// metafunction class: convert array-like type to matrix
	struct type_to_mat {
		template <typename VecT, typename = void>
		struct apply {
			static_assert(detail::dependent_false<VecT>::value, "type is not convertible to a matrix");
		};

		template <typename VecT>
		struct apply<VecT, detail::void_t<detail::matrix_value_t<VecT>>> {
			using type = basic_mat<detail::copy_type_t<detail::matrix_value_t<VecT>>, detail::mat_cols<VecT>::value, detail::mat_rows<VecT>::value>;
		};
	};

	namespace detail {
		namespace scalars {
			namespace functions {

				// scalar any; returns conversion to bool
				template <typename T, enable_if_want_bool_fns_t<T> = 0>
				inline auto any(const T &x) {
					return bool(x);
				}

				// scalar all; returns conversion to bool
				template <typename T, enable_if_want_bool_fns_t<T> = 0>
				inline auto all(const T &x) {
					return bool(x);
				}

			}
		}

		namespace vectors {
			namespace functions {

				// TODO description
				template <typename TypeMap = type_to_vec, typename F, typename ...ArgTs, typename = enable_if_array_t<ArgTs...>>
				CGRA_CONSTEXPR_FUNCTION auto zip_with(F f, ArgTs &&...args) {
					using value_t = std::decay_t<decltype(f(array_traits<std::decay_t<ArgTs>>::template get<0>(std::forward<ArgTs>(args))...))>;
					using size = array_min_size<ArgTs...>;
					using vec_t = typename TypeMap::template apply<basic_vec<value_t, size::value>>::type;
					using iseq = std::make_index_sequence<size::value>;
					return zip_with_impl<vec_t>(f, iseq(), intellisense_constify(std::forward<ArgTs>(args))...);
				}

				// TODO fix description
				// Produce a scalar by applying f(T1,T2) -> T3 to adjacent pairs of elements
				// from vector a in left-to-right order starting with f(z, v[0])
				// Typically T1 = T3 and T2 is a vector of some T
				template <typename F, typename T1, typename VecT, typename = enable_if_array_t<VecT>>
				CGRA_CONSTEXPR_FUNCTION auto fold(F f, T1 &&t1, VecT &&v) {
					return fold_impl<0, array_size<VecT>::value>::apply(f, std::forward<T1>(t1), std::forward<VecT>(v));
				}

				// fill an array-like type VecT with copies of a (relatively) scalar-like value of type T
				template <typename VecT, typename T>
				CGRA_CONSTEXPR_FUNCTION inline VecT fill(const T &t) {
					// VecT should not be constrained to just arrays: fill<float>() etc should work
					return VecT{fill_repeat<VecT>(t)};
				}

				// sum of all x in v, i.e., v[0] + v[1] + ...
				template <typename VecT, typename = enable_if_array_t<VecT>>
				inline auto sum(const VecT &v) {
					return fold(detail::op::add(), array_value_t<VecT>{}, v);
				}

				// product of all x in v, i.e., v[0] * v[1] * ...
				template <typename VecT, typename = enable_if_array_t<VecT>>
				inline auto product(const VecT &v) {
					return fold(detail::op::mul(), array_value_t<VecT>{1}, v);
				}

				// dot product of v1 and v2, i.e., (v1[0] * v2[0]) + (v1[1] * v2[1]) + ...
				template <typename VecT1, typename VecT2, typename = enable_if_array_t<VecT1, VecT2>>
				inline auto dot(const VecT1 &v1, const VecT2 &v2) {
					auto vprod = zip_with(detail::op::mul(), v1, v2);
					return fold(detail::op::add(), array_value_t<decltype(vprod)>{}, std::move(vprod));
				}

				// true iff any component of v is true; empty => false
				template <typename VecT, enable_if_array_t<VecT> = 0>
				inline auto any(const VecT &v) {
					using cgra::detail::scalars::any;
					return fold([](const auto &x1, const auto &x2) { return any(x1) || any(x2); }, false, v);
				}

				// true iff all components of v are true; empty => true
				template <typename VecT, enable_if_array_t<VecT> = 0>
				inline auto all(const VecT &v) {
					using cgra::detail::scalars::all;
					return fold([](const auto &x1, const auto &x2) { return all(x1) && all(x2); }, true, v);
				}

				// cast array-like to basic_vec<T, N> where T and N are deduced
				template <typename VecT, enable_if_array_t<VecT> = 0>
				inline auto vec_cast(const VecT &v) {
					using result_t = typename type_to_vec::template apply<VecT>::type;
					return result_t{v};
				}

				// cast array-like to basic_vec<T, N> where N is deduced
				template <typename T, typename VecT, enable_if_array_t<VecT> = 0>
				inline auto vec_cast(const VecT &v) {
					using result_t = basic_vec<T, array_size<VecT>::value>;
					return result_t{v};
				}

				// cast array-like to basic_vec<T, N> where T is deduced
				template <size_t N, typename VecT, enable_if_array_t<VecT> = 0>
				inline auto vec_cast(const VecT &v) {
					using result_t = basic_vec<copy_type_t<array_value_t<VecT>>, N>;
					return result_t{v};
				}

			}
		}

		namespace matrices {
			namespace functions {

				// cast array-like to basic_mat<T, Cols, Rows> where T, Cols and Rows are deduced
				template <typename MatT, enable_if_array_t<MatT> = 0>
				inline auto mat_cast(const MatT &m) {
					using result_t = typename type_to_mat::template apply<MatT>::type;
					return result_t{m};
				}

				// cast array-like to basic_mat<T, Cols, Rows> where Cols and Rows are deduced
				template <typename T, typename MatT, enable_if_array_t<MatT> = 0>
				inline auto mat_cast(const MatT &m) {
					using result_t = basic_mat<T, mat_cols<MatT>::value, mat_rows<MatT>::value>;
					return result_t{m};
				}

				// cast array-like to basic_mat<T, Cols, Rows> where T is deduced
				template <size_t Cols, size_t Rows, typename MatT, enable_if_array_t<MatT> = 0>
				inline auto mat_cast(const MatT &m) {
					using result_t = basic_mat<copy_type_t<matrix_value_t<MatT>>, Cols, Rows>;
					return result_t{m};
				}

			}
		}
	}




	//    ______   .______    _______ .______          ___   .___________.  ______   .______           ______   ____    ____  _______ .______       __        ______        ___       _______       _______.  //
	//   /  __  \  |   _  \  |   ____||   _  \        /   \  |           | /  __  \  |   _  \         /  __  \  \   \  /   / |   ____||   _  \     |  |      /  __  \      /   \     |       \     /       |  //
	//  |  |  |  | |  |_)  | |  |__   |  |_)  |      /  ^  \ `---|  |----`|  |  |  | |  |_)  |       |  |  |  |  \   \/   /  |  |__   |  |_)  |    |  |     |  |  |  |    /  ^  \    |  .--.  |   |   (----`  //
	//  |  |  |  | |   ___/  |   __|  |      /      /  /_\  \    |  |     |  |  |  | |      /        |  |  |  |   \      /   |   __|  |      /     |  |     |  |  |  |   /  /_\  \   |  |  |  |    \   \      //
	//  |  `--'  | |  |      |  |____ |  |\  \----./  _____  \   |  |     |  `--'  | |  |\  \----.   |  `--'  |    \    /    |  |____ |  |\  \----.|  `----.|  `--'  |  /  _____  \  |  '--'  |.----)   |     //
	//   \______/  | _|      |_______|| _| `._____/__/     \__\  |__|      \______/  | _| `._____|    \______/      \__/     |_______|| _| `._____||_______| \______/  /__/     \__\ |_______/ |_______/      //
	//                                                                                                                                                                                                        //
	//========================================================================================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				// quat add assign
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline basic_quat<T1> & operator+=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					lhs.w += rhs.w;
					lhs.x += rhs.x;
					lhs.y += rhs.y;
					lhs.z += rhs.z;
					return lhs;
				}

				// quat sub assign
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline basic_quat<T1> & operator-=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					lhs.w -= rhs.w;
					lhs.x -= rhs.x;
					lhs.y -= rhs.y;
					lhs.z -= rhs.z;
					return lhs;
				}

				// quat mul assign
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline basic_quat<T1> & operator*=(basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					return lhs = basic_quat<T1>{lhs * rhs};
				}

				// quat mul assign scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline basic_quat<T1> & operator*=(basic_quat<T1> &lhs, const T2 &rhs) {
					lhs.w *= rhs;
					lhs.x *= rhs;
					lhs.y *= rhs;
					lhs.z *= rhs;
					return lhs;
				}

				// quat div assign scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline basic_quat<T1> & operator/=(basic_quat<T1> &lhs, const T2 &rhs) {
					lhs.w /= rhs;
					lhs.x /= rhs;
					lhs.y /= rhs;
					lhs.z /= rhs;
					return lhs;
				}

				// quat negate
				template <typename T>
				inline auto operator-(const basic_quat<T> &rhs) {
					basic_quat<T> q;
					q.w = -rhs.w;
					q.x = -rhs.x;
					q.y = -rhs.y;
					q.z = -rhs.z;
					return q;
				}

				// quat add
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator+(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					basic_quat<decltype(T1() + T2())> q{lhs};
					return q += rhs;
				}

				// quat sub
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator-(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					basic_quat<decltype(T1() - T2())> q{lhs};
					return q -= rhs;
				}

				// quat mul
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator*(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					basic_quat<decltype(T1() * T2())> q{lhs};
					q.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
					q.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
					q.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
					q.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
					return q;
				}

				// quat mul vec3 (rotate vec3 by quat)
				// TODO allow other vector types
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator*(const basic_quat<T1> &lhs, const basic_vec<T2, 3> &rhs) {
					using common_t = decltype(T1() * T2());
					basic_quat<common_t> p{common_t{}, rhs};
					p = lhs * p * inverse(lhs);
					return basic_vec<common_t, 3>(p.x, p.y, p.z);
				}

				// quat mul right scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator*(const basic_quat<T1> &lhs, const T2 &rhs) {
					basic_quat<decltype(T1() * T2())> q{lhs};
					return q *= rhs;
				}

				// quat mul left scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator*(const T1 &lhs, const basic_quat<T2> &rhs) {
					basic_quat<decltype(T1() * T2())> q{rhs};
					return q *= lhs;
				}

				// quat div right scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator/(const basic_quat<T1> &lhs, const T2 &rhs) {
					basic_quat<decltype(T1() * T2())> q{lhs};
					return q /= rhs;
				}

				// quat div left scalar
				template <typename T1, typename T2, enable_if_mutually_convertible_t<T1, T2> = 0>
				inline auto operator/(const T1 &lhs, const basic_quat<T2> &rhs) {
					return lhs * inverse(rhs);
				}

				// quat equal
				template <typename T1, typename T2>
				inline auto operator==(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					return std::tie(lhs.w, lhs.x, lhs.y, lhs.z) == std::tie(rhs.w, rhs.x, rhs.y, rhs.z);
				}

				// quat not-equal
				template <typename T1, typename T2>
				inline auto operator!=(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					return !(lhs == rhs);
				}

				// quat less-than
				template <typename T1, typename T2>
				inline auto operator<(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					return std::tie(lhs.w, lhs.x, lhs.y, lhs.z) < std::tie(rhs.w, rhs.x, rhs.y, rhs.z);
				}

			}
		}

		namespace vectors {
			namespace functions {

				// vec add_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator+=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::add_assign(), lhs, rhs);
					return lhs;
				}

				// vec add_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator+=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::add_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec sub_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator-=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::sub_assign(), lhs, rhs);
					return lhs;
				}

				// vec sub_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator-=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::sub_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec mul_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator*=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::mul_assign(), lhs, rhs);
					return lhs;
				}

				// vec mul_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator*=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::mul_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec div_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator/=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::div_assign(), lhs, rhs);
					return lhs;
				}

				// vec div_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator/=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::div_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec remainder (mod) assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator%=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::mod_assign(), lhs, rhs);
					return lhs;
				}

				// vec remainder (mod) assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator%=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::mod_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec lshift_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator<<=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::lshift_assign(), lhs, rhs);
					return lhs;
				}

				// vec lshift_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator<<=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::lshift_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec rshift_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator>>=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::rshift_assign(), lhs, rhs);
					return lhs;
				}

				// vec rshift_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator>>=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::rshift_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec bitwise_or_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator|=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::bitwise_or_assign(), lhs, rhs);
					return lhs;
				}

				// vec bitwise_or_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator|=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::bitwise_or_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec bitwise_xor_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator^=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::bitwise_xor_assign(), lhs, rhs);
					return lhs;
				}

				// vec bitwise_xor_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator^=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::bitwise_xor_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec bitwise_and_assign
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline VecT1 & operator&=(VecT1 &lhs, const VecT2 &rhs) {
					zip_with(detail::op::bitwise_and_assign(), lhs, rhs);
					return lhs;
				}

				// vec bitwise_and_assign scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline VecT & operator&=(VecT &lhs, const T &rhs) {
					zip_with(detail::op::bitwise_and_assign(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
					return lhs;
				}

				// vec negate
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto operator-(const VecT &rhs) {
					return zip_with(detail::op::neg(), rhs);
				}

				// vec logical_not
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto operator!(const VecT &rhs) {
					return zip_with(detail::op::logical_not(), rhs);
				}

				// vec bitwise_not
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto operator~(const VecT &rhs) {
					return zip_with(detail::op::bitwise_not(), rhs);
				}

				// vec add
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator+(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::add(), lhs, rhs);
				}

				// vec add right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator+(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::add(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec add left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator+(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::add(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec sub
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator-(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::sub(), lhs, rhs);
				}

				// vec sub right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator-(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::sub(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec sub left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator-(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::sub(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec mul
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator*(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::mul(), lhs, rhs);
				}

				// vec mul right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator*(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::mul(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec mul left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator*(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::mul(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec div
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator/(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::div(), lhs, rhs);
				}

				// vec div right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator/(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::div(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec div left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator/(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::div(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec remainder (mod)
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator%(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::mod(), lhs, rhs);
				}

				// vec remainder (mod) right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator%(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::mod(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec remainder (mod) left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator%(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::mod(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec lshift
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator<<(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::lshift(), lhs, rhs);
				}

				// vec lshift right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator<<(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::lshift(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec lshift left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator<<(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::lshift(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec rshift
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator >> (const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::rshift(), lhs, rhs);
				}

				// vec rshift right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator >> (const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::rshift(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec rshift left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator >> (const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::rshift(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec logical_or
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator||(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::logical_or(), lhs, rhs);
				}

				// vec logical_or right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator||(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::logical_or(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec logical_or left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator||(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::logical_or(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec logical_and
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator&&(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::logical_and(), lhs, rhs);
				}

				// vec logical_and right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator&&(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::logical_and(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec logical_and left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator&&(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::logical_and(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec bitwise_or
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator|(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::bitwise_or(), lhs, rhs);
				}

				// vec bitwise_or right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator|(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::bitwise_or(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec bitwise_or left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator|(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::bitwise_or(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec bitwise_xor
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator^(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::bitwise_xor(), lhs, rhs);
				}

				// vec bitwise_xor right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator^(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::bitwise_xor(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec bitwise_xor left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator^(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::bitwise_xor(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec bitwise_and
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator&(const VecT1 &lhs, const VecT2 &rhs) {
					return zip_with(detail::op::bitwise_and(), lhs, rhs);
				}

				// vec bitwise_and right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator&(const VecT &lhs, const T &rhs) {
					return zip_with(detail::op::bitwise_and(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
				}

				// vec bitwise_and left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto operator&(const T &lhs, const VecT &rhs) {
					return zip_with(detail::op::bitwise_and(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
				}

				// vec equal
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator==(const VecT1 &lhs, const VecT2 &rhs) {
					return fold(detail::op::logical_and(), true, zip_with(detail::op::equal(), lhs, rhs));
				}

				// vec not-equal
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator!=(const VecT1 &lhs, const VecT2 &rhs) {
					return fold(detail::op::logical_or(), false, zip_with(detail::op::not_equal(), lhs, rhs));
				}

				// vec less-than
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto operator<(const VecT1 &lhs, const VecT2 &rhs) {
					// TODO nicely
					return reinterpret_cast<const std::array<array_value_t<VecT1>, array_size<VecT1>::value> &>(lhs)
						< reinterpret_cast<const std::array<array_value_t<VecT2>, array_size<VecT2>::value> &>(rhs);
				}

			}
		}

		namespace matrices {
			namespace functions {

				// mat add_assign
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline MatT1 & operator+=(MatT1 &lhs, const MatT2 &rhs) {
					zip_with(detail::op::add_assign(), lhs, rhs);
					return lhs;
				}

				// mat add_assign scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline MatT & operator+=(MatT &lhs, const T &rhs) {
					zip_with(detail::op::add_assign(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
					return lhs;
				}

				// mat sub_assign
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline MatT1 & operator-=(MatT1 &lhs, const MatT2 &rhs) {
					zip_with(detail::op::sub_assign(), lhs, rhs);
					return lhs;
				}

				// mat sub_assign scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline MatT & operator-=(MatT &lhs, const T &rhs) {
					zip_with(detail::op::sub_assign(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
					return lhs;
				}

				// mat mul_assign
				template <typename MatT1, typename MatT2, enable_if_matrix_mul_compatible_t<MatT1, MatT2> = 0>
				inline MatT1 & operator*=(MatT1 &lhs, const MatT2 &rhs) {
					return lhs = lhs * rhs;
				}

				// mat mul_assign scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline MatT & operator*=(MatT &lhs, const T &rhs) {
					zip_with(detail::op::mul_assign(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
					return lhs;
				}

				// mat div_assign scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline MatT & operator/=(MatT &lhs, const T &rhs) {
					zip_with(detail::op::div_assign(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
					return lhs;
				}

				// mat negate
				template <typename MatT, enable_if_matrix_t<MatT> = 0>
				inline auto operator-(const MatT &rhs) {
					return zip_with<type_to_mat>(detail::op::neg(), rhs);
				}

				// mat add
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto operator+(const MatT1 &lhs, const MatT2 &rhs) {
					return zip_with<type_to_mat>(detail::op::add(), lhs, rhs);
				}

				// mat add right scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator+(const MatT &lhs, const T &rhs) {
					return zip_with<type_to_mat>(detail::op::add(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
				}

				// mat add left scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator+(const T &lhs, const MatT &rhs) {
					return zip_with<type_to_mat>(detail::op::add(), repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(lhs), rhs);
				}

				// mat sub
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto operator-(const MatT1 &lhs, const MatT2 &rhs) {
					return zip_with<type_to_mat>(detail::op::sub(), lhs, rhs);
				}

				// mat sub right scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator-(const MatT &lhs, const T &rhs) {
					return zip_with<type_to_mat>(detail::op::sub(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
				}

				// mat sub left scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator-(const T &lhs, const MatT &rhs) {
					return zip_with<type_to_mat>(detail::op::sub(), repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(lhs), rhs);
				}

				// mat mul
				template <typename MatT1, typename MatT2, enable_if_matrix_mul_compatible_t<MatT1, MatT2> = 0>
				inline auto operator*(const MatT1 &lhs, const MatT2 &rhs) {
					return zip_with<type_to_mat>([&](auto &rcol) { return dot(lhs, rcol); }, rhs);
				}

				// mat mul right scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator*(const MatT &lhs, const T &rhs) {
					return zip_with<type_to_mat>(detail::op::mul(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
				}

				// mat mul left scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator*(const T &lhs, const MatT &rhs) {
					return zip_with<type_to_mat>(detail::op::mul(), repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(lhs), rhs);
				}

				// mat div right scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator/(const MatT &lhs, const T &rhs) {
					return zip_with<type_to_mat>(detail::op::div(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
				}

				// mat div left scalar
				template <typename MatT, typename T, enable_if_matrix_scalar_compatible_t<MatT, T> = 0>
				inline auto operator/(const T &lhs, const MatT &rhs) {
					return lhs * inverse(rhs);
				}

				// mat mul right vec
				template <typename MatT, typename VecT, enable_if_matrix_mul_col_compatible_t<MatT, VecT> = 0>
				inline auto operator*(const MatT &lhs, const VecT &rhs) {
					return dot(lhs, rhs);
				}

				// mat mul left vec
				template <typename MatT, typename VecT, enable_if_matrix_mul_row_compatible_t<MatT, VecT> = 0>
				inline auto operator*(const VecT &lhs, const MatT &rhs) {
					return zip_with([&](auto &rcol) { return dot(lhs, rcol); }, rhs);
				}
				
				// mat equal
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto operator==(const MatT1 &lhs, const MatT2 &rhs) {
					return fold(detail::op::logical_and(), true, zip_with(detail::op::equal(), lhs, rhs));
				}

				// mat not-equal
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto operator!=(const MatT1 &lhs, const MatT2 &rhs) {
					return fold(detail::op::logical_or(), false, zip_with(detail::op::not_equal(), lhs, rhs));
				}

				// mat less-than
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto operator<(const MatT1 &lhs, const MatT2 &rhs) {
					return lhs.as_vec() < rhs.as_vec();
				}

			}
		}
	}




	// .___________..______       __    _______     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	// |           ||   _  \     |  |  /  _____|   |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	// `---|  |----`|  |_)  |    |  | |  |  __     |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//     |  |     |      /     |  | |  | |_ |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//     |  |     |  |\  \----.|  | |  |__| |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//     |__|     | _| `._____||__|  \______|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                          //
	//==========================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				using std::sin;
				using std::cos;
				using std::tan;
				using std::asin;
				using std::acos;
				using std::atan;
				using std::sinh;
				using std::cosh;
				using std::tanh;
				using std::asinh;
				using std::acosh;
				using std::atanh;

				// secant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto sec(const T &x) {
					return T(1) / cos(x);
				}

				// cosecant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto csc(const T &x) {
					return T(1) / sin(x);
				}

				// cotangent
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto cot(const T &x) {
					return cos(x) / sin(x);
				}

				// inverse secant 
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto asec(const T &x) {
					return acos(T(1) / fpromote_t<T>(x));
				}

				// inverse cosecant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto acsc(const T &x) {
					return asin(T(1) / fpromote_t<T>(x));
				}

				// inverse cotangent
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto acot(const T &x) {
					return atan(T(1) / fpromote_t<T>(x));
				}

				// hyperbolic secant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto sech(const T &x) {
					return T(1) / cosh(x);
				}

				// hyperbolic cosecant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto csch(const T &x) {
					return T(1) / sinh(x);
				}

				// hyperbolic cotangent
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto coth(const T &x) {
					return cosh(x) / sinh(x);
				}

				// inverse hyperbolic secant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto asech(const T &x) {
					return acosh(T(1) / fpromote_t<T>(x));
				}

				// inverse hyperbolic cosecant
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto acsch(const T &x) {
					return asinh(T(1) / fpromote_t<T>(x));
				}

				// inverse hyperbolic cotangent
				template <typename T, enable_if_want_trig_fns_t<T> = 0>
				inline auto acoth(const T &x) {
					return atanh(T(1) / fpromote_t<T>(x));
				}

				// Converts degrees to radians, i.e., x * pi/180
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline auto radians(const T &x) {
					return x * fpromote_t<T>(pi / 180.0);
				}

				// Converts radians to degrees, i.e., x * 180/pi
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline auto degrees(const T &x) {
					return x * fpromote_t<T>(180.0 / pi);
				}

				// Arc tangent. Returns an angle whose tangent is y/x
				// The signs of x and y are used to determine what quadrant the angle is in
				// The range of values returned by this function is [−pi, pi]
				// Results are undefined if x and y are both 0
				template <typename Ty, typename Tx, enable_if_arithmetic_t<Ty, Tx> = 0>
				inline auto atan(const Ty &y, const Tx &x) {
					return std::atan2(y, x);
				}

			}
		}

		namespace vectors {

			// TODO angle() specializations for N={0,1}
			template <size_t N>
			struct angle_impl {
				template <typename VecT1, typename VecT2>
				static auto go(const VecT1 &v1, const VecT2 &v2) {
					using cgra::detail::scalars::acos;
					return acos(dot(v1, v2) / (length(v1) * length(v2)));
				}
			};

			template <>
			struct angle_impl<2> {
				template <typename VecT1, typename VecT2>
				static auto go(const VecT1 &v1, const VecT2 &v2) {
					// promote to vec3
					return angle_impl<3>::go(vec_cast<3>(v1), vec_cast<3>(v2));
				}
			};

			template <>
			struct angle_impl<3> {
				template <typename VecT1, typename VecT2>
				static auto go(const VecT1 &v1, const VecT2 &v2) {
					// use asin and |v1 x v2| for vec3 (its a lot more precise for small angles)
					using cgra::detail::scalars::asin;
					return asin(length(cross(v1, v2)) / (length(v1) * length(v2)));
				}
			};

			namespace functions {

				// angle between 2 vectors (radians)
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto angle(const VecT1 &v1, const VecT2 &v2) {
					return angle_impl<array_size<VecT1>::value>::go(v1, v2);
				}

				// vec sin
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sin(const VecT &v) {
					using cgra::detail::scalars::sin;
					return zip_with([](const auto &x) { return sin(x); }, v);
				}

				// vec cosine
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto cos(const VecT &v) {
					using cgra::detail::scalars::cos;
					return zip_with([](const auto &x) { return cos(x); }, v);
				}

				// vec tangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto tan(const VecT &v) {
					using cgra::detail::scalars::tan;
					return zip_with([](const auto &x) { return tan(x); }, v);
				}

				// vec secant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sec(const VecT &v) {
					using cgra::detail::scalars::sec;
					return zip_with([](const auto &x) { return sec(x); }, v);
				}

				// vec cosecant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto csc(const VecT &v) {
					using cgra::detail::scalars::csc;
					return zip_with([](const auto &x) { return csc(x); }, v);
				}

				// vec cotangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto cot(const VecT &v) {
					using cgra::detail::scalars::cot;
					return zip_with([](const auto &x) { return cot(x); }, v);
				}

				// vec inverse sin
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto asin(const VecT &v) {
					using cgra::detail::scalars::asin;
					return zip_with([](const auto &x) { return asin(x); }, v);
				}

				// vec inverse cosine
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acos(const VecT &v) {
					using cgra::detail::scalars::acos;
					return zip_with([](const auto &x) { return acos(x); }, v);
				}

				// vec inverse tangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto atan(const VecT &v) {
					using cgra::detail::scalars::atan;
					return zip_with([](const auto &x) { return atan(x); }, v);
				}

				// vec inverse secant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto asec(const VecT &v) {
					using cgra::detail::scalars::asec;
					return zip_with([](const auto &x) { return asec(x); }, v);
				}

				// vec inverse cosecant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acsc(const VecT &v) {
					using cgra::detail::scalars::acsc;
					return zip_with([](const auto &x) { return acsc(x); }, v);
				}

				// vec inverse cotangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acot(const VecT &v) {
					using cgra::detail::scalars::acot;
					return zip_with([](const auto &x) { return acot(x); }, v);
				}

				// vec hyperbolic sin
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sinh(const VecT &v) {
					using cgra::detail::scalars::sinh;
					return zip_with([](const auto &x) { return sinh(x); }, v);
				}

				// vec hyperbolic cosine
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto cosh(const VecT &v) {
					using cgra::detail::scalars::cosh;
					return zip_with([](const auto &x) { return cosh(x); }, v);
				}

				// vec hyperbolic tangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto tanh(const VecT &v) {
					using cgra::detail::scalars::tanh;
					return zip_with([](const auto &x) { return tanh(x); }, v);
				}

				// vec hyperbolic secant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sech(const VecT &v) {
					using cgra::detail::scalars::sech;
					return zip_with([](const auto &x) { return sech(x); }, v);
				}

				// vec hyperbolic cosecant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto csch(const VecT &v) {
					using cgra::detail::scalars::csch;
					return zip_with([](const auto &x) { return csch(x); }, v);
				}

				// vec hyperbolic cotangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto coth(const VecT &v) {
					using cgra::detail::scalars::coth;
					return zip_with([](const auto &x) { return coth(x); }, v);
				}

				// vec inverse hyperbolic sin
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto asinh(const VecT &v) {
					using cgra::detail::scalars::asinh;
					return zip_with([](const auto &x) { return asinh(x); }, v);
				}

				// vec inverse hyperbolic cosine
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acosh(const VecT &v) {
					using cgra::detail::scalars::acosh;
					return zip_with([](const auto &x) { return acosh(x); }, v);
				}

				// vec inverse hyperbolic tangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto atanh(const VecT &v) {
					using cgra::detail::scalars::atanh;
					return zip_with([](const auto &x) { return atanh(x); }, v);
				}

				// vec inverse hyperbolic secant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto asech(const VecT &v) {
					using cgra::detail::scalars::asech;
					return zip_with([](const auto &x) { return asech(x); }, v);
				}

				// vec inverse hyperbolic cosecant
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acsch(const VecT &v) {
					using cgra::detail::scalars::acsch;
					return zip_with([](const auto &x) { return acsch(x); }, v);
				}

				// vec inverse hyperbolic cotangent
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto acoth(const VecT &v) {
					using cgra::detail::scalars::acoth;
					return zip_with([](const auto &x) { return acoth(x); }, v);
				}

				// vec degrees to radians
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto radians(const VecT &v) {
					using cgra::detail::scalars::radians;
					return zip_with([](const auto &x) { return radians(x); }, v);
				}

				// vec radians to degrees
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto degrees(const VecT &v) {
					using cgra::detail::scalars::degrees;
					return zip_with([](const auto &x) { return degrees(x); }, v);
				}

				// vec inverse tangent (2-arg)
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto atan(const VecT1 &vy, const VecT2 &vx) {
					using cgra::detail::scalars::atan;
					return zip_with([](const auto &xy, const auto &xx) { return atan(xy, xx); }, vy, vx);
				}

				// vec inverse tangent (2-arg) right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto atan(const VecT &vy, const T &x) {
					using cgra::detail::scalars::atan;
					return zip_with([&](const auto &xy) { return atan(xy, x); }, vy);
				}

				// vec inverse tangent (2-arg) left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto atan(const T &y, const VecT &vx) {
					using cgra::detail::scalars::atan;
					return zip_with([&](const auto &xx) { return atan(y, xx); }, vx);
				}

			}
		}
	}
	



	//   __________   ___ .______     ______   .__   __.  _______ .__   __. .___________. __       ___       __          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   ____\  \ /  / |   _  \   /  __  \  |  \ |  | |   ____||  \ |  | |           ||  |     /   \     |  |        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |__   \  V  /  |  |_)  | |  |  |  | |   \|  | |  |__   |   \|  | `---|  |----`|  |    /  ^  \    |  |        |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |   __|   >   <   |   ___/  |  |  |  | |  . `  | |   __|  |  . `  |     |  |     |  |   /  /_\  \   |  |        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |____ /  .  \  |  |      |  `--'  | |  |\   | |  |____ |  |\   |     |  |     |  |  /  _____  \  |  `----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  |_______/__/ \__\ | _|       \______/  |__| \__| |_______||__| \__|     |__|     |__| /__/     \__\ |_______|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                               //
	//===============================================================================================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				using std::exp;
				using std::exp2;
				using std::expm1;
				using std::log;
				using std::log2;
				using std::log10;
				using std::log1p;
				using std::pow;
				using std::sqrt;
				using std::cbrt;
				using std::hypot;

			}
		}

		namespace vectors {
			namespace functions {
				
				// TODO vector hypot

				// vec exp
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto exp(const VecT &v) {
					using cgra::detail::scalars::exp;
					return zip_with([](const auto &x) { return exp(x); }, v);
				}

				// vec exp base 2
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto exp2(const VecT &v) {
					using cgra::detail::scalars::exp2;
					return zip_with([](const auto &x) { return exp2(x); }, v);
				}

				// vec exp(x) - 1
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto expm1(const VecT &v) {
					using cgra::detail::scalars::expm1;
					return zip_with([](const auto &x) { return expm1(x); }, v);
				}

				// vec log
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto log(const VecT &v) {
					using cgra::detail::scalars::log;
					return zip_with([](const auto &x) { return log(x); }, v);
				}

				// vec log2
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto log2(const VecT &v) {
					using cgra::detail::scalars::log2;
					return zip_with([](const auto &x) { return log2(x); }, v);
				}

				// vec log10
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto log10(const VecT &v) {
					using cgra::detail::scalars::log10;
					return zip_with([](const auto &x) { return log10(x); }, v);
				}

				// vec log(1 + x)
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto log1p(const VecT &v) {
					using cgra::detail::scalars::log1p;
					return zip_with([](const auto &x) { return log1p(x); }, v);
				}

				// vec sqrt
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sqrt(const VecT &v) {
					using cgra::detail::scalars::sqrt;
					return zip_with([](const auto &x) { return sqrt(x); }, v);
				}

				// vec cbrt
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto cbrt(const VecT &v) {
					using cgra::detail::scalars::cbrt;
					return zip_with([](const auto &x) { return cbrt(x); }, v);
				}

				// vec pow
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto pow(const VecT1 &vx, const VecT2 &va) {
					using cgra::detail::scalars::pow;
					return zip_with([](const auto &xx, const auto &xa) { return pow(xx, xa); }, vx, va);
				}

				// vec pow right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto pow(const VecT &vx, const T &a) {
					using cgra::detail::scalars::pow;
					return zip_with([&](const auto &xx) { return pow(xx, a); }, vx);
				}

				// vec pow left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto pow(const T &x, const VecT &va) {
					using cgra::detail::scalars::pow;
					return zip_with([&](const auto &xa) { return pow(x, xa); }, va);
				}

			}
		}
	}

	


	//    ______   ______   .___  ___. .___  ___.   ______   .__   __.     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//   /      | /  __  \  |   \/   | |   \/   |  /  __  \  |  \ |  |    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  ,----'|  |  |  | |  \  /  | |  \  /  | |  |  |  | |   \|  |    |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |     |  |  |  | |  |\/|  | |  |\/|  | |  |  |  | |  . `  |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  `----.|  `--'  | |  |  |  | |  |  |  | |  `--'  | |  |\   |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//   \______| \______/  |__|  |__| |__|  |__|  \______/  |__| \__|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                 //
	//=================================================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				using std::abs;
				using std::floor;
				using std::ceil;
				using std::min;
				using std::max;
				using std::isnan;
				using std::isinf;

				// eg: inf<float>()
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline T inf() {
					static_assert(std::numeric_limits<T>::has_infinity, "type has no infinity");
					return std::numeric_limits<T>::infinity();
				}

				// eg: nan<float>()
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline T nan() {
					static_assert(std::numeric_limits<T>::has_quiet_NaN, "type has no NaN");
					return std::numeric_limits<T>::quiet_NaN();
				}

				// eg: epsilon<float>()
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline T epsilon() {
					return std::numeric_limits<T>::epsilon();
				}

				// Returns 1 if x > 0, 0 if x = 0, or –1 if x < 0
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline int sign(const T &x) {
					return (T(0) < x) - (x < T(0));
				}

				// Fractional component of x. Returns x–floor(x)
				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline auto fract(const T &x) {
					return x - floor(x);
				}

				// Modulus. Returns x-m*floor(x/m)
				// note that this has different behaviour on negatives than std::fmod
				template <typename Tx, typename Tm, enable_if_want_real_fns_t<Tx, Tm> = 0>
				inline auto mod(const Tx &x, const Tm &m) {
					// on integers, promote to fp; consistent with std::fmod etc
					return x - m * floor(x / fpromote_t<Tm>(m));
				}

				// Linear blend of x1 and x2, i.e., x1*(1−t) + x2*t
				template <typename Tx1, typename Tx2, typename Tt, enable_if_want_linear_fns_t<Tx1, Tx2, Tt> = 0>
				inline auto mix(const Tx1 &x1, const Tx2 &x2, const Tt &t) {
					return x1 * (fpromote_t<Tt>(1) - t) + x2 * t;
				}

				// Boolean 'blend' of x1 and x2, i.e., t ? x2 : x1
				// behaves nicely even if x1 or x2 is nan/inf
				template <typename Tx1, typename Tx2, enable_if_want_linear_fns_t<Tx1, Tx2> = 0>
				inline auto mix(const Tx1 &x1, const Tx2 &x2, bool t) {
					return t ? x2 : x1;
				}

				// heterogeneous version of std::min
				template <typename T1, typename T2, enable_if_want_real_fns_t<T1, T2> = 0>
				inline auto min(const T1 &x1, const T2 &x2) {
					// should not fpromote here
					using value_t = decltype(x1 + x2);
					return min(value_t(x1), value_t(x2));
				}

				// heterogeneous version of std::max
				template <typename T1, typename T2, enable_if_want_real_fns_t<T1, T2> = 0>
				inline auto max(const T1 &x1, const T2 &x2) {
					// should not fpromote here
					using value_t = decltype(x1 + x2);
					return max(value_t(x1), value_t(x2));
				}

				// Returns min(max(x, lower), upper)
				template <typename Tx, typename Tl, typename Tu, enable_if_want_real_fns_t<Tx, Tl, Tu> = 0>
				inline auto clamp(const Tx &x, const Tl &lower, const Tu &upper) {
					// should not fpromote here
					using value_t = decltype(x + lower + upper);
					return min(max(value_t(x), value_t(lower)), value_t(upper));
				}

				// Returns 0 if x < edge, 1 otherwise
				template <typename Te, typename Tx, enable_if_want_real_fns_t<Te, Tx> = 0>
				inline auto step(const Te &edge, const Tx &x) {
					// why does glsl have the args in this order?
					using value_t = fpromote_arith_t<Te, Tx>;
					return (x < edge) ? value_t(0) : value_t(1);
				}

				// Returns 0 if x <= edge0 and 1 if x >= edge1 and performs smooth
				// Hermite interpolation between 0 and 1 when edge0 < x < edge1
				// This is useful in cases where you would want a threshold
				// function with a smooth transition. Results are undefined if edge0 >= edge1.
				template <typename Te0, typename Te1, typename Tx, enable_if_want_real_fns_t<Te0, Te1, Tx> = 0>
				inline auto smoothstep(const Te0 &edge0, const Te1 &edge1, const Tx &x) {
					using value_t = fpromote_arith_t<Te0, Te1, Tx>;
					auto t = clamp((x - value_t(edge0)) / (edge1 - value_t(edge0)), value_t(0), value_t(1));
					return t * t * (value_t(3) - value_t(2) * t);
				}

			}
		}

		namespace vectors {
			namespace functions {

				// vec inf
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto inf() {
					using cgra::detail::scalars::inf;
					return copy_type_t<VecT>{inf<array_value_t<VecT>>()};
				}

				// vec nan
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto nan() {
					using cgra::detail::scalars::nan;
					return copy_type_t<VecT>{nan<array_value_t<VecT>>()};
				}

				// vec epsilon
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto epsilon() {
					using cgra::detail::scalars::epsilon;
					return copy_type_t<VecT>{epsilon<array_value_t<VecT>>()};
				}

				// vec abs
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto abs(const VecT &v) {
					using cgra::detail::scalars::abs;
					return zip_with([](const auto &x) { return abs(x); }, v);
				}

				// vec floor
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto floor(const VecT &v) {
					using cgra::detail::scalars::floor;
					return zip_with([](const auto &x) { return floor(x); }, v);
				}

				// vec ceil
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto ceil(const VecT &v) {
					using cgra::detail::scalars::ceil;
					return zip_with([](const auto &x) { return ceil(x); }, v);
				}

				// vec isnan
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto isnan(const VecT &v) {
					using cgra::detail::scalars::isnan;
					return zip_with([](const auto &x) { return isnan(x); }, v);
				}

				// vec isinf
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto isinf(const VecT &v) {
					using cgra::detail::scalars::isinf;
					return zip_with([](const auto &x) { return isinf(x); }, v);
				}

				// vec sign
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto sign(const VecT &v) {
					using cgra::detail::scalars::sign;
					return zip_with([](const auto &x) { return sign(x); }, v);
				}

				// vec fract
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto fract(const VecT &v) {
					using cgra::detail::scalars::fract;
					return zip_with([](const auto &x) { return fract(x); }, v);
				}

				// vec mod
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto mod(const VecT1 &vx, const VecT2 &vm) {
					using cgra::detail::scalars::mod;
					return zip_with([](const auto &xx, const auto &xm) { return mod(xx, xm); }, vx, vm);
				}

				// vec mod right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto mod(const VecT &vx, const T &m) {
					using cgra::detail::scalars::mod;
					return zip_with([&](const auto &xx) { return mod(xx, m); }, vx);
				}

				// vec mod left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto mod(const T &x, const VecT &vm) {
					using cgra::detail::scalars::mod;
					return zip_with([&](const auto &xm) { return mod(x, xm); }, vm);
				}

				// vec mix
				template <typename VecT1, typename VecT2, typename VecTt, enable_if_vector_compatible_t<VecT1, VecT2, VecTt> = 0>
				inline auto mix(const VecT1 &v1, const VecT2 &v2, const VecTt &vt) {
					using cgra::detail::scalars::mix;
					return zip_with([](const auto &x1, const auto &x2, const auto &xt) { return mix(x1, x2, xt); }, v1, v2, vt);
				}

				// vec mix (t)-scalar
				template <typename VecT1, typename VecT2, typename Tt, enable_if_vector_compatible_t<VecT1, VecT2> = 0, enable_if_vector_scalar_compatible_t<VecT1, Tt> = 0>
				inline auto mix(const VecT1 &v1, const VecT2 &v2, const Tt &t) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &x1, const auto &x2) { return mix(x1, x2, t); }, v1, v2);
				}

				// vec mix (right)-scalar
				template <typename VecT1, typename T2, typename VecTt, enable_if_vector_compatible_t<VecT1, VecTt> = 0, enable_if_vector_scalar_compatible_t<VecT1, T2> = 0>
				inline auto mix(const VecT1 &v1, const T2 &x2, const VecTt &vt) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &x1, const auto &xt) { return mix(x1, x2, xt); }, v1, vt);
				}

				// vec mix (right,t)-scalar
				template <typename VecT1, typename T2, typename Tt, enable_if_vector_scalar_compatible_t<VecT1, T2> = 0, enable_if_vector_scalar_compatible_t<VecT1, Tt> = 0>
				inline auto mix(const VecT1 &v1, const T2 &x2, const Tt &t) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &x1) { return mix(x1, x2, t); }, v1);
				}

				// vec mix (left)-scalar
				template <typename T1, typename VecT2, typename VecTt, enable_if_vector_compatible_t<VecT2, VecTt> = 0, enable_if_vector_scalar_compatible_t<VecT2, T1> = 0>
				inline auto mix(const T1 &x1, const VecT2 &v2, const VecTt &vt) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &x2, const auto &xt) { return mix(x1, x2, xt); }, v2, vt);
				}

				// vec mix (left,t)-scalar
				template <typename T1, typename VecT2, typename Tt, enable_if_vector_scalar_compatible_t<VecT2, T1> = 0, enable_if_vector_scalar_compatible_t<VecT2, Tt> = 0>
				inline auto mix(const T1 &x1, const VecT2 &v2, const Tt &t) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &x2) { return mix(x1, x2, t); }, v2);
				}

				// vec mix (left,right)-scalar
				template <typename T1, typename T2, typename VecTt, enable_if_vector_scalar_compatible_t<VecTt, T1> = 0, enable_if_vector_scalar_compatible_t<VecTt, T2> = 0>
				inline auto mix(const T1 &x1, const T2 &x2, const VecTt &vt) {
					using cgra::detail::scalars::mix;
					return zip_with([&](const auto &xt) { return mix(x1, x2, xt); }, vt);
				}

				// vec clamp
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto clamp(const VecT1 &vx, const VecT2 &vlower) {
					using cgra::detail::scalars::clamp;
					return zip_with([](const auto &xx, const auto &xlower) { return clamp(xx, xlower); }, vx, vlower);
				}

				// vec clamp right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto clamp(const VecT &vx, const T &lower) {
					using cgra::detail::scalars::clamp;
					return zip_with([&](const auto &xx) { return clamp(xx, lower); }, vx);
				}

				// vec clamp left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto clamp(const T &x, const VecT &vlower) {
					using cgra::detail::scalars::clamp;
					return zip_with([&](const auto &xlower) { return clamp(x, xlower); }, vlower);
				}

				// vec step
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto step(const VecT1 &vedge, const VecT2 &vx) {
					using cgra::detail::scalars::step;
					return zip_with([](const auto &xedge, const auto &xx) { return step(xedge, xx); }, vedge, vx);
				}

				// vec step right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto step(const VecT &vedge, const T &x) {
					using cgra::detail::scalars::step;
					return zip_with([&](const auto &xedge) { return step(xedge, x); }, vedge);
				}

				// vec step left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto step(const T &edge, const VecT &vx) {
					using cgra::detail::scalars::step;
					return zip_with([&](const auto &xx) { return step(edge, xx); }, vx);
				}

				// vec smoothstep
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto smoothstep(const VecT1 &vedge, const VecT2 &vx) {
					using cgra::detail::scalars::smoothstep;
					return zip_with([](const auto &xedge, const auto &xx) { return smoothstep(xedge, xx); }, vedge, vx);
				}

				// vec smoothstep right scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto smoothstep(const VecT &vedge, const T &x) {
					using cgra::detail::scalars::smoothstep;
					return zip_with([&](const auto &xedge) { return smoothstep(xedge, x); }, vedge);
				}

				// vec smoothstep left scalar
				template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
				inline auto smoothstep(const T &edge, const VecT &vx) {
					using cgra::detail::scalars::smoothstep;
					return zip_with([&](const auto &xx) { return smoothstep(edge, xx); }, vx);
				}

				// vec element-wise min
				template <typename T1, size_t N, typename T2, enable_if_vector_scalar_compatible_t<basic_vec<T1, N>, T2> = 0>
				inline auto min(const basic_vec<T1, N> &vx1, const T2 &x2) {
					using cgra::detail::scalars::min;
					return zip_with([&](const auto &x1) { return min(x1, x2); }, vx1);
				}

				// vec element-wise min
				template <typename T1, typename T2, size_t N>
				inline auto min(const basic_vec<T1, N> &vx1, const basic_vec<T2, N> &vx2) {
					using cgra::detail::scalars::min;
					return zip_with([](const auto &x1, const auto &x2) { return min(x1, x2); }, vx1, vx2);
				}

				// vec element-wise min
				template <typename T, size_t N>
				inline auto min(const basic_vec<T, N> &vx1, const basic_vec<T, N> &vx2) {
					using cgra::detail::scalars::min;
					return zip_with([](const auto &x1, const auto &x2) { return min(x1, x2); }, vx1, vx2);
				}

				// vec element-wise max
				template <typename T1, size_t N, typename T2, enable_if_vector_scalar_compatible_t<basic_vec<T1, N>, T2> = 0>
				inline auto max(const basic_vec<T1, N> &vx1, const T2 &x2) {
					using cgra::detail::scalars::max;
					return zip_with([&](const auto &x1) { return max(x1, x2); }, vx1);
				}

				// vec element-wise max
				template <typename T1, typename T2, size_t N>
				inline auto max(const basic_vec<T1, N> &vx1, const basic_vec<T2, N> &vx2) {
					using cgra::detail::scalars::max;
					return zip_with([](const auto &x1, const auto &x2) { return max(x1, x2); }, vx1, vx2);
				}

				// vec element-wise max
				template <typename T, size_t N>
				inline auto max(const basic_vec<T, N> &vx1, const basic_vec<T, N> &vx2) {
					using cgra::detail::scalars::max;
					return zip_with([](const auto &x1, const auto &x2) { return max(x1, x2); }, vx1, vx2);
				}

			}
		}
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

	// Element-wise function for x in v1, y in v2 and a in va
	// Selects which vector each returned component comes from
	// For a component of a that is false, the corresponding component of x is returned
	// For a component of a that is true, the corresponding component of y is returned.
	// Components of x and y that are not selected are allowed to be invalid floating-point
	// values and will have no effect on the results
	// Thus, this provides different functionality than, for example, 
	//     vecT mix(vecT x, vecT y, vecT(a))
	// where a is a Boolean vector.
	
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
	
	namespace detail {
		namespace vectors {

			template <size_t N>
			struct cross_impl {
				template <typename VecT1, typename VecT2>
				static void go(const VecT1 &v1, const VecT2 &v2) {
					static_assert(dependent_false<VecT1, VecT2>::value, "cross product does not exist");
				}
			};

			template <>
			struct cross_impl<3> {
				template <typename VecT1, typename VecT2>
				static auto go(const VecT1 &v1, const VecT2 &v2) {
					return basic_vec<decltype(v1[0] * v2[0]), 3>(
						v1[1] * v2[2] - v1[2] * v2[1],
						v1[2] * v2[0] - v1[0] * v2[2],
						v1[0] * v2[1] - v1[1] * v2[0]
					);
				}
			};

			template <>
			struct cross_impl<7> {
				template <typename VecT1, typename VecT2>
				static void go(const VecT1 &v1, const VecT2 &v2) {
					static_assert(dependent_false<VecT1, VecT2>::value, "7D cross product not implemented (yet?)");
				}
			};

			namespace functions {

				// Returns the length of vector v, i.e. sqrt(v[0]^2 + v[1]^2  + ...)
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto length(const VecT &v) {
					using cgra::detail::scalars::sqrt;
					return sqrt(sum(v * v));
				}

				// Returns the distance between p1 and p2, i.e., length (p1 – p2)
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto distance(const VecT1 &p1, const VecT2 &p2) {
					return length(p1 - p2);
				}

				// Returns a vector in the same direction as v but with a length of 1
				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto normalize(const VecT &v) {
					return v / length(v);
				}

				// If dot(nref, i) < 0 return n, otherwise return -n
				template <typename VecT1, typename VecT2, typename VecT3, enable_if_vector_compatible_t<VecT1, VecT2, VecT3> = 0>
				inline auto faceforward(const VecT1 &n, const VecT2 &i, const VecT3 &nref) {
					return (dot(nref, i) < 0) ? n : -n;
				}

				// For the incident vector i and surface orientation n,
				// returns the reflection direction:
				//     I – 2 * dot(N, I) * N
				// n must already be normalized in order to achieve the desired result.
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto reflect(const VecT1 &i, const VecT2 &n) {
					return i - 2 * dot(n, i) * n;
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
				template <typename VecT1, typename VecT2, typename T3, enable_if_vector_compatible_t<VecT1, VecT2> = 0, enable_if_vector_scalar_compatible_t<VecT1, T3> = 0>
				inline auto refract(const VecT1 &i, const VecT2 &n, const T3 &eta) {
					using cgra::detail::scalars::sqrt;
					auto k = 1 - eta * eta * (1 - dot(n, i) * dot(n, i));
					auto r = eta * i - (eta * dot(n, i) + sqrt(k)) * n;
					return k < 0 ? decltype(r)() : r;
				}

				// vector projection of v onto n
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto project(const VecT1 &v, const VecT2 &n) {
					return n * (dot(v, n) / dot(n, n));
				}

				// vector rejection of v from n
				// projects v onto the plane defined by the normal n
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto reject(const VecT1 &v, const VecT2 &n) {
					return v - project(v, n);
				}

				// vec cross product
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto cross(const VecT1 &v1, const VecT2 &v2) {
					return cross_impl<array_size<VecT1>::value>::go(v1, v2);
				}

			}
		}
	}

	


	//  .______       _______  __          ___   .___________. __    ______   .__   __.      ___       __          _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//  |   _  \     |   ____||  |        /   \  |           ||  |  /  __  \  |  \ |  |     /   \     |  |        |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |_)  |    |  |__   |  |       /  ^  \ `---|  |----`|  | |  |  |  | |   \|  |    /  ^  \    |  |        |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |      /     |   __|  |  |      /  /_\  \    |  |     |  | |  |  |  | |  . `  |   /  /_\  \   |  |        |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  |\  \----.|  |____ |  `----./  _____  \   |  |     |  | |  `--'  | |  |\   |  /  _____  \  |  `----.   |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//  | _| `._____||_______||_______/__/     \__\  |__|     |__|  \______/  |__| \__| /__/     \__\ |_______|   |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                         //
	//=========================================================================================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				template <typename T, enable_if_want_real_fns_t<T> = 0>
				inline auto nearzero(const T &x, const T &tol) {
					return abs(x) < tol;
				}

			}
		}

		namespace vectors {
			namespace functions {

				template <typename VecT, enable_if_vector_t<VecT> = 0>
				inline auto nearzero(const VecT &v, const VecT &vtol) {
					using cgra::detail::scalars::nearzero;
					return zip_with([](const auto &x, const auto &tol) { return nearzero(x, tol); }, v, vtol);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x < y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto less_than(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x <= y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto less_than_equal(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x > y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto greater_than(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x >= y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto greater_than_equal(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x == y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto equal(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Element-wise function for x in v1 and y in v2
				// Returns the comparison of x != y
				template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
				inline auto not_equal(const VecT1 &v1, const VecT2 &v2) {
					return zip_with([](const auto &x1, const auto &x2) { return x1 < x2; }, v1, v2);
				}

				// Note : C++ does not support "not" as a function name so it has been omitted;
				// however, operator! does work for vectors

			}
		}
	}



	
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
		namespace matrices {

			template <typename T>
			inline T det2x2(
				const T &e00, const T &e01,
				const T &e10, const T &e11
			) {
				return e00 * e11 - e10 * e01;
			}

			// this would actually be fine on integers, but we force the type if fpromoting for consistency
			template <typename T>
			inline T det3x3(
				const T &e00, const T &e01, const T &e02,
				const T &e10, const T &e11, const T &e12,
				const T &e20, const T &e21, const T &e22
			) {
				T d{0};
				d += e00 * e11 * e22;
				d += e01 * e12 * e20;
				d += e02 * e10 * e21;
				d -= e00 * e12 * e21;
				d -= e01 * e10 * e22;
				d -= e02 * e11 * e20;
				return d;
			}

			template <size_t Cols, size_t Rows, typename = void>
			struct inverse_impl {
				template <typename MatT>
				static void go(const MatT &m) {
					static_assert(dependent_false<MatT>::value, "only square matrices are invertible");
				}
			};

			template <size_t Cols>
			struct inverse_impl<Cols, Cols> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using std::swap;
					// TODO needs abs, div (quat?)
					using cgra::detail::scalars::abs;
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					auto mtemp = mat_cast<value_t>(m);
					auto mr = decltype(mtemp){1};
					// run column-wise gauss-jordan elimination on mtemp, apply same ops to mr
					size_t col = 0;
					for (size_t r = 0; r < mat_rows<MatT>::value; r++) {
						// swap cols so [col][r] is as large in magnitude as possible
						size_t swcol = col;
						auto swmax = abs(mtemp[col][r]);
						for (size_t j = col + 1; j < Cols; j++) {
							const auto t = abs(mtemp[j][r]);
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
						// TODO better singularity detection
						const auto q = value_t(1) / mtemp[col][r];
						if (all(!isinf(q))) {
							// largest usable abs value was > 0, continue => zero rest of row
							for (size_t j = 0; j < Cols; j++) {
								if (j != col) {
									// && abs(mtemp[j][r]) > 0
									const auto f = mtemp[j][r] * q;
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
					if (Cols != col) throw singular_matrix_error();
					// mr is now the inverse
					return mr;
				}
			};

			template <>
			struct inverse_impl<0, 0> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					return decltype(mat_cast<value_t>(m)){};
				}
			};

			template <>
			struct inverse_impl<1, 1> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					return decltype(mat_cast<value_t>(m)){matrix_value_t<MatT>{1} / m[0][0]};
				}
			};

			template <>
			struct inverse_impl<2, 2> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					auto r = decltype(mat_cast<value_t>(m)){};
					const auto det = determinant(m);
					const auto invdet = value_t(1) / det;
					if (any(isinf(invdet) || isnan(invdet) || isinf(det))) throw singular_matrix_error();
					r[0][0] = m[1][1] * invdet;
					r[0][1] = -m[0][1] * invdet;
					r[1][0] = -m[1][0] * invdet;
					r[1][1] = m[0][0] * invdet;
					return r;
				}
			};

			template <>
			struct inverse_impl<3, 3> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					auto r = decltype(mat_cast<value_t>(m)){};
					// first column of cofactors, can use for determinant
					const auto c00 = det2x2<value_t>(m[1][1], m[1][2], m[2][1], m[2][2]);
					const auto c01 = -det2x2<value_t>(m[1][0], m[1][2], m[2][0], m[2][2]);
					const auto c02 = det2x2<value_t>(m[1][0], m[1][1], m[2][0], m[2][1]);
					// get determinant by expanding about first column
					const auto det = m[0][0] * c00 + m[0][1] * c01 + m[0][2] * c02;
					const auto invdet = value_t(1) / det;
					if (any(isinf(invdet) || isnan(invdet) || isinf(det))) throw singular_matrix_error();
					// transpose of cofactor matrix * (1 / det)
					r[0][0] = c00 * invdet;
					r[1][0] = c01 * invdet;
					r[2][0] = c02 * invdet;
					r[0][1] = -det2x2<value_t>(m[0][1], m[0][2], m[2][1], m[2][2]) * invdet;
					r[1][1] = det2x2<value_t>(m[0][0], m[0][2], m[2][0], m[2][2]) * invdet;
					r[2][1] = -det2x2<value_t>(m[0][0], m[0][1], m[2][0], m[2][1]) * invdet;
					r[0][2] = det2x2<value_t>(m[0][1], m[0][2], m[1][1], m[1][2]) * invdet;
					r[1][2] = -det2x2<value_t>(m[0][0], m[0][2], m[1][0], m[1][2]) * invdet;
					r[2][2] = det2x2<value_t>(m[0][0], m[0][1], m[1][0], m[1][1]) * invdet;
					return r;
				}
			};

			template <>
			struct inverse_impl<4, 4> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					auto r = decltype(mat_cast<value_t>(m)){};
					// first column of cofactors, can use for determinant
					const auto c0 = det3x3<value_t>(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
					const auto c1 = -det3x3<value_t>(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
					const auto c2 = det3x3<value_t>(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
					const auto c3 = -det3x3<value_t>(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
					// get determinant by expanding about first column
					const auto det = m[0][0] * c0 + m[0][1] * c1 + m[0][2] * c2 + m[0][3] * c3;
					const auto invdet = value_t(1) / det;
					if (any(isinf(invdet) || isnan(invdet) || isinf(det))) throw singular_matrix_error();
					// transpose of cofactor matrix * (1 / det)
					r[0][0] = c0 * invdet;
					r[1][0] = c1 * invdet;
					r[2][0] = c2 * invdet;
					r[3][0] = c3 * invdet;
					r[0][1] = -det3x3<value_t>(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]) * invdet;
					r[1][1] = det3x3<value_t>(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]) * invdet;
					r[2][1] = -det3x3<value_t>(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]) * invdet;
					r[3][1] = det3x3<value_t>(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]) * invdet;
					r[0][2] = det3x3<value_t>(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]) * invdet;
					r[1][2] = -det3x3<value_t>(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]) * invdet;
					r[2][2] = det3x3<value_t>(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]) * invdet;
					r[3][2] = -det3x3<value_t>(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]) * invdet;
					r[0][3] = -det3x3<value_t>(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]) * invdet;
					r[1][3] = det3x3<value_t>(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]) * invdet;
					r[2][3] = -det3x3<value_t>(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]) * invdet;
					r[3][3] = det3x3<value_t>(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]) * invdet;
					return r;
				}
			};

			template <size_t Cols, size_t Rows, typename = void>
			struct determinant_impl {
				template <typename MatT>
				static void go(const MatT &) {
					static_assert(dependent_false<MatT>::value, "general-case determinant not implemented (yet?)");
				}
			};

			template <size_t Cols, size_t Rows>
			struct determinant_impl<Cols, Rows, std::enable_if_t<(Cols != Rows)>> {
				template <typename MatT>
				static auto go(const MatT &) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					// determinant of non-square matrix is always 0
					return value_t(0);
				}
			};

			template <>
			struct determinant_impl<0, 0> {
				template <typename MatT>
				static auto go(const MatT &) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					// i'm just gonna say this is 1
					return value_t(1);
				}
			};

			template <>
			struct determinant_impl<1, 1> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					return value_t(m[0][0]);
				}
			};

			template <>
			struct determinant_impl<2, 2> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					return det2x2<value_t>(m[0][0], m[0][1], m[1][0], m[1][1]);
				}
			};

			template <>
			struct determinant_impl<3, 3> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					return det3x3<value_t>(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
				}
			};

			template <>
			struct determinant_impl<4, 4> {
				template <typename MatT>
				static auto go(const MatT &m) {
					using value_t = fpromote_t<matrix_value_t<MatT>>;
					auto d = value_t(0);
					// expand about first column
					d += m[0][0] * det3x3<value_t>(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
					d -= m[0][1] * det3x3<value_t>(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
					d += m[0][2] * det3x3<value_t>(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
					d -= m[0][3] * det3x3<value_t>(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
					return d;
				}
			};

			namespace functions {

				// matrix inverse (error if not invertible)
				template <typename MatT, enable_if_matrix_t<MatT> = 0>
				inline auto inverse(const MatT &m) {
					return inverse_impl<mat_cols<MatT>::value, mat_rows<MatT>::value>::go(m);
				}

				// matrix determinant
				template <typename MatT, enable_if_matrix_t<MatT> = 0>
				inline auto determinant(const MatT &m) {
					return determinant_impl<mat_cols<MatT>::value, mat_rows<MatT>::value>::go(m);
				}

				// matrix transpose
				template <typename MatT, enable_if_matrix_t<MatT> = 0>
				inline auto transpose(const MatT &m) {
					auto r = copy_type_t<MatT>{};
					for (size_t j = 0; j < mat_cols<MatT>::value; ++j) {
						for (size_t i = 0; i < mat_rows<MatT>::value; ++i) {
							r[i][j] = m[j][i];
						}
					}
					return r;
				}

				// matrix component-wise multiplication 
				// see (*) operator overload for matrix product
				template <typename MatT1, typename MatT2, enable_if_matrix_compatible_t<MatT1, MatT2> = 0>
				inline auto matrix_comp_mult(const MatT1 &lhs, const MatT2 &rhs) {
					return zip_with<type_to_mat>(op::mul(), lhs, rhs);
				}

			}
		}

		namespace vectors {
			namespace functions {

				// vector outer product
				// matrix multiplication where lhs is column, rhs is row
				template <typename VecT1, typename VecT2, std::enable_if_t<is_element_compatible<VecT1, VecT2>::value, int> = 0>
				inline auto outer_product(const VecT1 &lhs, const VecT2 &rhs) {
					// TODO this could be constrained better
					return zip_with<type_to_mat>(op::mul(), repeat_vec<const VecT1 &, array_size<VecT2>::value>(lhs), rhs);
				}

			}
		}
	}




	//    ______      __    __       ___   .___________. _______ .______      .__   __.  __    ______   .__   __.     _______  __    __  .__   __.   ______ .___________. __    ______   .__   __.      _______.  //
	//   /  __  \    |  |  |  |     /   \  |           ||   ____||   _  \     |  \ |  | |  |  /  __  \  |  \ |  |    |   ____||  |  |  | |  \ |  |  /      ||           ||  |  /  __  \  |  \ |  |     /       |  //
	//  |  |  |  |   |  |  |  |    /  ^  \ `---|  |----`|  |__   |  |_)  |    |   \|  | |  | |  |  |  | |   \|  |    |  |__   |  |  |  | |   \|  | |  ,----'`---|  |----`|  | |  |  |  | |   \|  |    |   (----`  //
	//  |  |  |  |   |  |  |  |   /  /_\  \    |  |     |   __|  |      /     |  . `  | |  | |  |  |  | |  . `  |    |   __|  |  |  |  | |  . `  | |  |         |  |     |  | |  |  |  | |  . `  |     \   \      //
	//  |  `--'  '--.|  `--'  |  /  _____  \   |  |     |  |____ |  |\  \----.|  |\   | |  | |  `--'  | |  |\   |    |  |     |  `--'  | |  |\   | |  `----.    |  |     |  | |  `--'  | |  |\   | .----)   |     //
	//   \_____\_____\\______/  /__/     \__\  |__|     |_______|| _| `._____||__| \__| |__|  \______/  |__| \__|    |__|      \______/  |__| \__|  \______|    |__|     |__|  \______/  |__| \__| |_______/      //
	//                                                                                                                                                                                                            //
	//============================================================================================================================================================================================================//

	namespace detail {
		namespace scalars {
			namespace functions {

				// quat dot product
				template <typename T1, typename T2>
				inline auto dot(const basic_quat<T1> &lhs, const basic_quat<T2> &rhs) {
					return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
				}

				// quat conjugate (== inverse for unit quat)
				template <typename T>
				inline auto conj(const basic_quat<T> &q) {
					return basic_quat<T>{q.w, -q.x, -q.y, -q.z};
				}

				// quat inverse
				template <typename T>
				inline auto inverse(const basic_quat<T> &q) {
					// yes, this should indeed be the square of the norm
					return conj(q) * (fpromote_t<T>(1) / dot(q, q));
				}

				// quat rotation angle
				template <typename T>
				inline auto angle(const basic_quat<T> &q) {
					return 2 * acos(q.w);
				}

				// quat rotation axis
				template <typename T>
				inline auto axis(const basic_quat<T> &q) {
					return normalize(basic_vec<T, 3>{q.x, q.y, q.z});
				}

				// quat power
				template <typename Tq, typename Tp>
				inline auto pow(const basic_quat<Tq> &q, const Tp &power) {
					using value_t = fpromote_arith_t<Tq, Tp>;
					const auto qm = abs(q);
					const auto theta = acos(q.w / qm);
					const auto ivm = 1 / sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
					if (isinf(ivm)) return basic_quat<value_t>{1} * pow(qm, power);
					const auto ivm_power_theta = ivm * power * theta;
					basic_quat<value_t> p{0, q.x * ivm_power_theta, q.y * ivm_power_theta, q.z * ivm_power_theta};
					return exp(p) * pow(qm, power);
				}

				// quat exp
				template <typename T>
				inline auto exp(const basic_quat<T> &q) {
					using value_t = fpromote_t<T>;
					const auto expw = exp(q.w);
					const auto vm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
					const auto ivm = 1 / vm;
					if (isinf(ivm)) return basic_quat<value_t>{expw * cos(vm)};
					const auto vf = expw * sin(vm) * ivm;
					return basic_quat<value_t>{expw * cos(vm), q.x * vf, q.y * vf, q.z * vf};
				}

				// quat log
				template <typename T>
				inline auto log(const basic_quat<T> &q) {
					using value_t = fpromote_t<T>;
					const auto qm = abs(q);
					const auto ivm = 1 / sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
					if (isinf(ivm)) return basic_quat<value_t>(log(qm), 0, 0, 0);
					const auto vf = acos(q.w / qm) * ivm;
					return basic_quat<value_t>{log(qm), q.x * vf, q.y * vf, q.z * vf};
				}

				// magnitude of quaternion
				template <typename T>
				inline auto abs(const basic_quat<T> &q) {
					return sqrt(dot(q, q));
				}

				// returns unit quaternion
				template <typename T>
				inline auto normalize(const basic_quat<T> &q) {
					return q / abs(q);
				}

				// quat mix is implemented by normal scalar mix

				// quat normalized lerp
				template <typename T1, typename T2, typename Tt>
				inline auto nlerp(const basic_quat<T1> &q1, const basic_quat<T2> &q2, const Tt &t) {
					return normalize(mix(q1, q2, t));
				}

				// quat spherical linear interpolation (slerp)
				template <typename T1, typename T2, typename Tt>
				inline auto slerp(const basic_quat<T1> &q1, const basic_quat<T2> &q2, const Tt &t) {
					using value_t = fpromote_arith_t<T1, T2, Tt>;
					auto q = normalize(q1);
					auto p = normalize(q2);
					value_t epsilon{0.0001};
					if (dot(p, q) < value_t(0)) {
						q = q * value_t{-1};
					}
					const auto dpq = dot(p, q);
					if ((value_t(1) - dpq) > epsilon) {
						const auto w = acos(dpq);
						return ((sin((value_t(1) - t) * w) * p) + (sin(t * w) * q)) / sin(w);
					}
					return (value_t(1) - t) * p + t * q;
				}

				// returns the rotation (in radians) of the quaternion around a given axis
				template <typename Tq, typename Ta>
				inline auto project(const basic_quat<Tq> &q, const basic_vec<Ta, 3> &ax) {
					using value_t = fpromote_arith_t<Tq, Ta>;
					const auto nv = normalize(ax);
					// find a tangent to nv
					basic_vec<value_t, 3> tangent{1, 0, 0};
					if (abs(dot(tangent, nv)) > value_t(0.7331)) {
						// anti-leet
						tangent = basic_vec<value_t, 3>{0, 1, 0};
					}
					tangent = normalize(cross(nv, tangent));
					// transform, reject and dot values to get angle
					const auto transformed = q * tangent;
					const auto projection = normalize(reject(transformed, nv));
					return angle(tangent, projection);
				}

			}
		}
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

	template <typename MatT>
	inline auto shear(int t_dim, int s_dim, typename MatT::value_t f) {
		// FIXME shear transform specification
		MatT m{ 1 };
		m[t_dim][s_dim] = f;
		return m;
	}


	// Functions for constructing 2d transformations
	//

	template <typename T>
	inline auto rotate2(const T &x) {
		basic_mat<detail::fpromote_t<T>, 3, 3> r{1};
		r[0][0] = cos(x);
		r[1][0] = -sin(x);
		r[1][1] = cos(x);
		r[0][1] = sin(x);
		return r;
	}

	template <typename Tx, typename Ty>
	inline auto scale2(const Tx &x, const Ty &y) {
		basic_mat<detail::fpromote_arith_t<Tx, Ty>, 3, 3> r{1};
		r[0][0] = x;
		r[1][1] = y;
		return r;
	}

	template <typename T>
	inline auto scale2(const T &x) {
		return scale2(x, x);
	}

	template <typename T>
	inline auto scale2(const basic_vec<T, 2> &v) {
		return scale2(v.x, v.y);
	}

	template <typename Tx, typename Ty>
	inline auto translate2(const Tx &x, const Ty &y) {
		basic_mat<detail::fpromote_arith_t<Tx, Ty>, 3, 3> r{1};
		r[3][0] = x;
		r[3][1] = y;
		return r;
	}

	template <typename T>
	inline auto translate2(const T &x) {
		// TODO is this overload useful?
		return translate2(x, x);
	}

	template <typename T>
	inline auto translate2(const basic_vec<T, 2> &v) {
		return translate2(v.x, v.y);
	}


	// Functions for constructing 3d transformations
	//

	template <typename Te, typename Tf, typename Tu>
	inline auto lookat(const basic_vec<Te, 3> &eye, const basic_vec<Tf, 3> &focus, const basic_vec<Tu, 3> &up) {
		// TODO Nan check
		using value_t = detail::fpromote_arith_t<Te, Tf, Tu>;
		const auto vz = normalize(eye - focus);
		const auto vx = normalize(cross(up, vz));
		const auto vy = normalize(cross(vz, vx));
		basic_mat<value_t, 4, 4> r{vx, vy, vz, eye};
		r[3][3] = value_t(1);
		return inverse(r);
	}

	// fovy: vertical field of view in radians; aspect is w/h
	template <typename Ty, typename Ta, typename Tn, typename Tf>
	inline auto perspective(const Ty &fovy, const Ta &aspect, const Tn &znear, const Tf &zfar) {
		// TODO Nan check
		// lol wtf, fast approximation
		// (seriously, where did this come from?)
		// typename MatT::value_t f = typename MatT::value_t(1) / (fovy / typename MatT::value_t(2));
		using value_t = detail::fpromote_arith_t<Ty, Ta, Tn, Tf>;
		// real equation
		const auto f = cot(fovy / value_t(2));
		basic_mat<value_t, 4, 4> r{0};
		r[0][0] = f / aspect;
		r[1][1] = f;
		r[2][2] = (zfar + value_t(znear)) / (znear - value_t(zfar));
		r[3][2] = (2 * zfar * value_t(znear)) / (znear - value_t(zfar));
		r[2][3] = -1;
		return r;
	}

	template <typename Tl, typename Tr, typename Tb, typename Tt, typename Tn, typename Tf>
	inline auto orthographic(const Tl &left, const Tr &right, const Tb &bottom, const Tt &top, const Tn &znear, const Tf &zfar) {
		// TODO Nan check
		using value_t = detail::fpromote_arith_t<Tl, Tr, Tb, Tt, Tn, Tf>;
		basic_mat<value_t, 4, 4> r{0};
		r[0][0] = value_t(2) / (right - value_t(left));
		r[3][0] = (right + value_t(left)) / (right - value_t(left));
		r[1][1] = value_t(2) / (top - value_t(bottom));
		r[3][1] = (top + value_t(bottom)) / (top - value_t(bottom));
		r[2][2] = -value_t(2) / (zfar - value_t(znear));
		r[3][2] = (zfar + value_t(znear)) / (zfar - value_t(znear));
		r[3][3] = value_t(1);
		return r;
	}

	template <typename T>
	inline auto rotate3x(const T &x) {
		using value_t = detail::fpromote_t<T>;
		basic_mat<value_t, 4, 4> r{1};
		r[1][1] = cos(x);
		r[2][1] = -sin(x);
		r[1][2] = sin(x);
		r[2][2] = cos(x);
		return r;
	}

	template <typename T>
	inline auto rotate3y(const T &x) {
		using value_t = detail::fpromote_t<T>;
		basic_mat<value_t, 4, 4> r{1};
		r[0][0] = cos(x);
		r[2][0] = sin(x);
		r[0][2] = -sin(x);
		r[2][2] = cos(x);
		return r;
	}

	template <typename T>
	inline auto rotate3z(const T &x) {
		using value_t = detail::fpromote_t<T>;
		basic_mat<value_t, 4, 4> r{1};
		r[0][0] = cos(x);
		r[1][0] = -sin(x);
		r[0][1] = sin(x);
		r[1][1] = cos(x);
		return r;
	}

	template <typename T>
	inline auto rotate3(const basic_quat<T> &q) {
		using value_t = detail::fpromote_t<T>;
		return basic_mat<value_t, 4, 4>{basic_quat<value_t>{q}};
	}

	template <typename Tx, typename Ty, typename Tz>
	inline auto scale3(const Tx &x, const Ty &y, const Tz &z) {
		using value_t = detail::fpromote_arith_t<Tx, Ty, Tz>;
		basic_mat<value_t, 4, 4> r{1};
		r[0][0] = x;
		r[1][1] = y;
		r[2][2] = z;
		return r;
	}

	template <typename T>
	inline auto scale3(const T &x) {
		return scale3(x, x, x);
	}

	template <typename T>
	inline auto scale3(const basic_vec<T, 3> &v) {
		return scale3(v.x, v.y, v.z);
	}

	template <typename Tx, typename Ty, typename Tz>
	inline auto translate3(const Tx &x, const Ty &y, const Tz &z) {
		using value_t = detail::fpromote_arith_t<Tx, Ty, Tz>;
		basic_mat<value_t, 4, 4> r{1};
		r[3][0] = x;
		r[3][1] = y;
		r[3][2] = z;
		return r;
	}

	template <typename T>
	inline auto translate3(const T &x) {
		// TODO is this overload useful?
		return translate3(x, x, x);
	}

	template <typename T>
	inline auto translate3(const basic_vec<T, 3> &v) {
		return translate3(v.x, v.y, v.z);
	}

	// Euler angle constuctor
	// body-3-2-1 euler rotation
	// TODO nan checking
	template <typename Tx, typename Ty, typename Tz>
	inline auto euler(const Tx &rx, const Ty &ry, const Tz &rz) {
		using value_t = detail::fpromote_arith_t<Tx, Ty, Tz>;
		basic_vec<value_t, 4> rotx{sin(rx / value_t(2)), 0, 0, cos(rx / value_t(2))};
		basic_vec<value_t, 4> roty{0, sin(ry / value_t(2)), 0, cos(ry / value_t(2))};
		basic_vec<value_t, 4> rotz{0, 0, sin(rz / value_t(2)), cos(rz / value_t(2))};
		basic_quat<value_t> q{rotx * outer_product(roty, rotz)};
		return q;
	}

	// rotation around a given axis
	// normalizes the axis for you
	// TODO nan checking
	template <typename Ta, typename Tx>
	inline auto axisangle(const basic_vec<Ta, 3> &ax, const Tx &x) {
		using value_t = detail::fpromote_arith_t<Ta, Tx>;
		return basic_quat<value_t>{cos(x / value_t(2)), sin(x / value_t(2)) * normalize(ax)};
	}

	// rotation around a given axis using axis's magnitude as angle
	// TODO nan checking
	template <typename T>
	inline auto axisangle(const basic_vec<T, 3> &ax) {
		using value_t = detail::fpromote_t<T>;
		const auto x = length(ax);
		return basic_quat<value_t>{cos(x / value_t(2)), sin(x / value_t(2)) * ax / x};
	}

	// rotation between 2 vectors
	// TODO nan checking
	template <typename Tf, typename Tt>
	inline auto fromto(const basic_vec<Tf, 3> &from, const basic_vec<Tt, 3> &to) {
		using value_t = detail::fpromote_arith_t<Tf, Tt>;
		const auto x = angle(from, to);
		return basic_quat<value_t>{cos(x / value_t(2)), sin(x / value_t(2)) * normalize(cross(from, to))};
	}




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
			const auto x = m_elem_dist(g, elem_dist_type::param_type(param.a(), param.b()));
			const auto ax = normalize(m_vec_dist(g, vec_dist_type::param_type(basic_vec<T, 3>(-1), basic_vec<T, 3>(1))));
			r = axisangle(normalize(ax), x);
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
	template <typename T>
	inline size_t hash_combine(std::size_t seed, const T &x) {
		std::hash<T> h;
		return seed ^ (h(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	}
}

namespace std {
	
	// vec hash
	template <typename T, size_t N>
	struct hash<cgra::basic_vec<T, N>> {
		inline size_t operator()(const cgra::basic_vec<T, N> &v) const {
			// 73: Sheldon Cooper's favorite number
			// TODO where did ^ come from?
			return cgra::fold(cgra::hash_combine<T>, 73, v);
		}
	};

	// mat hash
	template <typename T, size_t Cols, size_t Rows>
	struct hash<cgra::basic_mat<T, Cols, Rows>> {
		inline size_t operator()(const cgra::basic_mat<T, Cols, Rows> &m) const {
			return cgra::fold(cgra::hash_combine<basic_vec<T, Rows>>, 73, m);
		}
	};

	// quat hash
	template <typename T>
	struct hash<cgra::basic_quat<T>> {
		inline size_t operator()(const cgra::basic_quat<T> &q) const {
			return cgra::fold(cgra::hash_combine<T>, 73, cgra::basic_vec<T, 4>{q});
		}
	};
}


#undef CGRA_DEFINE_MAGIC_CTOR
#undef CGRA_DEFINE_DEFAULT_CTOR

