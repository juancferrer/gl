add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)



### GLFW ###
add_subdirectory (glfw-3.0.4)
include_directories(
	glfw-3.0.4/include/GLFW/
)

### GLEW ###
set(GLEW_SOURCE
	glew-1.10.0/src/glew.c
)
set(GLEW_HEADERS
)
add_library( GLEW_110 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)
target_link_libraries(GLEW_110
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)
