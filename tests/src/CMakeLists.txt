
# Source files
set(sources
	"${PROJECT_SOURCE_DIR}/../cgra_math.hpp"
	"main.cpp"
	"math_test.hpp"
	"math_test.cpp"
	"math_basic_vec_test.cpp"
)

# Visual Studio debugger visualization
set(natvis
	"${PROJECT_SOURCE_DIR}/../cgra_math.natvis"
)

# Add executable target and link libraries
add_executable(cgra_math_test ${sources} ${natvis})

source_group(source FILES ${sources})

