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

Terrain terrain(nX, nY, 10.0f, 10.0f);
std::vector<unsigned short> indices; //Triangles concaténés dans une liste
std::vector<std::vector<unsigned short> > triangles;
std::vector<Vertex> indexed_vertices;
GLuint vertexbuffer;    
GLuint elementbuffer;


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
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
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
    GLuint programID = LoadShaders( "Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/

    //Chargement du fichier de maillage
    //std::string filename("Meshes/chair.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    //generate a terrain
    terrain.create(indices, triangles, indexed_vertices);

    // Create and load the textures
    Texture heightMap("Heightmaps/heightmap-1024x1024.png", false);
    //Texture heightMap("Heightmap_Mountain.png", false);
    heightMap.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Texture grassTexture("Textures/grass.png", false);
    grassTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    Texture rockTexture("Textures/rock.png", false);
    rockTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Texture snowrockTexture("Textures/snowrocks.png", false);
    snowrockTexture.loadTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(Vertex), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

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

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)0            // array buffer offset
                    );


        // couleur
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)sizeof(glm::vec3)            // array buffer offset
                    );

        // normal
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    2,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)(sizeof(glm::vec3) * 2)            // array buffer offset
                    );

        // uv
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    3,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)(sizeof(glm::vec3) * 3)           // array buffer offset
                    );




        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // activer texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightMap.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grassTexture.getID());

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, rockTexture.getID());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, snowrockTexture.getID());

        // Obtenir l'emplacement de l'uniform `u_texture`
        GLuint textureID1 = glGetUniformLocation(programID, "u_heightMap");
        GLuint textureID2 = glGetUniformLocation(programID, "u_grassTexture");
        GLuint textureID3 = glGetUniformLocation(programID, "u_rockTexture");
        GLuint textureID4 = glGetUniformLocation(programID, "u_snowrockTexture");

        glUniform1i(textureID1, 0); 
        glUniform1i(textureID2, 1); 
        glUniform1i(textureID3, 2); 
        glUniform1i(textureID4, 3); 

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        glDisableVertexAttribArray(0);

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
    terrain.handleInputs(window, vertexbuffer, elementbuffer, indices, triangles, indexed_vertices);

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

