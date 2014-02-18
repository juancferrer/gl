// Include standard headers
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/ShaderUtils.hpp"

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        std::cerr << "Failed to initialize GLFW\n";
        glfwTerminate();
        return -1;
    }

    // Window hints to get sick OpenGL version
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Triangle", NULL, NULL);
    if( window == NULL ){
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

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

    // Setup the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    // The data for the triangle
    static const GLfloat triangleVertices[] = {
        // Triangle vertices
        -1.0f, -1.0f, 0.0f,  // Bottom left
         1.0f, -1.0f, 0.0f,  // Bottom right
         0.0f,  1.0f, 0.0f,  // Top
        // Vertex color
         1.0f,  0.0f, 0.0f, // Red
         0.0f,  1.0f, 0.0f, // Green
         0.0f,  0.0f, 1.0f, // Blue
    };

    // Create the VBO and bind the triangle data  to it
    GLuint triangleVBO;
    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Vertex attrib
    glVertexAttribPointer(
        0,                  // attribute 0, vertices
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Color attrib
    glVertexAttribPointer(
        1,                  // attribute 1, colors
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)(sizeof(float)*3*3) // array buffer offset sizeof(float) * Num floats in vec3 * Num vertices
    );

    GLsizei windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    GLint blinkingColorPtr = glGetUniformLocation(shaderProgram, "blinkingColor");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projMat = glm::perspective(45.0f, (float)(windowWidth / windowHeight), 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 camMat  = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 modelMat = glm::mat4(1.0f);  // Changes for each model !
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvpMat = projMat * camMat * modelMat; // Remember, matrix multiplication is the other way around

    GLuint mvpMatPtr = glGetUniformLocation(shaderProgram, "mvpMat");
    glUniformMatrix4fv(mvpMatPtr, 1, GL_FALSE, &mvpMat[0][0]);

    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        // Get the current window size, and set the viewport to be that size
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        GLfloat foo = std::abs(std::sin(glfwGetTime()));
        glUniform3f(blinkingColorPtr, foo, foo, foo); // Set the blinking color to Sin(time)
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    // Cleanup VBO
    glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &vao);

    return 0;
}

