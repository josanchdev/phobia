#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture(const std::string& filePath);
    ~Texture();
    
    void bind() const;
    void unbind() const;
    
    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    
private:
    GLuint textureID;
    int width, height, channels;
};
