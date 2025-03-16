#include "camera.hpp"

// window
GLFWwindow* window;
int windowWidth = 1024;  // Taille initiale de la fenêtre
int windowHeight = 768;  // Taille initiale de la fenêtre
float renderScale = 1.0f;  // Facteur d'échelle pour le framebuffer (1.0 = même résolution)

glm::vec4 backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 0.0f);

// camera
Camera camera(glm::vec3(0.0f, 1.0f,  2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f), 0.0f, 2.5f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

// render to texture to display in ImGui
unsigned int framebuffer = 0;
unsigned int textureColorbuffer = 0;
unsigned int rbo = 0;
int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;