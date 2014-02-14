cmake_minimum_required(VERSION 2.8)
project( Triangle )
find_package( OpenGL REQUIRED ) #defines the OPENGL_LIBRARY var

# Compile external dependencies 
add_subdirectory (deps)

include_directories(
	deps/glfw-3.0.4/include/GLFW/
	deps/glm-0.9.5.2/
	deps/glew-1.10.0/include/
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    ${GLFW_LIBRARIES}
    GLEW_110
    Xxf86vm
    pthread
    Xi
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(${PROJECT_NAME} 
    main.cpp
    common/shader.hpp
    common/shader.cpp 
)

target_link_libraries( ${PROJECT_NAME} 
    ${ALL_LIBS} 
)


add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders"
   #COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/"
)
