cmake_minimum_required(VERSION 2.8)
project( Triangle )
find_package( OpenGL REQUIRED ) #defines the OPENGL_LIBRARY var
find_package( OpenCV REQUIRED ) #defines Opencv LIBS and INCLUDE_DIRS

# Make the shaders show up in QTCreator/Visual Studio
FILE(GLOB Shader_Files
        ${CMAKE_SOURCE_DIR}/shaders/*.fsh
        ${CMAKE_SOURCE_DIR}/shaders/*.vsh
)
SOURCE_GROUP(shaders FILES shader_files )

# Compile external dependencies
add_subdirectory(deps)

include_directories(
    deps/glfw-3.0.4/include/GLFW/
    deps/glm-0.9.5.2/glm
    deps/glew-1.10.0/include/
    ${OpenCV_INCLUDE_DIRS}
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        ${GLFW_LIBRARIES}
        GLEW_110
        ${OpenCV_LIBS}
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
        common/ShaderUtils.cpp
        common/objmodel.cpp
        ${Shader_Files}
)


target_link_libraries( ${PROJECT_NAME}
    ${ALL_LIBS}
)


add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders"
)

add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_CURRENT_BINARY_DIR}/models"
)
