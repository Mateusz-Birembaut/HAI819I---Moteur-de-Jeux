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


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Include Imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace glm;

#include <common/globals.hpp>
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
#include <common/UI/ImGui.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void resizeFramebuffer(int width, int height) {
    if (width <= 0 || height <= 0)
        return;
        
    framebufferWidth = static_cast<int>(width * renderScale);
    framebufferHeight = static_cast<int>(height * renderScale);
    
    // reconfigure texture
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferWidth, framebufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    // reconfigure renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferWidth, framebufferHeight);
    
    // check if framebuffer is complete
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete after resize!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupFramebuffer() {
    // Génération des objets OpenGL
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &textureColorbuffer);
    glGenRenderbuffers(1, &rbo);
    
    // Configuration du framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    // Attacher texture couleur
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferWidth, framebufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    // Renderbuffer pour profondeur et stencil
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    // Vérification
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void processInput(GLFWwindow *window);

/*******************************************************************************/

void checkGLError(const char* operation) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error after " << operation << ": " << error << std::endl;
    }
}


int main(void) {
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

    // Créer la fenêtre avec les dimensions initiales
    window = glfwCreateWindow(windowWidth, windowHeight, "TP Moteur de jeux - GLFW", NULL, NULL);
    // ... vérifications ...
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    // Centrer la fenêtre sur l'écran
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    int monitorX = mode->width;
    int monitorY = mode->height;
    glfwSetWindowPos(window, (monitorX - windowWidth) / 2, (monitorY - windowHeight) / 2);


    glfwMakeContextCurrent(window);

    // Définir la fonction de callback pour le redimensionnement
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    initImGui(window);

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/

    // Create and load the textures
    Texture earthTexture("../src/Textures/earthTexture.jpg", true);
    earthTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    Texture moonTexture("../src/Textures/moonTexture.jpg", false);
    moonTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    SceneGraph sceneGraph;

    Mesh sphereMesh;
    Sphere::create(sphereMesh, 90,90, 1.0f);

    Mesh terrainMesh;
    Terrain::create(terrainMesh, 120,120, 1.0f, 1.0f);

    GameObject sun;
    sun.mesh = &sphereMesh;
    sun.transformation.translation = glm::vec3(0.0f, 0.0f, 0.0f); 

    GameObject earth;
    earth.mesh = &sphereMesh;
    earth.texture = &earthTexture;
    earth.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    earth.transformation.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    earth.transformation.rotationSpeed = 10;
    earth.transformation.continuouslyRotate = glm::bvec3(false, true, false);

    GameObject moon;
    moon.mesh = &sphereMesh; 
    moon.texture = &moonTexture;
    moon.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    moon.transformation.scale = glm::vec3(0.25f, 0.25f, 0.25f);

    GameObject terrain;
    terrain.mesh = &terrainMesh;
    terrain.texture = &earthTexture;
    terrain.transformation.translation = glm::vec3(0.0f, -3.0f, 0.0f);
    terrain.transformation.scale = glm::vec3(10.0f, 0.0f, 10.0f);

    earth.addChild(&moon);
    sun.addChild(&earth);
 
    sceneGraph.addObject(&terrain);
    sceneGraph.addObject(&sun);

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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Initialiser les dimensions du framebuffer
    framebufferWidth = static_cast<int>(windowWidth * renderScale);
    framebufferHeight = static_cast<int>(windowHeight * renderScale);
    
    setupFramebuffer();


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

        // render to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        camera.sendMatricesToShader(programID, windowWidth, windowHeight);

        sceneGraph.updateAll(deltaTime);
        sceneGraph.drawAll(programID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        renderImGui(&sceneGraph, textureColorbuffer, framebufferWidth, framebufferHeight);

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Mettre à jour les dimensions de la fenêtre
    windowWidth = width;
    windowHeight = height;
    
    // Ajuster le viewport OpenGL
    glViewport(0, 0, width, height);
    
    // Redimensionner le framebuffer
    resizeFramebuffer(width, height);
}

