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

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;
/*******************************************************************************/

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};

/* void setTesselatedSquare ( std::vector<unsigned short> & indices, 
    std::vector<std::vector<unsigned short> > & triangles,
    std::vector<glm::vec3> & indexed_vertices)  {
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();

    int nX = 16;
    int nY = 16;

    for (int i = 0; i < nX; i++) {
        for (int j = 0; j < nY; j++) { 
            float t_x, t_z, x, y, z;

            t_x = (float(i) / float(nX - 1))*2;
            t_z = (float(j) / float(nY - 1))*2;
            
            x = t_x -1;
            z = t_z -1;

            y =  static_cast<float>(std::rand()) / RAND_MAX * 0.06f - 0.03f;
            
            glm::vec3 position = glm::vec3(x, y, z);

            indexed_vertices.push_back(position);
        }
    }
    for (int i = 0; i < nX; i ++) {
        for (int j = 0; j < nY; j++) {
            if(i != nX-1 && j != nY-1){
                short unsigned int de_base = i*nX + j;
                short unsigned int voisin_droite = i*nX + j+1;
                short unsigned int voisin_haut = (i+1)*nX + j;
                short unsigned int voisin_haut_droite = (i+1)*nX + j+1;


                indices.push_back(de_base);
                indices.push_back(voisin_haut);
                indices.push_back(voisin_droite);
                triangles.push_back({de_base, voisin_haut, voisin_droite});


                indices.push_back(voisin_haut_droite);
                indices.push_back(voisin_droite);
                indices.push_back(voisin_haut);
                triangles.push_back({voisin_haut_droite, voisin_droite, voisin_haut});

            }
        }
    }
} */

void setTesselatedSquare ( std::vector<unsigned short> & indices, 
    std::vector<std::vector<unsigned short> > & triangles,
    std::vector<Vertex> & indexed_vertices)  {
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();

    int nX = 16;
    int nY = 16;

    for (int i = 0; i < nX; i++) {
        for (int j = 0; j < nY; j++) { 
            float t_x, t_z, x, y, z;

            t_x = (float(i) / float(nX - 1))*2;
            t_z = (float(j) / float(nY - 1))*2;
            
            x = t_x -1;
            z = t_z -1;

            y =  static_cast<float>(std::rand()) / RAND_MAX * 0.06f - 0.03f;
            
            glm::vec3 position = glm::vec3(x, y, z);

            Vertex v;
            v.position = position;
            v.color = position;
            v.normal = glm::vec3(0.0, 1.0, 0.0);
            //v.uv = glm::vec2(i/(nX-1) , j/(nY-1));
            v.uv = glm::vec2(0.0,0.0);

            indexed_vertices.push_back(v);
        }
    }
    for (int i = 0; i < nX; i ++) {
        for (int j = 0; j < nY; j++) {
            if(i != nX-1 && j != nY-1){
                short unsigned int de_base = i*nX + j;
                short unsigned int voisin_droite = i*nX + j+1;
                short unsigned int voisin_haut = (i+1)*nX + j;
                short unsigned int voisin_haut_droite = (i+1)*nX + j+1;


                indices.push_back(de_base);
                indices.push_back(voisin_haut);
                indices.push_back(voisin_droite);
                triangles.push_back({de_base, voisin_haut, voisin_droite});


                indices.push_back(voisin_haut_droite);
                indices.push_back(voisin_droite);
                indices.push_back(voisin_haut);
                triangles.push_back({voisin_haut_droite, voisin_droite, voisin_haut});

            }
        }
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
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    //std::vector<glm::vec3> indexed_vertices;

    std::vector<Vertex> indexed_vertices;

    //Chargement du fichier de maillage
    std::string filename("chair.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    setTesselatedSquare(indices, triangles, indexed_vertices);

    // Load it into a VBO


    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    /* 
    int width, height, nrChannels;
    unsigned char *data = stbi_load("sol.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    */

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3) * 4, &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
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

        glm::mat4 viewMatrix = glm::lookAt(camera_position, camera_target + camera_position, camera_up);

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
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)(sizeof(glm::vec3) * 3)           // array buffer offset
                    );




        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // activer texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Obtenir l'emplacement de l'uniform `u_texture`
        GLuint textureID = glGetUniformLocation(programID, "u_texture");

        glUniform1i(textureID, 0);

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        glm::vec3 camera_right = glm::cross(camera_up, camera_target);
        camera_position += cameraSpeed * camera_right;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        glm::vec3 camera_right = glm::cross( camera_up, camera_target);
        camera_position -= cameraSpeed * camera_right;
    }

    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

