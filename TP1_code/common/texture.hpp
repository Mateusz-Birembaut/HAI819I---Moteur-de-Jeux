#ifndef TEXTURE_HPP
#define TEXTURE_HPP


class Texture {
    public:
        Texture(const char* path = nullptr,
                bool flipVertically = false,
                bool flipHorizontally = false)
        : path(path),
          width(0), height(0), nrChannels(0),
          data(nullptr),
          flipVertically(flipVertically), flipHorizontally(flipHorizontally),
          format(GL_RGB),
          textureID(0)
        {}
    
        ~Texture() {
            if (data) {
                stbi_image_free(data);
            }
        }
    
        void load() {
            stbi_set_flip_vertically_on_load(flipVertically);
            data = stbi_load(path, &width, &height, &nrChannels, 0);
            if (!data) {
                std::cerr << "Failed to load texture" << std::endl;
                return;
            }
    
            if (nrChannels == 1) {
                format = GL_RED;
            } else if (nrChannels == 3) {
                format = GL_RGB;
            } else if (nrChannels == 4) {
                format = GL_RGBA;
            } else {
                std::cerr << "Unsupported number of channels." << std::endl;
                stbi_image_free(data);
                data = nullptr;
            }
        }
    
        void createTexture() {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
    
        void setParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        }
    
        void updateGLTexture() {
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
    
        GLuint getID() const {
            return textureID;
        }
    
    private:
        const char* path;
        int width;
        int height;
        int nrChannels;
        unsigned char* data;
        bool flipVertically;
        bool flipHorizontally;
    public:
        GLenum format;
        GLuint textureID;
    };

#endif