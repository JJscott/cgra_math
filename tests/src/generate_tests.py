




val_a_str = 'val_t val_a = random<val_t>(val_t{-1}, val_t{1});'
val_b_str = 'val_t val_b = random<val_t>(val_t{-1}, val_t{1});'
val_c_str = 'val_t val_c = random<val_t>(val_t{-1}, val_t{1});'
vec_a_str = 'vec_t vec_a = random<vec_t>(vec_t{-1}, vec_t{1});'
vec_b_str = 'vec_t vec_b = random<vec_t>(vec_t{-1}, vec_t{1});'
vec_c_str = 'vec_t vec_c = random<vec_t>(vec_t{-1}, vec_t{1});'
vec_na_str = 'vec_t vec_nb = normalize(vec_a)'
vec_nb_str = 'vec_t vec_nb = normalize(vec_b)'
vec_nc_str = 'vec_t vec_nc = normalize(vec_c)'
vec_pos_str = 'vec_t vec_pos = random<vec_t>(val_t{0}, val_t{1});'
vec_neg_str = 'vec_t vec_neg = random<vec_t>(val_t{-1}, val_t{0});'

vector_test_str = """
template <typename T, size_t N>
inline bool {test_name}() {
	using namespace cgra;
	using vec_t = basic_val<T, N>;
	using val_t = vec_t::value_t; // same as T
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		{initialization}
		if (!({test})) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	if (fail_fract > max_fail_fract) {
		return false;
	}
	return true;
}
"""

vector_test_file_str = """
#include <cgra_math.hpp>
#include "math_test.hpp"

"""

def create_vector_tests():

	with open("vector_tests.json") as vector_json :
		vector_json_data = json.load(vector_json)

	for section_name in vector_json_data :
		for section in vector_json_data[section_name] :
			pass

	pass