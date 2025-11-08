#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& filePath) : textureID(0), width(0), height(0), channels(0) {
    // Load image using stb_image
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }
    
    std::cout << "Loaded texture: " << filePath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    
    // Determine format based on channels
    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;
    else if (channels == 1) format = GL_RED;
    
    // Create OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Free image data
    stbi_image_free(data);
}

Texture::~Texture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
