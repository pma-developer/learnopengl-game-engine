
#pragma once

#include <iostream>
#include <string>
#include <stb_image.h>
#include <glad/glad.h>;

class Texture
{
private:
    GLuint textureId;
    GLenum textureUnit;
    GLenum textureType;

public:
    Texture(GLenum textureUnit, GLenum textureType);
    ~Texture();
    void bind() const;
    GLuint setImage2D(std::string path) const;

private:
    void setImage2DFromFile(std::string path) const;
    GLuint genTexture() const;
    void setImageData(int width, int height, int nrChannels, unsigned char* data) const;
};