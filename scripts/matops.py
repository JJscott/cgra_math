
# note:
# mat*mat needs to do matrix mul
# mat/mat should be removed (left or right division?)
# mat*vec, vec*mat need to be added

assign_ops = [
	('+=', 'add_assign', None),
	('-=', 'sub_assign', None),
	('*=', 'mul_assign', None),
	('/=', 'div_assign', None)
]

unary_ops = [
	('-', 'neg', 'negate')
]

binary_ops = [
	('+', 'add', None),
	('-', 'sub', None),
	('*', 'mul', None),
	('/', 'div', None)
]

assign_op_str = '''// mat {comment}
template <typename MatT1, typename MatT2, typename = enable_if_matrix_compatible_t<MatT1, MatT2>>
inline MatT1 & operator{op}(MatT1 &lhs, const MatT2 &rhs) {{
	zip_with<type_to_mat>(detail::op::{func}(), lhs, rhs);
	return lhs;
}}

// mat {comment} scalar
template <typename MatT, typename T, typename = enable_if_matrix_scalar_compatible_t<MatT, T>, typename = void>
inline MatT & operator{op}(MatT &lhs, const T &rhs) {{
	zip_with<type_to_mat>(detail::op::{func}(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
	return lhs;
}}
'''

unary_op_str= '''// mat {comment}
template <typename MatT, typename = enable_if_matrix_t<MatT>>
inline auto operator{op}(const MatT &rhs) {{
	return zip_with<type_to_mat>(detail::op::{func}(), rhs);
}}
'''

binary_op_str = '''// mat {comment}
template <typename MatT1, typename MatT2, typename = enable_if_matrix_compatible_t<MatT1, MatT2>>
inline auto operator{op}(const MatT1 &lhs, const MatT2 &rhs) {{
	return zip_with<type_to_mat>(detail::op::{func}(), lhs, rhs);
}}

// mat {comment} right scalar
template <typename MatT, typename T, typename = enable_if_matrix_scalar_compatible_t<MatT, T>, typename = void>
inline auto operator{op}(const MatT &lhs, const T &rhs) {{
	return zip_with<type_to_mat>(detail::op::{func}(), lhs, repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(rhs));
}}

// mat {comment} left scalar
template <typename MatT, typename T, typename = enable_if_matrix_scalar_compatible_t<MatT, T>, typename = void, typename = void>
inline auto operator{op}(const T &lhs, const MatT &rhs) {{
	return zip_with<type_to_mat>(detail::op::{func}(), repeat_vec_vec<T, mat_cols<MatT>::value, mat_rows<MatT>::value>(lhs), rhs);
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

