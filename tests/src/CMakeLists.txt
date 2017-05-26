
# Source files
set(sources
	"../../cgra_math.hpp"
	"main.cpp"
	"math_test.hpp"
	"math_test.cpp"
)


# Add executable target and link libraries
add_executable(cgra_math_test ${headers} ${sources})

source_group(source FILES ${headers} ${sources})

