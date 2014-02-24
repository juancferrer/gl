#include <iostream>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

GLuint initTexture(){
    // Setup the texture
    GLuint texturePtr;
    glGenTextures(1, &texturePtr);
    glBindTexture(GL_TEXTURE_2D, texturePtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cv::Mat textureData = cv::imread("models/monkey.png", CV_LOAD_IMAGE_COLOR);
    cv::flip(textureData, textureData, 0); // Flip the image upside down for OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.cols, textureData.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData.data);
    return texturePtr;
}

GLuint initVAO(){
    // Setup the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

GLuint initModelVBO(ObjModel model){
    GLuint modelVBO;
    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, model.verticesCount * 12, model.positions, GL_STATIC_DRAW);

    // Vertex attrib
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0, vertices
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    return modelVBO;
}

GLuint initTexelsVBO(ObjModel model){

    // Create the texels VBO and bind texture data to it
    GLuint texelsVBO;
    glGenBuffers(1, &texelsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texelsVBO);
    glBufferData(GL_ARRAY_BUFFER, model.verticesCount * 9 , model.texels, GL_STATIC_DRAW);

    // Texture attrib
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    return texelsVBO;
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
    GLuint texturePtr = initTexture();
    GLuint vao = initVAO();

    // Create the model, and load the model data into model VBO
    ObjModel model = ObjModel("models/monkey.obj");
    GLuint modelVBO = initModelVBO(model);

    // Create the texture VBO
    GLuint texelsVBO = initTexelsVBO(model);

    // Get the texture sampler
    GLuint samplerPtr = glGetUniformLocation(shaderProgram, "sampler");
    glUniform1i(samplerPtr, 0); // Set sampler in fragment shader to 0

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projMat = glm::perspective(45.0f, (float)(windowWidth / windowHeight), 0.1f, 100.0f);
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 modelMat = glm::mat4(1.0f);  // Changes for each model !

    // Ptr to matrix projection inside vertex shader
    GLuint mvpMatPtr = glGetUniformLocation(shaderProgram, "mvpMat");

    do{

        float xPos = glm::cos(glfwGetTime());
        float zPos = glm::sin(glfwGetTime());
        // Camera matrix
        glm::mat4 camMat  = glm::lookAt(
            glm::vec3(5 * xPos, 5 * xPos, 5 * zPos), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 mvpMat = projMat * camMat * modelMat; // Remember, matrix multiplication is the other way around
        glUniformMatrix4fv(mvpMatPtr, 1, GL_FALSE, &mvpMat[0][0]); // Update the projection inside the shader

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the current window size, and set the viewport to be that size
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, model.verticesCount * 3); // 3 indices starting at 0 -> 1 triangle

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    // Cleanup VBO
    glDeleteBuffers(1, &modelVBO);
    glDeleteVertexArrays(1, &vao);
    return 0;
}
