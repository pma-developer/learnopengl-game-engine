
#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>

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
    void setImage2DFromFile(std::string path) const;
    GLenum getTextureUnit() const;

private:
    GLuint genTexture() const;
    void setImageData(int width, int height, int nrChannels, unsigned char* data) const;
};