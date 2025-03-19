#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "../Rendering/Camera.hpp"



// window
inline GLFWwindow* window;
inline int windowWidth = 1024;  // Taille initiale de la fenêtre
inline int windowHeight = 768;  // Taille initiale de la fenêtre
inline float renderScale = 1.0f;  // Facteur d'échelle pour le framebuffer (1.0 = même résolution)

inline glm::vec4 backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 0.0f);

// timing
inline float deltaTime = 0.0f;	// time between current frame and last frame
inline float lastFrame = 0.0f;

//rotation
inline float angle = 0.;
inline float zoom = 1.;


inline bool pauseAnimations = false;





#endif