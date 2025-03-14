// Include standard headers
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "common/stb_image.cpp"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/terrain.hpp>
#include <common/texture.hpp>
#include <common/camera.hpp>
#include <common/Mesh.hpp>
#include <common/sphere.hpp>
#include <common/GameObject.hpp>
#include <common/SceneGraph.hpp>

//params
int nX = 16;
int nY = 16;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f,  2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f), 0.0f, 2.5f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

void processInput(GLFWwindow *window);

/*******************************************************************************/

void checkGLError(const char* operation) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error after " << operation << ": " << error << std::endl;
    }
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP Moteur de jeux - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "../src/Shaders/vertex_shader.glsl", "../src/Shaders/fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/

    SceneGraph sceneGraph;

    Mesh sphereMesh;
    Sphere::create(sphereMesh, 90,90, 1.0f);

    GameObject sun;
    sun.objectID = 2;
    sun.mesh = &sphereMesh;
    sun.transformation.translation = glm::vec3(0.0f, 0.0f, 0.0f); 


    GameObject earth;
    earth.mesh = &sphereMesh;
    earth.objectID = 0;
    earth.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    earth.transformation.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    earth.transformation.rotationSpeed = 300;
    earth.transformation.eulerRot = glm::vec3(0.0f, 1.0f, 0.0f);

    GameObject moon;
    moon.mesh = &sphereMesh;
    moon.objectID = 1;    
    moon.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    moon.transformation.scale = glm::vec3(0.25f, 0.25f, 0.25f);

    earth.addChild(&moon);
    sun.addChild(&earth);
 

    sceneGraph.addObject(&sun);

    //int sphereCount = 1000;
    //std::vector<GameObject<Sphere>*> testSpheres = generatePerformanceSpheres(sphereCount);

    // Create and load the textures
    Texture earthTexture("../src/Textures/earthTexture.jpg", true);
    earthTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Texture moonTexture("../src/Textures/moonTexture.jpg", false);
    moonTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    GLuint vertexbuffer;
    GLuint elementbuffer;
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &elementbuffer);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);


        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change
        
        glm::mat4 modelMatrix = glm::mat4(1.0);
        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), camera.getTarget() + camera.getPosition(), camera.getUp());
        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.f / 3.f, 0.1f, 100.0f);

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        GLint loc_transformations = glGetUniformLocation(programID, "u_model");
        GLint loc_ViewMatrix = glGetUniformLocation(programID, "u_view");
        GLint lov_ProjectionMatrix = glGetUniformLocation(programID, "u_projection");

        glUniformMatrix4fv(loc_ViewMatrix, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(lov_ProjectionMatrix, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(loc_transformations, 1, GL_FALSE, &modelMatrix[0][0]);

        /****************************************/



        // activer texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moonTexture.getID());


        // Obtenir l'emplacement de l'uniform `u_texture`
        GLuint textureID1 = glGetUniformLocation(programID, "u_earthTexture");
        GLuint textureID2 = glGetUniformLocation(programID, "u_moonTexture");

        glUniform1i(textureID1, 0); 
        glUniform1i(textureID2, 1); 


        sceneGraph.updateAll(deltaTime);
        sceneGraph.drawAll(programID);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //terrain.handleInputs(window);

    camera.handleCameraInputs(deltaTime, window);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

