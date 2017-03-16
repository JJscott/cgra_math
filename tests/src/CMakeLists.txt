
# Source files
SET(sources
	"main.cpp"
	"test_suite.hpp"
	"test_angle_trigonometry_functions.hpp"
	"test_common_functions.hpp"
	"test_exponential_functions.hpp"
	"test_geometric_functions.hpp"
	"test_operator_overloads.hpp"
	"test_relational_functions.hpp"
)


# Add executable target and link libraries
add_executable(${CGRA_PROJECT} ${headers} ${sources})


# Sources from subdirectories
# list your subdirectories here
# ex : add_subdirectory(example)


# Set source groups (helper method)
target_source_group_tree(${CGRA_PROJECT})