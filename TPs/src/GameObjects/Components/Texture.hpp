#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>  

#define DEBUG_TEXTURE false

class Texture {
private:
    const char* path;
    int width;
    int height;
    int nrChannels;
    unsigned char* data;
    bool flipVertically;
    bool isPowerOfTwo;

public:
    GLenum format;
    GLuint textureID;

    Texture(const char* path = nullptr, bool flipVertically = false);
    ~Texture();

    void loadTexture(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
    bool load(bool test = false);
    void createTexture();
    void setParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
    void updateGLTexture();
    void bind(GLenum textureUnit = GL_TEXTURE0) const;
    GLuint getID() const;
    const char* getName() const;
    float sampleTexture(float x, float z);
};

#endif
