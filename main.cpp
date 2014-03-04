#include <iostream>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "common/objmodel.h"
#include "common/ShaderUtils.hpp"

void initGLFW(){
    // Initialise GLFW
    if( !glfwInit() )
    {
        std::cerr << "Failed to initialize GLFW\n";
        glfwTerminate();
    }

    // Window hints to get sick OpenGL version
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

GLFWwindow* initWindow(){
    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow( 1024, 768, "Triangle", NULL, NULL);
    if( window == NULL ){
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    // Ensure we can capture the keys being pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    return window;
}

void initGLEW(){
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
    }
}

GLuint initShaderProgram(){

    // Load the shaders
    GLuint vertexShader = ShaderUtils::shaderFromFile("shaders/simple.vsh", GL_VERTEX_SHADER);
    GLuint fragmentShader = ShaderUtils::shaderFromFile("shaders/simple.fsh", GL_FRAGMENT_SHADER);

    // Create the program and link shaders to program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Use our program
    glUseProgram(shaderProgram);
    return shaderProgram;
}

void initGLStuff(){
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    // Turn on cool stuff
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
}

int main( void ){
    initGLFW();
    GLFWwindow *window = initWindow();
    // Setup the window size
    GLsizei windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    initGLEW();
    initGLStuff();

    GLuint shaderProgram = initShaderProgram();

    // Create the monkey model
    ObjModel monkeyModel = ObjModel("models/monkey.obj", "models/monkey.png");

    // Create the cube model
    ObjModel cubeModel = ObjModel("models/cube.obj", "models/cube.png");

    // Get the texture sampler
    GLuint textureSampler = glGetUniformLocation(shaderProgram, "sampler");
    glUniform1i(textureSampler, 0); // Set sampler in fragment shader to 0

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projMat = glm::perspective(45.0f, (float)(windowWidth / windowHeight), 0.1f, 100.0f);

    // Ptr to matrix projection inside vertex shader
    GLuint mvpMatPtr = glGetUniformLocation(shaderProgram, "mvpMat");

    do{
        float xPos = glm::cos(glfwGetTime());
        float zPos = glm::sin(glfwGetTime());
        // Camera matrix
        glm::mat4 camMat  = glm::lookAt(
            glm::vec3(5 * xPos, 5 * xPos, 5 * zPos), // Camera is circling model
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        // Monkey model at origin
        glm::mat4 monkeyModelMat = glm::mat4(1.0f);  // Identity matrix (model is at origin)

        // Cube model at 0,-2,0 (below monkey)
        //glm::mat4 cubeModelMat = glm::translate(0.0f, -2.0f, 0.0f);
        glm::mat4 cubeModelMat = glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the current window size, and set the viewport to be that size
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw the monkey
        glBindVertexArray(monkeyModel.vao);
        glm::mat4 mvpMonkeyMat = projMat * camMat * monkeyModelMat;
        glUniformMatrix4fv(mvpMatPtr, 1, GL_FALSE, &mvpMonkeyMat[0][0]); // Update the projection inside the shader
        glBindTexture(GL_TEXTURE_2D, monkeyModel.texturePtr);
        glDrawArrays(GL_TRIANGLES, 0, monkeyModel.vertices.size());

        // Draw the cube
        glBindVertexArray(cubeModel.vao);
        glm::mat4 mvpCubeMat = projMat * camMat * cubeModelMat;
        glUniformMatrix4fv(mvpMatPtr, 1, GL_FALSE, &mvpCubeMat[0][0]); // Update the projection inside the shader
        glBindTexture(GL_TEXTURE_2D, cubeModel.texturePtr);
        glDrawArrays(GL_TRIANGLES, 0, cubeModel.vertices.size());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    // Cleanup VBO
    /*
    glDeleteBuffers(1, &monkeyVBO);
    glDeleteVertexArrays(1, &monkeyVAO);
    */
    return 0;
}
