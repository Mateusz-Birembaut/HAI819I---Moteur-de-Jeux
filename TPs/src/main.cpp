// Include standard headers
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>



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

#include <common/stb_image.cpp>
#include <common/shader.hpp>
#include <common/objloader.hpp>

#include "Ressources/Globals.hpp"
#include "Ressources/RessourceManager.hpp"

#include "GameObjects/GameObject.hpp"
#include "GameObjects/SceneGraph.hpp"
#include "GameObjects/Components/Mesh.hpp"
#include "GameObjects/Components/Texture.hpp"

#include "Utils/Terrain.hpp"
#include "Utils/Sphere.hpp"

#include "Rendering/Camera.hpp"
#include "Rendering/SceneRenderer.hpp"

#include "UI/ImGui.hpp"





void framebuffer_size_callback(GLFWwindow* window, int width, int height);



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

    initImGui(window);

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/

    // Create and load the textures

    // TODO ajouter message d'erreur si la texture n'est pas chargée

    RessourceManager& ressourceManager = RessourceManager::getInstance();

    Texture* earthTexture = ressourceManager.addTexture("earth" ,"../src/Assets/Textures/damier.jpg", true);
    Texture* moonTexture = ressourceManager.addTexture("moon" ,"../src/Assets/Textures/moonTexture.jpg", true);

    Mesh* sphereMesh = ressourceManager.addMesh("sphere");
    Sphere::create(*sphereMesh, 90,90, 1.0f);

    Mesh* terrainMesh = ressourceManager.addMesh("terrain");
    Terrain::create(*terrainMesh, 120,120, 1.0f, 1.0f);

    GameObject sun;
    sun.mesh = sphereMesh;
    sun.transformation.translation = glm::vec3(0.0f, 0.0f, 0.0f); 


    GameObject earth;
    earth.mesh = sphereMesh;
    earth.texture = earthTexture;
    earth.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    earth.transformation.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    earth.transformation.rotationSpeed = 10;
    earth.transformation.continuouslyRotate = glm::bvec3(false, true, false);


    GameObject moon;
    moon.mesh = sphereMesh; 
    moon.texture = moonTexture;
    moon.transformation.translation = glm::vec3(4.0f, 0.0f, 0.0f); 
    moon.transformation.scale = glm::vec3(0.25f, 0.25f, 0.25f);

    GameObject terrain;
    terrain.mesh = terrainMesh;
    terrain.texture = earthTexture;
    terrain.transformation.translation = glm::vec3(0.0f, -3.0f, 0.0f);
    terrain.transformation.scale = glm::vec3(10.0f, 0.0f, 10.0f);

    earth.addChild(&moon);
    sun.addChild(&earth);

    SceneGraph& sceneGraph = SceneGraph::getInstance();
 
    sceneGraph.addObject(&terrain);
    sceneGraph.addObject(&sun);


    // For speed computation
    //double lastTime = glfwGetTime();
    //int nbFrames = 0;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Initialiser les dimensions du framebuffer    
    SceneRenderer& sceneRenderer = SceneRenderer::getInstance();
    sceneRenderer.setupFramebuffer(windowWidth, windowHeight, renderScale);

    Console& console = Console::getInstance();

    std::cout << "Debut de la boucle principale" << std::endl;

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

        if (sceneRenderer.isInitialized()) {
            if(!sceneRenderer.render(deltaTime)){
                console.addLog("Scene Renderer error");
            }

        }
        
        renderImGui();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    sceneRenderer.cleanup();

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
    SceneRenderer::getInstance().resizeFramebuffer(width, height, renderScale);
}

