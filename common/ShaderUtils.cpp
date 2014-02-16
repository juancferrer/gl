#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "ShaderUtils.hpp"

namespace ShaderUtils{

GLuint shaderFromFile(const char * path, GLenum shaderType){

    // Create the shaders
    GLuint shaderID = glCreateShader(shaderType);

    // Open the filestream
    std::ifstream fin;
    fin.open(path);
    if(!fin.is_open()){
        fin.close();
        std::cout << "Impossible to open " << path << std::endl;
        return 0;
    }
    // Read the source, and close the file
    std::string source((std::istreambuf_iterator<GLchar>(fin)), std::istreambuf_iterator<GLchar>());
    fin.close();

    // Compile Shader
    std::cout << "Compiling shader : " << path << std::endl;
    char const * sourcePtr = source.c_str();
    glShaderSource(shaderID, 1, &sourcePtr , NULL);
    glCompileShader(shaderID);

    // Check Shader
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
    if ( logLength > 0 ){
        GLchar *log = new GLchar[logLength + 1];
        glGetShaderInfoLog(shaderID, logLength, NULL, log);
        std::cout << &log << std::endl;
    }
    return shaderID;
}
}
