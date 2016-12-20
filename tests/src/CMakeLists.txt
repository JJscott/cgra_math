
# Source files
SET(sources
	"main.cpp"
)


# Add executable target and link libraries
add_executable(${CGRA_PROJECT} ${headers} ${sources})


# Sources from subdirectories
# list your subdirectories here
# ex : add_subdirectory(example)


# Set source groups (helper method)
target_source_group_tree(${CGRA_PROJECT})