#include <GL/glew.h>
#include <iostream>

#include "SceneRenderer.hpp"
#include "../GameObjects/SceneGraphOctree.hpp"
#include "Camera.hpp"
#include "../Ressources/Globals.hpp"


SceneRenderer::SceneRenderer() {
    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
    }
}

SceneRenderer& SceneRenderer::getInstance() {
    static SceneRenderer instance;
    return instance;
}

bool SceneRenderer::isInitialized() {
    return initialized;
}

int SceneRenderer::getframebufferWidth() {
    return framebufferWidth;
}

int SceneRenderer::getframebufferHeight() {
    return framebufferHeight;
}

unsigned int SceneRenderer::getTextureColorbuffer() {
    return textureColorbuffer;
}

void SceneRenderer::setPauseAnimations(bool pause) {
    pauseAnimations = pause;
}

bool SceneRenderer::getPauseAnimations() {
    return pauseAnimations;
}

void SceneRenderer::resizeFramebuffer(int width, int height, float scale) {
    try {
        if (width <= 0 || height <= 0)
            return;

        framebufferWidth = static_cast<int>(width * scale);
        framebufferHeight = static_cast<int>(height * scale);

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
    } catch (const std::exception& e) {
        Console::getInstance().addLog(e.what());
    }
}

bool SceneRenderer::setupFramebuffer(float width, float height, float scale) {
    try {
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // Cull triangles which normal is not towards the camera
        glEnable(GL_CULL_FACE);
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            std::cout << "Current working directory: " << cwd << std::endl;
        } else {
            perror("getcwd() error");
        }

        // Create and compile our GLSL program from the shaders
        programID = LoadShaders("../src/Assets/Shaders/vertex_shader.glsl", "../src/Assets/Shaders/fragment_shader.glsl");
        if (programID == 0) {
            std::cout << "Error loading shaders" << std::endl;
            Console::getInstance().addLog("Error loading shaders");
            return false;
        }

        // Génération des objets OpenGL
        glGenFramebuffers(1, &framebuffer);
        glGenTextures(1, &textureColorbuffer);
        glGenRenderbuffers(1, &rbo);

        glGenBuffers(1, &vertexbuffer);
        glGenBuffers(1, &elementbuffer);

        // Get a handle for our "LightPosition" uniform
        glUseProgram(programID);
        LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        // Configuration du framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        framebufferWidth = static_cast<int>(width * scale);
        framebufferHeight = static_cast<int>(height * scale);

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
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Console::getInstance().addLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        initialized = true;
        return true;

    } catch (const std::exception& e) {
        Console::getInstance().addLog(e.what());
        cleanup();
        return false;
    }
}

bool SceneRenderer::render(float deltaTime) {
    try {
        if (initialized == false)
            return false;

        // render to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        Camera::getInstance().sendMatricesToShader(programID, windowWidth, windowHeight);

        if (!pauseAnimations) {
            SceneGraph::getInstance().updateAll(deltaTime);
        }
        SceneGraph::getInstance().drawAll(programID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    } catch (const std::exception& e) {
        Console::getInstance().addLog(e.what());
        cleanup();
        return false;
    }
}

void SceneRenderer::cleanup() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &rbo);
}
