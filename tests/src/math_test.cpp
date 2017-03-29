
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <typeinfo>

#include <cgra_math.hpp>
#include "math_test.hpp"


using namespace std;
using namespace cgra;
using namespace test;


int total = 10000;





void vector_compile_test() {
	
	// constructors
	//

	basic_vec<float, 0> v0;
	basic_vec<float, 1> v1;
	basic_vec<float, 2> v2;
	basic_vec<float, 3> v3;
	basic_vec<float, 4> v4;
	basic_vec<float, 5> v5;

	v0 = basic_vec<float, 0>();
	v1 = basic_vec<float, 1>();
	v2 = basic_vec<float, 2>();
	v3 = basic_vec<float, 3>();
	v4 = basic_vec<float, 4>();
	v5 = basic_vec<float, 5>();


	


}



//Main program
// 
void test::run_tests() {
	vector_compile_test();
	vector_tests();
	cin.get();
}