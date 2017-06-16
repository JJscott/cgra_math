

import json

variable_name_dict = {
'val_a':'val_t val_a = random<val_t>(val_t{-1}, val_t{1});',
'val_b':'val_t val_b = random<val_t>(val_t{-1}, val_t{1});',
'val_c':'val_t val_c = random<val_t>(val_t{-1}, val_t{1});',
'vec_a':'vec_t vec_a = random<vec_t>(vec_t{-1}, vec_t{1});',
'vec_b':'vec_t vec_b = random<vec_t>(vec_t{-1}, vec_t{1});',
'vec_c':'vec_t vec_c = random<vec_t>(vec_t{-1}, vec_t{1});',
'mat_a':'mat_t val_a = random<mat_t>(val_t{-1}, val_t{1});',
'mat_b':'mat_t val_b = random<mat_t>(val_t{-1}, val_t{1});',
'mat_c':'mat_t val_c = random<mat_t>(val_t{-1}, val_t{1});',
'vec_pos':'vec_t vec_pos = random<vec_t>(val_t{0}, val_t{1});',
'vec_neg':'vec_t vec_neg = random<vec_t>(val_t{-1}, val_t{0});'
}


vec_test_template_str = """
template <typename T, size_t N>
inline bool {test_name}() {{
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = vec_t::value_t; // same as T
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {{
		{initialization}
		if (!({test})) fail_count++;
	}}
	float fail_fract = float(fail_count) / max_iter;
	if (fail_fract > max_fail_fract) {{
		return false;
	}}
	return true;
}}
"""

source_test_file_str = """
#include <cgra_math.hpp>
#include "math_test.hpp"



"""


run_vector_test_str = "\touput_test({function_name}, {function_name}<{type}, {N}>());"



def create_cxx_test_str(test_template, test_name, test_str):
	# find all instances of vectors to init
	init = '\n'.join([variable_name_dict[var_name] for var_name in variable_name_dict if (test_str.find(var_name) >= 0)])
	return test_template.format(test_name=test_name, initialization=init, test=test_str)




def create_vector_tests():

	with open("vector_tests.json") as vector_json :
		vector_json_data = json.load(vector_json)

		function_str_list = []
		test_str_list = []

		for section in vector_json_data :
			for test in vector_json_data[section] :
				test_name = test[u'tag']
				test_str = test[u'test']
				if u'N' in test:
					for n in test[u'N'] if u'N' in test else xrange(1, 5):
						pass

				function_str_list.append(create_cxx_test_str(vec_test_template_str, test_name, test_str))
				return



if __name__ == "__main__":
	create_vector_tests()