
assign_ops = [
	('+=', 'add_assign', None),
	('-=', 'sub_assign', None),
	('*=', 'mul_assign', None),
	('/=', 'div_assign', None),
	('%=', 'mod_assign', 'remainder (mod) assign'),
	('<<=', 'lshift_assign', None),
	('>>=', 'rshift_assign', None),
	('|=', 'bitwise_or_assign', None),
	('^=', 'bitwise_xor_assign', None),
	('&=', 'bitwise_and_assign', None)
]

unary_ops = [
	('-', 'neg', 'negate'),
	('!', 'logical_not', None),
	('~', 'bitwise_not', None)
]

binary_ops = [
	('+', 'add', None),
	('-', 'sub', None),
	('*', 'mul', None),
	('/', 'div', None),
	('%', 'mod', 'remainder (mod)'),
	('<<', 'lshift', None),
	('>>', 'rshift', None),
	('||', 'logical_or', None),
	('&&', 'logical_and', None),
	('|', 'bitwise_or', None),
	('^', 'bitwise_xor', None),
	('&', 'bitwise_and', None),
]

assign_op_str = '''// vec {comment}
template <typename VecT1, typename VecT2, typename = enable_if_vector_compatible_t<VecT1, VecT2>>
inline VecT1 & operator{op}(VecT1 &lhs, const VecT2 &rhs) {{
	zip_with(detail::op::{func}(), lhs, rhs);
	return lhs;
}}

// vec {comment} scalar
template <typename VecT, typename T, typename = enable_if_vector_scalar_compatible_t<VecT, T>, typename = void>
inline VecT & operator{op}(VecT &lhs, const T &rhs) {{
	zip_with(detail::op::{func}(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
	return lhs;
}}
'''

unary_op_str= '''// vec {comment}
template <typename VecT, typename = enable_if_vector_t<VecT>>
inline auto operator{op}(const VecT &rhs) {{
	return zip_with(detail::op::{func}(), rhs);
}}
'''

binary_op_str = '''// vec {comment}
template <typename VecT1, typename VecT2, typename = enable_if_vector_compatible_t<VecT1, VecT2>>
inline auto operator{op}(const VecT1 &lhs, const VecT2 &rhs) {{
	return zip_with(detail::op::{func}(), lhs, rhs);
}}

// vec {comment} right scalar
template <typename VecT, typename T, typename = enable_if_vector_scalar_compatible_t<VecT, T>, typename = void>
inline auto operator{op}(const VecT &lhs, const T &rhs) {{
	return zip_with(detail::op::{func}(), lhs, repeat_vec<T, array_size<VecT>::value>(rhs));
}}

// vec {comment} left scalar
template <typename VecT, typename T, typename = enable_if_vector_scalar_compatible_t<VecT, T>, typename = void, typename = void>
inline auto operator{op}(const T &lhs, const VecT &rhs) {{
	return zip_with(detail::op::{func}(), repeat_vec<T, array_size<VecT>::value>(lhs), rhs);
}}
'''


def main():
	
	for op, func, comment in assign_ops:
		print assign_op_str.format(op=op, func=func, comment = comment if comment is not None else func)
	# }
	
	for op, func, comment in unary_ops:
		print unary_op_str.format(op=op, func=func, comment = comment if comment is not None else func)
	# }
	
	for op, func, comment in binary_ops:
		print binary_op_str.format(op=op, func=func, comment = comment if comment is not None else func)
	# }
	
# }

if __name__ == '__main__': main()

