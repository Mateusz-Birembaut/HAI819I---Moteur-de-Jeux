
#define STB_IMAGE_IMPLEMENTATION
#include "../../../common/stb_image.h"  // Add this line

#include "Texture.hpp"
#include <glm/glm.hpp>  // Add this line to include the GLM library


Texture::Texture(const char* path, bool flipVertically)
    : path(path),
      width(0),
      height(0),
      nrChannels(0),
      data(nullptr),
      flipVertically(flipVertically),
      format(GL_RGB),
      textureID(0) {}

Texture::~Texture() {
    if (data) {
        stbi_image_free(data);
    }
}

void Texture::loadTexture(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) {
    load();
    createTexture();
    setParameters(wrapS, wrapT, minFilter, magFilter);
    updateGLTexture();
}

bool Texture::load(bool test) {
    if (data != nullptr) {
        return true;
    }

    stbi_set_flip_vertically_on_load(flipVertically);
    data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }

    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        std::cout << "Unsupported number of channels." << std::endl;
        stbi_image_free(data);
        data = nullptr;
        return false;
    }

    isPowerOfTwo = (width % 2 == 0 && height % 2 == 0);

    if (DEBUG_TEXTURE) {
        std::cout << "Loaded texture: " << path << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Number of channels: " << nrChannels << std::endl;
        std::cout << "Format: " << format << std::endl;
        std::cout << "Is power of two: " << isPowerOfTwo << std::endl;
    }

    if (test) {
        stbi_image_free(data);
        data = nullptr;
        return true;
    }

    return true;
}

void Texture::createTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::setParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) {
    if (nrChannels == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }
}

void Texture::updateGLTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 width,
                 height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint Texture::getID() const {
    return textureID;
}

const char* Texture::getName() const {
    return path;
}


float Texture::sampleTexture(float u, float v) {
    if (data == nullptr) {
        std::cerr << "Texture data is not loaded." << std::endl;
        return 0.0f;
    }

    float x = u * width;
    float y = v * height;

    int pixelX = floor(x);
    int pixelY = floor(y);

    int pixelXNext = std::min(pixelX + 1, width - 1);
    int pixelYNext = std::min(pixelY + 1, height - 1);

    if (pixelX < 0 || pixelX >= width || pixelY < 0 || pixelY >= height) {
        std::cerr << "Pixel coordinates out of bounds." << std::endl;
        return 0.0f;
    }

    float fracX = x - pixelX;
    float fracY = y - pixelY;

    auto getPixelValue = [this](int px, int py) -> float {
        unsigned char* pixel = data + (py * width + px) * nrChannels;
        if (format == GL_RED) {
            return static_cast<float>(pixel[0]) / 255.0f;
        } else if (format == GL_RGB || format == GL_RGBA) {
            return (pixel[0] + pixel[1] + pixel[2]) / 3.0f / 255.0f;
        }
        return 0.0f;
    };

    float v00 = getPixelValue(pixelX, pixelY);
    float v10 = getPixelValue(pixelXNext, pixelY);
    float v01 = getPixelValue(pixelX, pixelYNext);
    float v11 = getPixelValue(pixelXNext, pixelYNext);

    float top = v00 * (1.0f - fracX) + v10 * fracX;
    float bottom = v01 * (1.0f - fracX) + v11 * fracX;
    float value = top * (1.0f - fracY) + bottom * fracY;

    return value;
}