#ifndef TEXTURE_HPP
#define TEXTURE_HPP

struct Texture_data {
    const char* path = nullptr;
    int width = 0, height = 0, nrChannels = 0;
    unsigned char* data = nullptr;
    bool flipVertically = false, flipHorizontally = false;
    GLenum format = GL_RGB;  // Example default format

    Texture_data() = default;  // Default constructor
};


// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


void loadTexture(Texture_data & t_d) { 
    
    stbi_set_flip_vertically_on_load(t_d.flipVertically); // Pour que l'image ne soit pas inversée
    t_d.data = stbi_load(t_d.path, &t_d.width, &t_d.height, &t_d.nrChannels, 0);

    if (t_d.data) {
        std::cout << "Texture loaded successfully:" << std::endl;
        std::cout << "Width: " << t_d.width << std::endl;
        std::cout << "Height: " << t_d.height << std::endl;
        std::cout << "Channels: " << t_d.nrChannels << std::endl;

        // Vérifiez si les dimensions sont des puissances de 2
        bool isPowerOf2 = (t_d.width & (t_d.width - 1)) == 0 && (t_d.height & (t_d.height - 1)) == 0;
        if (!isPowerOf2) {
            std::cout << "Warning: Texture dimensions are not power of 2" << std::endl;
        }

        // Détermine le format en fonction du nombre de canaux
        if (t_d.nrChannels == 1 ) {
            t_d.format = GL_RED;
        }
        else if (t_d.nrChannels == 3)
            t_d.format = GL_RGB;
        else if (t_d.nrChannels == 4)
            t_d.format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << t_d.nrChannels << std::endl;
            stbi_image_free(t_d.data);
        }
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
}


#endif