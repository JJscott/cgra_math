
# MANUAL ALTERATIONS NEEDED:
# some 'binary' functions are actually ternary

unary_fns = [
	('sin', 'sin'),
	('cos', 'cosine'),
	('tan', 'tangent'),
	('sec', 'secant'),
	('csc', 'cosecant'),
	('cot', 'cotangent'),
	('asin', 'inverse sin'),
	('acos', 'inverse cosine'),
	('atan', 'inverse tangent'),
	('asec', 'inverse secant'),
	('acsc', 'inverse cosecant'),
	('acot', 'inverse cotangent'),
	('sinh', 'hyperbolic sin'),
	('cosh', 'hyperbolic cosine'),
	('tanh', 'hyperbolic tangent'),
	('sech', 'hyperbolic secant'),
	('csch', 'hyperbolic cosecant'),
	('coth', 'hyperbolic cotangent'),
	('asinh', 'inverse hyperbolic sin'),
	('acosh', 'inverse hyperbolic cosine'),
	('atanh', 'inverse hyperbolic tangent'),
	('asech', 'inverse hyperbolic secant'),
	('acsch', 'inverse hyperbolic cosecant'),
	('acoth', 'inverse hyperbolic cotangent'),
	('radians', 'degrees to radians'),
	('degrees', 'radians to degrees'),
	('exp', 'exp'),
	('exp2', 'exp base 2'),
	('expm1', 'exp(x) - 1'),
	('log', 'log'),
	('log2', 'log2'),
	('log10', 'log10'),
	('log1p', 'log(1 + x)'),
	('sqrt', 'sqrt'),
	('cbrt', 'cbrt'),
	('abs', 'abs'),
	('floor', 'floor'),
	('ceil', 'ceil'),
	('isnan', 'isnan'),
	('isinf', 'isinf'),
	('sign', 'sign'),
	('fract', 'fract'),
	# TODO etc
]

binary_fns = [
	('atan', 'inverse tangent (2-arg)', 'y', 'x'),
	('pow', 'pow', 'x', 'a'),
	('mod', 'mod', 'x', 'm'),
	('step', 'step', 'edge', 'x'),
	# TODO etc
]

ternary_fns = [
	('mix', 'mix', 'x1', 'x2', 't'),
	('clamp', 'clamp', 'x', 'lower', 'upper'),
	('smoothstep', 'smoothstep', 'edge0', 'edge1', 'x'),
	# TODO etc
]

unary_fn_str = '''// vec {comment}
template <typename VecT, enable_if_vector_t<VecT> = 0>
inline auto {func}(const VecT &v) {{
	using cgra::detail::scalars::{func};
	return zip_with([](const auto &x) {{ return {func}(x); }}, v);
}}
'''

binary_fn_str = '''// vec {comment}
template <typename VecT1, typename VecT2, enable_if_vector_compatible_t<VecT1, VecT2> = 0>
inline auto {func}(const VecT1 &v{arg1}, const VecT2 &v{arg2}) {{
	using cgra::detail::scalars::{func};
	return zip_with([](const auto &x{arg1}, const auto &x{arg2}) {{ return {func}(x{arg1}, x{arg2}); }}, v{arg1}, v{arg2});
}}

// vec {comment} right scalar
template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
inline auto {func}(const VecT &v{arg1}, const T &{arg2}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg1}) {{ return {func}(x{arg1}, {arg2}); }}, v{arg1});
}}

// vec {comment} left scalar
template <typename VecT, typename T, enable_if_vector_scalar_compatible_t<VecT, T> = 0>
inline auto {func}(const T &{arg1}, const VecT &v{arg2}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg2}) {{ return {func}({arg1}, x{arg2}); }}, v{arg2});
}}
'''

ternary_fn_str = '''// vec {comment}
template <typename VecT1, typename VecT2, typename VecT3, enable_if_vector_compatible_t<VecT1, VecT2, VecT3> = 0>
inline auto {func}(const VecT1 &v{arg1}, const VecT2 &v{arg2}, const VecT3 &v{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([](const auto &x{arg1}, const auto &x{arg2}, const auto &x{arg3}) {{ return {func}(x{arg1}, x{arg2}, x{arg3}); }}, v{arg1}, v{arg2}, v{arg3});
}}

// vec {comment} ({arg3})-scalar
template <typename VecT1, typename VecT2, typename T3, enable_if_vector_compatible_t<VecT1, VecT2> = 0, enable_if_vector_scalar_compatible_t<VecT1, T3> = 0>
inline auto {func}(const VecT1 &v{arg1}, const VecT2 &v{arg2}, const T3 &{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg1}, const auto &x{arg2}) {{ return {func}(x{arg1}, x{arg2}, {arg3}); }}, v{arg1}, v{arg2});
}}

// vec {comment} ({arg2})-scalar
template <typename VecT1, typename T2, typename VecT3, enable_if_vector_compatible_t<VecT1, VecT3> = 0, enable_if_vector_scalar_compatible_t<VecT1, T2> = 0>
inline auto {func}(const VecT1 &v{arg1}, const T2 &{arg2}, const VecT3 &v{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg1}, const auto &x{arg3}) {{ return {func}(x{arg1}, {arg2}, x{arg3}); }}, v{arg1}, v{arg3});
}}

// vec {comment} ({arg2},{arg3})-scalar
template <typename VecT1, typename T2, typename T3, enable_if_vector_scalar_compatible_t<VecT1, T2> = 0, enable_if_vector_scalar_compatible_t<VecT1, T3> = 0>
inline auto {func}(const VecT1 &v{arg1}, const T2 &{arg2}, const T3 &{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg1}) {{ return {func}(x{arg1}, {arg2}, {arg3}); }}, v{arg1});
}}

// vec {comment} ({arg1})-scalar
template <typename T1, typename VecT2, typename VecT3, enable_if_vector_compatible_t<VecT2, VecT3> = 0, enable_if_vector_scalar_compatible_t<VecT2, T1> = 0>
inline auto {func}(const T1 &{arg1}, const VecT2 &v{arg2}, const VecT3 &v{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg2}, const auto &x{arg3}) {{ return {func}({arg1}, x{arg2}, x{arg3}); }}, v{arg2}, v{arg3});
}}

// vec {comment} ({arg1},{arg3})-scalar
template <typename T1, typename VecT2, typename T3, enable_if_vector_scalar_compatible_t<VecT2, T1> = 0, enable_if_vector_scalar_compatible_t<VecT2, T3> = 0>
inline auto {func}(const T1 &{arg1}, const VecT2 &v{arg2}, const T3 &{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg2}) {{ return {func}({arg1}, x{arg2}, {arg3}); }}, v{arg2});
}}

// vec {comment} ({arg1},{arg2})-scalar
template <typename T1, typename T2, typename VecT3, enable_if_vector_scalar_compatible_t<VecT3, T1> = 0, enable_if_vector_scalar_compatible_t<VecT3, T2> = 0>
inline auto {func}(const T1 &{arg1}, const T2 &{arg2}, const VecT3 &v{arg3}) {{
	using cgra::detail::scalars::{func};
	return zip_with([&](const auto &x{arg3}) {{ return {func}({arg1}, {arg2}, x{arg3}); }}, v{arg3});
}}
'''


def main():
	
	for func, comment in unary_fns:
		print unary_fn_str.format(func=func, comment=comment)
	# }
	
	for func, comment, arg1, arg2 in binary_fns:
		print binary_fn_str.format(func=func, comment=comment, arg1=arg1, arg2=arg2)
	# }
	
	for func, comment, arg1, arg2, arg3 in ternary_fns:
		print ternary_fn_str.format(func=func, comment=comment, arg1=arg1, arg2=arg2, arg3=arg3)
	# }
	
# }

if __name__ == '__main__': main()






