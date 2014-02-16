#ifndef SHADER_HPP
#define SHADER_HPP
#include <GL/glew.h>

namespace ShaderUtils{
    // Returns ptr to shader source ready to attach to program
    GLuint shaderFromFile(const char *path, GLenum shaderType);
};
#endif
